#include "../include/packet.h"

#include <cstring>

Packet::Packet(const std::byte* buffer, std::size_t size) {
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

Packet::~Packet() {}

std::bitset<3> Packet::getVersionNumber() const {
  return versionNumber_;
}

std::bitset<1> Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

std::bitset<5> Packet::getAppIdSource() const {
  return appIdSource_;
}

std::bitset<5> Packet::getAppIdDestination() const {
  return appIdDestination_;
}

std::bitset<16> Packet::getSequenceControl() const {
  return sequenceControl_;
}

std::bitset<16> Packet::getLength() const {
  return length_;
}

std::bitset<4> Packet::getAck() const {
  return ack_;
}

std::bitset<8> Packet::getServiceType() const {
  return serviceType_;
}

std::bitset<8> Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

std::vector<std::byte> Packet::getAppData() const {
  return appData_;
}

std::bitset<3> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}
