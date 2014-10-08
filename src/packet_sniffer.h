#pragma once

#include "globals.h"
#include "filter.h"

#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>

#include<netinet/ip_icmp.h>   //Declarations for icmp header
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

void* sniff(void *val);

int process_packet(unsigned char* packet, int size);
