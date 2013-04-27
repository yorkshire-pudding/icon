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
  $form['bundles'] = array(
    '#tree' => TRUE,
  );
  foreach (icon_bundles() as $bundle) {
    $form['bundles'][$bundle['name']]['#bundle'] = $bundle;
    $form['bundles'][$bundle['name']]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $bundle['status'],
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

function icon_bundle_overview_form_submit($form, $form_state) {
  $bundles = icon_bundles();
  $saved_bundles = $form_state['values']['bundles'];
  $clear_caches = FALSE;
  foreach ($bundles as $name => $bundle) {
    if (!isset($saved_bundles[$name]['status'])) {
      continue;
    }
    $status = $saved_bundles[$name]['status'];
    if ($status !== $bundle['status']) {
      if (($status ? icon_bundle_enable($bundle) : icon_bundle_disable($bundle))) {
        icon_clear_all_caches();
      }
    }
  }
}

function theme_icon_bundle_overview_form($variables) {
  $output = '';
  $form = $variables['form'];
  $header = array(
    'enable' => t('Enabled'),
    'count' => t('Icons'),
    'bundle' => t('Bundle'),
    'provider' => t('Provider'),
    'operations' => array(
      'data' => t('Operations'),
      'colspan' => 3,
    ),
  );
  $destination = drupal_get_destination();
  $rows = array();
  foreach (element_children($form['bundles']) as $key) {
    $row = array();
    $bundle = $form['bundles'][$key]['#bundle'];
    $row[] = array(
      'data' => drupal_render($form['bundles'][$key]['status']),
      'style' => 'width: 0; text-align: center;',
    );
    $row[] = array(
      'data' => count($bundle['icons']),
      'style' => 'width: 0; text-align: center;',
    );
    $row[] = $bundle['title'] . (!empty($bundle['version']) ? ' - ' . $bundle['version'] : '');
    if ($provider = icon_provider_load($bundle['provider'])) {
      $row[] = !empty($provider['url']) ? l($provider['title'], $provider['url']) : $provider['title'];
    }
    else {
      $row[] = !empty($bundle['url']) ? l($bundle['provider'], $bundle['url']) : $bundle['provider'];
    }
    $row['view'] = '';
    $row['configure'] = '';
    $row['delete'] = '';

    $operations = array();
    if ($bundle['status']) {
      $operations['view'] = t('View');
    }
    if (!empty($bundle['import'])) {
      $operations['delete'] = '<i class="iconapi-admin-cancel"></i>' . t('Delete');
    }
    elseif (!empty($bundle['overriden'])) {
      $operations['reset'] = t('Reset');
    }
    // Convert operations into links.
    foreach ($operations as $action => $title) {
      $row[($action === 'reset' ? 'delete' : $action)] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $title,
          '#href' => ICON_ADMIN_PATH . '/bundle/' . $bundle['name'] . ($action === 'view' ? '' : '/' . $action),
          '#options' => array('html' => TRUE, 'query' => $destination),
        ),
        'style' => 'width: 0; white-space: nowrap;',
      );
    }
    $rows[] = $row;
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No bundles currently installed.'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}

function icon_bundle_delete_form($form, $form_state, $bundle) {
  return confirm_form($form, t('Delete %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, NULL, t('Delete'));
}

function icon_bundle_delete_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    icon_bundle_delete($form_state['build_info']['args'][0]);
  }
}

function icon_bundle_reset_form($form, $form_state, $bundle) {
  if (empty($bundle['overriden'])) {
    drupal_set_message(t('The icon bundle %bundle is not currently overridden in the database.', array('%bundle' => $bundle['title'])), 'error');
    drupal_goto(ICON_ADMIN_PATH);
  }
  return confirm_form($form, t('Reset %bundle icon bundle?', array('%bundle' => $bundle['title'])), ICON_ADMIN_PATH, NULL, t('Disable'));
}

function icon_bundle_reset_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    icon_bundle_delete($form_state['build_info']['args'][0]);
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