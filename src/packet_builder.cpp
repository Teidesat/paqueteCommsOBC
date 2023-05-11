#include "../include/packet_builder.h"

PacketBuilder::PacketBuilder() {}

PacketBuilder::~PacketBuilder() {}

Packet PacketBuilder::getPacket() const {
  return packet_;
}

void PacketBuilder::newPacket(
  const uint8_t versionNumber,
  const uint8_t appIdSource,
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

void PacketBuilder::setDataFieldHeader(
    const bool ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype
) {
  packet_.setDataFieldHeader(true);
  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}

/*
  * Telecommand Verification Service headers.
  *    
*/

void PacketBuilder::addCommandVerificationHeader(
  const uint8_t appIdSource,
  const uint8_t sequenceFlags,
  const uint16_t sequenceCount
) {
  std::array<uint8_t, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  packet_.setAppData(appData);
}

void PacketBuilder::addCommandVerificationHeader(
  const uint8_t appIdSource,
  const uint8_t sequenceFlags,
  const uint8_t sequenceCount,
  const uint8_t code
) {
  std::array<uint8_t, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  std::memcpy(&appData, &code, sizeof(code));
  packet_.setAppData(appData);
}

void PacketBuilder::addCommandVerificationHeader(
  const uint8_t appIdSource,
  const uint8_t sequenceFlags,
  const uint8_t sequenceCount,
  const uint8_t code,
  const std::vector<uint8_t>& parameters
) {
  std::array<uint8_t, Packet::APP_DATA_SIZE> appData{}; // default initialize
  std::memcpy(&appData, &appIdSource, sizeof(appIdSource));
  std::memcpy(&appData, &sequenceFlags, sizeof(sequenceFlags));
  std::memcpy(&appData, &sequenceCount, sizeof(sequenceCount));
  std::memcpy(&appData, &code, sizeof(code));
  std::copy(parameters.begin(), parameters.end(), appData.begin());
  packet_.setAppData(appData);
}
