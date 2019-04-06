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

void Server::handle(String method, char *path, String (*f)(void)) {
    switch (this->_module) {
        case ethercard:
            String response = "HTTP/1.1 200 OK\nContent-Type: application/json\n\n";
            word len = ether.packetReceive();
            word pos = ether.packetLoop(len);

            if (pos) {
                char *req = (char *)Ethernet::buffer + pos;
                // TODO: Remove debugging lines
                Serial.println("Incoming request: ");
                Serial.println(req);
                if (strstr(req, path) != NULL) {
                    response.concat(f());
                } else {
                    response = "HTTP/1.1 404 Not Found\n\n404 page not found\n";
                }

                char page[response.length() + 1];
                response.toCharArray(page, response.length() + 1);
                memcpy(ether.tcpOffset(), page, sizeof page);
                ether.httpServerReply(sizeof page - 1);
            }
    }
}

void Server::describe() {
    switch (this->_module) {
        case ethercard:
            ether.printIp("IP: ", ether.myip);
            ether.printIp("Netmask: ", ether.netmask);
            ether.printIp("GW: ", ether.gwip);
    }
}