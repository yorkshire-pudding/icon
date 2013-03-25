<?php

/**
 * @file utilities.inc
 * Provides useful functions for common tasks.
 */

/**
 * Determines whether an extension implements a hook.
 */
function icon_extension_hook($type, $extension, $hook) {
  $hooks = &drupal_static(__FUNCTION__, array());
  // Ensure the extension's main file is loaded.
  if (!isset($hooks[$type][$extension])) {
    drupal_load($type, $extension);
  }
  // Check to see if the hook is implemented.
  if (!isset($hooks[$type][$extension][$hook])) {
    $hooks[$type][$extension][$hook] = ('module' === $type ? module_hook($extension, $hook) : function_exists($extension . '_' . $hook));
  }
  return $hooks[$type][$extension][$hook];
}

/**
 * Determines which extensions are implementing a hook.
 */
function icon_extension_implements($hook) {
  $implements = &drupal_static(__FUNCTION__, array());
  if (!isset($implements[$hook])) {
    $implements[$hook] = array();
    // Gather the modules that implement the hook.
    foreach (module_implements($hook) as $module) {
      $implements[$hook][$module] = 'module';
    }
    // Gather the themes that implement the hook.
    // @see drupal_alter()
    global $theme, $base_theme_info;
    if (isset($theme)) {
      $theme_keys = array();
      foreach ($base_theme_info as $base) {
        $theme_keys[] = $base->name;
      }
      $theme_keys[] = $theme;
      foreach ($theme_keys as $theme_key) {
        $function = $theme_key . '_' . $hook;
        if (function_exists($function)) {
          $implements[$hook][$theme_key] = 'theme';
        }
      }
    }
  }
  return $implements[$hook];
}

/**
 * Invokes a hook in a particular extension.
 */
function icon_extension_invoke($type, $extension, $hook) {
  $args = func_get_args();
  // Remove $type, $extension and $hook from the arguments.
  unset($args[0], $args[1], $args[2]);
  if (icon_extension_hook($type, $extension, $hook)) {
    return call_user_func_array($extension . '_' . $hook, $args);
  }
}
