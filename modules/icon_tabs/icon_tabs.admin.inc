<?php
/**
 * @file
 * Functions only needed on configuration page.
 */

/**
 * Implements hook_admin().
 *
 * Create admin settings for for icon Tabs.
 */
function icon_tabs_admin() {
  $form = array();

  // Icon Tabs general settings.
  $form['tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local Tasks'),
    '#weight' => 1,
    '#collapsible' => FALSE,
  );
  $form['tabs']['icon_tabs_hide_default'] = array(
    '#type' => 'checkbox',
    '#title' => t("Hide Drupal's default tabs"),
    '#default_value' => variable_get('icon_tabs_hide_default', 1),
    '#description' => t("Hides the Default local tasks tabs provided by Drupal if Icon Tabs block is rendered."),
  );
  $form['tabs']['fred_tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fred Tabs theme'),
    '#weight' => 1,
    '#collapsible' => FALSE,
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use 'Fred tabs' theme"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_theme', 0),
    '#description' => t("Style tabs with the 'Fred tabs' theme as well as adding icons."),
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_vertical'] = array(
    '#type' => 'checkbox',
    '#title' => t("Vertically stack tabs"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_vertical', 0),
    '#description' => t("Vertically stack tabs for larger screen widths. This will not be ideal for all themes. For best results ensure the block is in a region that spans the full width of the content area or is at least always on the left side. <strong>NOTE: The <a href='/admin/structure/block/manage/icon_tabs/icon-tabs/configure'>Icon Tabs block</a> must be used for this setting.</strong>"),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_fred_tabs_theme"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_vertical_width'] = array(
    '#type' => 'textfield',
    '#title' => t("Vertical stack width"),
    '#default_value' => variable_get('icon_tabs_fred_tabs_vertical_width', 1260),
    '#description' => t("The screen pixel width at which the tabs will stack vertically. Do not include 'px'. Set to 0 always stack tabs."),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon_tabs_fred_tabs_vertical"]' => array('checked' => FALSE),
      ),
    ),
  );

  // If Environment Indicator is enabled.
  if (module_exists('environment_indicator')) {
    $form['tabs']['fred_tabs']['icon_tabs_fred_tabs_environment_indicator'] = array(
      '#type' => 'checkbox',
      '#title' => t("Environment Indicator integration"),
      '#default_value' => variable_get('icon_tabs_fred_tabs_environment_indicator', FALSE),
      '#description' => t("Style the tabs using the colors of the current environment."),
      '#states' => array(
        'invisible' => array(
          ':input[name="icon_tabs_fred_tabs_theme"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  // The icon selection.
  $form['icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Icons'),
    '#description' => t("<strong>NOTE: Tabs must have been generated at least once to show up on this list.</strong>"),
    '#weight' => 3,
  );

  // Get the current list of tabs that we know about.
  $tab_icons = variable_get('icon_tabs_classes', array());

  // Placeholder input displays the icon. The actual form input holds the
  // icon code value which is what is actually stored but not displayed to
  // the user.
  $icon_input = theme('textfield', array(
    'element' => array(
      '#size' => 1,
      '#autocomplete_path' => FALSE,
      '#attributes' => array(
        'type' => 'text',
        'class' => array('selected-icon'),
        'readonly' => TRUE,
      ),
    )));

  // Clear input link.
  $clear_link = l(t('Clear'), '', array(
    'attributes' => array(
      'class' => array('icon-clear'),
    )));

  // Build an array or all of the tab titles.
  foreach ($tab_icons as $tab) {
    $count[] = $tab['title'];
  }
  // Count how many instances of each title there is.
  $count = (isset($count)) ? array_count_values($count) : 0;

  // Go through the tabs and add a form element for each one.
  foreach ($tab_icons as $key => $tab) {
    // Get the default values.
    $defaults = array_merge(icon_tabs_defaults(), !empty($tab['icon']) ? $tab['icon'] : array());

    // If tabs share a title, group them together or if its the deafult tab.
    if ($count[$tab['title']] > 1 || $key == 'default') {
      // The tab title.
      $category = strtolower($tab['title']);
      // Create groups for tabs with the same title.
      if (!isset($form['icons'][$category])) {
        $form['icons'][$category] = array(
          '#type' => 'fieldset',
          '#title' => $tab['title'],
          '#weight' => ($key == 'default') ? -1 : 1,
          '#collapsible' => ($key == 'default') ? FALSE : TRUE,
          '#collapsed' => ($key == 'default') ? FALSE : TRUE,
          '#description' => ($key == 'default') ? t('The default icon used for tabs. Unset this to not display any icon by default.') : FALSE,
        );
      }
      $form['icons'][$category][$key] = array(
        '#type' => 'icon_selector',
        '#title' => $tab['title'] . ' (' . $key . ')',
        '#default_bundle' => $defaults['bundle'],
        '#default_icon' => $defaults['icon'],
      );
    }
    else {
      $form['icons'][$key] = array(
        '#type' => 'icon_selector',
        '#title' => $tab['title'] . ' (' . $key . ')',
        '#weight' => 2,
        '#default_bundle' => $defaults['bundle'],
        '#default_icon' => $defaults['icon'],
      );
    }
  }

  // Custom submit and validate handlers to update icon codes.
  $form['#submit'][] = '_icon_tabs_admin_submit';
  $form['#validate'][] = '_icon_tabs_admin_validate';

  return system_settings_form($form);
}

/**
 * Custom validate handler for module admin form.
 */
function _icon_tabs_admin_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['icon_tabs_fred_tabs_vertical_width'])) {
    form_set_error('icon_tabs_fred_tabs_vertical_width', t("Verical stack width must be a number (do not include 'px')"));
  }
}

/**
 * Custom submit handler for module admin form.
 */
function _icon_tabs_admin_submit($form, &$form_state) {
  // Get the current list of tabs that we know about.
  $tab_icons = variable_get('icon_tabs_classes', array());

  // Loop through the tabs and update the code with the value from the form.
  foreach ($tab_icons as $key => $tab) {
    $tab_icons[$key]['icon'] = $form_state['values'][$key];
  }

  // If there have been any changes update the variable.
  if ($tab_icons != variable_get('icon_tabs_classes', array())) {
    variable_set('icon_tabs_classes', $tab_icons);
  }

}
