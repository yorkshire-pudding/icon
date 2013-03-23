<?php

/**
 * @file icon.api.php
 * Hooks provided by the Icon API module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define bundles.
 *
 * @return $bundles
 *   A collection of bundles.
 */
function hook_icon_bundle_info() {
  $bundles = array();
  $bundles['my_bundle'] = array(
    // icons: required, associative array with the icon machine name set as the key.
    // This array can be structured to your liking. You must, however, ensure
    // that the theme callback understands the structure that is created here.
    'icons' => array(
      'alert', // Icon names (filenames, without extension)
      'info',
      'warning',
    ),
    // title: optional, human readable title for the bundle. Defaults to machine name.
    'title' => t('My Bundle'),
    // import: optional, declare that this bundle supports importing a compressed archive file.
    'import' => FALSE, // or TRUE,
    // settings: optional, associative array of default settings.
    'settings' => array(),
  );
  return $bundles;
}

/**
 * Implements template_preprocess_icon_MODULE().
 */
function template_preprocess_icon_MODULE(&$variables) {
  // Add custom classes here.
  $variables['attributes']['class'][] = 'custom-class';
}

/**
 * Implements template_process_icon_MODULE().
 */
function template_process_icon_MODULE(&$variables) {
  // Not likely used, but available nonetheless.
}

/**
 * Implements theme_icon_MODULE().
 * Returns an image of the requested icon.
 */
function theme_icon_MODULE($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  // If you have only one bundle, you can ommit the if statement.
  if ($bundle['name'] === 'my_bundle') {
    $path = drupal_get_path('module', 'my_module') . '/icons/' . $icon . '.png';
    if ($info = image_get_info($path)) {
      $output = theme('image', array(
        'path' => $path,
        'height' => $info['height'],
        'width' => $info['width'],
        'attributes' => $variables['attributes'],
      ));
    }
  }
  return $output;
}

/**
 * @TODO: Implement the following hooks.
 */

/**
 * Delete callback for bundle.
 */
function hook_icon_bundle_delete($bundle) {
}

/**
 * Save callback for bundle.
 */
function hook_icon_bundle_save($bundle) {
}

/**
 * Process callback for bundle import.
 */
function hook_icon_bundle_import_process($file, &$form_state) {
}

/**
 * Validate callback for bundle import.
 */
function hook_icon_bundle_import_validate($form, &$form_state) {
}

/**
 * Submit callback for bundle import.
 */
function hook_icon_bundle_import_submit($form, &$form_state) {
}

/*
 * @} End of "addtogroup hooks".
 */
