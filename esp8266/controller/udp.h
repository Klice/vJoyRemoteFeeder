#ifndef UDP_H
#define UDP_H

#include <Arduino.h>
#include <WiFiUdp.h>

extern WiFiUDP udp;

void sendUDP(String string, IPAddress ip, unsigned int port);

#endif