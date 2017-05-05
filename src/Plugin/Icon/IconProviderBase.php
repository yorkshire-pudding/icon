<?php

namespace Drupal\icon\Plugin\Icon;

use Drupal\Core\Plugin\PluginBase;
use Symfony\Component\DependencyInjection\ContainerAwareTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class IconProviderBase
 */
abstract class IconProviderBase extends PluginBase implements IconProviderInterface {

  use ContainerAwareTrait;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ContainerInterface $container = NULL) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    if (!isset($container)) {
      $container = \Drupal::getContainer();
    }
    $this->setContainer($container);
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static($configuration, $plugin_id, $plugin_definition, $container);
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return isset($this->pluginDefinition['label']) ? $this->pluginDefinition['label'] : $this->getPluginId();
  }

  /**
   * {@inheritdoc}
   */
  public function getUrl() {
    return isset($this->pluginDefinition['url']) ? $this->pluginDefinition['url'] : '';
  }

  /**
   * {@inheritdoc}
   */
  public function getSettings() {
    return isset($this->pluginDefinition['settings']) ? $this->pluginDefinition['settings'] : [];
  }

}
