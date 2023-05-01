#include "../include/packet_buffer.h"

#include <cassert>

PacketBuffer::PacketBuffer() {

}

PacketBuffer::~PacketBuffer() {}

Packet PacketBuffer::readPacket(const std::byte* buffer, std::size_t size) {
  assert(sizeof(buffer) < Packet::PACKET_HEADER_SIZE);
    std::memcpy(&versionNumber_, buffer, sizeof(versionNumber_));
  buffer += sizeof(versionNumber_);
  size -= sizeof(versionNumber_);

  std::memcpy(&dataFieldHeader_, buffer, sizeof(dataFieldHeader_));
  buffer += sizeof(dataFieldHeader_);
  size -= sizeof(dataFieldHeader_);

  std::memcpy(&appIdSource_, buffer, sizeof(appIdSource_));
  buffer += sizeof(appIdSource_);
  size -= sizeof(appIdSource_);

  std::memcpy(&appIdDestination_, buffer, sizeof(appIdDestination_));
  buffer += sizeof(appIdDestination_);
  size -= sizeof(appIdDestination_);

  std::memcpy(&length_, buffer, sizeof(length_));
  buffer += sizeof(appIdDestination_);
  size -= sizeof(appIdDestination_);
  
  if (dataFieldHeader_[0] == true) {
    std::memcpy(&ack_, buffer, sizeof(ack_));
    buffer += sizeof(ack_);
    size -= sizeof(ack_);

    std::memcpy(&serviceType_, buffer, sizeof(serviceType_));
    buffer += sizeof(serviceType_);
    size -= sizeof(serviceType_);

    std::memcpy(&serviceSubtype_, buffer, sizeof(serviceSubtype_));
    buffer += sizeof(serviceSubtype_);
    size -= sizeof(serviceSubtype_);

    appData_.reserve(size);
    appData_.insert(appData_.begin(), buffer, buffer + size - sizeof(packetErrorControl_));
  }

  std::memcpy(&packetErrorControl_, buffer, sizeof(packetErrorControl_));
  buffer += sizeof(packetErrorControl_);
  size -= sizeof(packetErrorControl_);

  
}

void PacketBuffer::writePacket(const std::byte* buffer, Packet packet) {
  
}

