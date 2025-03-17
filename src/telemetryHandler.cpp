#include "include/telemetryHandler.h"
#include <iostream>
#include <cstring>

TelemetryHandler::TelemetryHandler(const std::string &filename)
{
    logFile.open(filename, std::ios::out); // truncates file each run
    if (!logFile.is_open())
    {
        std::cerr << "Unable to open log file: " << filename << "\n";
    }
    else
    {
        // Optional: write CSV header at the start
        logFile << "SessionTime,Speed,Gear,RPM\n";
    }
}

TelemetryHandler::~TelemetryHandler()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
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

    if (logFile.is_open())
    {
        logFile << telemetry->m_header.m_sessionTime << ", "
                << myCar.m_speed << ", "
                << static_cast<int>(myCar.m_gear) << ", "
                << myCar.m_throttle << ", "
                << myCar.m_brake << ", "
                << myCar.m_engineRPM << std::endl;
    }

    // opzionale: stampa a schermo per debug
    std::cout << "Speed: " << myCar.m_speed << " km/h, Gear: " << (int)myCar.m_gear << "\n";
}
