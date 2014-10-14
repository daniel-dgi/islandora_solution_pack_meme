<?php

/**
 * @file
 * Drush scripts for the Islandora Meme Solution Pack.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_solution_pack_meme_drush_command() {
  $commands = array();
  $commands['islandora-solution-pack-meme-query-solr'] = array(
    'description' => dt('Queries the solr index for various memes.'),
    'examples' => array(
      'drush -u 1 islandora-solution-pack-meme-query-solr' => dt('Runs this script.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $commands;
}

/**
 * Drush command for querying the solr index.
 */
function drush_islandora_solution_pack_meme_query_solr() {
  // Solr querying code goes here.
}
