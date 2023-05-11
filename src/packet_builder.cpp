#include "../include/packet_builder.h"

PacketBuilder::PacketBuilder() {}

PacketBuilder::~PacketBuilder() {}

Packet PacketBuilder::getPacket() const {
  return packet_;
}

void PacketBuilder::newPacket(
  const std::byte versionNumber,
  const std::byte appIdSource,
  const std::byte appIdDestination,
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

void PacketBuilder::setDataFieldHeader(
    const std::byte ack,
    const std::byte serviceType,
    const std::byte serviceSubtype
) {
  packet_.setDataFieldHeader(std::byte{0b1});
  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

void PacketBuilder::addCommandVerificationHeader(
  const std::byte appIdSource,
  const std::byte sequenceFlags,
  const std::array<std::byte, 2> sequenceCount
) {
  std::array<std::byte, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  packet_.setAppData(appData);
}

void PacketBuilder::addCommandVerificationHeader(
  const std::byte appIdSource,
  const std::byte sequenceFlags,
  const std::byte sequenceCount,
  const std::byte code
) {
  std::array<std::byte, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  std::memcpy(&appData, &code, sizeof(code));
  packet_.setAppData(appData);
}

void PacketBuilder::addCommandVerificationHeader(
  const std::byte appIdSource,
  const std::byte sequenceFlags,
  const std::byte sequenceCount,
  const std::byte code,
  const std::vector<std::byte>& parameters
) {
  std::array<std::byte, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  std::memcpy(&appData, &code, sizeof(code));
  std::copy(parameters.begin(), parameters.end(), appData.begin());
  packet_.setAppData(appData);
}
