<?php
/**
 * @file
 * Features hooks for the uuid_node features component.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_node_features_export_options() {
  $options = array();

  $types = node_type_get_names();

  $query = 'SELECT COUNT(n.nid), n.title, n.type, n.uuid
    FROM {node} n ORDER BY n.type, n.title ASC';
  $results = db_query($query);
  if ($results->fetchField() <= 500) {
    $query = 'SELECT n.nid, n.title, n.type, n.uuid
    FROM {node} n ORDER BY n.type, n.title ASC';
    $results = db_query($query);
    foreach ($results as $node) {
      $options[$node->uuid] = t('@type: @title', array(
        '@type'  => $types[$node->type],
        '@title' => $node->title,
      ));
    }
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_node_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['uuid_features'] = 'uuid_features';

  uuid_features_load_module_includes();
  $map = features_get_component_map('uuid_node');

  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Load the existing node, with a fresh cache.
    $node = node_load($nid, NULL, TRUE);
    if (isset($map[$uuid]) && !in_array($module_name, $map[$uuid])) {
      $module = current($map[$uuid]);
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the uuid.
    else {
      $export['features']['uuid_node'][$uuid] = $uuid;
      $pipe['node'][$node->type] = $node->type;
    }
    if (0 != $node->uid && 1 != $node->uid) {
      $uuid = current(entity_get_uuid_by_id('user', array($node->uid)));
      $pipe['uuid_user'][$uuid] = $uuid;
    }
    drupal_alter('uuid_node_features_export', $export, $pipe, $node);
    $data = array(
      'object' => $node,
      'bundle' => 'node',
      'type' => $node->type,
      'module' => $module_name,
    );
    drupal_alter('uuid_features_export', $export, $pipe, $data);
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_node_features_export_render($module, $data) {
  $translatables = $code = array();

  uuid_features_load_module_includes();

  $code[] = '  $nodes = array();';
  $code[] = '';
  $nids = entity_get_id_by_uuid('node', $data);
  foreach ($nids as $uuid => $nid) {
    // Only export the node if it exists.
    if ($nid === FALSE) {
      continue;
    }
    // Attempt to load the node, using a fresh cache.
    $node = node_load($nid, NULL, TRUE);
    if (empty($node)) {
      continue;
    }
    if (!empty($node->path)) {
      $node->pathauto_perform_alias = FALSE;
    }
    $export = clone $node;

    // Use date instead of created,
    // in the same format used by node_object_prepare.
    $export->date = format_date($export->created, 'custom', 'Y-m-d H:i:s O');

    // Don't cause conflicts with nid/vid/revision_timestamp/changed fields.
    unset($export->nid);
    unset($export->vid);
    unset($export->revision_uid);
    unset($export->revision_timestamp);
    unset($export->changed);
    unset($export->name);

    if ($export->uid != 0 && $export->uid != 1) {
      $uuid = entity_get_uuid_by_id('user', array($export->uid));
      $export->uid = current($uuid);
    }

    // The hook_alter signature is:
    // hook_uuid_node_features_export_render_alter(&$export, $node, $module);
    drupal_alter('uuid_node_features_export_render', $export, $node, $module);
    $data = array(
      'module' => $module,
      'bundle' => 'node',
      'type' => $node->type,
    );
    drupal_alter('uuid_features_export_render', $export, $node, $data);

    $code[] = '  $nodes[] = ' . features_var_export($export) . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $nodes;';
  $code = implode("\n", $code);
  return array('uuid_features_default_content' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_node_features_revert($module) {
  uuid_node_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds nodes based on UUID from code defaults.
 */
function uuid_node_features_rebuild($module) {
  $nodes = module_invoke($module, 'uuid_features_default_content');
  if (!empty($nodes)) {
    module_load_include('inc', 'node', 'node.pages');

    foreach ($nodes as $data) {
      $node = (object) $data;
      if ((0 != $node->uid) && (1 != $node->uid)) {
        $id = entity_get_id_by_uuid('user', array($node->uid));
        $id = current($id);
      }
      else {
        $id = $node->uid;
      }

      node_object_prepare($node);
      $node->uid = (int) $id;
      // Find the matching UUID, with a fresh cache.
      $nids = entity_get_id_by_uuid('node', array($node->uuid));
      $nid = array_key_exists($node->uuid, $nids) ? $nids[$node->uuid] : FALSE;
      if (!empty($nid)) {
        $existing = node_load($nid, NULL, TRUE);
        if (!empty($existing)) {
          $node->nid = $existing->nid;
          $node->vid = $existing->vid;
        }
        else {
          $node->is_new = TRUE;
        }
      }

      // The hook_alter signature is:
      // hook_uuid_node_features_rebuild_alter(&node, $module);
      drupal_alter('uuid_node_features_rebuild', $node, $module);
      $data = array(
        'module' => $module,
        'bundle' => 'node',
        'type' => $node->type,
      );
      drupal_alter('uuid_features_rebuild', $node, $data);
      $node = node_submit($node);

      node_save($node);
    }
  }
}
