#include "../include/packet_builder.h"

PacketBuilder::PacketBuilder() {}

PacketBuilder::~PacketBuilder() {}

Packet PacketBuilder::getPacket() const {
  return packet_;
}

void PacketBuilder::newPacket(const std::byte versionNumber,
    const std::byte appIdSource, const std::byte appIdDestination,
    const Packet::SequenceFlags sequenceControlFlags,
    const std::array<std::byte, 2> sequenceControlCount
) {
  packet_ = Packet();
  packet_.setVersionNumber(versionNumber);
  packet_.setAppIdSource(appIdSource);
  packet_.setAppIdDestination(appIdDestination);
  packet_.setSequenceControlCount(sequenceControlCount);
  packet_.setSequenceControlFlags(sequenceControlFlags);
}

void PacketBuilder::setDataFieldHeader(const std::byte ack,
    const std::byte serviceType, const std::byte serviceSubtype) {
  packet_.setDataFieldHeader(std::byte{0b1});
  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

void PacketBuilder::addCommandVerificationHeader(const std::byte appIdSource,
    const std::byte sequenceFlags, const std::array<std::byte, 2> sequenceCount) {
  packet_.pushData(appIdSource);
  packet_.pushData(sequenceFlags);
  packet_.pushData(sequenceCount);
}

void PacketBuilder::addCommandVerificationHeader(const std::byte appIdSource,
    const std::byte sequenceFlags, const std::byte sequenceCount,
    const std::byte code) {
  packet_.pushData(appIdSource);
  packet_.pushData(sequenceFlags);
  packet_.pushData(sequenceCount);
  packet_.pushData(code);
}

void PacketBuilder::addCommandVerificationHeader(const std::byte appIdSource,
    const std::byte sequenceFlags, const std::byte sequenceCount,
    const std::byte code, const std::vector<std::byte>& parameters) {
  packet_.pushData(appIdSource);
  packet_.pushData(sequenceFlags);
  packet_.pushData(sequenceCount);
  packet_.pushData(code);
  for (uint8_t i = 0; i < parameters.size(); ++i) {
    packet_.pushData(parameters[i]);
  }
}

// first byte may be the amount of addresses
void PacketBuilder::addCommandDistributionHeader(
    const std::vector<std::byte>& addresses) {
  if (addresses.size() > 1) {
    packet_.pushData(std::byte(addresses.size()));
  }
  for (uint8_t i = 0; i < addresses.size(); ++i) {
    packet_.pushData(addresses[i]);
  }
}

// first byte may be the amount of addresses
template <typename T>
void PacketBuilder::addCommandDistributionHeader(const pairs_t<T>& addressAndData) {
  if (addressAndData.size() > 1) {
    packet_.pushData(std::byte(addressAndData.size()));
  }
  for (uint8_t i = 0; i < addressAndData.size(); ++i) {
    packet_.pushData(addressAndData[i].first);
    packet_.pushData(addressAndData[i].second);
  }
}

void PacketBuilder::addCommandDistributionHeader(
    const pairs_t<std::byte>& outLineIDAndDuration) {
  for (uint8_t i = 0; i < outLineIDAndDuration.size(); ++i) {
    packet_.pushData(outLineIDAndDuration[i].first);
    packet_.pushData(outLineIDAndDuration[i].second);
  }
}
