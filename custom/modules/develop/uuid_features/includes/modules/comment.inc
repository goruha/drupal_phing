<?php
/**
 * @file
 * uuid_node hooks on behalf of the nodereference module.
 */

/**
 * Implements hook_uuid_features_export_render().
 */
function comment_uuid_features_export_alter(&$export, &$pipe, $data) {
  if ($data['bundle'] == 'node') {
    $uid = $data['object']->last_comment_uid;
    if ($uid != 0 && $uid != 1) {
      $uuid = entity_get_uuid_by_id('user', array($uid));
      $uuid = current($uuid);
      $pipe['uuid_user'][$uuid] = $uuid;
    }
  }
}

/**
 * Implements hook_uuid_features_export_render_alter().
 */
function comment_uuid_features_export_render_alter(&$export, $object, $data) {
  if ($data['bundle'] == 'node') {
    if ((0 != $export->last_comment_uid) && (1 != $export->last_comment_uid)) {
      $uuid = entity_get_uuid_by_id('user', array($export->last_comment_uid));
      $export->last_comment_uid = current($uuid);
    }
    unset($export->last_comment_timestamp);
    unset($export->cid);
    unset($export->comment_count);
  }
}

/**
 * Implements hook_uuid_features_rebuild_alter().
 * Replace noderef uuid's with a field array suitable for node_save().
 */
function comment_uuid_features_rebuild_alter(&$object, $data) {
  if ($data['bundle'] == 'node') {
    if ((0 != $object->last_comment_uid) && (1 != $object->last_comment_uid)) {
      $id = entity_get_id_by_uuid('user', array($object->last_comment_uid));
      $object->last_comment_uid = (int) current($id);
    }
  }
}
