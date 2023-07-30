#include "../include/packet_builder.h"

#include <array>
#include <vector>

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

void PacketBuilder::setDataFieldHeader(bool ack,
    uint8_t serviceType, uint8_t serviceSubtype) {
  packet_.setDataFieldHeader(true);
  packet_.setAck(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

void PacketBuilder::addCommandVerificationAppData(uint8_t appIdSource,
    uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
    uint16_t sequenceCount) {
  packet_.pushData(appIdSource);
  packet_.pushData(appIdDestination);
  packet_.pushData(static_cast<uint8_t>(sequenceFlags));
  std::array<uint8_t, 2> temp{};
  temp[0] = static_cast<uint8_t>(sequenceCount >> 8);
  temp[1] = static_cast<uint8_t>(sequenceCount);
  packet_.pushData(temp);
}

void PacketBuilder::addCommandVerificationAppData(uint8_t appIdSource,
    uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
    uint16_t sequenceCount, uint8_t code) {
  packet_.pushData(appIdSource);
  packet_.pushData(appIdDestination);
  packet_.pushData(static_cast<uint8_t>(sequenceFlags));
  std::array<uint8_t, 2> temp{};
  temp[0] = static_cast<uint8_t>(sequenceCount >> 8);
  temp[1] = static_cast<uint8_t>(sequenceCount);
  packet_.pushData(temp);
  packet_.pushData(code);
}

void PacketBuilder::addCommandVerificationAppData(uint8_t appIdSource,
    uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
    uint16_t sequenceCount, uint8_t code,
    const std::vector<uint8_t>& parameters) {
  packet_.pushData(appIdSource);
  packet_.pushData(appIdDestination);
  packet_.pushData(static_cast<uint8_t>(sequenceFlags));
  std::array<uint8_t, 2> temp{};
  temp[0] = static_cast<uint8_t>(sequenceCount >> 8);
  temp[1] = static_cast<uint8_t>(sequenceCount);
  packet_.pushData(temp);
  packet_.pushData(code);
  for (size_t i = 0; i < parameters.size(); ++i) {
    packet_.pushData(parameters[i]);
  }
}

// first byte may be the amount of addresses
// uint8_t because 5 bits are used for module addresses currently on the packet.
void PacketBuilder::addCommandDistributionAppData(
    const std::vector<uint8_t>& addresses) {
  if (addresses.size() > 1) {
    packet_.pushData(static_cast<uint8_t>(addresses.size()));
  }
  for (size_t i = 0; i < addresses.size(); ++i) {
    packet_.pushData(addresses[i]);
  }
}

void PacketBuilder::addCommandDistributionAppData(
    const pairs_t<uint8_t, uint8_t>& lineIDAndDuration) {
  if (lineIDAndDuration.size() > 1) {
    packet_.pushData(static_cast<uint8_t>(lineIDAndDuration.size()));
  }
  for (size_t i = 0; i < lineIDAndDuration.size(); ++i) {
    packet_.pushData(lineIDAndDuration[i].first);
    packet_.pushData(lineIDAndDuration[i].second);
  }
}

void PacketBuilder::addHousekeepingReportAppData(uint16_t structureId,
    PacketExtendedHousekeeping25::GenerationMode mode,
    const std::vector<uint8_t>& parameters) {
  packet_.pushData(structureId);
  packet_.pushData(static_cast<uint8_t>(mode));
  packet_.pushData(parameters);
}
