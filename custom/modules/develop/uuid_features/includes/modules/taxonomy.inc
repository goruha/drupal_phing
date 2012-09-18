<?php
/**
 * @file
 * uuid_node hooks on behalf of the nodereference module.
 */

/**
 * Implements hook_uuid_features_export_render().
 */
function taxonomy_uuid_features_export_alter(&$export, &$pipe, $data) {
  $data = (array) $data;
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'taxonomy_term_reference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($data['object']->$field_name as $lang => $values) {
        foreach ($values as $value) {
          if (!empty($value['tid'])) {
            $target_type = 'taxonomy_term';
            $uuid = entity_get_uuid_by_id($target_type, array($value['tid']));
            if (empty($uuid)) {
              uuid_sync_all();
              $uuid = entity_get_uuid_by_id($target_type, array($value['tid']));
            }
            $uuid = current($uuid);
            $pipe["uuid_term"][$uuid] = $uuid;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_features_export_render_alter().
 */
function taxonomy_uuid_features_export_render_alter(&$export, $object, $data) {
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'taxonomy_term_reference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['tid'])) {
            $uuid = entity_get_uuid_by_id('taxonomy_term', array($value['tid']));
            $uuid = !empty($uuid) ? current($uuid) : 0;
            $export->{$field_name}[$lang][$delta]['tid'] = $uuid;
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
function taxonomy_uuid_features_rebuild_alter(&$object, $data) {
  $entityreference_fields = _uuid_features_get_fields($data['bundle'], $data['type'], 'taxonomy_term_reference');
  if (!empty($entityreference_fields)) {
    foreach ($entityreference_fields as $field_name => $field) {
      foreach ($object->$field_name as $lang => $data) {
        foreach ($data as $delta => $value) {
          if (!empty($value['tid'])) {
            $uuid = entity_get_id_by_uuid('taxonomy_term', array($value['tid']));
            if (!empty($uuid)) {
              $uuid = current($uuid);
              $object->{$field_name}[$lang][$delta]['tid'] = $uuid;
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
