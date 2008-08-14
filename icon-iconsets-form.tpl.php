<?php
// $Id$

/**
 * @file block-admin-display-form.tpl.php
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $iconsets: An array of icon sets organized by type.
 * - $types: Names of icon set types.
 *
 * Each $block_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $block_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->throttle_check: Checkbox to enable throttling.
 * - $data->configure_link: Block configuration link.
 * - $data->delete_link: For deleting user added blocks.
 *
 * @see template_preprocess_block_admin_display_form()
 * @see theme_block_admin_display()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
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
    <tr class="region type-<?php print $type; ?>">
      <td colspan="4" class="region"><?php print $types[$type]; ?></td>
    </tr>
    <tr class="type-message type-<?php print $type; ?>-message <?php print empty($iconsets[$type]) ? 'type-empty' : 'type-populated'; ?>">
      <td colspan="4"><em><?php print t('No icon sets'); ?></em></td>
    </tr>
    <?php foreach ($iconsets[$type] as $iconset): ?>
    <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
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
