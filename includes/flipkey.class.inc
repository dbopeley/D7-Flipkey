<?php
/**
 * @name blank
 * @param string $path = the module's internal path, as defined in our .module file
 * This is the default module class, this just helps us keep our items separated from Drupal
 * and keeps the module hooks in the .module file so that we know our work is as separated from Drupal's hooks
 * @todo - add any more path variables we need here.
 */
class flipkey {
  
  //Build any object references we need here - when we include this file, PHP will looks for the construct and give us this object automatically.
  function __construct() {
    //Constructor functions.
  }
  
  //When we are done with our object, or do not reference it anymore PHP will automatically try and kill it off from memory if we have this set.
  function __destruct() {
    //Unset from memory.
    unset($this);
  }

  /**
   * @name _getFlipKeyReview
   * Queries the feeds for a valid XML source, and then brings back the results for us to use.
   * @param string $property_id = the remote id or property id you are using to query flipkey. Please see flipkey if you need more information on how widgets are assembled.
   * @param string $template = If you want to specify additional templates, for differing reviews you are creating, then you can create a different template using the pattern flipkey_widget__%template
   * @return string the themed output. You can preprocess this function too!
   */
  public static function _getFlipkeyReview($prid = NULL, $pnid = NULL) {
    //If we have a connection to the datasource, then we will return results.


if(is_array($prid)){
  if($prid['und'][0]['value']){
    $prid = $prid['und'][0]['value'];
  }
}


 	  	$reviewstring = db_query("SELECT flipkey_review_xml FROM {flipkey_reviews} WHERE flipkey_review_prid = :frp AND flipkey_review_pnid = :frn", array(':frp' => $prid,':frn' => $pnid ))->fetchField();
		
 	  //	kpr($reviewstring);
 	  	$reviewxml = simplexml_load_string($reviewstring);
		//kpr($reviewxml);
		
		//kpr($reviewxml);exit;
		
      //Parse our cleaned up XML.
      //$xml = @simplexml_load_string($response->data);
      $widget['reviews'] = array();
      $widget['stats'] = array();
      //kpr(isset($xml->err));
      if (is_object($reviewxml) && $reviewxml->children() && !isset($reviewxml->err)) {
        //Get the root pieces to put in our review and reset.
        $widget = array(
          'propertyid' => strtolower($prid),
          'propname'   => t('@str', array('@str' => (string) $reviewxml->propname)),
          'propstatus' => t('@str', array('@str' => (string) $reviewxml->propstatus)),
          'proplink'   => (string) $reviewxml->links->proplink,
        );
        //Overall Property Stats.
        if ($reviewxml->stats->children()) {
          foreach ($reviewxml->stats->children() as $key => $value) {
            $widget['stats'][$key] = (float) $value;
          }
        }
        //Reviews.
        if ($reviewxml->reviews->children()) {
          foreach ($reviewxml->reviews->children() as $key => $value) {
            $id = (int) $value->id;
            foreach ($value->children() as $piece => $item) {
              $item = (string) $item;
              $widget['reviews'][$id][$piece] = decode_entities($item);
            }
          }
        }
      } else {
        //If we have an error.
        $widget['error'] = t('Sorry, there are no reviews for this property.');
      }
      $widget['#theme'] = 'render_flipkey_widget';
      return $widget;
      //return theme(array($template,'flipkey_widget'), $widget);
    
  }
  
  
   /**
   * @name _findFlipkeyReview
   * Queries the feeds for a valid XML source, and then brings back the results for us to use.
   * @param string $property_id = the remote id or property id you are using to query flipkey. Please see flipkey if you need more information on how widgets are assembled.
   * @param string $template = If you want to specify additional templates, for differing reviews you are creating, then you can create a different template using the pattern flipkey_widget__%template
   * @return string the themed output. You can preprocess this function too!
   */ 
  public static function _findFlipkeyReview($pnid,$prid) {
  
		$erid = db_query("SELECT flipkey_review_id FROM {flipkey_reviews} WHERE flipkey_review_prid = :frp AND flipkey_review_pnid = :frn", array(':frp' => $prid,':frn' => $pnid ))->fetchField();
		return $erid;
  }

  /**
   * @name _getFlipKeyReview
   * Queries the feeds for a valid XML source, and then brings back the results for us to use.
   * @param string $property_id = the remote id or property id you are using to query flipkey. Please see flipkey if you need more information on how widgets are assembled.
   * @param string $template = If you want to specify additional templates, for differing reviews you are creating, then you can create a different template using the pattern flipkey_widget__%template
   * @return string the themed output. You can preprocess this function too!
   */
  public static function _importFlipkeyReview($import_cfg) {
    //If we have a connection to the datasource, then we will return results.
    $ffdid = trim(variable_get('flipkey_frontdesk_id',NULL));
    $fapik = trim(variable_get('flipkey_api_key',NULL));
    
    $prid = $import_cfg['prid'];
    $pnid = $import_cfg['pnid'];

    $update = array();
    
    //kpr($import_cfg); exit;
    
    if($ffdid && $fapik){
    	$file = 'http://'.FLIPKEY_BASE_URL_FEEDS .'/'.$fapik.'/'.$ffdid.'/'.$prid.'/';
    	
    	//kpr($file);exit;
    	$headers = array(
    			'Content-Type' => 'text/xml',
    			'Accept-Charset' => 'utf-8'
    		);
    	$response = drupal_http_request($file, $headers, $method = 'GET', $data = NULL, $retry = 0);
    	$response->data = preg_replace('/&/msU', '&amp;', $response->data);
    	$response->data = preg_replace('/[^(\x20-\x7F)]*/','', $response->data);

    	if ($response->data && $response->code == '200') {
      	//Parse our cleaned up XML.
				$reviewstring = $response->data;
				echo $reviewstring;
				 $reviewxml = @simplexml_load_string($response->data,'SimpleXMLElement', LIBXML_NOCDATA);
				//echo $reviewxml;
      	
      	if (!isset($reviewxml->err)) {
	      	if (is_object($reviewxml) && $reviewxml->children()) {
						//cast xml as an array and return
						$reviewarray = json_decode(json_encode((array) $reviewxml), 1);

						return $reviewarray;
			    }else{
		        $update['error'] = t('No Reviews');
		    	}
	    	}else{
	        $update['error'] = t('Response Error');
	    	}
   		}else{	
   			$update['error'] = t('Request Error');
    	}
    	
  	}else{
  		$update['error'] = t('Configuration Error');
  	}
  	
  	return $update;
  }

  /**
   * @name _calculateRating()
   * Allows us to calculate the width for the star ratings, so they can be easily styled.
   * @param integer $width = the width of the image you are making a star rating from.
   *  rating-default.png = 123px X 34px
   *  rating-default-small.png = 100px X 43px
   * @param integer $rating = the rating value you are evaluating.
   * @param integer $max = the maximum rating you can accept.
   * @return integer $width = the calculated width for the stars, so the rating populates properly like flipkey.
   */
  public static function _calculateRating($width = NULL, $rating = NULL, $max = 5) {
    if ($width && $rating) {
      //If we have a width and a rating to evaulate, then we can return a calculated result.
      if ($rating >= $max) {
        //If the rating is greater than the max, then we always give the width back.
        return $width;
      } else if ($rating <= 0) {
        //Else if we have a rating that is zero or less, then return no stars.
        return 0;
      } else {
        //If we are a rating that needs to be calculated, then calculate me!
        //First, find the percentage of the rating in relation to the max.
        $percent = ($rating/$max);
        //Second, find out the percentage width from the original, based on the $percentage variable.
        return round($width*$percent);
      }
    }
  }

  /**
   * @name _loadJS
   * Allows us to load the javascript files for the module anywhere in the module when we need it.
   */
  public static function _loadJS() {
    drupal_add_js(FLIPKEY_MODULE_PATH. '/js/jquery.module.js','module','header');
  }

  /**
   * @name _loadCSS
   * Allows us to load the css files for the module anywhere in the module when we need it.
   */
  public static function _loadCSS() {
    drupal_add_css(FLIPKEY_MODULE_PATH. '/css/flipkey-styles.css','module');
  }
  
}