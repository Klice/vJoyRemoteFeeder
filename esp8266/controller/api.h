#ifndef API_H
#define API_H

#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include "util.h"

#define HTTP_REST_PORT 80

typedef void (*RegisterListenerCallback) (IPAddress ip, unsigned int port);

extern ESP8266WebServer http_rest_server;

void init_api(RegisterListenerCallback registerer);
void config_rest_server_routing(RegisterListenerCallback registerer);
void register_listener();

#endif