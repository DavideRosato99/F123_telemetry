#include "udp_receiver.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <cstring>  // for std::strerror
#include <cerrno>   // for errno
#include <iostream>

UDPReceiver::UDPReceiver(const std::string &ip, unsigned short port)
    : m_ip(ip), m_port(port), m_socket(-1)
{
}

UDPReceiver::~UDPReceiver()
{
    if (m_socket >= 0)
    {
        close(m_socket);
    }
}

bool UDPReceiver::initSocket()
{
    // Create a UDP socket
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket < 0)
    {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Set up the address struct
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

    // Bind the socket
    int result = bind(m_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (result < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(m_socket);
        m_socket = -1;
        return false;
    }

    // Ready to receive
    return true;
}

int UDPReceiver::receive(char *buffer, int maxSize)
{
    if (m_socket < 0)
    {
        std::cerr << "Socket not initialized or invalid.\n";
        return -1;
    }

    sockaddr_in senderAddr;
    socklen_t addrLen = sizeof(senderAddr);

    // Block until we receive data or encounter an error
    int bytesReceived = recvfrom(
        m_socket,
        buffer,
        maxSize,
        0,
        reinterpret_cast<struct sockaddr *>(&senderAddr),
        &addrLen);

    if (bytesReceived < 0)
    {
        std::cerr << "recvfrom failed: " << strerror(errno) << std::endl;
        return -1;
    }

    return bytesReceived;
}
