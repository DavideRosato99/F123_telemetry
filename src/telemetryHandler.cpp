#include "include/telemetryHandler.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

TelemetryHandler::TelemetryHandler(const std::string &filename, const char *plotterIp, uint16_t plotterPort)
{
    logFile.open(filename, std::ios::out);
    if (!logFile.is_open())
    {
        std::cerr << "Unable to open file: " << filename << "\n";
    }
    else
    {
        logFile << "SessionTime,Speed,Gear,RPM\n";
    }

    plotterSocket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&plotterAddr, 0, sizeof(plotterAddr));
    plotterAddr.sin_family = AF_INET;
    plotterAddr.sin_port = htons(plotterPort);
    inet_pton(AF_INET, plotterIp, &plotterAddr.sin_addr);
}

TelemetryHandler::~TelemetryHandler()
{
    if (logFile.is_open())
        logFile.close();

    close(plotterSocket);
}

void TelemetryHandler::processPacket(const char *buffer, size_t size)
{
    if (size < sizeof(PacketHeader))
        return;

    const PacketHeader *header = reinterpret_cast<const PacketHeader *>(buffer);

    if (header->m_packetId == 6 && size >= sizeof(PacketCarTelemetryData))
    {
        handleCarTelemetry(reinterpret_cast<const PacketCarTelemetryData *>(buffer));
    }
}

void TelemetryHandler::handleCarTelemetry(const PacketCarTelemetryData *telemetry)
{
    const CarTelemetryData &myCar = telemetry->m_carTelemetryData[telemetry->m_header.m_playerCarIndex];

    float sessionTime = telemetry->m_header.m_sessionTime;
    int speed = myCar.m_speed;
    int gear = static_cast<int>(myCar.m_gear);
    int rpm = myCar.m_engineRPM;

    // Write CSV consistently
    if (logFile.is_open())
    {
        logFile << sessionTime << ","
                << speed << ","
                << gear << ","
                << rpm << "\n";
    }

    // Optional debug print
    std::cout << "Speed: " << speed << " km/h, Gear: " << gear << "\n";

    // Corrected function call
    sendToRustPlotter(sessionTime, speed, gear, rpm);
}

void TelemetryHandler::sendToRustPlotter(float sessionTime, int speed, int gear, int rpm)
{
    std::ostringstream oss;
    oss << sessionTime << "," << speed << "," << gear << "," << rpm;
    std::string msg = oss.str();

    sendto(plotterSocket, msg.c_str(), msg.size(), 0,
           (sockaddr *)&plotterAddr, sizeof(plotterAddr));
}
