<?php
/**
 * @file
 * uuid_node hooks on behalf of the nodereference module.
 */

/**
 * Implements hook_uuid_node_features_export_render().
 */
function entityreference_uuid_features_export_alter(&$export, &$pipe, $data) {
  $data = (array) $data;
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'entityreference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($data['object']->$field_name as $lang => $values) {
        foreach ($values as $value) {
          if (!empty($value['target_id'])) {
            $target_type = $field['settings']['target_type'];
            $uuid = entity_get_uuid_by_id($target_type, array($value['target_id']));
            if (empty($uuid)) {
              uuid_sync_all();
              $uuid = entity_get_uuid_by_id($target_type, array($value['target_id']));
            }
            $uuid = current($uuid);
            $pipe["uuid_$target_type"][$uuid] = $uuid;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_features_export_render_alter().
 */
function entityreference_uuid_features_export_render_alter(&$export, $object, $data) {
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'entityreference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['target_id'])) {
            $uuid = entity_get_uuid_by_id($field['settings']['target_type'], array($value['target_id']));
            $uuid = current($uuid);
            $export->{$field_name}[$lang][$delta]['target_id'] = $uuid;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_features_rebuild_alter().
 * Replace noderef uuid's with a field array suitable for node_save().
 */
function entityreference_uuid_features_rebuild_alter(&$object, $data) {
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'entityreference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['target_id'])) {
            $uuid = entity_get_id_by_uuid($field['settings']['target_type'], array($value['target_id']));
            $uuid = !empty($uuid) ? current($uuid) : 0;
            $object->{$field_name}[$lang][$delta]['target_id'] = $uuid;
          }
        }
      }
    }
  }
}
