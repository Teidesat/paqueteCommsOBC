#include "../../include/packet_extended/packet_extended_housekeeping_2.h"

PacketExtendedHousekeeping2::PacketExtendedHousekeeping2(
    const Packet& packet, uint16_t structureId,
    uint8_t collectionInterval, uint8_t amountOfSamples,
    const std::vector<uint8_t>& samples, uint8_t amountOfArrays,
    const std::vector<uint8_t>& arrays) :
      packetHousekeepingBasic_(packet, structureId),
      collectionInterval_(collectionInterval),
      amountOfSamples_(amountOfSamples),
      samples_(samples),
      amountOfArrays_(amountOfArrays),
      arrays_(arrays)
{}

PacketExtendedHousekeeping2::~PacketExtendedHousekeeping2() {}
