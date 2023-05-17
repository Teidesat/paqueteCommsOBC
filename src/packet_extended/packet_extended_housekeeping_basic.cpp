#include "../../include/packet_extended/packet_extended_housekeeping_basic.h"

std::unordered_map<uint16_t, std::vector<uint8_t>> PacketExtendedHousekeeping::structureIdDefinitions;

PacketExtendedHousekeeping::PacketExtendedHousekeeping(
    const Packet& packet, uint16_t structureId) :
        structureId_(structureId),
        packetBasic_(packet) {}

PacketExtendedHousekeeping::~PacketExtendedHousekeeping() {}

uint16_t PacketExtendedHousekeeping::getStructureIdentifier() {
  return structureId_;
}
