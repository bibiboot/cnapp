#include "globals.h"

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#define PACKET_SIZE1 12

struct data {
    uint32_t dest_mac;
    uint32_t src_mac;
};

int interface_addr(int sock, char *ifname, unsigned char *addr)
{
    struct ifreq ifr;

    /* retrieve corresponding source MAC */
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("Error in retrieveig mac address");
        printf("Error\n");
        return -EXIT_FAILURE;
    }
    memcpy(addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    return EXIT_SUCCESS;
}

int interface_index(int sock, char *ifname)
{
    struct ifreq ifr;

    /* retrieve source ethernet interface index */
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
        return -EXIT_FAILURE;

    return ifr.ifr_ifindex;
}

int get_socket(){
    int s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (s == -1) {
	printf("ERROR: socket\n");
	perror("SOCKET:");
	exit(1);
    }
    return s;
}

void create_socket_address(struct sockaddr_ll *socket_address, int src_index, uint32_t *dest_mac){
    /*prepare sockaddr_ll*/
    /*RAW communication*/
    socket_address->sll_family   = PF_PACKET;
    /*we don't use a protocoll above ethernet layer
    ->just use anything here*/
    //socket_address->sll_protocol = htons(ETH_P_IP);

    /*index of the network device
    see full code later how to retrieve it*/
    socket_address->sll_ifindex  = src_index;
    /*ARP hardware identifier is ethernet*/
    //socket_address->sll_hatype   = ARPHRD_ETHER;

    /*target is another host*/
    socket_address->sll_pkttype  = PACKET_OTHERHOST;

    /*address length*/
    //socket_address->sll_halen = ETH_ALEN;

    /*Assign the src mac address to the socket address*/
    //memcpy(socket_address->sll_addr, dest_mac, ETH_ALEN);
}

int create_packet(void *packet, uint32_t src_mac, uint32_t dest_mac)
{
    /*userdata in ethernet frame*/
    unsigned char* data = packet + 8;

    /*set the frame header*/
    memcpy((void*)packet, (void*)&dest_mac, 4);
    memcpy((void*)(packet+4), (void*)&src_mac, 4);

    memcpy((void*)(packet+8), "RAT", 4);
    return 4;

    /*
    int j;
    for (j = 0; j < 1500; j++)
        data[j] = (unsigned char)(5);
    data[j] = '\0';
    return 1500;
    */
}

void send_packet() {
    int s; /*socketdescriptor*/
    char if_name[IFNAMSIZ] = "eth0";
    struct ifreq ifr;
    /*target address*/
    struct sockaddr_ll socket_address;

    /*other host MAC address*/
    uint32_t dest_mac= htons(1);
    uint32_t src_mac = htons(2);

    /*Create raw socket*/
    s = get_socket();

    //Find src interface index
    int src_index = interface_index(s, if_name);

    /*prepare sockaddr_ll*/
    create_socket_address(&socket_address, src_index, &dest_mac);

    /*buffer for ethernet frame*/
    void* packet = (void*)malloc(PACKET_SIZE1);

    /*Create packet*/
    int payload_size = create_packet(packet, src_mac, dest_mac);

    /*send the packet*/
    int header_size = 8;
    int send_result = sendto(s, packet, header_size + payload_size ,
                             0, (struct sockaddr*)&socket_address,
                             sizeof(socket_address));
    if (send_result < 0){
	printf("ERROR: sendto\n");
	perror("sendto");
	exit(1);
    }
    printf("Bytes send: %d\n", send_result);

    fflush(stdout);
    fflush(LOGFILE);
}

int main(int argc, char *argv[]){

    send_packet();

    return 0;
}
