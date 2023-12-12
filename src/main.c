#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include "url_parser.h"

int main(int argc, char* argv[]){
    if (argc == 1){
        printf("Usage: %s [URL]\n", argv[0]);
        return 1;
    }
    if (0 == strcmp(argv[1],"-h") || 0 == strcmp(argv[1],"--help") ){
        printf("Usage: %s [URL]\n", argv[0]);
        return 0;
    }
    struct parsed_url *url;
    url = parse_url(argv[1]);
    int port;
    char *path;
    if (NULL == url->port){
        port = 80;
    } else {
        port = atoi(url->port);
    }
    if (NULL == url->path || 0 == strcmp("", url->path)){
        path = "index.html";
    } else {
        path = url->path;
    }


    struct hostent *host_entry;
    char *IPbuffer;
    host_entry = gethostbyname(url->host);
	IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
           AF_INET,
           htons(port),
           inet_addr(IPbuffer)
       };
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    char req[256];
    sprintf(req, "GET /%s HTTP/1.1\n\nHost: %s", url->path, url->host);
    send(sock, req, strlen(req), 0);
    char resp[256] = { 0 };
    recv(sock, &resp, 256, 0);
    write(2, resp, 256); // write to stderr for redirecting file output
    char buffer[1024] = { 0 };
    while (recv(sock, buffer, 1024, 0) > 0) {
        write(1, buffer, 1024);
    }
    parsed_url_free(url);
}