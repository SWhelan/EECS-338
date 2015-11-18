#include "client.h"

int main(int argc, char * argv[]){
    CLIENT * client;
    char * host_name;
    int response = 0;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s host_name\n", *argv);
        exit(1);
    }
    
    host_name = argv[1];
    
    if ((client = clnt_create(host_name, APPLICATION_PROGRAM, APPLICATION_VERSION, "udp")) == (CLIENT *) NULL) {
        clnt_pcreateerror(host_name);
        exit(2);
    }
    
    printf("Client Judy starting up.\n");
    get_cookies(JUDY, client);
}