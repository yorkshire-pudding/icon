// $Id$

/**
 * Restrict moving of icon sets to their region in the table.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 */
Drupal.behaviors.iconsetDrag = function(context) {
  var table = $('table#iconsets');
  var tableDrag = Drupal.tableDrag.iconsets; // Get the blocks tableDrag object.

  tableDrag.row.prototype.isValidSwap = function(row) {
    // Get the type name of the row being dragged.
    var typeName = $('tr.drag', table).attr('class').replace(/([^ ]+[ ]+)*type-([^ ]+)([ ]+[^ ]+)*/, '$2');

    // Only allow it to be swapped with a draggable row of the same type.
    if ($(row).is('.draggable.type-' + typeName)) {
      return true;
    }
    else {
      return false;
    }
  };
};
