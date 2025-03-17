#ifndef TELEMETRYSENDER_H
#define TELEMETRYSENDER_H

#include "../packetTypes.h"
#include <arpa/inet.h>

class TelemetrySender
{
public:
    TelemetrySender(const char *ip, uint16_t port);
    ~TelemetrySender();

    bool sendRandomTelemetry(float dt);

private:
    int sockfd;
    sockaddr_in servaddr;
    PacketCarTelemetryData packet;

    float randomFloat(float min, float max);
    int randomInt(int min, int max);
};

#endif // TELEMETRYSENDER_H
