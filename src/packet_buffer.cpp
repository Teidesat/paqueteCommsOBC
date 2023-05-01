#include "../include/packet_buffer.h"

#include <cassert>
#include <cstring>

PacketBuffer::PacketBuffer() {

}

PacketBuffer::~PacketBuffer() {}

Packet PacketBuffer::readPacket(const std::byte* buffer, std::size_t size) {
  assert(sizeof(buffer) < Packet::PACKET_HEADER_SIZE);
  
  std::bitset<Packet::VERSION_NUMBER_SIZE> versionNumber;
  std::bitset<Packet::DATA_FIELD_HEADER_SIZE> dataFieldHeader;
  std::bitset<Packet::APP_ID_SOURCE_SIZE> appIdSource;
  std::bitset<Packet::APP_ID_DESTINATION_SIZE> appIdDestination;
  std::bitset<Packet::SEQUENCE_CONTROL_SIZE> sequenceControl;
  std::bitset<Packet::LENGTH_SIZE> length;

  std::bitset<Packet::ACK_SIZE> ack;
  std::bitset<Packet::SERVICE_TYPE_SIZE> serviceType;
  std::bitset<Packet::SERVICE_SUBTYPE_SIZE> serviceSubtype;

  std::vector<std::byte> appData;

  std::bitset<Packet::PACKET_ERROR_CONTROL_SIZE> packetErrorControl;

  std::memcpy(&versionNumber, buffer, sizeof(versionNumber));
  buffer += sizeof(versionNumber);
  size -= sizeof(versionNumber);

  std::memcpy(&dataFieldHeader, buffer, sizeof(dataFieldHeader));
  buffer += sizeof(dataFieldHeader);
  size -= sizeof(dataFieldHeader);

  std::memcpy(&appIdSource, buffer, sizeof(appIdSource));
  buffer += sizeof(appIdSource);
  size -= sizeof(appIdSource);

  std::memcpy(&appIdDestination, buffer, sizeof(appIdDestination));
  buffer += sizeof(appIdDestination);
  size -= sizeof(appIdDestination);

  std::memcpy(&sequenceControl, buffer, sizeof(sequenceControl));
  buffer += sizeof(sequenceControl);
  size -= sizeof(sequenceControl);

  std::memcpy(&length, buffer, sizeof(length));
  buffer += sizeof(length);
  size -= sizeof(length);
  
  if (dataFieldHeader == 1) {
    std::memcpy(&ack, buffer, sizeof(ack));
    buffer += sizeof(ack);
    size -= sizeof(ack);

    std::memcpy(&serviceType, buffer, sizeof(serviceType));
    buffer += sizeof(serviceType);
    size -= sizeof(serviceType);

    std::memcpy(&serviceSubtype, buffer, sizeof(serviceSubtype));
    buffer += sizeof(serviceSubtype);
    size -= sizeof(serviceSubtype);

    appData.reserve(size);
    appData.insert(appData.begin(), buffer, buffer + size - sizeof(packetErrorControl));
  } else {
    ack = 0;
    serviceType = 0;
    serviceSubtype = 0;
  }

  std::memcpy(&packetErrorControl, buffer, sizeof(packetErrorControl));
  buffer += sizeof(packetErrorControl);
  size -= sizeof(packetErrorControl);

  return Packet(versionNumber, dataFieldHeader, appIdSource, appIdDestination,
      sequenceControl, length, ack, serviceType, serviceSubtype, appData,
      packetErrorControl);
}

void PacketBuffer::writePacket(std::byte* buffer, const Packet& packet) {
  
}

