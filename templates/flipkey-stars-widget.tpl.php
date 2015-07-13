<?php
/**
 * This is the template file for all flipkey widgets.
 * More documentation to come.
 */

// This is stupid but if we don't have a property id param for whatever reason,
// generate a semi-unique one instead to prevent repeated IDs in the markup.
$id = isset($widget['id']) ? $widget['id'] : mt_rand(99999, 9999999);

?>
<div id="flipkey-widget-<?php print $id; ?>" class="flipkey-widget">
  <?php if (!isset($widget['error'])): ?>
    <div id="normal_rating">
      <p>
      <div class="average-rating-label">Review Rating</div>
      <span class="rating-a">
        <span style="width:<?php print $widget['stats']['data']['width']; ?>px;"><?php print $widget['stats']['rating']; ?></span> of <?php print $widget['stats']['data']['max']; ?>
      </span>
      </p>
    </div>
  <?php endif; ?>
</div>
