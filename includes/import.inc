<?php

/**
 * @file import.inc
 * Provides import administrative callbacks and tasks.
 */

function icon_provider_import_form($form, $form_state) {
  // Build a list of providers.
  $providers = array(
    'automatic' => t('Automatic detection'),
  );
  foreach (icon_providers() as $provider) {
    $providers[$provider['name']] = $provider['title'] . (!empty($provider['url']) ? ' (' . l(t('website'), $provider['url']) . ')' : '');
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Bundle title'),
    '#description' => t('The human readable name to identify the bundle.'),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Bundle name'),
    '#description' => t('A unique name to identify the bundle. It must only contain lowercase letters, numbers, hyphens and underscores.'),
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'icon_bundle_load',
      'source' => array('title'),
    ),
  );
  $form['provider'] = array(
    '#type' => 'radios',
    '#title' => t('Available providers'),
    '#options' => $providers,
    '#default_value' => 'automatic',
    '#required' => TRUE,
  );
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Archive file'),
    '#description' => t('Files with the following extensions are allowed: %extensions', array('%extensions' => archiver_get_extensions())),
    '#size' => 40,
  );
  $form['bundle']['import'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

function icon_provider_import_form_validate($form, &$form_state) {
  // Detect the selected provider.
  $form_state['provider'] = icon_providers($form_state['values']['provider']);
  $provider = &$form_state['provider'];
  // Construct the beginnings of a bundle.
  $form_state['bundle'] = icon_bundle_defaults();
  $bundle = &$form_state['bundle'];
  $bundle['title'] = $form_state['values']['title'];
  $bundle['name'] = $form_state['values']['name'];
  // Validate supported archive extensions.
  $valid_extensions = archiver_get_extensions();
  $validators = array('file_validate_extensions' => array($valid_extensions));
  if (!$file = file_save_upload('file', $validators, NULL, FILE_EXISTS_REPLACE)) {
    form_set_error('file');
    file_delete($file);
    return;
  }
  // Extract the archive. This needs to be done now so providers can validate
  // against the archive file contents.
  $temp_dir = 'temporary://icon_api_' . md5(REQUEST_TIME);
  try {
    $archive = icon_archive_extract(drupal_realpath($file->uri), drupal_realpath($temp_dir));
  }
  catch (Exception $e) {
    form_set_error('file', $e->getMessage());
    file_delete($file);
    file_unmanaged_delete_recursive($temp_dir);
    return;
  }
  $extract_path = $temp_dir . '/' . $archive->dir;
  // Automatic detection. Iterate over each provider and find the first one that
  // can import the archive file.
  if (!$provider) {
    foreach (icon_providers() as $_provider) {
      $validation = icon_extension_invoke($_provider['type'], $_provider[$_provider['type']], 'icon_import_validate', $_provider, $extract_path);
      // Providers must return a validation of TRUE if it passes. Otherwise
      // they should send an error message to display instead.
      if ($validation === TRUE) {
        $provider = $_provider;
        break;
      }
    }
    if (!$provider) {
      form_set_error('file', t('Unable to automatically detect which provider should be used to import %file. Try manually selecting which provider to use or re-download the archive file from the provider.', array(
        '%file' => $file->filename,
      )));
      file_delete($file);
      file_unmanaged_delete_recursive($temp_dir);
      return;
    }
  }
  // A specific provider was selected, show the validation errors.
  else {
    $validation = icon_extension_invoke($provider['type'], $provider[$provider['type']], 'icon_import_validate', $provider, $extract_path);
    if ($validation !== TRUE) {
      form_set_error('file', $validation);
      file_delete($file);
      file_unmanaged_delete_recursive($temp_dir);
      return;
    }
  }
  // A provider has been determined. Merge in default bundle settings.
  $bundle = array_merge_recursive($bundle, $provider['default bundle']);
  $bundle['provider'] = $provider['name'];
  // Move the extracted archive to a permanent location.
  if (!$bundle['path'] = icon_file_move($temp_dir . '/' . $archive->dir, 'public://icon/' . $provider['name'], $bundle['name'])) {
    form_set_error('file', t('An unknown error occured while trying to permanently move the uploaded archive file'));
  }
  // Delete the uploaded archive file and temporary directory.
  file_delete($file);
  file_unmanaged_delete_recursive($temp_dir);
}

function icon_provider_import_form_submit($form, &$form_state) {
  $bundle = &$form_state['bundle'];
  $provider = &$form_state['provider'];
  icon_extension_invoke($provider['type'], $provider[$provider['type']], 'icon_import_process', $provider, $bundle);
  $form_state['redirect'] = array(ICON_ADMIN_PATH);
}
