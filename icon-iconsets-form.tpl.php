<?php
// $Id$

/**
 * @file icon-iconsets-form.tpl.php
 * Default theme implementation to configure icon sets.
 *
 * Available variables:
 * - $iconsets: An array of icon sets organized by type.
 * - $types: An array of icon set type names.
 * - $buttons: Submit and Reset buttons as well as hidden form elements.
 *
 * Each $iconsets[$type] contains:
 * - $screenshot: Its screenshot (or 'no screenshot' text).
 * - $name: The name of the icon set.
 * - $description: Its description.
 * - $status_checkbox: A checkbox for enabling/disabling the icon set.
 * - $weight_select: Drop-down menu for setting weights.
 *
 * Each $types[$type] contains:
 * - $title: The name of the type of icon set.
 * - $description: A description of the icon set type.
 *
 * @see template_preprocess_icon_iconsets_form()
 * @see theme_icon_iconsets_form()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  foreach ($iconsets as $type => $sets) {
    drupal_add_tabledrag('iconsets-'. $type, 'order', 'sibling', 'iconset-weight');
    drupal_add_tabledrag('iconsets-'. $type, 'order', 'sibling', 'iconset-weight');
  }
?>
<?php foreach ($iconsets as $type => $sets): ?>
  <h3><?php print $types[$type]['title']; ?></h3>
  <p><?php print $types[$type]['description']; ?></p>
  <table id="iconsets-<?php print $type; ?>" class="iconsets sticky-enabled">
    <thead>
      <tr>
        <th><?php print t('Screenshot'); ?></th>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Enabled'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php if (empty($iconsets[$type])): ?>
      <tr class="type-message type-empty">
        <td colspan="4"><em><?php print t('No icon sets'); ?></em></td>
      </tr>
      <?php else: ?>
      <?php $row = 0; ?>
      <?php foreach ($iconsets[$type] as $iconset): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> type-<?php print $type; ?>">
        <td><?php print $iconset->screenshot; ?></td>
        <td class="iconset-info"><h2><?php print $iconset->name; ?></h2><div class="description"><?php print $iconset->description; ?></div></td>
        <td><?php print $iconset->status_checkbox; ?></td>
        <td><?php print $iconset->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
      <?php endif; ?>
    </tbody>
  </table>
<?php endforeach; ?>

<?php print $buttons; ?>
