<?php

/**
 * @file
 * Content type editing UI.
 */

/**
 * Displays the content type admin overview page.
 */

function flipkey_admin_tools($form, $form_state) {

    $form['flipkey_tools'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Flipkey Management Tools'),
        '#collapsible' => FALSE,
        '#collapsed'   => FALSE
    );

    $attributes['class'] = 'update-flipkey';
    $attributes['query'] = drupal_get_destination();
    $lastupdated = 	variable_get('flipkey_last_updated_node', 0);
    $form['flipkey_tools']['updatelink'] = array(
        '#type'   =>'item',
        // '#title'  => t('Cycle Reviews'),
        '#prefix' => '<div style="border:solid 1px #000000;padding:5px;margin-bottom:10px;"><div style="font-size:10px;line-height:12px;">Clicking this link will update reviews in batches of 10 to prevent server timeouts and memory overloads. This may require multiple invocations before all Reviews have been initially imported .</div>',
        '#markup' => l('Cycle Reviews', 'flipkey/update/all', $attributes).'<br />' .'Last updated node: '.$lastupdated,
        '#suffix' => '</div>',
    );


    $ntypeskeyed = variable_get('flipkey_node_types',NULL);
    $ntypes = array_values($ntypeskeyed);
    $result = db_query("SELECT n.nid, n.title
    FROM {node} n
    WHERE n.type IN (:nt)
    ORDER BY title ASC", array(':nt' => $ntypes));

    $property_options = $result->fetchAllKeyed(0,1);
    $form['flipkey_tools']['prop_nid'] = array(
        '#type' => 'select',
        '#options' => $property_options,
        '#title' => t('Import Reviews for Individual Property'),
        '#description' => t('Enter the Node ID of the Property you want to import/update reviews for.'),
        '#default_value' => '',
        '#required' => FALSE,
        '#prefix' => '<div style="border:solid 1px #000000;padding:5px;margin-bottom:10px;">',
        '#suffix' => '</div>',
    );


    $form['flipkey_tools']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Import Reviews'),
        '#weight' => 10,
        '#submit' => array('flipkey_admin_tools_submit'),
    );

    $form['#validate'][] = 'flipkey_admin_tools_validate' ;

    return $form;

}

/**flipkeying_admin_settings
 * Submit handler for flipkey_admin_settings().
 */
function flipkey_admin_tools_validate($form, &$form_state) {

    $nid = $form_state['values']['prop_nid'];


	if($nid != ''){
        flipkey_execute_individual_update($nid);
	}else{
		drupal_set_message(t("Please enter a valid Property Node ID of the listing you wish to update or import reviews for."), 'warning');
	}
}


/**flipkeying_admin_settings
 * Submit handler for linlist_admin_settings().
 */
function flipkey_admin_tools_submit($form, &$form_state) {


}
