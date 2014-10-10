#pragma once

#include "globals.h"

#define ROUTER_MAC 0
#define NODE1_MAC 1
#define NODE2_MAC 2
#define NODE3_MAC 3

#define NODE1_IP 1
#define NODE2_IP 2
#define NODE3_IP 3

#define DATA_PORT 0
#define DATA_NACK 1

struct data {
    uint32_t dest_mac;
    uint32_t src_mac;
};

struct pattern {
    uint64_t pttn;
};

struct custom_ethernet {
    uint16_t dest_mac;
};

struct custom_ip {
    uint16_t src_ip;
    uint16_t dest_ip;
};

struct custom_udp {
    uint16_t port;
};


