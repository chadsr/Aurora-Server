#ifndef SERVER_H
#define SERVER_H

#define API_PORT 80
#define UDP_PORT 1337
#define JSON_BUFF_SIZE 1024
#define JSON_ERROR_SIZE 128
#define BUFF_SIZE 1024

extern ledSequence CurrentSequence;

void setupServer();
void handleUDPPacket();
void handleAPIClient();

#endif
