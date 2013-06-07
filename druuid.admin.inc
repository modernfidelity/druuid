<?php

/**
 * @file
 * Administration functions for the druuid module.
 *
 *
 * @TODO : attach the druuid field to the selected bundle types
 * @TODO : generate / save druuids on selected items
 *
 *
 */

/**
 * Menu callback: options for DrUUID.
 */
function druuid_admin_form($form) {

  $form = array();

  $form['attached']['druuid_bundles'] = array(
      '#title' => t('Select whcih entity bundle to attach Druuid field'),
      '#description' => t('These are the entities would you like to attach a Druuid field on.'),
      '#type' => 'checkboxes',
      '#options' => _druuid_get_system_bundles(),
      '#default_value' => variable_get('druuid_bundles', ''),
  );


  $form = system_settings_form($form);
  // add extra submit handler after the main one
  $form['#submit'][] = '_druuid_attach_field_to_bundle_types';

  return $form;
}

/**
 * Menu callback: options for UUID.
 */
function druuid_admin_sync_form() {

  $form = array();

  $form['sync'] = array(
      '#type' => 'fieldset',
      '#title' => t('Synchronization'),
  );

  $form['sync']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create missing UUIDs'),
      '#submit' => array('uuid_admin_sync_submit'),
  );

  return $form;
}

/**
 *
 * Druuid Attach and Sync form
 *
 * @return array
 *
 */
function druuid_admin_attach_form() {



  $form = array();

  $form['attach'] = array(
      '#type' => 'fieldset',
      '#title' => t('Synchronization'),
  );

  $form['attach']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Attach & Create DrUUIDs'),
  );

  return $form;
}

/**
 *
 */
function druuid_admin_attach_form_submit() {

  _druuid_attach_field_to_bundle_types();
}

/**
 * Helper function to get all the names of the current system bundles
 */
function _druuid_get_system_bundles() {

  $bundles = array();

  $entity_list = entity_get_info();
  // check whether an entity is fieldable as this module only adds field instances
  // via fieldable entity

  foreach ($entity_list as $entity => $entity_info) {
    if ($entity_info['fieldable']) {
      if (is_array($entity_info['bundles'])) {
        foreach ($entity_info['bundles'] as $bundle_entity => $bundle) {
          $bundles[$bundle['label']] = $bundle['label'] . " (" . drupal_strtoupper($entity) . " entity)";
        }
      }
    }
  }

  ksort($bundles);

  return $bundles;
}

/**
 * Helper function that returns entity info for all supported core modules,
 * relevant for DrUUID functionality.
 *
 */
function _druuid_get_system_entities() {

  $entityinfo = entity_get_info();

  ksort($entityinfo);

  return array_keys($entityinfo);
}

/**
 * Create and attach field instances to specific entity bundle types
 *
 */
function _druuid_attach_field_to_bundle_types() {

  try {
    $entity_list = entity_get_info();

    $bundle_list = variable_get('druuid_bundles', '');


    $field_name = 'field_druuid';

    $field = field_info_field($field_name);

    if (empty($field)) {
      $field = array(
          'field_name' => $field_name,
          'type' => 'field_druuid',
          'cardinality' => 1, // default
          'settings' => array(
              'max_length' => 128, // default
          ),
          'translatable' => TRUE, // default is FALSE
          'entity_types' => array(), // default
      );

      field_create_field($field);
    }

    foreach ($entity_list as $ent => $entity) {
      foreach ($entity['bundles'] as $bun => $bundle_name) {

        // build the instance of the field
        $instance = array(
            'field_name' => $field_name,
            'entity_type' => $ent,
            'bundle' => $bun,
            'label' => t('Druuid'),
            'required' => TRUE,
            'widget' => array(
                'type' => 'druuid',
                'settings' => array(
                    'size' => 128, // default
                ),
            ),
            'settings' => array(
                'text_processing' => 0, // default FIXME
                'user_register_form' => TRUE,
            ),
            'display' => array(
                'default' => array(
                    'label' => 'hidden', // default
                    'type' => 'hidden', // default
                    'settings' => array(), // default
                ),
            ),
        );

        // check if it's set
        if (isset($bundle_list[$bundle_name['label']]) && _druuid_check_bundle_selection($bundle_list[$bundle_name['label']])) {
          // check if it's already there
          if (FALSE === _druuid_instance_exists_in_bundle($ent, $field_name, $bun)) {
            field_create_instance($instance);
            drupal_set_message(t('Successfully added a druuid field instance for the @bundle bundle.', array('@bundle' => $bun)));
          }
        }
        else {
          // check if an instance is set and remove it
          // but if there is data? should we be brutal?
          $instance = field_read_instance($ent, $field_name, $bun);
          if ($instance) {
            field_delete_instance($instance); // cleans up if it's the last instance of the field
            watchdog('DrUUID', 'Deleting the druid field from the !bundle bundle', array('!bundle' => $bun));
            drupal_set_message(t('Successfully removed the druuid field instances from the @bundle bundle.', array('@bundle' => $bun)));
          }
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog('DrUUID', $e->getMessage());
  }
}

/**
 * Helper funtion
 */
function _druuid_check_bundle_selection($item) {

  if ($item !== 0) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Checks to see if an instance of a field is already on the entity bundle
 * @param string $entity_type
 * @param string $field_name
 * @param string $bundle
 * @return array | boolean
 */
function _druuid_instance_exists_in_bundle($entity_type, $field_name, $bundle) {

  return field_read_instance($entity_type, $field_name, $bundle);
}
