#include "../include/telemetrySender/telemetrySender.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

TelemetrySender::TelemetrySender(const char *ip, uint16_t port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);

    srand(static_cast<unsigned>(time(nullptr)));

    packet.m_header.m_packetFormat = 2023;
    packet.m_header.m_gameYear = 23;
    packet.m_header.m_gameMajorVersion = 1;
    packet.m_header.m_gameMinorVersion = 0;
    packet.m_header.m_packetVersion = 1;
    packet.m_header.m_packetId = 6; // Car Telemetry
    packet.m_header.m_sessionUID = 9876543210;
    packet.m_header.m_playerCarIndex = 0;
    packet.m_header.m_secondaryPlayerCarIndex = 255;
    packet.m_header.m_sessionTime = 0.0f;
    packet.m_header.m_frameIdentifier = 0;
    packet.m_header.m_overallFrameIdentifier = 0;
}

TelemetrySender::~TelemetrySender()
{
    close(sockfd);
}

float TelemetrySender::randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int TelemetrySender::randomInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool TelemetrySender::sendRandomTelemetry()
{
    packet.m_header.m_sessionTime += 1.0f;
    packet.m_header.m_frameIdentifier++;
    packet.m_header.m_overallFrameIdentifier++;

    CarTelemetryData &car = packet.m_carTelemetryData[0];

    car.m_speed = randomInt(100, 350);
    car.m_throttle = randomFloat(0.0f, 1.0f);
    car.m_steer = randomFloat(-1.0f, 1.0f);
    car.m_brake = randomFloat(0.0f, 1.0f);
    car.m_clutch = randomInt(0, 100);
    car.m_gear = randomInt(-1, 8);
    car.m_engineRPM = randomInt(6000, 15000);
    car.m_drs = randomInt(0, 1);
    car.m_revLightsPercent = randomInt(0, 100);
    car.m_revLightsBitValue = randomInt(0, 0x7FFF);

    for (int i = 0; i < 4; ++i)
    {
        car.m_brakesTemperature[i] = randomInt(200, 1200);
        car.m_tyresSurfaceTemperature[i] = randomInt(60, 120);
        car.m_tyresInnerTemperature[i] = randomInt(70, 130);
        car.m_tyresPressure[i] = randomFloat(20.0f, 25.0f);
        car.m_surfaceType[i] = 0;
    }

    car.m_engineTemperature = randomInt(80, 130);

    packet.m_mfdPanelIndex = 0;
    packet.m_mfdPanelIndexSecondaryPlayer = 255;
    packet.m_suggestedGear = randomInt(1, 8);

    ssize_t sent_bytes = sendto(sockfd, &packet, sizeof(packet), 0,
                                (sockaddr *)&servaddr, sizeof(servaddr));

    if (sent_bytes == sizeof(packet))
    {
        std::cout << "Sent telemetry packet | Speed: " << car.m_speed
                  << " km/h | Gear: " << (int)car.m_gear
                  << " | RPM: " << car.m_engineRPM << "\n";
        return true;
    }
    else
    {
        std::cerr << "Failed to send telemetry packet\n";
        return false;
    }
}
