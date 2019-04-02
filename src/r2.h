#ifndef r2_h
#define r2_h

#if ARDUINO >= 100
#include "Arduino.h"
#endif

// Define supported modules
#define ethercard 1

class Server {
public:
  Server(uint8_t module, const uint8_t *macAddr);
  int begin();
  bool DHCP();
  void handle();

private:
  uint8_t _module;
  const uint8_t *_macAddr;
};

#endif
