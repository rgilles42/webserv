# Config

This readme file will tell you how to read and how to create the config file

## Directives list


| Directive             | Syntax                                                                               | Default                  | Context          |
|-----------------------|--------------------------------------------------------------------------------------|--------------------------|------------------|
| server                | server { [directives] }                                                              | -                        | -                |
| listen                | listen [address][:port];                                                             | listen 0.0.0.0:80        | server           |
| server_name           | server_name [name];                                                                  | server_name "";          | server           |
| error_page            | error_page [status code] [path];                                                     | -                        | server, location |
| client_max_body_size  | client_max_body_size [size];                                                         | client_max_body_size 1m; | server, location |
| location              | location [path] { [directives] }                                                     | -                        | server, location |
| limit_except          | limit_except [methods];                                                              | -                        | location         |
| return                | return [status code] (text); <br /> return [status code] [URL]; <br /> return [URL]; | -                        | server, location |
| autoindex             | autoindex [on/off];                                                                  | autoindex off;           | server, location |
| root                  | root [path];                                                                         | root html;               | server, location |
| index                 | index [files];                                                                       | index index.html;        | server, location |
| upload_store          | upload_pass [directory];                                                             |                          | server, location |
| cgi_pass              | cgi_pass [path];                                                                     | -                        | location         |

## Lexer & Parser

The lexer will create datas that called `tokens` where all tokens have differents contexts.

If there's no one lexing error, the parser will take thoses `tokens` and check that everithing is fine, then create a data structure that contains everything properly formated.

After this, if there's no one parser error, we can bring this data structure to the Config class for building `servers` and starting the program with the right parameters.


In this config, there's different context :
- `simple`: that is a simple line, start with non-whitespace character and end with `;`
- `complex`: that is a block, start with a keyword, then some data, then a single `{`, contains one or multiples context when end with a `}`
- `document`: that is a file that will normaly be included has another context
- `comment`: that is just a comment line, it start with `#` and end at newline

So, for the parsing, you need to consider every kind of context (document support is not a mandatory part so let's skip it)

For example:
```conf
server {
	server_name localhost.com;
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
			"server_name": "localhost.com",
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

### Tokens

with this example, consider that there's multiple types of ending (`;` for `simple`, `}`, for `context`, `newline` for `comment`)

So `tokens` will be like this:
- `server {`: This is the start of a `complex` context, the content will normaly be stored into an `object`
- `listen 80`: This is a `simple` context, the content will normaly be stored into a `string` (because it's a server_name)
- `;`: This mark the end of previous `simple` context
- `# listen 0.0.0.0:8000;`: This is a `comment` context, it will be ended when there's a newline, this content doesn't need to be stored
- `location / {`: This is the start of a `complex` context, the content will normaly be stored into an `object`
- `limit_except GET POST DELETE`: This is a `simple` context, the content will normaly be stored into an `array` (because it's a limit_except)
- `;`: This mark the end of previous `simple` context
- `}`: This mark the end of previous `complex` context (for the location)
- `}`: This mark the end of previous `complex` context (for the server)

### Parser values

As you can see, every `directives` as a type target (`string`|`array`|`object`), see how it's gonna be stored :
- `server`: Will be stored as `object`, because every `server` can have one or multiple `directives`
- `listen`: Will be stored as `array`, because every `server` can listen on multiple hosts/ports
- `server_name`: Will be stored as `string`, because every `server` can have only one name
- `error_page`: Will be stored as `object`, because every `server` can have multiple custom error pages
- `client_max_body_size`: Will be stored as `string`, because every `server` can have only one of this
- `location`: Will be stored as `object`, because every `location` can have one or multiple `directives`
- `limit_except`: Will be stored as `array`, because every `server` can have multiple accepted methods
- `return`: Will be stored as `string`, because every `server` can have only one redirection
- `autoindex`: Will be stored as `string`, because every `server` can have only one autoindex toggler
- `root`: Will be stored as `string`, because every `server` can have only one root
- `index`: Will be stored as `array`, because every `server` can have multiple file index
- `upload_store`: Will be stored as `string`, because every `server` can only one upload target

and every `directives` can have special container:
- `server`: stored into an `array` of `object`, because the config can have one or multiple `server`
- `location`: stored into an `object` of `object`, because every `server`, `locations` can have one or multiple `location`


### Lexer & Parser errors

> Consider that warning doesn't stop the server start but error does.
It might be usefull to understand.

#### Lexer warnings/errors

- warnings:
  - `unkownWarning`: When i don't know..
- errors:
  - `configFileNotFoundError`: When the lexer cannot find the config file
  - `missingEndOfSimpleContextError`: When the lexer doesn't find `;` after a `simple` context
  - `missingEndOfComplexContextError`: When the lexer doesn't find `}` after a `complex` context

#### Parser warnings/errors

- warnings:
  - `multipleSameServerNameWarning`: When the parser find multiple servers with the same `server_name`, `host` and `port`
  - `unknownDirectiveWarning`: When the parser find an unknown `directive`, this will not affect anything, but need to comment useless things that are unknown `directive`
  - `directiveBadScopeWarning`: When a `directive` is in a context that cannot be supported, consider that is not a major issue because it will be ignored
  - `directiveNotWellFormatedWarning`: When a directive is not well formated like in the documentation
  - `multipleUniqueDirectiveWarning`: When a unique directive is defined more than 1 time (`server_name`, `limit_except`, etc..)
- errors:
  - `directoryNotFoundError`: When a root is defined but the directory is not found
  - `cgiNotFoundError`: When a cgi is defined but is not found
  - `unkownError`: When i don't know..