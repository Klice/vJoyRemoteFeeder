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
    StaticJsonDocument<500> jsonBody;
    String post_body = http_rest_server.arg("plain");
    IPAddress ip;
    unsigned int buttons[5];
    encoder encoders[5];
    DeserializationError error = deserializeJson(jsonBody, post_body);
    if (error) {
      http_rest_server.send(400);
    } else {
      http_rest_server.send(200);
    }
    ip.fromString(jsonBody["udp_host"].as<String>());
    unsigned int buttons_num = jsonArray2Int(jsonBody["pins"]["buttons"].as<JsonArray>(), buttons);
    unsigned int encoders_num = jsonArray2Encoder(jsonBody["pins"]["encoders"].as<JsonArray>(), encoders);
    register_callback(ip, (int)jsonBody["udp_port"], buttons, buttons_num, encoders, encoders_num);
  });
  http_rest_server.on("/index.html", HTTP_GET, []() {
      http_rest_server.send(200, "text/plain", "Hello World!");
  });
  http_rest_server.on("/description.xml", HTTP_GET, []() {
      SSDP.schema(http_rest_server.client());
  });
}


unsigned int jsonArray2Int(JsonArray array, unsigned int * numbers) {
  for (int i = 0; i < array.size(); ++i) {
    numbers[i] = array[i].as<int>();
  }
  return array.size();
}

unsigned int jsonArray2Encoder(JsonArray array, encoder * encoders) {
  for (int i = 0; i < array.size(); ++i) {
    encoders[i] = {
      array[i][0].as<int>(),
      array[i][1].as<int>(),
      direction::N
    };
  }
  return array.size();
}
