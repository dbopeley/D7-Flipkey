<?php
/**
 * This is the template file for all flipkey widgets.
 * More documentation to come.
 */

// This is stupid but if we don't have a property id param for whatever reason,
// generate a semi-unique one instead to prevent repeated IDs in the markup.
$id = isset($widget['id']) ? $widget['id'] : mt_rand(99999, 9999999);
if ($widget['no_reviews'] == false):
?>
<div id="flipkey-stars-widget-<?php print $id; ?>" class="flipkey-stars-widget">
    <div class="star-rating">
      <span class="rating-a">
        <span style="width:<?php print $widget['stats']['data']['width']; ?>px;"><?php print $widget['stats']['rating']; ?></span> of <?php print $widget['stats']['data']['max']; ?>
      </span>
    <?php if(isset($property_node_link)): ?>
      <span class="stars-widget-link"><?php print $property_node_link; ?></span>
    <?php endif; ?>
    </div>
</div>
<?php endif; ?>
