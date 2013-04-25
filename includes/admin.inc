<?php

/**
 * @file admin.inc
 * Provides administrative callbacks and tasks.
 */

/**
 * Icon Bundle Overview Form
 */
function icon_bundle_overview_form($form, $form_state) {
  $module_path = drupal_get_path('module', 'icon');
  $form['#attached'] = array(
    'css' => array(
      $module_path . '/css/iconapi-embedded.css',
      array(
        'type' => 'file',
        'data' => $module_path . '/css/iconapi-ie7-codes.css',
        'browser' => array(
          '!IE' => FALSE,
          'IE' => 'IE 7',
        ),
      )
    ),
  );
  $header = array(
    'bundle' => t('Bundle'),
    'provider' => t('Provider'),
    'count' => t('# of Icons'),
    'operations' => t('Operations'),
  );
  $enabled_bundles = 0;
  $total_icons = 0;
  $destination = drupal_get_destination();
  $rows = array();
  foreach (icon_bundles() as $bundle) {
    $row = array();
    $row[] = ($bundle['enabled'] ? l($bundle['title'], ICON_ADMIN_PATH . '/bundle/' . $bundle['name']) : $bundle['title']) . (!empty($bundle['version']) ? ' - ' . $bundle['version'] : '') . (!$bundle['enabled'] ? ' (disabled)' : '');
    $row[] = !empty($bundle['url']) ? l($bundle['provider'], $bundle['url']) : $bundle['provider'];
    $row[] = count($bundle['icons']);
    $operations = array();
    if ($bundle['enabled']) {
      $enabled_bundles += 1;
      $total_icons += count($bundle['icons']);
      if (user_access('administer icons')) {
        $operations['disable'] = array(
          'title' => '<i class="iconapi-admin-cancel"></i>' . t('Disable'),
          'href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/disable',
          'query' => $destination,
          'html' => TRUE,
        );
      }
    }
    elseif (user_access('administer icons')) {
      $operations['enable'] = array(
        'title' => '<i class="iconapi-admin-check"></i>' . t('Enable'),
        'href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . '/enable',
        'query' => $destination,
        'html' => TRUE,
      );
    }
    $row['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $row['operations'] = array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $row['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('html' => TRUE, 'query' => $link['query']),
        ),
      );
    }
    $rows[] = $row;
  }
  $form['bundles'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No bundles currently installed.'),
  );
  $form['totals'] = array(
    '#markup' => '<strong>' . t('Enabled bundles: !enabled_bundles', array('!enabled_bundles' => $enabled_bundles)) . '</strong><br /><strong>' . t('Icons available: !total', array('!total' => $total_icons)) . '</strong>',
  );
  return $form;
}

function icon_bundle_enable_form($form, $form_state, $bundle) {
  if ($bundle['enabled']) {
    drupal_set_message(t('The icon bundle %bundle is already enabled.', array('%bundle' => $bundle['title'])), 'error');
    drupal_goto(ICON_ADMIN_PATH);
  }
  return confirm_form($form, t('Enable %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, '', t('Enable'));
}

function icon_bundle_enable_form_submit($form, $form_state) {
  $bundle = $form_state['build_info']['args'][0];
  $disabled_bundles = drupal_map_assoc(variable_get('icon_disabled_bundles', array()));
  if (!empty($form_state['values']['confirm']) && !empty($disabled_bundles[$bundle['name']])) {
    unset($disabled_bundles[$bundle['name']]);
    variable_set('icon_disabled_bundles', array_values($disabled_bundles));
    icon_clear_all_caches();
  }
}

function icon_bundle_disable_form($form, $form_state, $bundle) {
  if (!$bundle['enabled']) {
    drupal_set_message(t('The icon bundle %bundle is already disabled.', array('%bundle' => $bundle['title'])), 'error');
    drupal_goto(ICON_ADMIN_PATH);
  }
  return confirm_form($form, t('Disable %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, '', t('Disable'));
}

function icon_bundle_disable_form_submit($form, $form_state) {
  $bundle = $form_state['build_info']['args'][0];
  $disabled_bundles = drupal_map_assoc(variable_get('icon_disabled_bundles', array()));
  if (!empty($form_state['values']['confirm']) && !in_array($bundle['name'], $disabled_bundles)) {
    $disabled_bundles[] = $bundle['name'];
    variable_set('icon_disabled_bundles', array_values($disabled_bundles));
    icon_clear_all_caches();
  }
}

function icon_bundle_list($bundle) {
  $build = array();
  foreach ($bundle['icons'] as $icon => $value) {
    $build[] = array(
      '#theme' => 'icon',
      '#bundle' => $bundle['name'],
      '#icon' => $icon,
    );
  }
  return $build;
}