#include "f1_23_packet_builder.h"

#include <cstring> // for std::memcpy
#include <vector>
#include <chrono>
#include <ctime>

/**
 * A small helper to pack any struct into a std::vector<uint8_t>.
 */
template <typename T>
std::vector<uint8_t> packStruct(const T &pkt)
{
    std::vector<uint8_t> buffer(sizeof(T));
    std::memcpy(buffer.data(), &pkt, sizeof(T));
    return buffer;
}

/**
 * Utility to fill in the common PacketHeader fields.
 * You can customize them to suit your testing.
 */
static void fillHeader(PacketHeader &header, uint8_t packetId)
{
    std::memset(&header, 0, sizeof(header));

    header.m_packetFormat = 2023;
    header.m_gameYear = 23;        // e.g. "F1 23"
    header.m_gameMajorVersion = 1; // e.g. "1"
    header.m_gameMinorVersion = 0; // e.g. "00"
    header.m_packetVersion = 1;    // All start from 1
    header.m_packetId = packetId;
    header.m_sessionUID = 0x1122334455667788ULL;
    header.m_sessionTime = 123.456f;
    header.m_frameIdentifier = 98765;
    header.m_overallFrameIdentifier = 98765;
    header.m_playerCarIndex = 0;            // Let's pretend our car is index 0
    header.m_secondaryPlayerCarIndex = 255; // No second player
}

std::vector<uint8_t> buildSessionPacket()
{
    PacketSessionData pkt;
    fillHeader(pkt.m_header, SESSION); // packetId=1

    // Fill out some dummy session fields
    pkt.m_weather = 2;           // Overcast
    pkt.m_trackTemperature = 27; // degrees C
    pkt.m_airTemperature = 22;
    pkt.m_totalLaps = 58;
    pkt.m_trackLength = 5300;     // e.g. 5.3km
    pkt.m_sessionType = 10;       // "R" Race
    pkt.m_trackId = 3;            // e.g. Sakhir (ID=3)
    pkt.m_formula = 0;            // F1 Modern
    pkt.m_sessionTimeLeft = 3600; // 60 minutes left
    pkt.m_sessionDuration = 5400; // 90 minutes total
    pkt.m_numMarshalZones = 1;
    pkt.m_marshalZones[0].m_zoneStart = 0.15f; // 15% around the track
    pkt.m_marshalZones[0].m_zoneFlag = 3;      // Yellow

    // etc. Fill more if desired...
    pkt.m_numWeatherForecastSamples = 2;
    pkt.m_weatherForecastSamples[0].m_sessionType = 10; // Race
    pkt.m_weatherForecastSamples[0].m_weather = 2;      // Overcast
    pkt.m_weatherForecastSamples[0].m_timeOffset = 15;  // 15 minutes from now
    pkt.m_weatherForecastSamples[1].m_sessionType = 10;
    pkt.m_weatherForecastSamples[1].m_weather = 3; // Light rain
    pkt.m_weatherForecastSamples[1].m_timeOffset = 30;

    // Convert the struct to bytes
    return packStruct(pkt);
}

std::vector<uint8_t> buildLapDataPacket()
{
    PacketLapData pkt;
    fillHeader(pkt.m_header, LAP_DATA); // packetId=2

    // Fill out dummy lap data for car 0
    pkt.m_lapData[0].m_lastLapTimeInMS = 92000;    // 1:32.000
    pkt.m_lapData[0].m_currentLapTimeInMS = 45000; // halfway around
    pkt.m_lapData[0].m_sector1TimeInMS = 31000;
    pkt.m_lapData[0].m_sector2TimeInMS = 35000;
    pkt.m_lapData[0].m_carPosition = 1; // P1
    pkt.m_lapData[0].m_currentLapNum = 5;
    pkt.m_lapData[0].m_numPitStops = 1;

    // Fill out car 1 as if it's behind
    pkt.m_lapData[1].m_lastLapTimeInMS = 95000; // 1:35
    pkt.m_lapData[1].m_currentLapTimeInMS = 47000;
    pkt.m_lapData[1].m_carPosition = 2;

    // etc. Fill others as needed

    // Convert the struct to bytes
    return packStruct(pkt);
}
