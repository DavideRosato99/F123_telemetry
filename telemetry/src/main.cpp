#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "udp_receiver.h"
#include "telemetry_parser.h"

int main()
{
    std::string ip = "127.0.0.1";
    unsigned short port = 20777;

    UDPReceiver receiver(ip, port);
    if (!receiver.initSocket())
    {
        std::cerr << "Failed to init UDP socket.\n";
        return 1;
    }

    const int BUFFER_SIZE = 2048;
    std::vector<uint8_t> buffer(BUFFER_SIZE);

    std::cout << "Listening on " << ip << ":" << port << " for F1 23 UDP telemetry...\n";

    while (true)
    {
        int bytesReceived = receiver.receive(reinterpret_cast<char *>(buffer.data()), BUFFER_SIZE);
        if (bytesReceived > 0)
        {
            // Parse
            std::string result = parseF1_23Packet(buffer.data(), (size_t)bytesReceived);
            std::cout << "Received " << bytesReceived << " bytes:\n"
                      << result << std::endl;
        }
        else
        {
            // Avoid tight spin if no data
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    return 0;
}
