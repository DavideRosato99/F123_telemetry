#ifndef TELEMETRYHANDLER_H
#define TELEMETRYHANDLER_H

#include "packetTypes.h"
#include <cstddef>
#include <fstream>
#include <string>

class TelemetryHandler
{
public:
    TelemetryHandler(const std::string &filename);
    ~TelemetryHandler();

    void processPacket(const char *buffer, size_t size);

private:
    void handleCarTelemetry(const PacketCarTelemetryData *telemetry);
    std::ofstream logFile;
};

#endif // TELEMETRYHANDLER_H
