#include "../include/packet_buffer_io.h"

#include <cassert>
#include <cstring>
#include <array>
#include <cmath>

PacketBufferIO::PacketBufferIO() {}

Packet PacketBufferIO::readPacket(const std::byte* ptrBuffer, std::size_t size) {
  uint8_t versionNumber;
  Packet::Bool8Enum dataFieldHeader;
  uint8_t appIdSource;
  uint8_t appIdDestination;
  Packet::SequenceFlags sequenceControlFlags;
  uint16_t sequenceControlCount;
  uint16_t length;

  Packet::Bool8Enum ccsds;
  uint8_t pusVersion;
  Packet::Bool8Enum ack;
  uint8_t serviceType;
  uint8_t serviceSubtype;

  std::array<std::byte, Packet::APP_DATA_SIZE> appData;

  std::array<std::byte, 2> packetErrorControl;

  std::memcpy(&versionNumber, ptrBuffer, sizeof(versionNumber));
  ptrBuffer += sizeof(versionNumber);
  size -= sizeof(versionNumber);

  std::memcpy(&dataFieldHeader, ptrBuffer, sizeof(dataFieldHeader));
  ptrBuffer += sizeof(dataFieldHeader);
  size -= sizeof(dataFieldHeader);

  std::memcpy(&appIdSource, ptrBuffer, sizeof(appIdSource));
  ptrBuffer += sizeof(appIdSource);
  size -= sizeof(appIdSource);

  std::memcpy(&appIdDestination, ptrBuffer, sizeof(appIdDestination));
  ptrBuffer += sizeof(appIdDestination);
  size -= sizeof(appIdDestination);

  std::memcpy(&sequenceControlFlags, ptrBuffer, sizeof(sequenceControlFlags));
  ptrBuffer += sizeof(sequenceControlFlags);
  size -= sizeof(sequenceControlFlags);

  std::memcpy(&sequenceControlCount, ptrBuffer, sizeof(sequenceControlCount));
  ptrBuffer += sizeof(sequenceControlCount);
  size -= sizeof(sequenceControlCount);

  std::memcpy(&length, ptrBuffer, sizeof(length));
  ptrBuffer += sizeof(length);
  size -= sizeof(length);
  
  if (dataFieldHeader == Packet::Bool8Enum::TRUE) {
    std::memcpy(&ccsds, ptrBuffer, sizeof(ccsds));
    ptrBuffer += sizeof(ccsds);
    size -= sizeof(ccsds);

    std::memcpy(&pusVersion, ptrBuffer, sizeof(pusVersion));
    ptrBuffer += sizeof(pusVersion);
    size -= sizeof(pusVersion);

    std::memcpy(&ack, ptrBuffer, sizeof(ack));
    ptrBuffer += sizeof(ack);
    size -= sizeof(ack);

    std::memcpy(&serviceType, ptrBuffer, sizeof(serviceType));
    ptrBuffer += sizeof(serviceType);
    size -= sizeof(serviceType);

    std::memcpy(&serviceSubtype, ptrBuffer, sizeof(serviceSubtype));
    ptrBuffer += sizeof(serviceSubtype);
    size -= sizeof(serviceSubtype);
  } else {
    // default values
    ccsds = Packet::Bool8Enum::FALSE;
    pusVersion = 1;
    ack = Packet::Bool8Enum::FALSE;
    serviceType = 0;
    serviceSubtype = 0;
  }

  std::memcpy(&appData, ptrBuffer, sizeof(appData));
  ptrBuffer += sizeof(appData);
  size -= sizeof(appData);

  std::memcpy(&packetErrorControl, ptrBuffer, sizeof(packetErrorControl));
  ptrBuffer += sizeof(packetErrorControl);
  size -= sizeof(packetErrorControl);

  return Packet(versionNumber, dataFieldHeader, appIdSource, appIdDestination,
      sequenceControlFlags, sequenceControlCount, length, ccsds, pusVersion,
      ack, serviceType, serviceSubtype, appData, packetErrorControl);
}

void PacketBufferIO::writePacket(std::byte* ptrBuffer, Packet& packet) {
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

  std::memcpy(ptrBuffer, &versionNumber, sizeof(versionNumber));
  ptrBuffer += sizeof(versionNumber);

  std::memcpy(ptrBuffer, &dataFieldHeader, sizeof(dataFieldHeader));
  ptrBuffer += sizeof(dataFieldHeader);

  std::memcpy(ptrBuffer, &appIdSource, sizeof(appIdSource));
  ptrBuffer += sizeof(appIdSource);

  std::memcpy(ptrBuffer, &appIdDestination, sizeof(appIdDestination));
  ptrBuffer += sizeof(appIdDestination);

  std::memcpy(ptrBuffer, &sequenceControlFlags, sizeof(sequenceControlFlags));
  ptrBuffer += sizeof(sequenceControlFlags);

  std::memcpy(ptrBuffer, &sequenceControlCount, sizeof(sequenceControlCount));
  ptrBuffer += sizeof(sequenceControlCount);

  std::memcpy(ptrBuffer, &length, sizeof(length));
  ptrBuffer += sizeof(length);
  
  if (dataFieldHeader == Packet::Bool8Enum::TRUE) {
    std::memcpy(ptrBuffer, &ccsds, sizeof(ccsds));
    ptrBuffer += sizeof(ccsds);

    std::memcpy(ptrBuffer, &pusVersion, sizeof(pusVersion));
    ptrBuffer += sizeof(pusVersion);

    std::memcpy(ptrBuffer, &ack, sizeof(ack));
    ptrBuffer += sizeof(ack);

    std::memcpy(ptrBuffer, &serviceType, sizeof(serviceType));
    ptrBuffer += sizeof(serviceType);

    std::memcpy(ptrBuffer, &serviceSubtype, sizeof(serviceSubtype));
    ptrBuffer += sizeof(serviceSubtype);
  }
  std::copy(appData.begin(), appData.end(), ptrBuffer);

  std::memcpy(ptrBuffer, &packetErrorControl, sizeof(packetErrorControl));
  ptrBuffer += sizeof(packetErrorControl);
}

template<typename T>
T PacketBufferIO::extractFieldFrom(T inputWord, uint8_t startIndex,
    size_t lengthOfField) {
  return (inputWord >> (startIndex % 8)) & (std::pow(2, lengthOfField) - 1);
}