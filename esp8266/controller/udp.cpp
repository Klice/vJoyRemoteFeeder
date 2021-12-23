#include "udp.h"

WiFiUDP udp;

void sendUDP(String string, IPAddress ip, unsigned int port) {
  // convert string to char array
  char msg[255];
  string.toCharArray(msg,255);
  udp.beginPacket(ip, port);
  udp.write(msg);
  udp.endPacket();
}