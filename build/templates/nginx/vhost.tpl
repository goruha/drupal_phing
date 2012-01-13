server {
	listen   80;
	server_name  [host] www.[host];
  root   [site_root];

	access_log  /var/log/nginx/localhost.access.log;
	client_max_body_size 150M;
	client_body_buffer_size 128k;
	location / {
	  root [site_root];
		index index.php  index.html index.htm;
      if (!-e $request_filename) {
        rewrite  ^(.*)$  /index.php?q=$1  last;
        break;
      }
	}

  location ^~ [drupal_files_dir]/imagecache/ {
    index  index.php index.html;

    if (!-e $request_filename) {
      rewrite  ^/(.*)$  /index.php?q=$1  last;
      break;
    }
  }

	error_page  404  /index.php;

	# redirect server error pages to the static page /50x.html
	#
	#error_page   500 502 503 504  /50x.html;
	#location = /50x.html {
	#	root   /var/www/nginx-default;
	#}

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

  location ~* ^.+\.(jpg|jpeg|gif|css|png|js|ico)$ {
    access_log        off;
    expires           30d;
  }

	# deny access to .htaccess files, if Apache's document root
	# concurs with nginx's one
	#
	location [drupal_files_dir]/backup_migrate/manual {
		deny  all;
	}
}