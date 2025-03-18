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
    TelemetryHandler(const std::string &folderPath, const char *plotterIp, uint16_t plotterPort);
    ~TelemetryHandler();

    void processPacket(const char *buffer, size_t size);

private:
    void handleMotionTelemetry(const PacketMotionData *telemetry);
    void handleSessionTelemetry(const PacketSessionData *telemetry);
    void handleLapDataTelemetry(const PacketLapData *telemetry);
    void handleEventTelemetry(const PacketEventData *telemetry);
    void handleCarSetupTelemetry(const PacketCarSetupData *telemetry);
    void handleCarTelemetry(const PacketCarTelemetryData *telemetry);
    void handleCarStatusTelemetry(const PacketCarStatusData *telemetry);
    void handleTyreSetsTelemetry(const PacketTyreSetsData *tyreSets);
    void handleMotionExTelemetry(const PacketMotionExData *motionEx);

    void initializeFiles(const std::string &folderPath);

    std::ofstream motionFile;
    std::ofstream sessionFile;
    std::ofstream lapDataFile;
    std::ofstream eventFile;
    std::ofstream carSetupFile;
    std::ofstream carTelemetryFile;
    std::ofstream carStatusFile;
    std::ofstream tyreSetsFile;
    std::ofstream motionExFile;

    int plotterSocket;
    sockaddr_in plotterAddr;

    void sendToRustPlotter(float sessionTime, int speed, int gear, int rpm);
};

#endif // TELEMETRYHANDLER_H
