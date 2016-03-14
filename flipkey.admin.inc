<?php
/**
 * @file admin.module.inc
 * Houses our admin functions.
 */

/**
 * Main settings form
 */
function flipkey_admin_settings() {
  //Main Form settings.

  $form['flipkey_config_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flipkey API Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $form['flipkey_config_fs']['flipkey_api_key'] = array(
    '#title' => t('API Key'),
    '#description' => t('This is the API key for your flipkey account.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('flipkey_api_key', NULL),
    '#max_length' => 75,
    '#weight' => -50,
  );

  $form['flipkey_config_fs']['flipkey_frontdesk_id'] = array(
    '#title' => t('Front desk ID'),
    '#description' => t('This is the front desk ID for your flipkey account.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('flipkey_frontdesk_id', NULL),
    '#max_length' => 12,
    '#weight' => -50,
  );


  $node_types = node_type_get_types();
  $node_type_options = array();
  foreach ($node_types as $key => $value) {
    $node_type_options[trim($key)] = trim($key);
  }

  $form['flipkey_config_fs']['flipkey_node_types'] = array(
    '#title' => t('Flipkey node types.'),
    '#description' => t('Select which node types flipkey should monitor for reviews.'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#required' => TRUE,
    '#options' => $node_type_options,
    '#default_value' => variable_get('flipkey_node_types', array()),
    '#weight' => -49,
  );

  $form['flipkey_config_fs']['flipkey_property_id_key'] = array(
    '#title' => t('Property ID key'),
    '#description' => t('For the Flipkey module to properly identify properties to store reviews for, there must be a field in the node content type that stores the Flipkey property Id. In some cases this field may already be available from the reservation system\'s imported data. If that is the case then you need only identify the field here. If not already present you will need to add a field to the content type and enter the resulting field name here.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flipkey_property_id_key', NULL),
    '#required' => TRUE,
    '#max_length' => 36,
    '#weight' => -48,
  );

  $form['flipkey_config_fs']['flipkey_review_key'] = array(
    '#title' => t('Review Field key'),
    '#description' => t('You will need to add a text field to the selected content types above to store the flipkey reviews. Once added enter the field id here. This will enable the module to correctly target that field when reviews are imported from Flipkey.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flipkey_review_key', NULL),
    '#required' => TRUE,
    '#max_length' => 36,
    '#weight' => -47,
  );

  $form['flipkey_config_fs']['flipkey_star_key'] = array(
    '#title' => t('Review Star key'),
    '#description' => t('You will need to add a text field to the selected content types above to provide a field to display the star rating'),
    '#type' => 'textfield',
    '#default_value' => variable_get('flipkey_star_key', NULL),
    '#required' => TRUE,
    '#max_length' => 36,
    '#weight' => -46,
  );

  $form['flipkey_config_fs']['flipkey_import_threshold'] = array(
    '#title'         => t('Import Threshold'),
    '#description'   => t('Set the number of properties to import reviews for per import iteration.'),
    '#type'          => 'select',
    '#options' => array(10=>'10', 20=>'20', 30=>'30', 40=>'40', 50=>'50' ),
    '#default_value'    => variable_get('flipkey_import_threshold', 10),
    '#weight'        => -45,
  );

  $form['flipkey_config_fs']['flipkey_cron_switch'] = array(
    '#title'         => t('Enable Review Import on Cron'),
    '#description'   => t('Check this box to enable invoking review imports automatically on cron.'),
    '#type'          => 'checkbox',
    '#default_value'    => variable_get('flipkey_cron_switch', 0),
    '#weight'        => -44,
  );

  $form['flipkey_config_fs']['flipkey_date_order'] = array(
    '#title'         => t('Review Date Order'),
    '#description'   => t('Imported reviews from flipkey typically import in no apparent order. You can set to order by date here.'),
    '#type'          => 'select',
    '#options' => array('ASC'=>'Ascending (Oldest First)','DESC' => 'Descending (Most Recent First)', 'DNO'=>'Do Not Re-order'),
    '#default_value'    => variable_get('flipkey_date_order', 'DESC'),
    '#weight'        => -43,
  );

  $form['flipkey_config_fs']['flipkey_rating_squelch'] = array(
    '#title'         => t('Rating Squelch'),
    '#description'   => t('Set minimum star rating allowed'),
    '#type'          => 'select',
    '#options' => array(0=>'Show All', 1=>'1', 2=>'2', 3=>'3', 4=>'4', 5=>'5' ),
    '#default_value'    => variable_get('flipkey_rating_squelch', 0),
    '#weight'        => -42,
  );

  $form['flipkey_config_fs']['flipkey_year_squelch'] = array(
    '#title'         => t('Year Squelch'),
    '#description'   => t('Set maximum years in past to show reviews for'),
    '#type'          => 'select',
    '#options' => array(0=>'No limit', 1=>'1', 2=>'2', 3=>'3', 4=>'4', 5=>'5' ),
    '#default_value'    => variable_get('flipkey_year_squelch', 0),
    '#weight'        => -41,
  );
  $form['flipkey_config_fs']['flipkey_review_filter'] = array(
    '#title'         => t('Filter Reviews by ID'),
    '#description'   => t('You can prevent individual reviews from appearing in the flipkey widget by entering review ids (one per line) of the reviews you want to filter out. If you do not know the review id you can find is the review source out put. In the source code, look for a data attribute called data-review-id wrapping the review you want to remove (e.g data-review-id="291214058")'),
    '#type'          => 'textarea',
    '#default_value'    => variable_get('flipkey_review_filter', ''),
    '#weight'        => -40,
  );


  $form['#validate'][] = 'flipkey_admin_settings_validate';
  return system_settings_form($form);
}


/**
 * flipkey_admin_settings
 * Validate handler for flipkey_admin_settings().
 */
function flipkey_admin_settings_validate($form, &$form_state) {

  if (!$form_state['values']['flipkey_api_key']) {
    form_set_error('flipkey_api_key', t('Please enter your Flipkey Account API Key.'));
  }
  if (!is_numeric($form_state['values']['flipkey_frontdesk_id'])) {
    form_set_error('flipkey_frontdesk_id', t('Please enter your numeric Flipkey Frontdesk ID'));
  }
  if (!$form_state['values']['flipkey_property_id_key']) {
    form_set_error('flipkey_property_id_key', t('Please in enter the fieldname used for the property key'));
  }
  if (count($form_state['values']['flipkey_node_types']) < 1) {
    form_set_error('flipkey_node_types', t('You must select at least one node type'));
  }
}

