#ifndef PACKETHEADER_H
#define PACKETHEADER_H

#include <cstdint>

#pragma pack(push, 1)
struct PacketHeader
{
    uint16_t m_packetFormat;
    uint8_t m_gameYear;
    uint8_t m_gameMajorVersion;
    uint8_t m_gameMinorVersion;
    uint8_t m_packetVersion;
    uint8_t m_packetId;
    uint64_t m_sessionUID;
    float m_sessionTime;
    uint32_t m_frameIdentifier;
    uint32_t m_overallFrameIdentifier;
    uint8_t m_playerCarIndex;
    uint8_t m_secondaryPlayerCarIndex;
};
#pragma pack(pop)

#endif // PACKETHEADER_H
