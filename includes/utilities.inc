<?php

/**
 * @file utilities.inc
 * Provides useful functions for common tasks.
 */

/**
 * Determines whether an extension implements a hook.
 *
 * @param $type
 *   The type of extension (theme or module).
 * @param $extension
 *   The name of the extension.
 * @param $hook
 *   The name of the hook (e.g. "help" or "menu").
 *
 * @return
 *   TRUE if the extension is both installed and enabled, and the hook is
 *   implemented in that extension.
 */
function icon_extension_hook($type, $extension, $hook) {
  $hooks = &drupal_static(__FUNCTION__, array());
  // Ensure the extension is loaded.
  if (!isset($hooks[$type][$extension])) {
    if ('module' === $type) {
      drupal_load($type, $extension);
    }
    // Themes cannot be loaded via drupal_load(), instead the theme's include
    // file must be loaded.
    elseif ('theme' === $type && ($include = icon_find_theme_include($extension))) {
      include_once $include;
    }
  }
  // Check to see if the hook is implemented.
  if (!isset($hooks[$type][$extension][$hook])) {
    $hooks[$type][$extension][$hook] = ('module' === $type ? module_hook($extension, $hook) : function_exists($extension . '_' . $hook));
  }
  return $hooks[$type][$extension][$hook];
}

/**
 * Determines which extensions are implementing a hook.
 *
 * @param $hook
 *   The name of the hook (e.g. "help" or "menu").
 *
 * @return
 *   An array with the names of the extensions which are implementing this hook.
 */
function icon_extension_implements($hook) {
  $implements = &drupal_static(__FUNCTION__, array());
  if (!isset($implements[$hook])) {
    $implements[$hook] = array();
    // Gather the modules that implement the hook.
    foreach (module_implements($hook) as $module) {
      $implements[$hook][$module] = 'module';
    }
    // Due to how this module caches data, using the global $base_theme_info
    // variable will not work here. That array only contains the active
    // theme. We need to determine whether all enabled themes have a hook
    // definition. To do this, themes must define an 'icon.inc' file somewhere
    // in their directory structure.
    foreach (icon_enabled_themes() as $theme) {
      if ($include = icon_find_theme_include($theme)) {
        @include_once $include;
        if (function_exists($theme . '_' . $hook)) {
          $implements[$hook][$theme] = 'theme';
        }
      }
    }
  }
  return $implements[$hook];
}

/**
 * Invokes a hook in a particular extension.
 *
 * @param $type
 *   The type of extension (theme or module).
 * @param $extension
 *   The name of the extension.
 * @param $hook
 *   The name of the hook to invoke.
 * @param ...
 *   Arguments to pass to the hook implementation.
 *
 * @return
 *   The return value of the hook implementation.
 */
function icon_extension_invoke($type, $extension, $hook) {
  $args = func_get_args();
  // Remove $type, $extension and $hook from the arguments.
  unset($args[0], $args[1], $args[2]);
  if (icon_extension_hook($type, $extension, $hook)) {
    return call_user_func_array($extension . '_' . $hook, $args);
  }
}

/**
 * Determine which themes are enabled.
 *
 * @return
 *   An array of theme names.
 */
function icon_enabled_themes() {
  $themes = &drupal_static(__FUNCTION__);
  if (!isset($themes)) {
    $themes = array();
    foreach (list_themes() as $theme) {
      if (!$theme->status || in_array($theme->name, $themes)) {
        continue;
      }
      $themes[] = $theme->name;
      // Merge in the base themes if this is a sub-theme.
      if (isset($theme->base_themes)) {
        $themes += $theme->base_themes;
      }
    }
  }
  return $themes;
}

/**
 * Find a specific theme icon include file.
 *
 * @param $theme
 *   The name of the theme to find an include file for.
 *
 * @return
 *   The include path or FALSE if not found.
 */
function icon_find_theme_include($theme) {
  static $includes;
  $themes = &drupal_static(__FUNCTION__, array());
  if (!isset($themes[$theme])) {
    $themes[$theme] = FALSE;
    if (!isset($includes)) {
      $includes = array_keys(drupal_system_listing('/icon.inc|icons.inc$/', 'themes', 'uri', 0));
    }
    foreach ($includes as $uri) {
      if (strpos($uri, drupal_get_path('theme', $theme)) !== FALSE) {
        $themes[$theme] = $uri;
        break;
      }
    }
  }
  return $themes[$theme];
}
