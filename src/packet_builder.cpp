#include "../include/packet_builder.h"

PacketBuilder::PacketBuilder() {}

PacketBuilder::~PacketBuilder() {}

Packet PacketBuilder::getPacket() const {
  return packet_;
}

void PacketBuilder::newPacket(
  const std::bitset<Packet::VERSION_NUMBER_SIZE>& versionNumber,
  const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
  const std::bitset<Packet::APP_ID_DESTINATION_SIZE>& appIdDestination,
  const Packet::SequenceFlags sequenceControlFlags,
  const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceControlCount
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
    const std::bitset<Packet::SERVICE_TYPE_SIZE>& serviceType,
    const std::bitset<Packet::SERVICE_SUBTYPE_SIZE>& serviceSubtype
) {
  packet_.setDataFieldHeader(true);
  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}
