#include "../../include/packet_extended/packet_extended_large_data_1.h"

PacketExtendedLargeData1::PacketExtendedLargeData1(const Packet& packet,
      uint8_t largeDataUnitId, uint16_t sequenceNumber,
      const std::vector<uint8_t>& data) :
  packetBasic_(packet),
  largeDataUnitId_(largeDataUnitId),
  sequenceNumber_(sequenceNumber),
  data_(data)
{}

PacketExtendedLargeData1::PacketExtendedLargeData1(const Packet& packet,
      uint8_t largeDataUnitId, uint16_t sequenceNumber,
      std::vector<uint8_t>&& data) :
  packetBasic_(packet),
  largeDataUnitId_(largeDataUnitId),
  sequenceNumber_(sequenceNumber),
  data_(std::move(data))
{}

// low level packet 
Packet& PacketExtendedLargeData1::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedLargeData1 PacketExtendedLargeData1::swapApplicationIdFields() {
  PacketExtendedLargeData1 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint8_t PacketExtendedLargeData1::getLargeDataUnitId() {
  return largeDataUnitId_;
}

uint16_t PacketExtendedLargeData1::getSequenceNumber() {
  return sequenceNumber_;
}

void PacketExtendedLargeData1::setSequenceNumber(const uint16_t newSequenceNumber) {
  sequenceNumber_ = newSequenceNumber;
}

std::vector<uint8_t>& PacketExtendedLargeData1::getData() {
  return data_;
}

void PacketExtendedLargeData1::setData(const std::vector<uint8_t>& newData) {
  data_ = newData;
}

void PacketExtendedLargeData1::setData(std::vector<uint8_t>&& newData) {
  data_ = std::move(newData);
}
