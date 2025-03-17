#ifndef TELEMETRYHANDLER_H
#define TELEMETRYHANDLER_H

#include "packetTypes.h"
#include <cstddef>
#include <fstream>
#include <string>
#include <arpa/inet.h>

class TelemetryHandler
{
public:
    TelemetryHandler(const std::string &filename, const char *plotterIp, uint16_t plotterPort);
    ~TelemetryHandler();

    void processPacket(const char *buffer, size_t size);

private:
    void handleCarTelemetry(const PacketCarTelemetryData *telemetry);
    std::ofstream logFile;

    int plotterSocket;
    sockaddr_in plotterAddr;

    void sendToRustPlotter(float sessionTime, int speed, int gear, int rpm);
};

#endif // TELEMETRYHANDLER_H
