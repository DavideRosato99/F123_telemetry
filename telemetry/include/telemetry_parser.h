#pragma once

#include <string>
#include <vector>
#include <cstddef> // size_t
#include <cstdint>

// Forward declarations of your big packet structs:
#include "f1_23_packets.h"

/**
 * Attempt to parse a generic F1 23 packet from raw data.
 *
 * Returns a string describing the contents or any warnings/errors encountered.
 * If you want to store the data in memory instead of just printing, adapt as needed.
 */
std::string parseF1_23Packet(const uint8_t *data, size_t length);
