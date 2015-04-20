<?php
/**
 * @file
 * Theme functions for rezFusion Vacation Rentals.
 */


/**
 * Implements hook_preprocess_HOOK().
 *
 * @see theme_vrweb_featured_amenities()
 */
function flipkey_preprocess_flipkey_reviews(&$variables) {

  drupal_add_js(drupal_get_path('module', 'flipkey') . '/js/flipkey.js');
  drupal_add_css(drupal_get_path('module', 'flipkey') . '/css/flipkey.css');
  $review_field_key = $variables['element']['#review_field_key'];
  if (isset($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE])) {


    $variables['widget'] = json_decode($variables['element']['#entity']->{$review_field_key}[LANGUAGE_NONE][0]['value'], 1);
    // Average Overall Rating output.
    $width = 0;
    $review_count = $variables['widget']['stats']['reviewcount'];

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
          return $a['date'] - $b['date'];
        });

        if ($sort == 'ASC') {
          $review_array = $rarr;
        }
        elseif ($sort == 'DESC') { //reverse the order
          $review_array = array_reverse($rarr, FALSE);
        }

      }
      foreach ($review_array as $key => $review) {
        if (isset($review['rating'])) {

          $rating = $review['rating'];
          $data = array(
            'width' => flipkey::_calculateRating(100, $rating),
            'max' => 5,
          );
          $review['data'] = $data;
          $variables['widget']['reviews'][$key] = $review;
        }
      }
    }
    else {

      $rating = $review_array['rating'];
      $data = array(
        'width' => flipkey::_calculateRating(100, $rating),
        'max' => 5,
      );

      $variables['widget']['reviews'][0]['data'] = $data;
    }
  }
  else {
    $variables['widget']['error'] = 'There are currently no reviews available for this property.';
  }
}
