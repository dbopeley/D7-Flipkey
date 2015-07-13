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
}


function flipkey_preprocess_flipkey_stars(&$variables) {
  drupal_add_css(drupal_get_path('module', 'flipkey') . '/assets/css/flipkey.css');
  $review_field_key = $variables['element']['#review_field_key'];

  if (isset($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE])) {

    $variables['widget'] = json_decode($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE][0]['value'], 1);
    // Average Overall Rating output.
    $width = 0;
    $review_count = $variables['widget']['stats']['reviewcount'];
    $review_squelch = variable_get('flipkey_rating_squelch', 0);
    $year_squelch = variable_get('flipkey_year_squelch', 0);
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
        $width = flipkey::_calculateRating(100, $variables['widget']['stats']['rating']);
      }

      $variables['widget']['stats']['data'] = array(
        'width' => $width,
        'max' => 5,
      );

      if ($review_count > 1) {

        $recalc_rating = false;
        foreach ($review_array as $key => $review) {
          $review_year = date('Y', $review['date']); //need if
          if($review_year < $year_limit && $year_squelch != 0){
            unset($variables['widget']['reviews'][$key]);
            $recalc_rating = true;
          }else{
            if (isset($review['rating'])) {
              $rating = $review['rating'];
              if($rating >= $review_squelch){
                $data = array(
                  'width' => flipkey::_calculateRating(100, $rating),
                  'max' => 5,
                );
                $review['data'] = $data;
                $variables['widget']['reviews'][$key] = $review;
              }else{
                unset($variables['widget']['reviews'][$key]);
                $recalc_rating = true;
              }
            }
          }
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
}
