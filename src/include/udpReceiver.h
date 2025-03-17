#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <cstdint>
#include <arpa/inet.h>

class UdpReceiver
{
public:
    UdpReceiver(uint16_t port);
    ~UdpReceiver();

    bool receivePacket(char *buffer, size_t bufferSize, size_t &receivedBytes);

private:
    int sockfd;
    struct sockaddr_in addr;
};

#endif // UDPRECEIVER_H
