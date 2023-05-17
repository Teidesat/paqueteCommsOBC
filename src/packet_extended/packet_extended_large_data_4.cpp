#include "../../include/packet_extended/packet_extended_large_data_4.h"

PacketExtendedLargeData4::PacketExtendedLargeData4(const Packet& packet,
      uint8_t largeDataUnitId, uint8_t reasonCode) :
  packetBasic_(packet),
  largeDataUnitId_(largeDataUnitId),
  reasonCode_(reasonCode)
{}

// low level packet 
Packet& PacketExtendedLargeData4::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedLargeData4 PacketExtendedLargeData4::swapApplicationIdFields() {
  PacketExtendedLargeData4 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint8_t PacketExtendedLargeData4::getLargeDataUnitId() {
  return largeDataUnitId_;
}

uint8_t PacketExtendedLargeData4::getReasonCode() {
  return reasonCode_;
}

void PacketExtendedLargeData4::setReasonCode(const uint8_t newReasonCode) {
  reasonCode_ = newReasonCode;
}
