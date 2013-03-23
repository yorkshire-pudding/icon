<?php

/**
 * @file icon.admin.inc
 * Provides administrative callbacks and tasks.
 */

/**
 * Icon Bundle Overview Form
 */
function icon_bundle_overview_form($form, $form_state, $theme = 'default') {
  // @TODO: remove this proof of concept test and replace with an overview form.
  if ($bundle = icon_bundle_load('lullacons_pack1')) {
    foreach ($bundle['icons'] as $icon) {
      $form[$icon] = array(
        '#theme' => 'icon',
        '#bundle' => 'lullacons_pack1',
        '#icon' => $icon,
      );
    }
  }
  return $form;
}
