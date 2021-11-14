server {

	# Config wich port & host is used for IPV4
	# By default port is always 80 and ip address 0.0.0.0
	# Prototype: listen [address][:port];
	listen 80;
	# listen 0.0.0.0:80;
	# listen 0.0.0.0;

	# Config the server_name of the current virtual server
	# By default server_name is always 80 and ip address 0.0.0.0
	# Prototype: server_name [server_name];
	server_name _;

	# Config the root folder of the current virtual server
	# Prototype: root [path];
	root ./default_pages;

	# Config error page that will be shown for the specified error of the current virtual server
	# Prototype: error_page [status code] [path];
	error_page 404 ./default_pages/404.html;

	# Config the limit for the client body size of the current virtual server
	# Prototype: client_max_body_size [size];
	client_max_body_size 1m;


	# Config a route for the current virtual server
	# Prototype: location [path] { [parameters] }
	location / {

		# Config allowed method for the current route
		# Prototype: limit_except [methods];
		limit_except GET POST DELETE;

	}

	location /google {

		# Config a redirection for the current route
		# Prototype: error_page [status code] [path];
		return 301 https://google.fr

	}

	location /auto {

		# Config autoindex to be enable or disable for the current route
		# Prototype: autoindex [on|off];
		autoindex on;

	}

	location /myroot {

		# Config the root folder for the current route
		# Prototype: root [path];
		root ./;

	}

	location /id {

		# Config the default index files for the current route
		# Prototype: autoindex [on|off];
		index index.html index.htm;

	}


	# CGI Things
	#location ~ \.php$ {
	#	include snippets/fastcgi-php.conf;
	#
	#	# With php-fpm (or other unix sockets):
	#	fastcgi_pass unix:/var/run/php/php7.0-fpm.sock;
	#	# With php-cgi (or other tcp sockets):
	#	fastcgi_pass 127.0.0.1:9000;
	#}

}
