#pragma once
#include <vector>
#include <cstdint>

// If you store f1_23_packets.h in the same folder:
#include "f1_23_packets.h"

/**
 * Build a dummy "Session" packet (ID=1) with some example fields populated.
 * Returns a raw byte array ready to send via UDP.
 */
std::vector<uint8_t> buildSessionPacket();

/**
 * Build a dummy "Lap Data" packet (ID=2) with some example fields populated.
 * Returns a raw byte array ready to send via UDP.
 */
std::vector<uint8_t> buildLapDataPacket();

/**
 * (Optional) You can add more functions if you want to test other packet IDs.
 */
