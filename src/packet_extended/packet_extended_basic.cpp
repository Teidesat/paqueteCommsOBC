#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedBasic::PacketExtendedBasic(const Packet& packet) :
    packet_(packet) {}

PacketExtendedBasic::PacketExtendedBasic(const PacketExtendedBasic& other) :
  packet_(other.packet_) {}

PacketExtendedBasic::~PacketExtendedBasic() {}

// low level packet 
Packet& PacketExtendedBasic::getPacket() {
  return packet_;
}

int PacketExtendedBasic::getVersionNumber() {
  return std::to_integer<int>(packet_.getVersionNumber());
}

int PacketExtendedBasic::getAppIdOrigin() {
  return std::to_integer<int>(packet_.getAppIdSource());
}

int PacketExtendedBasic::getAppIdDestination() {
  return std::to_integer<int>(packet_.getAppIdDestination());
}

int PacketExtendedBasic::getSequenceCount() {
  int output = 0;
  const std::array<std::byte, 2>& countBytes = packet_.getSequenceControlCount();
  std::memcpy(&output, &countBytes, 2);
  return output;
}

int PacketExtendedBasic::getLength() {
  int output = 0;
  const std::array<std::byte, 2>& lengthBytes = packet_.getLength();
  std::memcpy(&output, &lengthBytes, 2);
  return output;
}

int PacketExtendedBasic::getPUSVersion() {
  return std::to_integer<int>(packet_.getPUSVersion());
}

int PacketExtendedBasic::getServiceType() {
  return std::to_integer<int>(packet_.getServiceType());
}

int PacketExtendedBasic::getServiceSubtype() {
  return std::to_integer<int>(packet_.getServiceSubtype());
}

PacketExtendedBasic PacketExtendedBasic::getInstance() {
  return PacketExtendedBasic(*this);  
}

PacketExtendedBasic PacketExtendedBasic::getAcknowledgedVersion() {
  return PacketExtendedBasic(std::move(Packet()));
}

void PacketExtendedBasic::incrementSequenceCounter() {
  uint16_t count = 0;
  const auto& countBytes = packet_.getSequenceControlCount();
  std::memcpy(&count, &countBytes, sizeof(countBytes));
  if (count == std::numeric_limits<uint16_t>::max()) {
    packet_.setSequenceControlCount({std::byte(0), std::byte(0)});
  } else {
    const uint16_t newCount = count + 1;
    std::array<std::byte, 2> newCountArray;
    std::memcpy(&newCountArray, &newCount, sizeof(newCount));
    packet_.setSequenceControlCount(newCountArray);
  }
}

PacketExtendedBasic PacketExtendedBasic::swapApplicationIdFields() {
  PacketExtendedBasic&& copy = this->getInstance();
  copy.getPacket().setAppIdSource(packet_.getAppIdDestination());
  copy.getPacket().setAppIdDestination(packet_.getAppIdSource());
  return std::move(copy);
}
