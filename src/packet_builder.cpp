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
