#include "filter.h"

#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen

#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

/**
 * Allow a packet to be sniffed or not.
 * First filter to incoming packet.
 * TCP = 6 | ICMP = 1 | UDP = 17
 */
bool is_allowed(unsigned char *packet, int data_size)
{
    struct sockaddr_in source, dest;
    struct ethhdr *eth = (struct ethhdr *)packet;
    struct iphdr *iph = (struct iphdr*)(packet + sizeof(struct ethhdr));

    /* Get src and dest ip of the incoming packet */
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    /*
     * Only allow TCP or UDP or ICMP , and matches the given
     * interface MAC address
     */
    if (iph->protocol ==  17 || iph->protocol == 1 || iph->protocol == 6) {
        return true;
    }
    return false;
}
