#ifndef API_H
#define API_H

#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <ArduinoJson.h>

#define HTTP_REST_PORT 80

enum direction { CW, CCW, N };

typedef struct {
    int left;
    int right;
    direction state;
} encoder;

typedef void (*RegisterListenerCallback) (IPAddress ip, unsigned int port, unsigned int new_buttons[], unsigned int new_buttons_num, encoder new_encoders[], unsigned int new_encoders_num);

extern ESP8266WebServer http_rest_server;

void init_api(RegisterListenerCallback registerer);
void config_rest_server_routing(RegisterListenerCallback registerer);
unsigned int jsonArray2Int (JsonArray array, unsigned int * number);
unsigned int jsonArray2Encoder(JsonArray array, encoder * encoders);

#endif
