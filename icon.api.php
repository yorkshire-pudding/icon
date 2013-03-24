<?php

/**
 * @file icon.api.php
 * Module hooks, theming hooks and form elements provided by the Icon API module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define information about icon providers and bundles.
 *
 * @return $providers
 *   An associative array containing the provider details, each containing a collection of bundles.
 * @see lullacons_icon_info()
 */
function hook_icon_info() {
  $providers['provider_machine_name'] = array(
    // title: optional, human readable title for the provider. Defaults to provider_machine_name.
    'title' => t('My Provider Name'),
    // url: optional, URL the points to more information about the provider. Used on the icon overview page.
    'url' => 'http://example.com/icons_provided',
    // file: optional, name of the file where the preprocessing and theming functions reside. By default this is the .module file.
    'file' => 'bundles.inc',
    // path: optional, path to where the file above is located. It is by default the path of the module that implements this hook.
    'path' => drupal_get_path('module', 'my_module') . '/includes',
    // bundles: required, associative array containing the following structure.
    'bundles' => array(
      'bundle_machine_name' => array(
        // title: optional, human readable title for the bundle. Defaults to bundle_machine_name.
        'title' => t('My Bundle'),
        // icons: required, associative array with the icon machine name set as the key.
        // This array can be structured to your liking. You must, however, ensure
        // that the theme callback understands the structure that is created here.
        'icons' => array(
          'alert', // Icon names (filenames, without extension)
          'info',
          'warning',
        ),
        // import: optional, declare that this bundle supports importing a compressed archive file.
        'import' => FALSE, // or TRUE,
        // settings: optional, associative array of default settings.
        'settings' => array(),
      ),
    ),
  );
  return $providers;
}

/**
 * Modify icon information before it gets cached.
 */
function hook_icon_info_alter(&$info) {
}

/**
 * Modify icon bundles before it gets cached.
 */
function hook_icon_bundles_alter(&$bundle) {
}

/**
 * Modify icon providers before it gets cached.
 */
function hook_icon_providers_alter(&$info) {
}

/**
 * Implements hook_preprocess_icon_PROVIDER().
 * @see lullacons_preprocess_icon_lullabot()
 */
function hook_preprocess_icon_PROVIDER(&$variables) {
  // Add custom classes here.
  $variables['attributes']['class'][] = 'custom-class';
}

/**
 * Implements hook_process_icon_PROVIDER().
 */
function hook_process_icon_PROVIDER(&$variables) {
  // Not likely used, but available nonetheless.
}



// @TODO Implement the following hooks.

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


/**
 * @addtogroup themable
 * @{
 */

/**
 * Implements theme_icon_PROVIDER().
 * Returns an image of the requested icon.
 */
function theme_icon_PROVIDER($variables) {
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

/*
 * @} End of "addtogroup themable".
 */


/**
 * @addtogroup input_element
 * @{
 */

/**
 * Implements the icon_selector input element.
 * The icon_selector sets a tree state of TRUE, values will be inside
 * the element's value tree as "bundle" and "icon".
 * 
 * @param string #type
 *   The type of element to render, must be: icon_selector.
 * @param string #title (optional, default: "Icon")
 *   The title of the fieldset.
 * @param boolean #collapsible (optional, default: TRUE)
 *   Determine whether the fieldset is collapsible.
 * @param boolean #collapsed (optional, default: FALSE)
 *   Determine whether the fieldset should initialize in a collapsed state.
 * @param string #default_bundle (optional)
 *   Machine name of the default bundle to initialize with.
 * @param string #default_icon (optional)
 *   Machine name of the default icon to initialize with.
 *  
 * @see icon_block_form_alter()
 * @see icon_block_form_submit()
 */
function _icon_selector_element_info() {
  return array(
    '#type' => 'icon_selector',
    '#title' => t('Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_bundle' => '',
    '#default_icon' => '',
  );
}

/*
 * @} End of "addtogroup input_element".
 */
