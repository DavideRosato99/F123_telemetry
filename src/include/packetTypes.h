#ifndef PACKETTYPES_H
#define PACKETTYPES_H

#include "packetHeader.h"

#pragma pack(push, 1)
struct CarTelemetryData
{
    uint16_t m_speed;
    float m_throttle;
    float m_steer;
    float m_brake;
    uint8_t m_clutch;
    int8_t m_gear;
    uint16_t m_engineRPM;
    uint8_t m_drs;
    uint8_t m_revLightsPercent;
    uint16_t m_revLightsBitValue;
    uint16_t m_brakesTemperature[4];
    uint8_t m_tyresSurfaceTemperature[4];
    uint8_t m_tyresInnerTemperature[4];
    uint16_t m_engineTemperature;
    float m_tyresPressure[4];
    uint8_t m_surfaceType[4];
};

struct PacketCarTelemetryData
{
    PacketHeader m_header;
    CarTelemetryData m_carTelemetryData[22];
    uint8_t m_mfdPanelIndex;
    uint8_t m_mfdPanelIndexSecondaryPlayer;
    int8_t m_suggestedGear;
};
#pragma pack(pop)

#endif // PACKETTYPES_H
