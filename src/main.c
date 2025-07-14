#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "vldwm/vldwm.h"

static void print_usage(const char *progname) {
    printf("Usage: %s [options]\n", progname);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
    printf("  -d, --debug    Enable debug mode\n");
}

static void print_version(void) {
    printf("VLDWM Display Server v1.0.0\n");
    printf("A custom display server implementation\n");
}

int main(int argc, char *argv[]) {
    int opt;
    bool debug = false;
    
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"debug", no_argument, 0, 'd'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "hvd", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 'v':
                print_version();
                return 0;
            case 'd':
                debug = true;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    printf("Starting VLDWM Display Server...\n");
    
    struct vldwm_server *server = vldwm_server_create();
    if (!server) {
        fprintf(stderr, "Failed to create server\n");
        return 1;
    }
    
    int ret = vldwm_server_run(server);
    
    vldwm_server_destroy(server);
    
    return ret;
}
