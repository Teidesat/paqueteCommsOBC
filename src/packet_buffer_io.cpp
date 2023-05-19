#include "../include/packet_buffer_io.h"

#include <cassert>
#include <cstring>
#include <array>
#include <cmath>

PacketBufferIO::PacketBufferIO() {}

/**
 * Some bytes have multiple fields. Depending on if the word has multiple
 *    fields or not I will increase or not the ptrBuffer.
 * 
 *  Map of byte to fields:
 * XXX X X 0 XX...	|  std::byte =  version, type, dfh, 0, source...
 * ...XXX XXXXX	    |  std::byte =  ...source, destination
 * XXXXXXXX...	    |  std::byte =  length...
 * ...XXXXXXXX	    |  std::byte =  ...length
 * XX XXXXXX...	    |  std::byte =  seq_ctrl_flags, seq_ctrl_count...
 * ...XXXXXXXX	    |  std::byte =  ...seq_ctrl_count
 * X XXX XXXX       |  std::byte =  ccsds, pus, ack
 * XXXXXXXX     	  |  std::byte =  type
 * .XXXXXXX...	    |  std::byte =  subtype
 * 
 * (...)            |  (application data)
 * 
 * 00000000...      !  std::byte =  error control... (all 0 this byte)
 * XXXXXXXX         !  std::byte =  ...error control
 * 
 * Note: extractFieldFrom() assumes that the rightmost bit is index 0 on
 *    each byte.
 */
Packet PacketBufferIO::readPacket(const std::byte* ptrBuffer, std::size_t size) {

  // **** TODO: measure if coarcing to bitset<> is faster for bit indexing than current method.

  const uint8_t versionNumber = (static_cast<uint8_t>(ptrBuffer[0] >> 5));

  Packet::Bool8Enum dataFieldHeader;

  if (extractFieldFrom(ptrBuffer[0], 3, 1) == 1) {
    dataFieldHeader = Packet::Bool8Enum::TRUE;
  } else {
    dataFieldHeader = Packet::Bool8Enum::FALSE;
  }

  // 000000XX, 00000XXX -> 000XX000, 00000XXX -> do OR.
  uint8_t appIdSourcePartial1 = extractFieldFrom(ptrBuffer[0], 0, 3);
  uint8_t appIdSourcePartial2 = static_cast<uint8_t>(ptrBuffer[1] >> 5);
  uint8_t appIdSource = (appIdSourcePartial1 << 3) | appIdSourcePartial2;

  uint8_t appIdDestination = extractFieldFrom(ptrBuffer[1], 0, 5);

  // because length is 2 bytes and not 1 then calculate first least significant
  // and after that the more significant, then memcpy on the allocation.

  // **** TODO: measure whether it is faster to cast or to use a pointer with memcpy.
  uint16_t length =
      (static_cast<uint16_t>(ptrBuffer[2]) << 8) |
      static_cast<uint8_t>(ptrBuffer[3]);

  uint8_t flags = static_cast<uint8_t>(ptrBuffer[4] >> 6);
  Packet::SequenceFlags sequenceControlFlags =
      static_cast<Packet::SequenceFlags>(flags);
  
  uint8_t countMostSignificant = extractFieldFrom(ptrBuffer[4], 0, 6);
  uint8_t countLeastSignificant = static_cast<uint8_t>(ptrBuffer[5]);
  uint16_t sequenceControlCount =
      static_cast<uint16_t>(countMostSignificant << 8) | countLeastSignificant;

  size -= 6;
  size_t packetIndex = 6;

  Packet::Bool8Enum ccsds;
  uint8_t pusVersion;
  Packet::Bool8Enum ack;
  uint8_t serviceType;
  uint8_t serviceSubtype;
  if (dataFieldHeader == Packet::Bool8Enum::TRUE) {
    if (extractFieldFrom(ptrBuffer[6], 7, 1) == 1) {
      ccsds = Packet::Bool8Enum::TRUE;
    } else {
      ccsds = Packet::Bool8Enum::FALSE;
    }
    pusVersion = extractFieldFrom(ptrBuffer[6], 4, 3);
    if (extractFieldFrom(ptrBuffer[6], 0, 4) == 1) {
      ack = Packet::Bool8Enum::TRUE;
    } else {
      ack = Packet::Bool8Enum::FALSE;
    }
    serviceType = static_cast<uint8_t>(ptrBuffer[7]);
    serviceSubtype = static_cast<uint8_t>(ptrBuffer[8]);

    size -= 3;
    packetIndex = 9;
  } else {
    ccsds = Packet::Bool8Enum::FALSE;
    pusVersion = 1;
    ack = Packet::Bool8Enum::FALSE;
    serviceType = 0;
    serviceSubtype = 0;
  }

  std::array<std::byte, Packet::APP_DATA_SIZE> appData;

  while (size > 2) {
    appData[packetIndex] = ptrBuffer[packetIndex];
    ++packetIndex;
  }

  std::array<std::byte, 2> packetErrorControl;
  packetErrorControl[0] = ptrBuffer[packetIndex];
  ++packetIndex;
  packetErrorControl[1] = ptrBuffer[packetIndex];
  ++packetIndex;

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

uint8_t PacketBufferIO::extractFieldFrom(std::byte inputByte, uint8_t startIndex,
    size_t lengthOfField) {
  return static_cast<uint8_t>(
    (inputByte >> (startIndex % 8)) & std::byte((std::pow(2, lengthOfField) - 1)));
}

// std::array<std::byte, 2> PacketBufferIO::combineFields(const std::byte* inputArray,
//     size_t sizeOfArray) {
  
// }

// uint16_t combineFields(const uint8_t* inputArray, size_t sizeOfArray) {

//   return 0;
// }

