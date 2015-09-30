<?php
/**
 * @file
 * Drush command/hook implementation for replacing datastreams.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_datastream_replace_drush_command() {
  $commands = array();

  $commands['islandora_datastream_replace'] = array(
    'description' => dt('Replaces a datastream in all objects given a file list in a directory.'),
    'options' => array(
      'namespace' => array(
        'description' => dt('The namespace of the pids.'),
        'required' => TRUE,
      ),
      'source' => array(
        'description' => dt('The directory to get the datastreams and pid# from.'),
        'required' => TRUE,
      ),
    ),
    'aliases' => array('idre'),
    'drupal dependencies' => array(
      'islandora',
    ),
    'examples' => array(
      'drush -u 1 islandora_datastream_replace --source=/tmp --namespace=test2 ' => dt('Replacing MODS datastream in object.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $commands;
}

/**
 * Batch process validation handler.
 */
function drush_islandora_datastream_replace_islandora_datastream_replace_validate() {
  $directory = drush_get_option('source');
  if (!is_writable($directory)) {
    return drush_set_error('This is not is a directory', dt('The specified source directory, !dir, is not valid.', array('!dir' => $directory)));
  }
}

/**
 * Command callback to replace datastreams.
 */
function drush_islandora_datastream_replace() {
  batch_set(islandora_datastream_replace_create_batch());
  drush_backend_batch_process();
}

/**
 * Constructs a batch used to update things via Drush.
 */
function islandora_datastream_replace_create_batch() {
  return array(
    'operations' => array(
      array('islandora_datastream_replace_batch_operation',
        array(
          drush_get_option('namespace'),
          drush_get_option('source'),
        ),
      ),
    ),
    'title' => t('Importing datastreams...'),
    'init_message' => t('Preparing to import datastream.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_datastream_replace') . '/islandora_datastream_replace.drush.inc',
  );
}


/**
 * Constructs and performs the datastream replace operation.
 *
 * @param string $namespace
 *   The namespace to work in.
 * @param string $source
 *   The source directory to read the files from.
 * @param array $context
 *   The context of the Drupal batch.
 */
function islandora_datastream_replace_batch_operation($namespace, $source, &$context) {
  $sandbox = &$context['sandbox'];
  //  query of source directory
  //
  $files = scandir($source);
  $nspart=$namespace.'_';
  //count expected files in this directory for sandbox
  $lscommand="ls -1 $source/$nspart*.xml | wc -l";
  $numfiles=exec($lscommand);
  $dsid='MODS';
  // loop through files
  foreach ($files as $fil) {
    // work with xml files only
    if ((substr($fil,-4,4)=='.xml')&&(strstr($fil,$nspart))) {
      // get name without extension
      $xbase=basename($fil,'.xml');
      // get pieces of filename
      $xparts=explode('_',$xbase); 
      //find namespace equivalent
      $nstest=$xparts[0];
      print "\n nstest=$nstest \n";
      //get number part of pid 
      $pidnum=$xparts[1];
      print "pidnum=$pidnum /n";
      if ($nstest!=$namespace) {
        print " bad namespace \n";
        exit();
      }
      // assemble filename that is like pid
      $pidname=$nstest.'_'.$pidnum;
      //  assemble pid from filename parts
      $pid=$nstest.':'.$pidnum;      
      // prepare tuque and islandora includes
      module_load_include('inc', 'islandora', 'includes/mimetype.utils');
      module_load_include('inc', 'islandora', 'utilities');
      $object = islandora_object_load($pid);
      if (isset($object[$dsid])) {
        // assemble current source filename
        $source_directory = $source.'/'.$pidname.'.xml';
        $object[$dsid]->setContentFromFile($source_directory);
        $success = TRUE;
      }
      else {
        $success = FALSE;
      }
      if ($success) {
        drush_log(dt("Datastream replacement succeeded for @pid. ", array('@pid' => $object->id, )), 'success');
      }
      else {
        drush_log(dt("Datastream replacement failed for @pid. ", array('@pid' => $object->id, )), 'error');
      }
    }// end if right file
  }// end foreach
}// end function
