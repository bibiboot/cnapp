#include "globals.h"
#include "packet_sniffer.h"

void *sender(void *val)
{
    int payload_size = 4;
    char payload[] = "RAT";

    void* packet = (void*)malloc(payload_size + C_HLEN);

    /*Create packet*/
    create_packet(packet, globals.dest_node, DATA_PORT, payload, payload_size);
    int packet_size = payload_size + C_HLEN;

    send_packet_on_line("eth0", packet, packet_size);
}

void start(){
    void *val;
    pthread_create(&globals.sniff_th, 0, sniff, val);
    pthread_create(&globals.sender_th, 0, sender, val);
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Two arguments required\n");
        exit(1);
    }

    /**
     * argument 1: Source node
     * argument 2: Destination node
     */
    globals.src_node = atoi(argv[1]);
    globals.dest_node = atoi(argv[2]);

    /* Create file descriptor to write the packet */
    create_log_file();

    globals.send_sock_fd = get_socket();

    start();

    pthread_join(globals.sniff_th, NULL);
    pthread_join(globals.sender_th, NULL);

    return 0;
}
