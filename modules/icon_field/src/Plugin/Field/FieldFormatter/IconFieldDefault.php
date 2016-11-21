<?php /**
 * @file
 * Contains \Drupal\icon_field\Plugin\Field\FieldFormatter\IconFieldDefault.
 */

namespace Drupal\icon_field\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FormatterBase;

/**
 * @FieldFormatter(
 *  id = "icon_field_default",
 *  label = @Translation("Icon"),
 *  field_types = {"icon_field"}
 * )
 */
class IconFieldDefault extends FormatterBase {

  /**
   * @FIXME
   * Move all logic relating to the icon_field_default formatter into this
   * class. For more information, see:
   *
   * https://www.drupal.org/node/1805846
   * https://api.drupal.org/api/drupal/core%21lib%21Drupal%21Core%21Field%21FormatterInterface.php/interface/FormatterInterface/8
   * https://api.drupal.org/api/drupal/core%21lib%21Drupal%21Core%21Field%21FormatterBase.php/class/FormatterBase/8
   */

}
