server {
	listen   80;
	server_name  [host] www.[host];
  root   [site_root];

  index index.php  index.html index.htm;
  access_log  /var/log/nginx/localhost.access.log;

	client_max_body_size 150M;
	client_body_buffer_size 128k;

  error_page  404  /index.php;

  location ~* ^.+\.(bmp|svg|svgz|zip|gzip|bz2|rar|jpg|jpeg|gif|css|png|js|ico|pdf|gz)$ {
    access_log        off;
    expires           30d;
  }

  location / {
    try_files $uri @rewrite;
  }

  location @rewrite {
    rewrite ^/(.*)$ /index.php;
  }

  # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
  #
  location ~ \.php$ {
    fastcgi_pass unix:/var/run/php5-fpm.sock;
    fastcgi_index  index.php;
    fastcgi_param  SCRIPT_FILENAME  $document_root/$fastcgi_script_name;
    fastcgi_param  QUERY_STRING     $query_string;
    fastcgi_param  REQUEST_METHOD   $request_method;
    fastcgi_param  CONTENT_TYPE     $content_type;
    fastcgi_param  CONTENT_LENGTH   $content_length;
    include        fastcgi_params;
    fastcgi_intercept_errors on;
    break;
  }

  location ~ ^/sites/.*/files/imagecache/ {
    try_files $uri @rewrite;
  }

  location ~ ^/sites/.*/files/styles/ {
    try_files $uri @rewrite;
  }

  location [drupal_files_dir]/backup_migrate/manual {
    deny  all;
  }
}
