<?php
/**
 * @file
 * uuid_node hooks on behalf of the filefield module.
 */

/**
 * Implements hook_uuid_node_features_export_alter().
 */
function file_uuid_features_export_alter(&$export, &$pipe, $data) {
  $data = (array) $data;
  $fields = _file_uuid_fields($data);
  if (!empty($fields)) {
    foreach ($fields as $field_name => $field) {
      foreach ($data['object']->$field_name as $lang => $values) {
        foreach ($values as $value) {
          if (!empty($value['fid'])) {
            $uuid = $value['uuid'];
            $map = features_get_component_map('uuid_file');
            if (isset($map[$uuid]) && !in_array($data['module'], $map[$uuid])) {
              $module = current($map[$uuid]);
              $export['dependencies'][$module] = $module;
            }
            // Otherwise, export the uuid.
            else {
              $export['features']['uuid_file'][$uuid] = $uuid;
            }
          }
        }
      }
    }
  }
}

/**
 * Get file fields
 *
 * @param array $data
 *   Data
 *
 * @return array
 *   Fields list
 */
function _file_uuid_fields($data) {
  $fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'file');
  $fields += _uuid_features_get_fields($data['bundle'], $data['type'], 'image');
  return $fields;
}

/**
 * Implements hook_uuid_features_export_render_alter().
 */
function file_uuid_features_export_render_alter(&$export, $object, $data) {
  $fields = _file_uuid_fields($data);
  if (!empty($fields)) {
    foreach ($fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['fid'])) {
            $export->{$field_name}[$lang][$delta] = $value;
            $file = file_load($value['fid']);
            foreach ((array) $file as $key => $tmp) {
              if ($key != 'uuid') {
                unset($export->{$field_name}[$lang][$delta][$key]);
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_node_features_rebuild_alter().
 * Replace filefield uuid's with a field array suitable for node_save().
 */
function file_uuid_features_rebuild_alter(&$object, $data) {
  $fields = _file_uuid_fields($data);
  if (!empty($fields)) {
    foreach ($fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['uuid'])) {
            $id = entity_get_id_by_uuid('file', array($value['uuid']));
            if (!empty($id) && $file = file_load(current($id))) {
              $object->{$field_name}[$lang][$delta] += (array) $file;
            }
            else {
              unset($object->{$field_name}[$lang][$delta]);
            }
          }
        }
        if (empty($object->{$field_name}[$lang])) {
          unset($object->{$field_name}[$lang]);
        }
      }
      if (empty($object->{$field_name})) {
        unset($object->{$field_name});
      }
    }
  }
}
