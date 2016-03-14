<?php
/**
 * @file
 * Theme functions for FLIPKEY VACATION RENTALS REVIEWS.
 */


function flipkey_preprocess_flipkey_reviews(&$variables) {
  drupal_add_css(drupal_get_path('module', 'flipkey') . '/assets/css/flipkey.css');
  $review_field_key = $variables['element']['#review_field_key'];

  if (isset($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE])) {

    $variables['widget'] = json_decode($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE][0]['value'], 1);

    // Average Overall Rating output.
    $width = 0;
    $review_count = $variables['widget']['stats']['reviewcount'];
    $review_squelch = variable_get('flipkey_rating_squelch', 0);
    $year_squelch = variable_get('flipkey_year_squelch', 0);
    $rid_filter= array();
    $review_filter = variable_get('flipkey_review_filter', '');
    //$review_filter_array = preg_split('/\r\n|[\r\n]/', $review_filter);
    if($review_filter){
      $review_filter_array = explode("\n", $review_filter);
      foreach($review_filter_array as $i => $v){
        if(strstr($v,':')){
          $sub_arr = explode(':', $v);
          $rid = $sub_arr[0];
        }else{
          $rid = $v;
        }
        $rid_filter[] = $rid;
      }
    }

    $current_year = date('Y');
    $year_limit = $current_year - $year_squelch;
    if ($variables['widget']['propstatus'] == 'active' && is_numeric($review_count)) {
      if ($review_count > 1) {
        $review_array = $variables['widget']['reviews']['review'];
        $variables['widget']['reviews'] = $review_array;
      }
      else {
        $review_array = $variables['widget']['reviews']['review'];
        $variables['widget']['reviews'] = array($review_array);
      }

      if (isset($variables['widget']['stats']['rating'])) {
        $width = flipkey::_calculateRating(123, $variables['widget']['stats']['rating']);
      }

      $variables['widget']['stats']['data'] = array(
        'width' => $width,
        'max' => 5,
      );

      if ($review_count > 1) {

        $sort = variable_get('flipkey_date_order', 'DESC');
        //Sort reviews by review date DESC

        if ($sort != 'DNO') {
          $rarr = $review_array;

          //ensure reviews are ordered by data ASC in the first place
          usort($rarr, function ($a, $b) {
            return $a['date'] - $b['date']; //Notice: Undefined index: date in {closure}() (line 51 of /Applications/MAMP/htdocs/emr/sites/all/modules/custom/flipkey/flipkey.theme.inc).
          });

          if ($sort == 'ASC') {
            $review_array = $rarr;
          }
          elseif ($sort == 'DESC') { //reverse the order
            $review_array = array_reverse($rarr, FALSE);
          }
        }

        $recalc_rating = false;

        foreach ($review_array as $key => $review) {

          $rid = $review['id'];

          if(isset($review['date'])) {
            $review_year = date('Y', $review['date']);
            if ($review_year < $year_limit && $year_squelch != 0) {
              unset($variables['widget']['reviews'][$key]);
              $recalc_rating = TRUE;
            }
            else {
              if (isset($review['rating'])) {
                $rating = $review['rating'];
                if ($rating >= $review_squelch) {
                  $data = array(
                    'width' => flipkey::_calculateRating(100, $rating),
                    'max' => 5,
                  );
                  $review['data'] = $data;
                  $variables['widget']['reviews'][$key] = $review;
                }
                else {
                  unset($variables['widget']['reviews'][$key]);
                  $recalc_rating = TRUE;
                }
              }
            }
          }

          $rid_filter = array_map('trim', $rid_filter);
          if(in_array($rid, $rid_filter)){
            unset($variables['widget']['reviews'][$key]);
            $recalc_rating = TRUE;
          }

        }

        if($recalc_rating){
          $rating_total = 0;
          $rating_count = count($variables['widget']['reviews']);
          if($rating_count > 0){
            foreach ($variables['widget']['reviews'] as $key => $review) {
              $rating_total = $rating_total + $review['rating'];
            }
            $recalulated_rating = $rating_total/$rating_count;
            $width = flipkey::_calculateRating(123, $recalulated_rating);
            $variables['widget']['stats']['data'] = array(
              'width' => $width,
              'max' => 5,
            );
          }
        }

        if(!count($variables['widget']['reviews'])){
          $variables['widget']['error'] = 'There are currently no reviews available for this property.';
        }
      }
      else {
        $rating = $review_array['rating'];
        if($rating >= $review_squelch) {
          $data = array(
            'width' => flipkey::_calculateRating(100, $rating),
            'max' => 5,
          );
          $variables['widget']['reviews'][0]['data'] = $data;
        }else{
          $variables['widget']['error'] = 'There are currently no reviews available for this property.';
        }
      }
    }else {
      $variables['widget']['error'] = 'There are currently no reviews available for this property.';
    }
  }
  else {
    $variables['widget']['error'] = 'There are currently no reviews available for this property.';
  }
  $local_reviews_enabled = false;
  $display_local_reviews = true;
  if(module_exists('trt_local_reviews')){
    $local_reviews_enabled = true;
  }
  //insert local reviews
  if($display_local_reviews && $local_reviews_enabled) {
    $nid = $variables['element']['#entity']->nid;
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'local_review')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->fieldCondition('field_local_review_property', 'target_id', $nid, '=')
      ->addMetaData('account', user_load(1)); // Run the query as user 1.

    $result = $query->execute();

    if (isset($result['node'])) {
      $local_review_nids = array_keys($result['node']);
      $local_reviews = entity_load('node', $local_review_nids);

      if(!empty($local_reviews)) {
        $local_review_arr = array();
        foreach ($local_reviews as $i => $lr) {

          $width = flipkey::_calculateRating(100, $lr->field_local_review_rating[LANGUAGE_NONE][0]['value']);
          if(isset($lr->field_local_review_managertext[LANGUAGE_NONE])){
            $mgrtext = $lr->field_local_review_managertext[LANGUAGE_NONE][0]['value'];
            if(isset($lr->field_local_review_mgrdate[LANGUAGE_NONE])){
              $mgrdate = $lr->field_local_review_mgrdate[LANGUAGE_NONE][0]['value'];
            }else{
              $mgrdate = '';
            }
          }else{
            $mgrtext = '';
            $mgrdate = '';
          }
          $lr_item = array(
            '@attributes' => array('type' => 'struct'),
            'id' => $lr->nid,
            'username' => $lr->field_local_review_username[LANGUAGE_NONE][0]['value'],
            'userlocation' => $lr->field_local_review_userlocation[LANGUAGE_NONE][0]['value'],
            'title' => $lr->title,
            'date' => $lr->field_local_review_date[LANGUAGE_NONE][0]['value'],
            'fdate' => date('m/d/Y', $lr->field_local_review_date[LANGUAGE_NONE][0]['value']),
            'text' => $lr->field_local_review_text[LANGUAGE_NONE][0]['value'],
            'displayname' => $lr->field_local_review_displayname[LANGUAGE_NONE][0]['value'],
            'rating' => $lr->field_local_review_rating[LANGUAGE_NONE][0]['value'],
            'status' => 'active',
            'mgrtext' => $mgrtext,
            'mgrdate' => $mgrdate,
            'advancedurl' => array(),
            'data' => array('width' => $width, 'max' => 5)
          );

          $local_review_arr[] = $lr_item;

        }

        if (!empty($local_review_arr)) {
          if($variables['widget']['error']){
            unset($variables['widget']['error']);
            $merged_reviews = $local_review_arr;
          }else{
            $merged_reviews = array_merge($local_review_arr, $variables['widget']['reviews']);
          }
          $sort = variable_get('flipkey_date_order', 'DESC');
          if ($sort != 'DNO') {
            $rarr = $merged_reviews;

            //ensure reviews are ordered by data ASC in the first place
            usort($rarr, function ($a, $b) {
              return $a['date'] - $b['date']; //Notice: Undefined index: date in {closure}() (line 51 of /Applications/MAMP/htdocs/emr/sites/all/modules/custom/flipkey/flipkey.theme.inc).
            });

            if ($sort == 'ASC') {
              $merged_reviews = $rarr;
            }
            elseif ($sort == 'DESC') { //reverse the order
              $merged_reviews = array_reverse($rarr, FALSE);
            }
          }

          $variables['widget']['reviews'] = $merged_reviews;
          $rating_total = 0;
          $rating_count = count($merged_reviews);
          if ($rating_count > 0) {
            foreach ($merged_reviews as $key => $review) {
              $rating_total = $rating_total + $review['rating'];
            }
            $recalulated_rating = $rating_total / $rating_count;
            $width = flipkey::_calculateRating(123, $recalulated_rating);
            $variables['widget']['stats']['data'] = array(
              'width' => $width,
              'max' => 5,
            );
          }
        }
      }
    }
  } //end local reviews
}


function flipkey_preprocess_flipkey_stars(&$variables) {

  drupal_add_css(drupal_get_path('module', 'flipkey') . '/assets/css/flipkey.css');
  $review_field_key = $variables['element']['#review_field_key'];
  $local_reviews = flipkey_local_reviews($variables['element']['#entity']->nid);
  $recalc_rating = false;

  if (isset($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE]) || $local_reviews) {
    if(isset($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE])){
      $variables['widget'] = json_decode($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE][0]['value'], 1);
      $review_count = $variables['widget']['stats']['reviewcount'];
    }


    $review_squelch = variable_get('flipkey_rating_squelch', 0);
    $year_squelch = variable_get('flipkey_year_squelch', 0);
    $current_year = date('Y');
    $year_limit = $current_year - $year_squelch;

    //initially set stars width from base rating in widget
    $variables['widget']['stats']['data'] = array(
      'width' => flipkey::_calculateRating(100, $variables['widget']['stats']['rating']),
      'max' => 5,
    );
    $variables['widget']['no_reviews'] = false;
    //next apply review squelching by year and rating and local reviews if they exist
    if (($variables['widget']['propstatus'] == 'active' && is_numeric($review_count)) || $local_reviews) {
      if ($review_count > 1) { //multiple reviews exist
        $review_array = $variables['widget']['reviews']['review'];

        //do filtering for flipkey settings
        foreach ($review_array as $key => $review) {

          //filter year squelch
          $review_year = date('Y', $review['date']);
          if($review_year < $year_limit && $year_squelch != 0){
            unset($variables['widget']['reviews'][$key]);
            $recalc_rating = true;
          }

           //move onto rating squelch
          if (isset($review['rating'])) {
            $rating = $review['rating'];
            if($rating < $review_squelch){
              unset($variables['widget']['reviews'][$key]);
              $recalc_rating = true;
            }
          }

        }
      }
      else { //only a single review for property
        $review_array = array($variables['widget']['reviews']['review']);

        $review_year = date('Y', $review_array[0]['date']);
        if($review_year < $year_limit && $year_squelch != 0){
          unset($variables['widget']['reviews'][0]);
        }

        if (isset($review_array[0]['rating'])) {
          $rating = $review_array[0]['rating'];
          if($rating < $review_squelch){
            unset($variables['widget']['reviews'][0]);
            $recalc_rating = true;
          }
        }
      }

      //if we squelched all reviews then set no review flag to true
      if(count($variables['widget']['reviews']) == 0){
        $variables['widget']['no_reviews'] = true;
      }

      //merge in local reviews if they exist
      if($local_reviews){
        if(is_null($review_array[0])){ //has local reviews but no flipkey reviews : set $review_array to local reviews
          $review_array = $local_reviews;
        }else{ //has local reviews and flipkey reviews : merge $review_array to local_reviews
          $review_array = array_merge($local_reviews, $review_array);
        }
        $recalc_rating = true; //flag to recalc rating based on modified reviews array
        $variables['widget']['stats']['reviewcount'] = count($review_array);
        $variables['widget']['reviews'] = $review_array;
        $variables['widget']['no_reviews'] = false;
      }


      if($recalc_rating){

        $rating_total = 0;
        $rating_count = count($variables['widget']['reviews']);
        if($rating_count > 0) {
          foreach ($variables['widget']['reviews'] as $key => $review) {
            $rating_total = $rating_total + $review['rating'];
          }
          $recalulated_rating = $rating_total / $rating_count;
          $width = flipkey::_calculateRating(100, $recalulated_rating);
          $variables['widget']['stats']['data'] = array(
            'width' => $width,
            'max' => 5,
          );
        }
      }
      $linkconfig =  array(
        'fragment' => 'reviews',
        'attributes' => array(
          'class' => 'review-link',
        )
      );
      $variables['property_node_link'] = l('Read Reviews('.$variables['widget']['stats']['reviewcount'].')', 'node/'.$variables['element']['#entity']->nid, $linkconfig);

    }else {
      $variables['widget']['no_reviews'] = true;
    }
  }
  else {
    $variables['widget']['no_reviews'] = true;
  }

}
