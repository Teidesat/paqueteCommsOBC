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

Packet& PacketExtendedHousekeeping2::getPacket() {
  return packetHousekeepingBasic_.getPacket();
}

PacketExtendedHousekeeping2 PacketExtendedHousekeeping2::swapApplicationIdFields() {
  PacketExtendedHousekeeping2 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint16_t PacketExtendedHousekeeping2::getStructureIdentifier() {
  return packetHousekeepingBasic_.getStructureIdentifier();
}
