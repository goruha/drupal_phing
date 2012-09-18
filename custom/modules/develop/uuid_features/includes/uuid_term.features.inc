<?php
/**
 * @file
 * Features hooks for the uuid_term features component.
 */

/**
 * Get term id from its uuid
 * @param string $uuid
 */
function uuid_taxonomy_term_find($uuid) {
  $ids = entity_get_id_by_uuid('taxonomy_term', array($uuid));
  return array_key_exists($uuid, $ids) ? $ids[$uuid] : FALSE;
}

/**
 * Implements hook_features_export_options().
 */
function uuid_term_features_export_options() {
  $options = array();

  $query = 'SELECT t.tid, t.name, v.name AS vname, t.uuid
    FROM {taxonomy_term_data} t
    INNER JOIN {taxonomy_vocabulary} v ON t.vid = v.vid
    ORDER BY v.name ASC, t.name ASC';
  $results = db_query($query);
  foreach ($results as $term) {
    $options[$term->uuid] = $term->vname . ' - ' . $term->name;
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_term_features_export($data, &$export, $module_name = '') {
  uuid_features_load_module_includes();
  $pipe = array();

  $export['dependencies']['taxonomy'] = 'taxonomy';
  $export['dependencies']['uuid'] = 'uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';
  foreach ($data as $uuid) {
    _uuid_term_feature_export_term($export, $uuid, $module_name, $pipe);

    // Get the machine name of the term's vocabulary.
    $voc_machine_name = db_query('SELECT v.machine_name FROM {taxonomy_vocabulary} v
      INNER JOIN {taxonomy_term_data} t ON t.vid = v.vid
      WHERE t.uuid = :uuid', array(':uuid' => $uuid))->fetchField();

    // Add the vocab to the pipe
    $pipe['features']['taxonomy'][$voc_machine_name] = $voc_machine_name;

    // Get the UUID of the term's parents.
    $tid = uuid_taxonomy_term_find($uuid);
    $parents = taxonomy_get_parents($tid);
    foreach ($parents as $parent) {
      _uuid_term_feature_export_term($export, $parent->uuid, $module_name, $pipe);
    }
  }

  return $pipe;
}


/**
 * Helper function to export term with dependence in case
 * other module allready provide it.
 *
 * @param array $export
 *   Export array
 * @param string $uuid
 *   UUID for the exported term
 * @param string $module_name
 *  Module name runing export
 */
function _uuid_term_feature_export_term(&$export, $uuid, $module_name, $pipe) {
  $map = features_get_component_map('uuid_term');
  if (isset($map[$uuid]) && !in_array($module_name, $map[$uuid])) {
    $module = current($map[$uuid]);
    $export['dependencies'][$module] = $module;
  }
  // Otherwise, export the uuid.
  else {
    $export['features']['uuid_term'][$uuid] = $uuid;
  }
  $tid = uuid_taxonomy_term_find($uuid);
  $term = taxonomy_term_load($tid);
  $data = array(
    'object' => $term,
    'bundle' => 'taxonomy_term',
    'type' => $term->vocabulary_machine_name,
    'module' => $module_name,
  );
  drupal_alter('uuid_features_export', $export, $pipe, $data);
}

/**
 * Implements hook_features_export_render().
 */
function uuid_term_features_export_render($module = 'foo', $data) {
  uuid_features_load_module_includes();
  $translatables = $code = array();

  $code[] = '  $terms = array();';
  $code[] = '';
  sort($data);
  foreach ($data as $uuid) {
    $tid = uuid_taxonomy_term_find($uuid);
    $term = taxonomy_term_load($tid);
    if (!$term) {
      continue;
    }

    // Blacklist of term properties.
    $blacklist = array('vid', 'tid', 'vocabulary_machine_name');
    $export = (object) clone $term;
    foreach ($blacklist as $key) {
      unset($export->$key);
    }

    $export->vocabulary = $term->vocabulary_machine_name;

    // Get the UUID of the term's parent terms.
    $parents = taxonomy_get_parents($tid);
    foreach ($parents as $parent) {
      $export->parent[] = $parent->uuid;
    }
    $data = array(
      'module' => $module,
      'bundle' => 'taxonomy_term',
      'type' => $export->vocabulary,
    );
    drupal_alter('uuid_features_export_render', $export, $term, $module, $data);
    $code[] = '  $terms[] = ' . features_var_export($export, '  ') . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $terms;';
  $code = implode("\n", $code);
  return array('uuid_features_default_terms' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_term_features_revert($module) {
  uuid_term_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds terms based on UUID from code defaults.
 */
function uuid_term_features_rebuild($module) {
  uuid_features_load_module_includes();
  $terms = module_invoke($module, 'uuid_features_default_terms');
  if (!empty($terms)) {
    foreach ($terms as $data) {
      $existing = uuid_taxonomy_term_find($data['uuid']);

      if (!empty($existing)) {
        $data['tid'] = $existing;
      }

      $vid = db_query('SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = :machine_name', array(':machine_name' => $data['vocabulary']))->fetchField();
      if ($vid) {
        $data['vid'] = $vid;

        if (isset($data['parent']) && is_array($data['parent'])) {
          foreach ($data['parent'] as $key => $parent_uuid) {
            $data['parent'][$key] = uuid_taxonomy_term_find($parent_uuid);
          }
        }

        $term = (object) $data;
        $data = array(
          'module' => $module,
          'bundle' => 'taxonomy_term',
          'type' => $term->vocabulary_machine_name,
        );
        drupal_alter('uuid_features_rebuild', $term, $data);
        $ret = taxonomy_term_save($term);
      }

    }
  }
}
