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

