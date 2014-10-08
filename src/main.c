#include "globals.h"
#include "packet_sniffer.h"

int main(int argc, char *argv[])
{
    /* Create file descriptor to write the packet */
    create_log_file();

    void *val;
    sniff(val);
    return 0;
}
