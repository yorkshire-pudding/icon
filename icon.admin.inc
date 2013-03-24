<?php

/**
 * @file icon.admin.inc
 * Provides administrative callbacks and tasks.
 */

/**
 * Icon Bundle Overview Form
 */
function icon_bundle_overview() {
  $build = array();
  $header = array(
    'bundle' => t('Bundle'),
    'provider' => t('Provider'),
    'count' => t('# of Icons'),
    t('Operations'),
  );
  $rows = array();
  foreach (icon_info() as $provider) {
    foreach ($provider['bundles'] as $bundle) {
      $row = array(
        $bundle['title'],
        !empty($provider['url']) ? l($provider['title'], $provider['url']) : $provider['title'],
        count($bundle['icons']),
        '',
      );
      $rows[] = $row;
    }
  }
  $build['bundles'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No bundles currently installed.'),
  );
  return drupal_render($build);
}
