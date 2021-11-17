# Config

This readme file will tell you how to read and how to create the config file

## Directives list


| Directive             | Syntax                                                                               | Default                  | Context          |
|-----------------------|--------------------------------------------------------------------------------------|--------------------------|------------------|
| listen                | listen [address][:port];                                                             | listen 0.0.0.0:80        | server           |
| server_name           | server_name [name];                                                                  | server_name "";          | server           |
| error_page            | error_page [status code] [path];                                                     | -                        | server, location |
| client_max_body_size  | client_max_body_size [size];                                                         | client_max_body_size 1m; | server, location |
| location              | location [path] { [parameters] }                                                     | -                        | server, location |
| limit_except          | limit_except [methods];                                                              | -                        | location         |
| return                | return [status code] (text); <br /> return [status code] [URL]; <br /> return [URL]; | -                        | server, location |
| autoindex             | autoindex [on/off];                                                                  | autoindex off;           | server, location |
| root                  | root [path];                                                                         | root html;               | server, location |
| index                 | index [files];                                                                       | index index.html;        | server, location |
| upload_store          | upload_pass [directory];                                                             |                          | server, location |
| cgi_pass              | cgi_pass [path];                                                                     | -                        | location         |

## Parsing


In this config, there's different context :
- `simple`: that is a simple line, start with non-whitespace character and end with `;`
- `complex`: that is a block, start with a keyword, then some data, then a single `{`, contains one or multiples context when end with a `}`
- `document`: that is a file that will normaly be included has another context
- `comment`: that is just a comment line, it start with `#` and end at newline

So, for the parsing, you need to consider every kind of context (document support is not a mandatory part so let's skip it)

For example:
```conf
server {
	listen 80;
	# listen 0.0.0.0:8000;

	location / {
		limit_except GET POST DELETE;
	}
}
```

Here, there's 1 `complex` context that holding 1 `simple` context, 1 `comment` context and 1 `complex` context, that, himself contain 1 `simple` context

So we need to create a system, that can contains multiples contexts type like an `object` or `array` in `JSON`.

Here's the example of what should bring this config in `JSON` format.

(it's not really nice like this, i need to think about better things, this is cool in JavaScript but not in C++)
```json
{
	"servers": [
		{
			"listen": "80",
			"locations": {
				"/": {
					"limit_except": [
						"GET",
						"POST",
						"DELETE"
					]
				},
			}
		}
	]
}
```