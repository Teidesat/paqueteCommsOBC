#include "../include/packet_builder.h"

#include <array>

PacketBuilder::PacketBuilder() {}

Packet& PacketBuilder::getPacket() {
  return packet_;
}

void PacketBuilder::newPacket(const uint8_t versionNumber, const uint8_t appIdSource,
      const uint8_t appIdDestination,
      const Packet::SequenceFlags sequenceControlFlags,
      const uint16_t sequenceControlCount
) {
  packet_ = Packet();
  packet_.setVersionNumber(versionNumber);
  packet_.setAppIdSource(appIdSource);
  packet_.setAppIdDestination(appIdDestination);
  packet_.setSequenceControlCount(sequenceControlCount);
  packet_.setSequenceControlFlags(sequenceControlFlags);
}

void PacketBuilder::setDataFieldHeader(const bool ack, const uint8_t serviceType,
      const uint8_t serviceSubtype) {
  packet_.setDataFieldHeader(true);
  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

void PacketBuilder::addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount) {
  packet_.pushData(std::byte(appIdSource));
  packet_.pushData(std::byte(sequenceFlags));
  std::array<std::byte, 2> temp;
  std::memcpy(&temp, &sequenceCount, sizeof(sequenceCount));
  packet_.pushData(temp);
}

void PacketBuilder::addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount,
      const uint8_t code) {
  this->addCommandVerificationHeader(appIdSource, sequenceFlags, sequenceCount);
  packet_.pushData(std::byte(code));
}

void PacketBuilder::addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount,
      const uint8_t code, const std::vector<std::byte>& parameters) {
  this->addCommandVerificationHeader(appIdSource, sequenceFlags, sequenceCount, code);
  for (uint8_t i = 0; i < parameters.size(); ++i) {
    packet_.pushData(parameters[i]);
  }
}

// first byte may be the amount of addresses
// uint8_t because 5 bits are used for module addresses currently on the packet.
void PacketBuilder::addCommandDistributionHeader(
    const std::vector<uint8_t>& addresses) {
  if (addresses.size() > 1) {
    packet_.pushData(std::byte(addresses.size()));
  }
  for (uint8_t i = 0; i < addresses.size(); ++i) {
    packet_.pushData(std::byte(addresses[i]));
  }
}

// first byte may be the amount of addresses
template <typename T>
void PacketBuilder::addCommandDistributionHeader(
    const pairs_t<uint8_t, T>& addressAndData
) {
  if (addressAndData.size() > 1) {
    packet_.pushData(std::byte(addressAndData.size()));
  }
  for (uint8_t i = 0; i < addressAndData.size(); ++i) {
    packet_.pushData(addressAndData[i].first);
    packet_.pushData(addressAndData[i].second);
  }
}

void PacketBuilder::addCommandDistributionHeader(
    const pairs_t<int, std::byte>& lineIDAndDuration) {
  for (uint8_t i = 0; i < lineIDAndDuration.size(); ++i) {
    packet_.pushData(std::byte(lineIDAndDuration[i].first));
    packet_.pushData(lineIDAndDuration[i].second);
  }
}

void PacketBuilder::addHousekeepingReportHeader(uint16_t structureId,
    PacketExtendedHousekeeping25::GenerationMode mode,
    const std::vector<std::byte>& parameters) {
  packet_.pushData(structureId);
  packet_.pushData(std::byte(mode));
  packet_.pushData(parameters);
}
