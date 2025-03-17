#include "include/telemetryHandler.h"
#include <iostream>
#include <cstring>

TelemetryHandler::TelemetryHandler(const std::string &filename)
{
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile.is_open())
    {
        std::cerr << "Failed to open file: " << filename << "\n";
    }
}

TelemetryHandler::~TelemetryHandler()
{
    if (logFile.is_open())
        logFile.close();
}

void TelemetryHandler::processPacket(const char *buffer, size_t size)
{
    if (size < sizeof(PacketHeader))
        return;

    const PacketHeader *header = reinterpret_cast<const PacketHeader *>(buffer);

    switch (header->m_packetId)
    {
    case 6: // Car Telemetry
        if (size >= sizeof(PacketCarTelemetryData))
        {
            handleCarTelemetry(reinterpret_cast<const PacketCarTelemetryData *>(buffer));
        }
        break;
    default:
        break;
    }
}

void TelemetryHandler::handleCarTelemetry(const PacketCarTelemetryData *telemetry)
{
    const CarTelemetryData &myCar = telemetry->m_carTelemetryData[telemetry->m_header.m_playerCarIndex];

    std::cout << "Speed: " << myCar.m_speed << " km/h\n";
    std::cout << "Gear: " << (int)myCar.m_gear << "\n";
    std::cout << "Throttle: " << myCar.m_throttle << "\n";
    std::cout << "Brake: " << myCar.m_brake << "\n";
    std::cout << "RPM: " << myCar.m_engineRPM << "\n";
}
