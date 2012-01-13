<VirtualHost *:80>                                                                                 
        ServerAdmin webmaster@localhost                                                            
        ServerName [host]
        ServerAlias  [host] www.[host]

        DocumentRoot [site_root]
        <Directory [site_root]/>
                Options Indexes FollowSymLinks MultiViews
                AllowOverride all
                Order allow,deny
                allow from all
        </Directory>

        ErrorLog /var/log/apache2/error.log

        # Possible values include: debug, info, notice, warn, error, crit,
        # alert, emerg.
        #LogLevel warn

        CustomLog /var/log/apache2/access.log combined
</VirtualHost>
