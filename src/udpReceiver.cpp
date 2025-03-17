#include "include/udpReceiver.h"
#include <unistd.h>
#include <cstring>

UdpReceiver::UdpReceiver(uint16_t port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
}

UdpReceiver::~UdpReceiver()
{
    close(sockfd);
}

bool UdpReceiver::receivePacket(char *buffer, size_t bufferSize, size_t &receivedBytes)
{
    int bytes = recvfrom(sockfd, buffer, bufferSize, 0, NULL, NULL);
    if (bytes < 0)
    {
        receivedBytes = 0;
        return false;
    }
    receivedBytes = static_cast<size_t>(bytes);
    return true;
}
