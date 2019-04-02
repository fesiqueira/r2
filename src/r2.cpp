#include "r2.h"
#include <EtherCard.h>

Server::Server(uint8_t module, const uint8_t *macAddr) {
  _module = module;
  _macAddr = macAddr;
}

byte Ethernet::buffer[700];
int Server::begin() {
  switch (this->_module) {
  case ethercard:
    return ether.begin(sizeof Ethernet::buffer, this->_macAddr, SS);
  default:
    return 0;
  }
}

bool Server::DHCP() {
  switch (this->_module) {
  case ethercard:
    return ether.dhcpSetup();
  default:
    return false;
  }
}

void Server::handle() {
  switch (this->_module) {
  case ethercard:
    ether.packetLoop(ether.packetReceive());
  }
}
