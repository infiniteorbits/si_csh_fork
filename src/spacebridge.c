
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include <csp/interfaces/csp_if_can.h>
#include <csp/drivers/can_socketcan.h>


void usage(void)
{
	printf("usage: spacebridge\n");
	printf("\n");
	printf("Copyright (c) 2021 Space Inventor ApS <info@spaceinventor.com>\n");
	printf("\n");
	printf("Options:\n");
	printf(" -c INTERFACE,\tUse INTERFACE as CAN interface\n");
	printf(" -z ZMQ_IP\tIP of zmqproxy node\n");
}


int main(int argc, char **argv) {

    char *can_dev = "can0";
    char * csp_zmqhub_addr = "localhost";

    int c;
    while ((c = getopt(argc, argv, "+hc:z:")) != -1) {
		switch (c) {
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
		case 'c':
			can_dev = optarg;
			break;
		case 'z':
			csp_zmqhub_addr = optarg;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	csp_conf.version = 2;
	csp_conf.hostname = "csh";
	csp_conf.model = "linux";
	csp_init();

	csp_iface_t * can_if;
    int res = csp_can_socketcan_open_and_add_interface(can_dev, NULL, 1, 8, 1, 1000000, false, &can_if);
    if (res < 0) {
        printf("failed to add CAN interface [%s]\n", can_dev);
    }

	csp_iface_t * zmq_if;
	csp_zmqhub_init(0, csp_zmqhub_addr, 0, &zmq_if);
    zmq_if->name = "ZMQ";

#if (CSP_HAVE_STDIO)
    csp_iflist_print();
#endif

    csp_bridge_set_interfaces(zmq_if, can_if);

    while(1) {
		csp_bridge_work();
	}

}