#include "telemetry_parser.h"
#include <sstream>
#include <cstring>  // for memcpy
#include <iostream> // optional

// A helper to safely copy a struct from raw buffer
template <typename T>
bool copyStruct(const uint8_t *data, size_t length, T &outStruct)
{
    if (length < sizeof(T))
    {
        return false; // not enough bytes
    }
    std::memcpy(&outStruct, data, sizeof(T));
    return true;
}

std::string parseF1_23Packet(const uint8_t *data, size_t length)
{
    if (!data || length < sizeof(PacketHeader))
    {
        return "Packet too small for even a header!";
    }

    // 1) Parse the header
    PacketHeader header{};
    std::memcpy(&header, data, sizeof(PacketHeader));

    std::ostringstream oss;
    oss << "----- F1 23 Packet -----\n"
        << "PacketFormat: " << header.m_packetFormat << "\n"
        << "GameYear:     " << (int)header.m_gameYear << "\n"
        << "GameVersion:  " << (int)header.m_gameMajorVersion
        << "." << (int)header.m_gameMinorVersion << "\n"
        << "PacketVersion:" << (int)header.m_packetVersion << "\n"
        << "PacketId:     " << (int)header.m_packetId << "\n"
        << "SessionUID:   " << header.m_sessionUID << "\n"
        << "SessionTime:  " << header.m_sessionTime << "\n"
        << "FrameId:      " << header.m_frameIdentifier << "\n"
        << "OverallFrame: " << header.m_overallFrameIdentifier << "\n"
        << "PlayerCarIdx: " << (int)header.m_playerCarIndex << "\n"
        << "SecPlayerIdx: " << (int)header.m_secondaryPlayerCarIndex << "\n\n";

    // 2) Switch on packet ID
    switch (header.m_packetId)
    {
    case MOTION:
    {
        PacketMotionData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketMotionData\n";
            break;
        }
        // For example, show data from the player's car (playerCarIndex)
        int idx = header.m_playerCarIndex;
        if (idx < 22)
        {
            auto &car = pkt.m_carMotionData[idx];
            oss << "MOTION Packet - Player Car Xpos=" << car.m_worldPositionX
                << " Ypos=" << car.m_worldPositionY
                << " Zpos=" << car.m_worldPositionZ << "\n";
        }
        else
        {
            oss << "MOTION Packet - Invalid playerCarIndex\n";
        }
        break;
    }

    case SESSION:
    {
        PacketSessionData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketSessionData\n";
            break;
        }
        oss << "SESSION Packet - TrackId=" << (int)pkt.m_trackId
            << " Weather=" << (int)pkt.m_weather
            << " Temperature=" << (int)pkt.m_airTemperature << "C\n";
        break;
    }

    case LAP_DATA:
    {
        PacketLapData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketLapData\n";
            break;
        }
        // For example, show player's current lap
        int idx = header.m_playerCarIndex;
        if (idx < 22)
        {
            auto &lap = pkt.m_lapData[idx];
            oss << "LAP_DATA Packet - Player Car Lap#=" << (int)lap.m_currentLapNum
                << " LastLapTime=" << lap.m_lastLapTimeInMS << "ms\n";
        }
        break;
    }

    case EVENT:
    {
        PacketEventData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketEventData\n";
            break;
        }
        // The eventStringCode is 4 bytes, e.g. "SSTA"
        std::string code(reinterpret_cast<const char *>(pkt.m_eventStringCode), 4);
        oss << "EVENT Packet - Code=" << code << "\n";
        // You could interpret "FTLP", "RTMT", etc. and parse the union
        break;
    }

    case PARTICIPANTS:
    {
        PacketParticipantsData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketParticipantsData\n";
            break;
        }
        oss << "PARTICIPANTS Packet - numActiveCars="
            << (int)pkt.m_numActiveCars << "\n";
        if (pkt.m_numActiveCars > 0)
        {
            // Example: Print player name
            auto &p = pkt.m_participants[header.m_playerCarIndex];
            oss << "Player Name: " << p.m_name << "\n";
        }
        break;
    }

    case CAR_SETUPS:
    {
        PacketCarSetupData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketCarSetupData\n";
            break;
        }
        oss << "CAR_SETUPS Packet\n";
        // e.g. check player's frontWing
        auto &setup = pkt.m_carSetups[header.m_playerCarIndex];
        oss << " PlayerCar frontWing=" << (int)setup.m_frontWing << "\n";
        break;
    }

    case CAR_TELEMETRY:
    {
        PacketCarTelemetryData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketCarTelemetryData\n";
            break;
        }
        auto &telem = pkt.m_carTelemetryData[header.m_playerCarIndex];
        oss << "CAR_TELEMETRY Packet - Speed=" << telem.m_speed
            << "kph Throttle=" << telem.m_throttle << "\n";
        break;
    }

    case CAR_STATUS:
    {
        PacketCarStatusData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketCarStatusData\n";
            break;
        }
        auto &status = pkt.m_carStatusData[header.m_playerCarIndex];
        oss << "CAR_STATUS Packet - FuelInTank=" << status.m_fuelInTank
            << " EnginePowerICE=" << status.m_enginePowerICE << "W\n";
        break;
    }

    case FINAL_CLASSIFICATION:
    {
        PacketFinalClassificationData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketFinalClassificationData\n";
            break;
        }
        oss << "FINAL_CLASSIFICATION Packet - numCars="
            << (int)pkt.m_numCars << "\n";
        if (pkt.m_numCars > 0)
        {
            auto &fc = pkt.m_classificationData[0];
            oss << " P1 bestLapTimeInMS=" << fc.m_bestLapTimeInMS
                << " totalRaceTime=" << fc.m_totalRaceTime << "\n";
        }
        break;
    }

    case LOBBY_INFO:
    {
        PacketLobbyInfoData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketLobbyInfoData\n";
            break;
        }
        oss << "LOBBY_INFO Packet - numPlayers=" << (int)pkt.m_numPlayers << "\n";
        break;
    }

    case CAR_DAMAGE:
    {
        PacketCarDamageData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketCarDamageData\n";
            break;
        }
        auto &dmg = pkt.m_carDamageData[header.m_playerCarIndex];
        oss << "CAR_DAMAGE Packet - FLWing=" << (int)dmg.m_frontLeftWingDamage
            << "% RL TyreWear=" << dmg.m_tyresWear[0] << "%\n";
        break;
    }

    case SESSION_HISTORY:
    {
        PacketSessionHistoryData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketSessionHistoryData\n";
            break;
        }
        oss << "SESSION_HISTORY Packet - CarIdx=" << (int)pkt.m_carIdx
            << " numLaps=" << (int)pkt.m_numLaps << "\n";
        break;
    }

    case TYRE_SETS:
    {
        PacketTyreSetsData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketTyreSetsData\n";
            break;
        }
        oss << "TYRE_SETS Packet - CarIdx=" << (int)pkt.m_carIdx << "\n";
        break;
    }

    case MOTION_EX:
    {
        PacketMotionExData pkt{};
        if (!copyStruct(data, length, pkt))
        {
            oss << "ERROR: Packet too small for PacketMotionExData\n";
            break;
        }
        oss << "MOTION_EX Packet\n";
        // Example: Print frontLeft suspensionPosition
        oss << " FL SuspensionPos=" << pkt.m_suspensionPosition[2] << "\n";
        break;
    }

    default:
        oss << "Unknown PacketID=" << (int)header.m_packetId << "\n";
        break;
    }

    return oss.str();
}
