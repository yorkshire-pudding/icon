<?php

/**
 * @file cache.inc
 * Provides hooks and functions for cache related tasks.
 */

/**
 * Implements hook_flush_caches().
 */
function icon_flush_caches() {
  $caches = array();
  $tables = array('cache_icon_bundles');
  foreach ($tables as $table) {
    if (db_table_exists($table)) {
      $caches[] = $table;
    }
  }
  return $caches;
}

function icon_clear_all_caches() {
  foreach (icon_flush_caches() as $bin) {
    cache_clear_all('*', $bin, TRUE);
  }
  cache_clear_all('icon_', 'cache', TRUE);
  icon_reset_static_cache();
}

function icon_reset_static_cache() {
  drupal_static_reset('icon_render_hooks');
  drupal_static_reset('icon_bundles');
  drupal_static_reset('icon_bundle_load');
  drupal_static_reset('icon_extension_hook');
  drupal_static_reset('icon_extension_implements');
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

