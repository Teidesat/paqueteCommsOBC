#include "../include/packet_operations.h"

#include <cassert>
#include <cstring>

PacketOperations::PacketOperations() {}

PacketOperations::~PacketOperations() {}

Packet PacketOperations::readPacket(const uint8_t* buffer, std::size_t size) {
  uint8_t versionNumber;
  uint8_t dataFieldHeader;
  uint8_t appIdSource;
  uint8_t appIdDestination;
  uint8_t sequenceControlFlags;
  uint8_t sequenceControlCount;
  uint8_t length;

  uint8_t ccsds;
  uint8_t pusVersion;
  uint8_t ack;
  uint8_t serviceType;
  uint8_t serviceSubtype;

  std::array<uint8_t, Packet::APP_DATA_SIZE> appData;

  uint8_t packetErrorControl;

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

void PacketOperations::writePacket(uint8_t* buffer, Packet& packet) {
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

