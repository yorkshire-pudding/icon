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
 * Define render hook information.
 *
 * @return $hooks
 *   An associative array containing:
 *   - render_name: A unique machine name. An associative array containing:
 *     - file: Optional, file where the preprocessing and theming hooks are
 *       defined. If omitted, this will fall back to the .module or template.php
 *       file respectively.
 *     - path: Optional, path where the file above is located. If omitted, this
 *       will fall back to the module or theme path that implemented this hook.
 *       This may be obvious to some, but if the path is not the default module
 *       or theme root path, the file must also be explicitly set.
 *
 * @see icon_icon_render_hooks()
 */
function hook_icon_render_hooks() {
  $hooks['image'] = array(
    'file' => 'icon.render.inc',
    'path' => drupal_get_path('module', 'icon') . '/includes',
  );
  // $hooks['sprite'] = array();
  return $hooks;
}

/**
 * Modify icon render hooks before they become cached.
 */
function hook_icon_render_hooks_alter(&$hooks) {
}

/**
 * Define information about icon bundles.
 *
 * @return $bundles
 *   An associative array containing:
 *   - bundle_name: A unique machine name. An associative array containing:
 *     - render: Required, name of the rendering hook this bundle should use.
 *       If omitted or the rendering hook is not implemented, then this bundle
 *       will be ignored.
 *     - icons: Required, an array containing icon data. The structure of this
 *       array entirely depends on which render hook is being used. If unsure,
 *       study the render hook's theme_icon_RENDER() implementation.
 *     - title: Optional, human readable title for the bundle. If omitted, it
 *       will fall back to using the bundle_name.
 *     - provider: Optional, name of the bundle provider. If omitted, it will
 *       fall back to using the module name that implements this hook.
 *     - url: Optional, URL for more information regarding the bundle.
 *     - version: Optional, supplemental information for identifying the bundle's
 *       revision iteration.
 *     - path: Optional, path to where the bundle's resource files are located.
 *       If omitted, it will fall back to using the path of the module that
 *       implements this hook.
 *     - import: Optional, a boolean that informs the API that this bundle supports
 *       dynamic bundles by uploading compressed archive files.
 *     - settings: Optional, an array containing setting data. The structure of
 *       this array entirely depends on which render hook is be used. If unsure,
 *       study the render hook's theme_icon_RENDER() implementation.
 *
 * @see icon_icon_bundles()
 */
function hook_icon_bundles() {
  $bundles['my_bundle'] = array(
    'render' => 'image',
    'title' => t('My Bundle'),
    'provider' => t('ACME Inc.'),
    'url' => 'http://example.com/about_my_bundle',
    'version' => 'v2',
    'path' => drupal_get_path('module', 'my_module') . '/icons',
    'import' => TRUE, // or FALSE,
    'icons' => array(
      'alert', // Icon names (filenames, without extension)
      'info',
      'warning',
    ),
    'settings' => array(
      'extension' => 'gif', // Defaults to 'png' for the render hook: image.
    ),
  );
  return $bundles;
}

/**
 * Modify icon bundles before they become cached.
 */
function hook_icon_bundles_alter(&$bundle) {
}

/**
 * Implements hook_preprocess_icon_RENDER_HOOK().
 * @see icon_preprocess_icon_image()
 */
function hook_preprocess_icon_RENDER_HOOK(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  if (!isset($bundle['settings']['extension'])) {
    $bundle['settings']['extension'] = 'png';
  }
  // Add custom classes here.
  $variables['attributes']['class'][] = $icon;
}

/**
 * Implements hook_process_icon_RENDER_HOOK().
 */
function hook_process_icon_RENDER_HOOK(&$variables) {
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

/**
 * Allows modules to be grouped together with Icon API on the permissions table.
 * 
 * @return The same type of array in hook_permission().
 *
 * @see icon_permission()
 * @see icon_block_icon_permission()
 * @see hook_permission()
 */
function hook_icon_permission() {
  return array(
    'administer block icons' => array(
      'title' => t('Administer block icons'),
      'description' => t('Grants selected roles the ability to administer icons in blocks.'),
    ),
  );
}

/*
 * @} End of "addtogroup hooks".
 */


/**
 * @addtogroup themable
 * @{
 */

/**
 * Implements theme_icon().
 * Pass the theming responsibility to the proper render hook.
 *
 * @param $variables
 *   An associative array containing:
 *   - attributes: Associative array of HTML attributes.
 *   - bundle: An associative array containing various properties.
 *   - icon: Name of the icon requested from the above bundle.
 *
 * @return HTML markup for the requested icon.
 *
 * @see hook_icon_bundles()
 */
function theme_icon($variables) {
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  if (!empty($bundle) && !empty($icon)) {
    return theme('icon_' . $bundle['render'], $variables);
  }
}

/**
 * Implements theme_icon_RENDER_HOOK().
 * Return an image of the requested icon.
 *
 * @param $variables
 *   An associative array containing:
 *   - attributes: Associative array of HTML attributes.
 *   - bundle: An associative array containing various properties.
 *   - icon: Name of the icon requested from the above bundle.
 *
 * @return HTML markup for the requested icon.
 *
 * @see hook_icon_bundles()
 * @see theme_icon_image()
 */
function theme_icon_RENDER_HOOK($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $image = $bundle['path'] . '/' . $icon . '.' . $bundle['settings']['extension'];
  if (file_exists($image) && ($info = image_get_info($image))) {
    $output = theme('image', array(
      'path' => $image,
      'height' => $info['height'],
      'width' => $info['width'],
      'attributes' => $variables['attributes'],
    ));
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
 * @param string #title
 *   Optional, the title of the fieldset.
 *   Default, "Icon".
 * @param boolean #collapsible
 *   Optional, determine whether the fieldset is collapsible.
 *   Default, TRUE.
 * @param boolean #collapsed
 *   Optional, determine whether the fieldset should initialize in a collapsed state.
 *   Default, FALSE.
 * @param string #default_bundle
 *   Optional, machine name of the default bundle to initialize with.
 * @param string #default_icon
 *   Optional, machine name of the default icon to initialize with.
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
