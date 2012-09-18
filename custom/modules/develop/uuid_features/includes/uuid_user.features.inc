<?php
/**
 * @file
 * Features hooks for the uuid_user features component.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_user_features_export_options() {
  $options = array();
  $query = db_select('users', 'u');
  $query->condition('u.uid', 0, '<>');
  $query->condition('u.uid', 1, '<>');
  $query->fields('u', array('uid', 'name', 'uuid'));

  $count = $query->countQuery();
  $result = $count->execute();
  if ($result->fetchField() <= 500) {
    $result = $query->execute();
    foreach ($result as $user) {
      $options[$user->uuid] = $user->name;
    }
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_user_features_export($data, &$export, $module_name = '') {
  uuid_features_load_module_includes();
  $pipe = array();
  $export['dependencies']['user'] = 'user';
  $export['dependencies']['uuid'] = 'uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';
  $map = features_get_component_map('uuid_user');
  foreach ($data as $uuid) {
    if (isset($map[$uuid]) && !in_array($module_name, $map[$uuid])) {
      $module = current($map[$uuid]);
      $export['dependencies'][$module] = $module;
    }
    // Otherwise, export the uuid.
    else {
      $export['features']['uuid_user'][$uuid] = $uuid;
    }
    $uids = entity_get_id_by_uuid('user', array($uuid));
    $uid = current($uids);
    $user = user_load($uid, TRUE);
    $alter_data = array(
      'object' => $user,
      'bundle' => 'user',
      'type' => 'user',
      'module' => $module_name,
    );
    drupal_alter('uuid_features_export', $export, $pipe, $alter_data);
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_user_features_export_render($module, $data) {
  uuid_features_load_module_includes();
  $code = array();

  $code[] = '  $users = array();';
  $code[] = '';
  $uids = entity_get_id_by_uuid('user', $data);
  foreach ($uids as $uid) {
    $user = user_load($uid);
    // Blacklist of term properties.
    $blacklist = array('uid', 'created', 'login', 'access', 'data');
    $export = (object) $user;
    $export = clone $export;
    foreach ($blacklist as $key) {
      unset($export->$key);
    }
    $alter_data = array(
      'module' => $module,
      'bundle' => 'user',
      'type' => 'user',
    );
    drupal_alter('uuid_features_export_render', $export, $user, $alter_data);
    $code[] = '  $users[] = ' . features_var_export($export, '  ') . ';';
  }

  $code[] = '  return $users;';
  $code = implode("\n", $code);
  return array('uuid_features_default_users' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_user_features_revert($module) {
  uuid_user_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds terms based on UUID from code defaults.
 */
function uuid_user_features_rebuild($module) {
  uuid_features_load_module_includes();
  $users = module_invoke($module, 'uuid_features_default_users');
  if (!empty($users)) {
    // we get all id's
    $uuids = array();
    foreach ($users as $user) {
      $uuids[] = $user['uuid'];
    }

    // get arrays like uuid => user
    $existing_users = array();
    foreach (entity_uuid_load('user', $uuids, array(), TRUE) as $existing_user) {
      $existing_users[$existing_user->uuid] = $existing_user;
    }

    foreach ($users as $data) {
      $existing_user = isset($existing_users[$data['uuid']]) ? $existing_users[$data['uuid']] : new stdClass();
      foreach ($data as $key => $value) {
        $existing_user->{$key} = $value;
      }
      $data = array(
        'module' => $module,
        'bundle' => 'user',
        'type' => 'user',
      );
      drupal_alter('uuid_features_rebuild', $existing_user, $data);
      entity_uuid_save('user', $existing_user);
    }
  }
}
