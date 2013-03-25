<?php

/**
 * @file cache.inc
 * Provides hooks and functions for cache related tasks.
 */

/**
 * Provides an array of cache bins used by this module.
 */
function icon_cache_bins() {
  return array(
    'cache_icon_bundles',
  );
}

/**
 * Implements hook_flush_caches().
 */
function icon_flush_caches() {
  icon_clear_all_caches();
  return array();
}

/**
 * Clears all caches used by the icon module.
 */
function icon_clear_all_caches() {
  foreach (icon_cache_bins() as $bin) {
    cache_clear_all('*', $bin, TRUE);
  }
  cache_clear_all('icon_', 'cache', TRUE);
  icon_reset_static_cache();
  // Rebuild icon caches.
  icon_render_hooks();
  icon_bundles();
}

/**
 * Clears all static caches used by the icon module.
 */
function icon_reset_static_cache() {
  $functions = array(
    'icon_bundles',
    'icon_bundle_load',
    'icon_extension_hook',
    'icon_extension_implements',
    'icon_find_theme_include',
    'icon_render_hooks',
  );
  foreach ($functions as $function) {
    drupal_static_reset($function);
  }
}

/**
 * Implements hook_modules_enabled().
 */
function icon_modules_enabled($modules) {
  // Clear all caches so the can be rebuilt.
  icon_clear_all_caches();
}

/**
 * Implements hook_modules_enabled().
 */
function icon_modules_disabled($modules) {
  // Clear all caches so the can be rebuilt.
  icon_clear_all_caches();
}

/**
 * Implements hook_themes_disabled().
 */
function icon_themes_disabled($themes) {
  // Clear all caches so the can be rebuilt.
  icon_clear_all_caches();
}

/**
 * Implements hook_themes_disabled().
 */
function icon_themes_enabled($themes) {
  // Clear all caches so the can be rebuilt.
  icon_clear_all_caches();
}

