#include "../include/packet_operations.h"

#include <cassert>
#include <cstring>

PacketOperations::PacketOperations() {}

PacketOperations::~PacketOperations() {}

Packet PacketOperations::readPacket(const std::byte* buffer, std::size_t size) {
  std::bitset<Packet::VERSION_NUMBER_SIZE> versionNumber;
  std::bitset<Packet::DATA_FIELD_HEADER_SIZE> dataFieldHeader;
  std::bitset<Packet::APP_ID_SOURCE_SIZE> appIdSource;
  std::bitset<Packet::APP_ID_DESTINATION_SIZE> appIdDestination;
  std::bitset<Packet::SEQUENCE_CONTROL_FLAGS_SIZE> sequenceControlFlags;
  std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE> sequenceControlCount;
  std::bitset<Packet::LENGTH_SIZE> length;

  std::bitset<Packet::CCSDS_SIZE> ccsds;
  std::bitset<Packet::PUS_VERSION_SIZE> pusVersion;
  std::bitset<Packet::ACK_SIZE> ack;
  std::bitset<Packet::SERVICE_TYPE_SIZE> serviceType;
  std::bitset<Packet::SERVICE_SUBTYPE_SIZE> serviceSubtype;

  std::array<std::byte, Packet::APP_DATA_SIZE> appData;

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

  std::memcpy(&sequenceControlFlags, buffer, sizeof(sequenceControlFlags));
  buffer += sizeof(sequenceControlFlags);
  size -= sizeof(sequenceControlFlags);

  std::memcpy(&sequenceControlCount, buffer, sizeof(sequenceControlCount));
  buffer += sizeof(sequenceControlCount);
  size -= sizeof(sequenceControlCount);

  std::memcpy(&length, buffer, sizeof(length));
  buffer += sizeof(length);
  size -= sizeof(length);
  
  if (dataFieldHeader == 1) {
    std::memcpy(&ccsds, buffer, sizeof(ccsds));
    buffer += sizeof(ccsds);
    size -= sizeof(ccsds);

    std::memcpy(&pusVersion, buffer, sizeof(pusVersion));
    buffer += sizeof(pusVersion);
    size -= sizeof(pusVersion);

    std::memcpy(&ack, buffer, sizeof(ack));
    buffer += sizeof(ack);
    size -= sizeof(ack);

    std::memcpy(&serviceType, buffer, sizeof(serviceType));
    buffer += sizeof(serviceType);
    size -= sizeof(serviceType);

    std::memcpy(&serviceSubtype, buffer, sizeof(serviceSubtype));
    buffer += sizeof(serviceSubtype);
    size -= sizeof(serviceSubtype);

    std::memcpy(&appData, buffer, sizeof(appData));
    buffer += sizeof(appData);
    size -= sizeof(appData);
  } else {
    ack = 0;
    serviceType = 0;
    serviceSubtype = 0;
  }

  std::memcpy(&packetErrorControl, buffer, sizeof(packetErrorControl));
  buffer += sizeof(packetErrorControl);
  size -= sizeof(packetErrorControl);

  return Packet(versionNumber, dataFieldHeader, appIdSource, appIdDestination,
      sequenceControlFlags, sequenceControlCount, length, ccsds, pusVersion,
      ack, serviceType, serviceSubtype, appData, packetErrorControl);
}

void PacketOperations::writePacket(std::byte* buffer, Packet& packet) {
  const auto& versionNumber = packet.getVersionNumber();
  const auto& dataFieldHeader = packet.getDataFieldHeader();
  const auto& appIdSource = packet.getAppIdSource();
  const auto& appIdDestination = packet.getAppIdDestination();
  const auto& sequenceControlFlags = packet.getSequenceControlFlags();
  const auto& sequenceControlCount = packet.getSequenceControlCount();
  const auto& length = packet.getLength();

  const auto& ccsds = packet.getCCSDS();
  const auto& pusVersion = packet.getPUSVersion();
  const auto& ack = packet.getAck();
  const auto& serviceType = packet.getServiceType();
  const auto& serviceSubtype = packet.getServiceSubtype();

  const auto& appData = packet.getAppData();

  const auto& packetErrorControl = packet.getPacketErrorControl();

  std::memcpy(buffer, &versionNumber, sizeof(versionNumber));
  buffer += sizeof(versionNumber);

  std::memcpy(buffer, &dataFieldHeader, sizeof(dataFieldHeader));
  buffer += sizeof(dataFieldHeader);

  std::memcpy(buffer, &appIdSource, sizeof(appIdSource));
  buffer += sizeof(appIdSource);

  std::memcpy(buffer, &appIdDestination, sizeof(appIdDestination));
  buffer += sizeof(appIdDestination);

  std::memcpy(buffer, &sequenceControlFlags, sizeof(sequenceControlFlags));
  buffer += sizeof(sequenceControlFlags);

  std::memcpy(buffer, &sequenceControlCount, sizeof(sequenceControlCount));
  buffer += sizeof(sequenceControlCount);

  std::memcpy(buffer, &length, sizeof(length));
  buffer += sizeof(length);
  
  if (dataFieldHeader == 1) {
    std::memcpy(buffer, &ccsds, sizeof(ccsds));
    buffer += sizeof(ccsds);

    std::memcpy(buffer, &pusVersion, sizeof(pusVersion));
    buffer += sizeof(pusVersion);

    std::memcpy(buffer, &ack, sizeof(ack));
    buffer += sizeof(ack);

    std::memcpy(buffer, &serviceType, sizeof(serviceType));
    buffer += sizeof(serviceType);

    std::memcpy(buffer, &serviceSubtype, sizeof(serviceSubtype));
    buffer += sizeof(serviceSubtype);

    std::copy(appData.begin(), appData.end(), buffer);
  }

  std::memcpy(buffer, &packetErrorControl, sizeof(packetErrorControl));
  buffer += sizeof(packetErrorControl);
}

