#ifndef r2_h
#define r2_h

#if ARDUINO >= 100
#include "Arduino.h"
#endif

// Define supported modules
#define ethercard 1

typedef char *(*HandlerFunc)(void);

class Route {
   public:
    Route();
    Route(char *method, char *path, HandlerFunc func);
    char *method();
    char *path();
    char *handle();

   private:
    char *_method;
    char *_path;
    HandlerFunc _func;
};

class Server {
   public:
    Server(uint8_t module, const uint8_t *macAddr);
    int begin();
    bool DHCP();

    void handle(char *method, char *path, HandlerFunc func);
    void get(char *path, HandlerFunc func);
    void post(char *path, HandlerFunc func);

    void listen();
    void describe();

   private:
    uint8_t _module;
    const uint8_t *_macAddr;
    Route *_routes;
    int _routesLen;
};

#endif
