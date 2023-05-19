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
 * XXX X X XXX...	  |  std::byte =  version, type, dfh, 0, source...
 * ...XX XXXXX X...	|  std::byte =  ...source, destination, length...
 * ...XXXXXXXX...	  |  std::byte =  ...length...
 * ...XXXXXXX X...	|  std::byte =  ...length, seq_ctrl_flags...
 * ...X XXXXXXX...	|  std::byte =  ...seq_ctrl_flags, seq_ctrl_count...
 * ...XXXXXXXX...	  |  std::byte =  ...seq_ctrl_count...
 * ...X X XXX XXX...|  std::byte =  ...seq_ctrl_count, ccsds, pus, ack...
 * ...X XXXXXXX...	|  std::byte =  ...ack, type...
 * ...X XXXXXXX...	|  std::byte =  ...type, subtype...
 * ...X XXXXXXX...	|  std::byte =  ...subtype app data(...)
 * 
 * Note: extractFieldFrom() assumes that the rightmost bit is index 0 on
 *    each byte.
 */
Packet PacketBufferIO::readPacket(const std::byte* ptrBuffer, std::size_t size) {

  // **** TODO: measure if coarcing to bitset<> is faster for bit indexing than current method.

  const uint8_t versionNumber = (extractFieldFrom(ptrBuffer[0], 5, 3));

  Packet::Bool8Enum dataFieldHeader;

  if (extractFieldFrom(ptrBuffer[0], 4, 1) == 1) {
    dataFieldHeader = Packet::Bool8Enum::TRUE;
  } else {
    dataFieldHeader = Packet::Bool8Enum::FALSE;
  }

  // extract field gives: 00000XXX, 000000XX, then do:
  // 000XXX00, 000000XX -> do OR.
  uint8_t appIdSourcePartial1 = extractFieldFrom(ptrBuffer[0], 0, 3);
  uint8_t appIdSourcePartial2 = static_cast<uint8_t>(ptrBuffer[1] >> 6);
  uint8_t appIdSource = (appIdSourcePartial1 << 2) | appIdSourcePartial2;

  uint8_t appIdDestination = extractFieldFrom(ptrBuffer[1], 1, 5);

  // for length, because it's spread in 3 different bytes, I extract 3 not 2
  uint8_t lengthPartial1 = static_cast<uint8_t>(ptrBuffer[1] << 7);
  uint8_t lengthPartial2 = static_cast<uint8_t>(ptrBuffer[2] >> 1);
  uint8_t lengthPartial3 = static_cast<uint8_t>(ptrBuffer[3] >> 1);

  // because length is 2 bytes and not 1 then calculate first least significant
  // and after that the more significant, then memcpy on the allocation.
  // ptrBuffer[2] is all length content, I need to push the Y to ptrBuffer[3]:
  // XXXXXXXY, XXXXXXX0 -> Y0000000, XXXXXXX0 -> Y0000000, 0XXXXXXX -> do OR.
  uint8_t lengthLeastSignificant =
      static_cast<uint8_t>(ptrBuffer[2] << 7) | (lengthPartial3 >> 1);

  // 0000000X, XXXXXXXX -> X0000000, XXXXXXXX -> X0000000, 0XXXXXXX -> do OR.
  uint8_t lengthMostSignificant = (lengthPartial1 << 8) | (lengthPartial2 >> 1);

  // **** TODO: measure whether it is faster to cast or to use a pointer with memcpy.
  uint16_t length =
      (static_cast<uint16_t>(lengthMostSignificant) << 8) | lengthLeastSignificant;

  uint8_t flags1 = extractFieldFrom(ptrBuffer[3], 0, 1);
  uint8_t flags2 = extractFieldFrom(ptrBuffer[4], 7, 1);
  uint8_t flags = (flags1 << 1) | flags2;
  Packet::SequenceFlags sequenceControlFlags =
      static_cast<Packet::SequenceFlags>(flags);
  
  uint16_t sequenceControlCount;

  // 0XXXXXXX, XXXXXXXX -> XXXXXXX0, XXXXXXXX -> XXXXXXX0, 0000000X -> do OR.
  uint8_t countMostSignificant = static_cast<uint8_t>((ptrBuffer[4] << 1)) |
                                 static_cast<uint8_t>((ptrBuffer[5] >> 7));
  
  // XXXXXXXX, X0000000 -> XXXXXXX0, X0000000 -> do OR.
  uint8_t countLeastSignificant = static_cast<uint8_t>((ptrBuffer[5] << 1)) |
                                  static_cast<uint8_t>((ptrBuffer[6] >> 7));

  Packet::Bool8Enum ccsds;
  if (extractFieldFrom(ptrBuffer[6], 6, 1) == 1) {
    ccsds = Packet::Bool8Enum::TRUE;
  } else {
    ccsds = Packet::Bool8Enum::FALSE;
  }
  uint8_t pusVersion = extractFieldFrom(ptrBuffer[6], 3, 3);

  uint8_t ackPartial1 = extractFieldFrom(ptrBuffer[6], 0, 3);
  uint8_t ackPartial2 = static_cast<uint8_t>(ptrBuffer[7] >> 7);
  uint8_t ackNumber = (ackPartial1 << 1) | ackPartial2;
  Packet::Bool8Enum ack;
  if (ackNumber == 1) {
    ack = Packet::Bool8Enum::TRUE;
  } else {
    ack = Packet::Bool8Enum::FALSE;
  }

  uint8_t serviceType = static_cast<uint8_t>(ptrBuffer[7] << 1) |
                        static_cast<uint8_t>(ptrBuffer[8] >> 7);
  uint8_t serviceSubtype = static_cast<uint8_t>(ptrBuffer[8] << 1) |
                           static_cast<uint8_t>(ptrBuffer[9] >> 7);

  std::array<std::byte, Packet::APP_DATA_SIZE> appData;

  std::array<std::byte, 2> packetErrorControl;

  // std::memcpy(&versionNumber, ptrBuffer, sizeof(versionNumber));
  // ptrBuffer += sizeof(versionNumber);
  // size -= sizeof(versionNumber);

  // std::memcpy(&dataFieldHeader, ptrBuffer, sizeof(dataFieldHeader));
  // ptrBuffer += sizeof(dataFieldHeader);
  // size -= sizeof(dataFieldHeader);

  // std::memcpy(&appIdSource, ptrBuffer, sizeof(appIdSource));
  // ptrBuffer += sizeof(appIdSource);
  // size -= sizeof(appIdSource);

  // std::memcpy(&appIdDestination, ptrBuffer, sizeof(appIdDestination));
  // ptrBuffer += sizeof(appIdDestination);
  // size -= sizeof(appIdDestination);

  // std::memcpy(&sequenceControlFlags, ptrBuffer, sizeof(sequenceControlFlags));
  // ptrBuffer += sizeof(sequenceControlFlags);
  // size -= sizeof(sequenceControlFlags);

  // std::memcpy(&sequenceControlCount, ptrBuffer, sizeof(sequenceControlCount));
  // ptrBuffer += sizeof(sequenceControlCount);
  // size -= sizeof(sequenceControlCount);

  // std::memcpy(&length, ptrBuffer, sizeof(length));
  // ptrBuffer += sizeof(length);
  // size -= sizeof(length);
  
  // if (dataFieldHeader == Packet::Bool8Enum::TRUE) {
  //   std::memcpy(&ccsds, ptrBuffer, sizeof(ccsds));
  //   ptrBuffer += sizeof(ccsds);
  //   size -= sizeof(ccsds);

  //   std::memcpy(&pusVersion, ptrBuffer, sizeof(pusVersion));
  //   ptrBuffer += sizeof(pusVersion);
  //   size -= sizeof(pusVersion);

  //   std::memcpy(&ack, ptrBuffer, sizeof(ack));
  //   ptrBuffer += sizeof(ack);
  //   size -= sizeof(ack);

  //   std::memcpy(&serviceType, ptrBuffer, sizeof(serviceType));
  //   ptrBuffer += sizeof(serviceType);
  //   size -= sizeof(serviceType);

  //   std::memcpy(&serviceSubtype, ptrBuffer, sizeof(serviceSubtype));
  //   ptrBuffer += sizeof(serviceSubtype);
  //   size -= sizeof(serviceSubtype);
  // } else {
  //   // default values
  //   ccsds = Packet::Bool8Enum::FALSE;
  //   pusVersion = 1;
  //   ack = Packet::Bool8Enum::FALSE;
  //   serviceType = 0;
  //   serviceSubtype = 0;
  // }

  // std::memcpy(&appData, ptrBuffer, sizeof(appData));
  // ptrBuffer += sizeof(appData);
  // size -= sizeof(appData);

  // std::memcpy(&packetErrorControl, ptrBuffer, sizeof(packetErrorControl));
  // ptrBuffer += sizeof(packetErrorControl);
  // size -= sizeof(packetErrorControl);

  // return Packet(versionNumber, dataFieldHeader, appIdSource, appIdDestination,
  //     sequenceControlFlags, sequenceControlCount, length, ccsds, pusVersion,
  //     ack, serviceType, serviceSubtype, appData, packetErrorControl);
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

