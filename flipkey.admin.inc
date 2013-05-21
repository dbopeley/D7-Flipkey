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
  		'#type'        => 'fieldset',
  		'#title'       => t('Flipkey API Settings'),
  		'#collapsible' => FALSE,
  		'#collapsed'   => FALSE
  );
  
  $form['flipkey_config_fs']['flipkey_api_key'] = array(
    '#title'            => t('API Key'),
    '#description'      => t('This is the API key for your flipkey account.'),
    '#type'             => 'textfield',
    '#required'         => TRUE,
    '#default_value'    => variable_get('flipkey_api_key', NULL),
    '#max_length'       => 75,
    '#weight'           => -50,
  );

  $form['flipkey_config_fs']['flipkey_frontdesk_id'] = array(
    '#title'            => t('Front desk ID'),
    '#description'      => t('This is the front desk ID for your flipkey account.'),
    '#type'             => 'textfield',
    '#required'         => TRUE,
    '#default_value'    => variable_get('flipkey_frontdesk_id', NULL),
    '#max_length'       => 12,
    '#weight'           => -50,
  );
    

  
  $node_types = node_type_get_types();
  $node_type_options = array();
  foreach ($node_types as $key => $value) {
  	$node_type_options[trim($key)]=trim($key);
  }
  
   
  $form['flipkey_config_fs']['flipkey_node_types'] = array(
    '#title'         => t('Flipkey node types.'),
    '#description'   => t('Select which node types flipkey should monitor for reviews.'),
    '#type'          => 'select',
    '#multiple'      => TRUE,
    '#required'      => TRUE,
    '#options'       => $node_type_options,
    '#default_value' => variable_get('flipkey_node_types',array()),
    '#weight'        => -49,
  );
  
  $form['flipkey_config_fs']['flipkey_property_id_key'] = array(
      '#title'         => t('Property ID key'),
      '#description'   => t('For the Flipkey module to properly identify properties to store reviews for, there must be a field in the node content type that stores the Flipkey property Id. In some cases this field may already be available from the reservation system\'s imported data. If that is the case then you need only identify the field here. If not already present you will need to add a field to the content type and enter the resulting field name here.'),
      '#type'          => 'textfield',
      '#default_value'    => variable_get('flipkey_property_id_key', NULL),
      '#required'      => TRUE,
      '#max_length'    => 36,
      '#weight'        => -48,
  );
  
  $form['flipkey_config_fs']['flipkey_review_key'] = array(
      '#title'         => t('Review Field key'),
      '#description'   => t('You will need to add a text field to the selected content types above to store the flipkey reviews. Once added enter the field id here. This will enable the module to correctly target that field when reviews are imported from Flipkey.'),
      '#type'          => 'textfield',
      '#default_value'    => variable_get('flipkey_review_key', NULL),
      '#required'      => TRUE,
      '#max_length'    => 36,
      '#weight'        => -47,
  );
  
    
      //Main Form settings.
  
  $form['flipkey_tools_fs'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Flipkey Tools'),
    '#description' => t('Utilities to manage your FlipKey Reviews'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  $attributes['class'] = 'update-flipkey';
  $attributes['query'] = drupal_get_destination();
  
 // $items[] = l('Update All Reviews', 'flipkey/update/all', $attributes);
  
  $lastupdated = 	variable_get('flipkey_last_updated_node', 0);
  
 // $items[] = 'Last updated node: '.$lastupdated;
  
  $form['flipkey_tools_fs']['updatelink'] = array(
  	'#type'=>'item',
    '#title'       => t('Manual Update'),
	'#markup' => ' <p>' .l('Update All Reviews', 'flipkey/update/all', $attributes).'<br />' .'Last updated node: '.$lastupdated. '</p>',

  );
  $form['#validate'][] = 'flipkey_admin_settings_validate' ;
  return system_settings_form($form);
}



/**
 * flipkey_admin_settings
 * Validate handler for flipkey_admin_settings().
*/
function flipkey_admin_settings_validate($form, &$form_state) {
//kpr($form_state);
	if(!$form_state['values']['flipkey_api_key']){
		form_set_error('flipkey_api_key', t('Please enter your Flipkey Account API Key.'));
	}
	if(!is_numeric($form_state['values']['flipkey_frontdesk_id'])){
		form_set_error('flipkey_frontdesk_id', t('Please enter your numeric Flipkey Frontdesk ID'));
	}
	if(!$form_state['values']['flipkey_property_id_key']){
		form_set_error('flipkey_property_id_key', t('Please in enter the fieldname used for the property key'));
	}
	if(count($form_state['values']['flipkey_node_types']) < 1){
		form_set_error('flipkey_node_types', t('You must select at least one node type'));
	}
}


/*
//Validate that the frontdesk_id is a numeric value only. Validate this.
function _flipkey_frontdesk_id_validate($element, &$form_state, $complete_form) {
  //If we are not numeric, then flag an error
  if (!is_numeric($element['#value'])) {
    form_error($element, t('You must enter a numeric value for the front desk key.'));
  }
}

//Validate that the api_key is a numeric value only. Validate this.
function _flipkey_api_key_validate($element, &$form_state, $complete_form) {
  //If we are not numeric, then flag an error
  if (!$element['#value']) {
    form_error($element, t('You must enter your Flipkey Account API Key.'));
  }
}
*/