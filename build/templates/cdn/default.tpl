$conf['cdn_status'] = 2;
$conf['cdn_mode'] = 'basic';
$conf['cdn_basic_mapping'] = <<<EOL
http://static.$cookie_domain/|.jpg .jpeg .png .gif .js .css
EOL;

