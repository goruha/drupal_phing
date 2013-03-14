$conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
$conf['memcache_key_prefix'] = '${env.memcache.prefix}';
$conf['cache_default_class'] = 'MemCacheDrupal';
$conf['memcache_servers'] = array(
  'localhost:11211' => 'default',
);
