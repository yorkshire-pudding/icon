<?php

/**
 * @file admin.inc
 * Provides administrative callbacks and tasks.
 */

/**
 * Icon Bundle Overview Form
 */
function icon_bundle_overview_form($form, $form_state) {
  $disabled_bundles = variable_get('icon_disabled_bundles', array());
  $header = array(
    'bundle' => t('Bundle'),
    'provider' => t('Provider'),
    'count' => t('# of Icons'),
    'operations' => t('Operations'),
  );
  $total_icons = 0;
  $destination = drupal_get_destination();
  $rows = array();
  foreach (icon_bundles() as $bundle) {
    $status = !in_array($bundle['name'], $disabled_bundles);
    $row = array();
    $row[] = ($status ? l($bundle['title'], ICON_ADMIN_PATH . '/' . $bundle['name']) : $bundle['title']) . (!empty($bundle['version']) ? ' - ' . $bundle['version'] : '') . (!$status ? ' (disabled)' : '');
    $row[] = !empty($bundle['url']) ? l($bundle['provider'], $bundle['url']) : $bundle['provider'];
    $row[] = count($bundle['icons']);
    $operations = array();
    if ($status) {
      $total_icons += count($bundle['icons']);
      if (user_access('administer icons')) {
        $operations['disable'] = array(
          'title' => t('Disable'),
          'href' => ICON_ADMIN_PATH . '/' . $bundle['name'] . '/disable',
          'query' => $destination,
        );
      }
    }
    elseif (user_access('administer icons')) {
      $operations['enable'] = array(
        'title' => t('Enable'),
        'href' => ICON_ADMIN_PATH . '/' . $bundle['name'] . '/enable',
        'query' => $destination,
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
          '#options' => array('query' => $link['query']),
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
  $form['total'] = array(
    '#markup' => '<h3>' . t('Total available icons: !total', array('!total' => $total_icons)) . '</h3>',
  );
  return $form;
}

function icon_bundle_enable_form($form, $form_state, $bundle) {
  if (icon_bundle_enabled($bundle)) {
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
  if (!icon_bundle_enabled($bundle)) {
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