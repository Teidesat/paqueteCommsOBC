#include "../include/packet_builder.h"

#include <array>
#include <vector>

PacketBuilder::PacketBuilder() {}

Packet& PacketBuilder::getPacket() {
  return packet_;
}

void PacketBuilder::newPacket(const uint8_t versionNumber, const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const Packet::SequenceFlags sequenceControlFlags,
    const uint16_t sequenceControlCount) {
  packet_ = Packet();
  packet_.setVersionNumber(versionNumber);
  packet_.setAppIdSource(appIdSource);
  packet_.setAppIdDestination(appIdDestination);
  packet_.setSequenceControlCount(sequenceControlCount);
  packet_.setSequenceControlFlags(sequenceControlFlags);
}

void PacketBuilder::setDataFieldHeader(const Packet::Bool8Enum ack,
    const uint8_t serviceType, const uint8_t serviceSubtype) {
  packet_.setDataFieldHeader(Packet::Bool8Enum::TRUE);
  packet_.setAck(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

void PacketBuilder::addCommandVerificationAppData(const uint8_t appIdSource,
    const uint8_t appIdDestination, const Packet::SequenceFlags sequenceFlags,
    const uint16_t sequenceCount) {
  packet_.pushData(std::byte(appIdSource));
  packet_.pushData(std::byte(appIdDestination));
  packet_.pushData(std::byte(sequenceFlags));
  std::array<std::byte, 2> temp;
  temp[0] = static_cast<std::byte>(sequenceCount >> 8);
  temp[1] = static_cast<std::byte>(sequenceCount);
  packet_.pushData(temp);
}

void PacketBuilder::addCommandVerificationAppData(const uint8_t appIdSource,
    const uint8_t appIdDestination, const Packet::SequenceFlags sequenceFlags,
    const uint16_t sequenceCount, const uint8_t code) {
  packet_.pushData(std::byte(appIdSource));
  packet_.pushData(std::byte(appIdDestination));
  packet_.pushData(std::byte(sequenceFlags));
  std::array<std::byte, 2> temp;
  temp[0] = static_cast<std::byte>(sequenceCount >> 8);
  temp[1] = static_cast<std::byte>(sequenceCount);
  packet_.pushData(std::byte(code));
}

void PacketBuilder::addCommandVerificationAppData(const uint8_t appIdSource,
    const uint8_t appIdDestination, const Packet::SequenceFlags sequenceFlags,
    const uint16_t sequenceCount, const uint8_t code,
    const std::vector<std::byte>& parameters) {
  packet_.pushData(std::byte(appIdSource));
  packet_.pushData(std::byte(appIdDestination));
  packet_.pushData(std::byte(sequenceFlags));
  std::array<std::byte, 2> temp;
  temp[0] = static_cast<std::byte>(sequenceCount >> 8);
  temp[1] = static_cast<std::byte>(sequenceCount);
  packet_.pushData(std::byte(code));  for (size_t i = 0; i < parameters.size(); ++i) {
    packet_.pushData(parameters[i]);
  }
}

// first byte may be the amount of addresses
// uint8_t because 5 bits are used for module addresses currently on the packet.
void PacketBuilder::addCommandDistributionAppData(
    const std::vector<uint8_t>& addresses) {
  if (addresses.size() > 1) {
    packet_.pushData(std::byte(addresses.size()));
  }
  for (size_t i = 0; i < addresses.size(); ++i) {
    packet_.pushData(std::byte(addresses[i]));
  }
}

void PacketBuilder::addCommandDistributionAppData(
    const pairs_t<int, std::byte>& lineIDAndDuration) {
  for (size_t i = 0; i < lineIDAndDuration.size(); ++i) {
    packet_.pushData(std::byte(lineIDAndDuration[i].first));
    packet_.pushData(lineIDAndDuration[i].second);
  }
}

void PacketBuilder::addHousekeepingReportAppData(uint16_t structureId,
    PacketExtendedHousekeeping25::GenerationMode mode,
    const std::vector<std::byte>& parameters) {
  packet_.pushData(structureId);
  packet_.pushData(std::byte(mode));
  packet_.pushData(parameters);
}
