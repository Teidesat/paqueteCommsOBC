#include "../include/packet_buffer_io.h"

#include <cassert>
#include <cstring>
#include <array>
#include <cmath>

/**
 * Some bytes have multiple fields. Depending on if the word has multiple
 *    fields or not I will increase or not the ptrBuffer.
 * 
 *  Map of byte to fields:
 * XXX X X 0 XX...	|  std::byte =  version, type, dfh, 0, source...
 * ...XXX XXXXX	    |  std::byte =  ...source, destination
 * XX XXXXXX...	    |  std::byte =  seq_ctrl_flags, seq_ctrl_count...
 * ...XXXXXXXX	    |  std::byte =  ...seq_ctrl_count
 * XXXXXXXX...	    |  std::byte =  length...
 * ...XXXXXXXX	    |  std::byte =  ...length
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
Packet PacketBufferIO::readPacket(const std::byte* ptrBuffer) {
  // **** TODO: measure if coarcing to bitset<> is faster for bit indexing than current method.

  const uint8_t versionNumber = (static_cast<uint8_t>(ptrBuffer[0] >> 5));

  bool dataFieldHeader;

  if (extractFieldFrom(ptrBuffer[0], 3, 1) == 1) {
    dataFieldHeader = true;
  } else {
    dataFieldHeader = false;
  }

  // 000000XX, 00000XXX -> 000XX000, 00000XXX -> do OR.
  uint8_t appIdSourcePartial1 = extractFieldFrom(ptrBuffer[0], 0, 3);
  uint8_t appIdSourcePartial2 = static_cast<uint8_t>(ptrBuffer[1] >> 5);
  uint8_t appIdSource = (appIdSourcePartial1 << 3) | appIdSourcePartial2;

  uint8_t appIdDestination = extractFieldFrom(ptrBuffer[1], 0, 5);

  uint8_t flags = static_cast<uint8_t>(ptrBuffer[2] >> 6);
  Packet::SequenceFlags sequenceControlFlags =
      static_cast<Packet::SequenceFlags>(flags);
  
  uint8_t countMostSignificant = extractFieldFrom(ptrBuffer[2], 0, 6);
  uint8_t countLeastSignificant = static_cast<uint8_t>(ptrBuffer[3]);
  uint16_t sequenceControlCount =
      static_cast<uint16_t>(countMostSignificant << 8) | countLeastSignificant;

  // **** TODO: measure whether it is faster to cast or to use a pointer with memcpy.

  uint16_t length =
      (static_cast<uint16_t>(ptrBuffer[4]) << 8) |
      static_cast<uint8_t>(ptrBuffer[5]);

  // From here I do one thing or another depending on if there is data
  // field header.

  bool ccsds;
  uint8_t pusVersion;
  bool ack;
  uint8_t serviceType;
  uint8_t serviceSubtype;

  size_t amountOfDataInAppData;
  size_t indexOfAppDataStart;
  if (dataFieldHeader == true) {
    if (extractFieldFrom(ptrBuffer[6], 7, 1) == 1) {
      ccsds = true;
    } else {
      ccsds = false;
    }
    pusVersion = extractFieldFrom(ptrBuffer[6], 4, 3);
    if (extractFieldFrom(ptrBuffer[6], 0, 4) == 1) {
      ack = true;
    } else {
      ack = false;
    }
    serviceType = static_cast<uint8_t>(ptrBuffer[7]);
    serviceSubtype = static_cast<uint8_t>(ptrBuffer[8]);
  
    amountOfDataInAppData = length - 3 - 2; // - datafieldheader, - errorcontrol
    indexOfAppDataStart = 9; // 9 because main header + dataFieldHeader
  } else {
    ccsds = false;
    pusVersion = 1;
    ack = false;
    serviceType = 0;
    serviceSubtype = 0;

    amountOfDataInAppData = length - 2; // - errorcontrol
    indexOfAppDataStart = 6; // 6 because only main header
  }

  std::array<uint8_t, Packet::APP_DATA_SIZE> appData{};

  // insert the app data of buffer into appData array.
  for (size_t i = 0; i < amountOfDataInAppData; ++i) {
    appData[i] = static_cast<uint8_t>(ptrBuffer[indexOfAppDataStart + i]);
  }

  // initialize the filler app data array bytes to 0
  for (size_t i = 0; i < Packet::APP_DATA_SIZE - amountOfDataInAppData; ++i) {
    appData[amountOfDataInAppData + i] = static_cast<uint8_t>(0);
  }

  std::array<uint8_t, 2> packetErrorControl{};
  packetErrorControl[1] = static_cast<uint8_t>(ptrBuffer[indexOfAppDataStart + amountOfDataInAppData]);
  packetErrorControl[0] = static_cast<uint8_t>(ptrBuffer[indexOfAppDataStart + amountOfDataInAppData + 1]);

  return {versionNumber, dataFieldHeader, appIdSource, appIdDestination,
          sequenceControlFlags, sequenceControlCount, length, ccsds, pusVersion,
          ack, serviceType, serviceSubtype, appData, packetErrorControl};
}

/**
 * See readPacket() for the structure.
 * 
 * I shift the words around so that a final OR results in the wanted byte.
*/
void PacketBufferIO::writePacket(std::byte* ptrBuffer, Packet& packet) {

  //main header
  // [0]
  std::byte versionNumber = static_cast<std::byte>(packet.getVersionNumber() << 5);

  // type is zero and app id has a 0 as first bit, so position
  // dataFieldHeader's bit tactically so that the final OR leaves those 2 zeros.

  std::byte dataFieldHeader =
      static_cast<std::byte>(packet.getDataFieldHeader()) << 3;
  std::byte source = static_cast<std::byte>(packet.getAppIdSource());
  std::byte appIdSourceMostSignificantPartial = source >> 3;

  // [1]
  std::byte appIdSourceLeastSignificantPartial =
      static_cast<std::byte>(extractFieldFrom(source, 0, 3) << 5);
  std::byte destination = static_cast<std::byte>(packet.getAppIdDestination());
  std::byte appIdDestination =
      static_cast<std::byte>(extractFieldFrom(destination, 0, 5));

  // [2]
  std::byte sequenceControlFlags =
      static_cast<std::byte>(packet.getSequenceControlFlags()) << 6;
  std::byte sequenceControlCountMostSignificant =
      static_cast<std::byte>(packet.getSequenceControlCount() >> 8);
  
  // [3]
  std::byte sequenceControlCountLeastSignificant =
      static_cast<std::byte>(packet.getSequenceControlCount());

  // [4]
  std::byte lengthMostSignificant =
      static_cast<std::byte>(packet.getLength() >> 8);

  // [5]
  std::byte lengthLeastSignificant =
      static_cast<std::byte>(packet.getLength());

  // write the main header bytes
  ptrBuffer[0] = versionNumber | dataFieldHeader | appIdSourceMostSignificantPartial;
  ptrBuffer[1] = appIdSourceLeastSignificantPartial | appIdDestination;
  ptrBuffer[2] = sequenceControlFlags | sequenceControlCountMostSignificant;
  ptrBuffer[3] = sequenceControlCountLeastSignificant;
  ptrBuffer[4] = lengthMostSignificant;
  ptrBuffer[5] = lengthLeastSignificant;

  const auto& appData = packet.getAppData();
  size_t amountOfDataInAppData;
  size_t indexOfAppDataStart;
  if (packet.getDataFieldHeader() == true) {
    // data field header
    // [6]
    std::byte ccsds = static_cast<std::byte>(packet.getCCSDS()) << 7;
    std::byte pusVersion = static_cast<std::byte>(packet.getPUSVersion()) << 4;
    std::byte ack = static_cast<std::byte>(packet.getAck());
    
    // [7]
    std::byte serviceType = static_cast<std::byte>(packet.getServiceType());

    // [8]
    std::byte serviceSubtype = static_cast<std::byte>(packet.getServiceSubtype());

    ptrBuffer[6] = ccsds | pusVersion | ack;
    ptrBuffer[7] = serviceType;
    ptrBuffer[8] = serviceSubtype;

    amountOfDataInAppData = packet.getLength() - 3 - 2; // - datafieldheader, - errorcontrol
    indexOfAppDataStart = 9;
  } else {
    amountOfDataInAppData = packet.getLength() - 2; // - errorcontrol
    indexOfAppDataStart = 6;
  }

  // write app data to buffer
  for (int i = 0; i < amountOfDataInAppData; ++i) {
    ptrBuffer[indexOfAppDataStart + i] = std::byte(appData[i]);
  }

  const auto packetErrorControl = packet.getPacketErrorControl();
  ptrBuffer[indexOfAppDataStart + amountOfDataInAppData] = std::byte(packetErrorControl[1]);
  ptrBuffer[indexOfAppDataStart + amountOfDataInAppData + 1] = std::byte(packetErrorControl[0]);
}

// Example for extractFieldFrom(0b00101000, 3, 3):
//   1.- 0b110101000 >> 3 = 0b00110101
//   2.- std:pow(2, 3) = 0b00001000, 0b00001000 - 1 = 0b00000111
//   3.- 0b00110101 & 0b00000111 = 0b00000101     <- returns that
uint8_t PacketBufferIO::extractFieldFrom(std::byte inputByte, uint8_t startIndex,
    size_t lengthOfField) {
  return static_cast<uint8_t>(
    (inputByte >> (startIndex % 8)) & std::byte((std::pow(2, lengthOfField) - 1)));
}
