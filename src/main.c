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
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\nConnection: close\r\n\r\n", url->path, url->host);
    send(sock, req, strlen(req), 0);
    char buffer[1024] = { 0 };
    char *resp = (char *)malloc(0);
    int num_recvd = 0;
    while((num_recvd = recv(sock, buffer, 1024, 0)) > 0)
	{
        buffer[num_recvd] = '\0';
        int i = strlen(resp)+num_recvd + 1;
        char *tmp = (char *)malloc(i);
        strcpy(tmp, resp);
        sprintf(tmp, "%s%s", resp, buffer);
        resp = tmp+3;
	}
    char *header_delimiter = "\r\n\r\n";
    char* body = strstr(resp, header_delimiter);
    int length = body - resp;
    char headers[length];
    memcpy(headers, resp, length);
    headers[length] = '\0';
    body = body + strlen(header_delimiter);
    write(2, headers, strlen(headers));
    write(2, "\n\n", strlen("\n\n"));
    write(1, body, strlen(body));
    close(sock);
    parsed_url_free(url);
}