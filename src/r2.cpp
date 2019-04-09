#include "r2.h"
#include <EtherCard.h>

Route::Route() {}

Route::Route(char *method, char *path, HandlerFunc func) {
    _method = method;
    _path = path;
    _func = func;
}

char *Route::method() {
    return _method;
}

char *Route::path() {
    return _path;
}

char *Route::handle() {
    return _func();
}

Server::Server(uint8_t module, const uint8_t *macAddr) {
    _module = module;
    _macAddr = macAddr;
    _routes = new Route[0];
    _routesLen = 0;
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

// #TODO: refactor path matching and code repetition
void Server::handle(char *method, char *path, HandlerFunc func) {
    _routes[_routesLen] = Route(method, strcat(path, " "), func);
    _routesLen++;
}

void Server::get(char *path, HandlerFunc func) {
    _routes[_routesLen] = Route("GET", strcat(path, " "), func);
    _routesLen++;
}

void Server::post(char *path, HandlerFunc func) {
    _routes[_routesLen] = Route("POST", strcat(path, " "), func);
    _routesLen++;
}

// #TODO: DONT REPEAT YOURSELF
// #TODO: add HTTP Method checking
void Server::listen() {
    switch (this->_module) {
        case ethercard:
            char *http_ok = "HTTP/1.1 200 OK\n\n";
            char *http_notfound = "HTTP/1.1 404 Not Found\n\n404 page not found\n";

            word len = ether.packetReceive();
            word pos = ether.packetLoop(len);

            if (pos) {
                char *req = (char *)Ethernet::buffer + pos;
                char *req_line = strtok(req, "\n");
                for (int i = 0; i < _routesLen; i++) {
                    if (strstr(req_line, _routes[i].path()) != NULL) {
                        char *handle_res = _routes[i].handle();
                        char res[strlen(http_ok) + strlen(handle_res)];

                        strcpy(res, http_ok);
                        strcat(res, handle_res);

                        memcpy(ether.tcpOffset(), res, strlen(res));
                        ether.httpServerReply(strlen(res));
                        return;
                    }
                }

                char res[strlen(http_notfound)];
                strcpy(res, http_notfound);

                memcpy(ether.tcpOffset(), res, strlen(res));
                ether.httpServerReply(strlen(res));
            }
    }
}

void Server::describe() {
    switch (this->_module) {
        case ethercard:
            ether.printIp("IP: ", ether.myip);
            ether.printIp("Netmask: ", ether.netmask);
            ether.printIp("GW: ", ether.gwip);
            Serial.println("Routes:");
            for (int i = 0; i < _routesLen; i++) {
                Serial.println(_routes[i].path());
            }
    }
}
