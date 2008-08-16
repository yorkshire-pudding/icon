<?php
// $Id$

/**
 * @file icon-iconsets-form.tpl.php
 * Default theme implementation to configure icon sets.
 *
 * Available variables:
 * - $iconsets: An array of icon sets organized by type.
 * - $types: Names of icon set types.
 * - $buttons: Submit and Reset buttons as well as hidden form elements.
 *
 * Each $iconsets[$type] contains:
 * - $screenshot: Its screenshot (or 'no screenshot' text).
 * - $name: The name of the icon set.
 * - $description: Its description.
 * - $status_checkbox: A checkbox for enabling/disabling the icon set.
 * - $weight_select: Drop-down menu for setting weights.
 *
 * @see template_preprocess_icon_iconsets_form()
 * @see theme_icon_iconsets_form()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'icon') .'/icon.js');
  foreach ($iconsets as $type => $sets) {
    // TODO: Restrict dragging of an item to within its section.
    drupal_add_tabledrag('iconsets', 'order', 'sibling', 'iconset-weight', 'iconset-weight-'. $type);
  }
?>
<table id="iconsets" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Screenshot'); ?></th>
      <th><?php print t('Name'); ?></th>
      <th><?php print t('Enabled'); ?></th>
      <th><?php print t('Weight'); ?></th>
    </tr>
  </thead>
  <tbody>
  <?php $row = 0; ?>
  <?php foreach ($iconsets as $type => $sets): ?>
    <tr class="iconset-type type-<?php print $type; ?>">
      <td colspan="4" class="region"><?php print $types[$type]; ?></td>
    </tr>
    <tr class="type-message type-<?php print $type; ?>-message <?php print empty($iconsets[$type]) ? 'type-empty' : 'type-populated'; ?>">
      <td colspan="4"><em><?php print t('No icon sets'); ?></em></td>
    </tr>
    <?php foreach ($iconsets[$type] as $iconset): ?>
    <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> type-<?php print $type; ?>">
      <td><?php print $iconset->screenshot; ?></td>
      <td class="iconset-info"><h2><?php print $iconset->name; ?></h2><div class="description"><?php print $iconset->description; ?></div></td>
      <td><?php print $iconset->status_checkbox; ?></td>
      <td><?php print $iconset->weight_select; ?></td>
    </tr>
    <?php $row++; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
  </tbody>
</table>
<?php print $buttons; ?>
