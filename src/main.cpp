#include "include/udpReceiver.h"
#include "include/telemetryHandler.h"
#include "include/testSender/telemetrySender.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <windows.h> // Sleep
#else
#include <unistd.h>
#endif

void runReceiver()
{
    const uint16_t port = 20777;
    UdpReceiver receiver(port);
    TelemetryHandler handler("telemetry_log.csv");

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
        sender.sendRandomTelemetry();
        sleep(1); // Windows sleep (1 second)
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
