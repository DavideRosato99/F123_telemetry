#include <iostream>
#include <thread>
#include <chrono>
#include <cstring> // for std::strerror
#include <cerrno>  // for errno
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "f1_23_packet_builder.h"

int main()
{
    // IP/port for your existing F1 23 telemetry receiver
    const char *targetIp = "127.0.0.1";
    unsigned short targetPort = 20777;

    // 1) Create UDP socket
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0)
    {
        std::cerr << "Failed to create socket: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // 2) Prepare remote address
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(targetPort);
    addr.sin_addr.s_addr = inet_addr(targetIp);

    std::cout << "Test Sender started. Sending to "
              << targetIp << ":" << targetPort << "\n";

    int counter = 0;

    while (true)
    {
        std::vector<uint8_t> packet;

        // Alternate each second between sending a Session packet and a Lap Data packet.
        if (counter % 2 == 0)
            packet = buildSessionPacket();
        else
            packet = buildLapDataPacket();

        // 3) Send the packet
        int bytesSent = sendto(
            sockFd,
            reinterpret_cast<const char *>(packet.data()),
            packet.size(),
            0,
            reinterpret_cast<sockaddr *>(&addr),
            sizeof(addr));

        if (bytesSent < 0)
        {
            std::cerr << "sendto failed: " << std::strerror(errno) << std::endl;
        }
        else
        {
            std::cout << "Sent " << bytesSent << " bytes ("
                      << ((counter % 2 == 0) ? "Session" : "LapData")
                      << ")\n";
        }

        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        counter++;
    }

    close(sockFd);
    return 0;
}
