#include "../../include/packet_extended/packet_extended_large_data_5.h"

PacketExtendedLargeData5::PacketExtendedLargeData5(const Packet& packet,
      uint8_t largeDataUnitId, uint16_t sequenceNumber) :
  packetBasic_(packet),
  largeDataUnitId_(largeDataUnitId),
  sequenceNumber_(sequenceNumber)
{}

// low level packet 
Packet& PacketExtendedLargeData5::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedLargeData5 PacketExtendedLargeData5::swapApplicationIdFields() {
  PacketExtendedLargeData5 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint8_t PacketExtendedLargeData5::getLargeDataUnitId() {
  return largeDataUnitId_;
}

uint16_t PacketExtendedLargeData5::getSequenceNumber() {
  return sequenceNumber_;
}

void PacketExtendedLargeData5::setSequenceNumber(const uint16_t newSequenceNumber) {
  sequenceNumber_ = newSequenceNumber;
}
