#include "include/udpReceiver.h"
#include "include/telemetryHandler.h"
#include "include/telemetrySender/telemetrySender.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

void runReceiver()
{
    const uint16_t port = 20777;
    UdpReceiver receiver(port);

    // Pass Rust receiver's IP and port (make sure it matches the Rust UDP listener)
    TelemetryHandler handler("logs/", "127.0.0.1", 20888);

    const size_t bufferSize = 2048;
    char buffer[bufferSize];
    size_t receivedBytes;

    std::cout << "[Receiver] Listening on port " << port << "...\n";

    while (true)
    {
        if (receiver.receivePacket(buffer, bufferSize, receivedBytes))
        {
            handler.processPacket(buffer, receivedBytes);
        }
    }
}

void runSender()
{
    TelemetrySender sender("127.0.0.1", 20777);
    std::cout << "[Sender] Sending telemetry...\n";

    while (true)
    {
        float dt = 0.01;
        sender.sendRandomTelemetry(dt);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./F123_telemetry.exe [sender|receiver]\n";
        return 1;
    }

    if (strcmp(argv[1], "sender") == 0)
        runSender();
    else if (strcmp(argv[1], "receiver") == 0)
        runReceiver();
    else
    {
        std::cerr << "Invalid argument: use 'sender' or 'receiver'\n";
        return 1;
    }

    return 0;
}
