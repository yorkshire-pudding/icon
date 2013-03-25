<?php

/**
 * @file render.inc
 * Provides hooks and theme callbacks for default render hooks.
 * @TODO add a "sprite" render hook.
 */

/**
 * Define render hook information.
 *
 * @see hook_icon_render_hooks()
 */
function icon_icon_render_hooks() {
  $hooks['image'] = array(
    'file' => 'render.inc',
    'path' => drupal_get_path('module', 'icon') . '/includes',
  );
  return $hooks;
}

/**
 * Implements hook_preprocess_icon_RENDER_HOOK().
 */
function icon_preprocess_icon_image(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  if (!isset($bundle['settings']['extension'])) {
    $bundle['settings']['extension'] = 'png';
  }
  // Add custom classes here.
  $variables['attributes']['class'][] = $icon;
}

/**
 * Implements theme_icon_RENDER_HOOK().
 * Return an image of the requested icon.
 */
function theme_icon_image($variables) {
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
