#include <r2.h>

static byte macAddr[] = {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};

Server Server(ethercard, macAddr);

void setup() {
  Serial.begin(57600);
  Serial.println(F("DHCP Example"));

  if (Server.begin() == 0) {
    Serial.println(F("Failed to access controller"));
  }

  if (!Server.DHCP()) {
    Serial.println(F("DHCP failed"));
  }
}

void loop() { Server.handle(); }
