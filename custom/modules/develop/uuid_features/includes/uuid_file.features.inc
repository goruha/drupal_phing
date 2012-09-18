<?php
/**
 * @file
 * Features hooks for the uuid_file features component.
 */

function uuid_file_features_export_options() {
  $options = array();

  $query = 'SELECT f.fid, f.filename, f.uuid
    FROM {file_managed} f ORDER BY f.filename ASC';
  $results = db_query($query);
  foreach ($results as $file) {
    $options[$file->uuid] = t('File:  @title', array(
      '@title' => $file->filename,
    ));
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_file_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['uuid_features'] = 'uuid_features';
  $map = features_get_component_map('uuid_file');
  foreach ($data as $uuid) {
    if (isset($map[$uuid]) && !in_array($module_name, $map[$uuid])) {
      $module = current($map[$uuid]);
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the uuid.
    else {
      $export['features']['uuid_file'][$uuid] = $uuid;
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_file_features_export_render($module, $data) {
  $translatables = $code = array();

  $code[] = '  $files = array();';
  $code[] = '';
  $fids = entity_get_id_by_uuid('file', $data);
  foreach ($fids as $fid) {
    $file = file_load($fid);
    $export = $file;
    // Blacklist of term properties.
    $blacklist = array('uid', 'fid', 'timestamp', 'filesize');
    foreach ($blacklist as $key) {
      unset($export->$key);
    }
    $real_path = drupal_realpath($file->uri);
    $export->data = file_exists($real_path) ? base64_encode(file_get_contents($real_path)) : '';
    $code[] = '  $files[] = ' . features_var_export($export, '  ') . ';';
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $files;';
  $code = implode("\n", $code);
  return array('uuid_features_default_files' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_file_features_revert($module) {
  uuid_file_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds files based on UUID from code defaults.
 */
function uuid_file_features_rebuild($module) {
  $files = module_invoke($module, 'uuid_features_default_files');
  if (!empty($files)) {
    foreach ($files as $data) {
      $content = base64_decode($data['data']);
      $path = dirname($data['uri']);
      if (!file_exists($path)) {
        drupal_mkdir($path, NULL, TRUE);
      }
      file_unmanaged_save_data($content, $data['uri'], FILE_EXISTS_REPLACE);
      unset($data['data']);

      $existed_file = entity_get_id_by_uuid('file', array($data['uuid']));
      $existed_file = current($existed_file);
      if (!empty($existed_file)) {
        $data['fid'] = $existed_file;
      }
      $data = (object) $data;
      // Copy the file and save to db.
      file_save($data);
    }
  }
}
