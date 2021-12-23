#include "api.h"

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

void init_api(RegisterListenerCallback register_callback) {
    config_rest_server_routing(register_callback);
    http_rest_server.begin();
    Serial.println("HTTP REST Server Started");
    Serial.println("Starting SSDP...");
    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(HTTP_REST_PORT);
    SSDP.setName("NodeMCU Game controller");
    SSDP.setURL("index.html");
    SSDP.setDeviceType("ArduinoGameController");
    SSDP.begin();
}

void config_rest_server_routing(RegisterListenerCallback register_callback) {
  http_rest_server.on("/", HTTP_GET, []() {
    http_rest_server.send(200, "text/html",
                          "Welcome to the ESP8266 Game Controller");
  });
  http_rest_server.on("/register", HTTP_POST, [register_callback]() {
    String post_body = http_rest_server.arg("plain");
    IPAddress ip;
    ip.fromString(getValue(post_body, ':', 0));
    register_callback(ip, getValue(post_body, ':', 1).toInt());
    http_rest_server.send(200);
  });
  http_rest_server.on("/index.html", HTTP_GET, []() {
      http_rest_server.send(200, "text/plain", "Hello World!");
  });
  http_rest_server.on("/description.xml", HTTP_GET, []() {
      SSDP.schema(http_rest_server.client());
  });
}
