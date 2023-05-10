#include "../include/packet_builder.h"

#include <cassert>

PacketBuilder::PacketBuilder() : hasAddedMainHeader_(false) {}

PacketBuilder::~PacketBuilder() {}

Packet PacketBuilder::getPacket() const {
  assert(hasAddedMainHeader_);
  return packet_;
}

// reset current construction
void PacketBuilder::newPacket() {
  packet_ = Packet();
}

void PacketBuilder::addMainHeader(const std::bitset<Packet::VERSION_NUMBER_SIZE>& versionNumber,
              const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
              const std::bitset<Packet::APP_ID_DESTINATION_SIZE>& appIdDestination,
              const Packet::SequenceFlags sequenceControlFlags,
              const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceControlCount) {

  packet_.setVersionNumber(versionNumber);
  packet_.setAppIdSource(appIdSource);
  packet_.setAppIdDestination(appIdDestination);
  packet_.setSequenceControlCount(sequenceControlCount);
  packet_.setSequenceControlFlags(sequenceControlFlags);

  hasAddedMainHeader_ = true;
}

void PacketBuilder::addDataFieldHeader(const bool ack,
              const std::bitset<Packet::SERVICE_TYPE_SIZE>& serviceType,
              const std::bitset<Packet::SERVICE_SUBTYPE_SIZE>& serviceSubtype) {

  packet_.setACK(ack);
  packet_.setServiceType(serviceType);
  packet_.setServiceSubtype(serviceSubtype);
}
