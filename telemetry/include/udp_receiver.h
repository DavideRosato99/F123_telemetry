#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <string>

/**
 * Minimal UDPReceiver class for Cygwin/POSIX sockets.
 */
class UDPReceiver
{
public:
    UDPReceiver(const std::string &ip, unsigned short port);
    ~UDPReceiver();

    /**
     * Initialize a POSIX socket and bind to IP:port
     */
    bool initSocket();

    /**
     * Receive a UDP packet (blocking).
     *
     * @param buffer Destination for received bytes
     * @param maxSize Size of the buffer
     * @return Number of bytes received, or -1 on error
     */
    int receive(char *buffer, int maxSize);

private:
    std::string m_ip;
    unsigned short m_port;
    int m_socket; // File descriptor for the socket
};

#endif // UDP_RECEIVER_H
