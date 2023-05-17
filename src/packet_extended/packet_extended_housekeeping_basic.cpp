#include "../../include/packet_extended/packet_extended_housekeeping_basic.h"

std::unordered_map<uint16_t, std::vector<uint8_t>> PacketExtendedHousekeeping::structureIdDefinitions;

PacketExtendedHousekeeping::PacketExtendedHousekeeping(
    const Packet& packet, uint16_t structureId) :
        structureId_(structureId),
        packetBasic_(packet) {}

Packet& PacketExtendedHousekeeping::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedHousekeeping PacketExtendedHousekeeping::swapApplicationIdFields() {
  PacketExtendedHousekeeping copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint16_t PacketExtendedHousekeeping::getStructureIdentifier() {
  return structureId_;
}
