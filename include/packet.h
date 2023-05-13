/**
 * @file packet.h
 * @author Marcos Barrios
 * 
 *  Packet representation, holds the data a packet has.
 * 
 *  Follows the ECSS-E-70-41A packet structure, but only one packet and some
 *  fields are not included.
 *
 */

#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <array>
#include <vector>

class Packet {
public:
  // *** constants ***
  inline static constexpr uint16_t APP_DATA_SIZE = 256; // inline for appdata array size

  enum class SequenceFlags : uint8_t {
    INITIAL = 0b01,
    INBETWEEN = 0b00,
    FINAL = 0b10,
    STAND_ALONE = 0b11
  };

  Packet();
  Packet(
    const std::byte versionNumber,
    const std::byte dataFieldHeader,
    const std::byte appIdSource,
    const std::byte appIdDestination,
    const std::byte sequenceControlFlags,
    const std::array<std::byte, 2> sequenceControlCount,
    const std::array<std::byte, 2> length,
    const std::byte ccsds,
    const std::byte pusVersion,
    const std::byte ack,
    const std::byte serviceType,
    const std::byte serviceSubtype,
    const std::array<std::byte, APP_DATA_SIZE>& appData,
    const std::array<std::byte, 2> packetErrorControl
  );
  Packet(const Packet& other);

  ~Packet();

  std::byte getVersionNumber() const;
  void setVersionNumber(const std::byte versionNumber);

  std::byte getDataFieldHeader() const;
  void setDataFieldHeader(const std::byte newValue);

  std::byte getAppIdSource() const;
  void setAppIdSource(const std::byte newAddress);

  std::byte getAppIdDestination() const;
  void setAppIdDestination(const std::byte newAddress);

  std::byte getSequenceControlFlags() const;
  void setSequenceControlFlags(const SequenceFlags newFlags);

  std::array<std::byte, 2> getSequenceControlCount() const;
  void setSequenceControlCount(const std::array<std::byte, 2> newCount);

  std::array<std::byte, 2> getLength() const;
  void setLength(const std::array<std::byte, 2> amountOfBytes);

  std::byte getCCSDS() const;
  void setCCSDS(const std::byte ccsds);

  std::byte getPUSVersion() const;
  void setPUSVersion(const std::byte pusVersion);

  std::byte getAck() const;
  void setACK(const std::byte newValue);

  std::byte getServiceType() const;
  void setServiceType(const std::byte typeId);

  std::byte getServiceSubtype() const;
  void setServiceSubtype(const std::byte subtypeId);

  std::array<std::byte, 2> getPacketErrorControl() const;
  void setPacketErrorControl(const std::array<std::byte, 2> crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<std::byte, APP_DATA_SIZE> getAppData();
  void setAppData(const std::array<std::byte, APP_DATA_SIZE>& ptrNewAppData);

  // To avoid the client making copies of app data to substitute old app data.
  // It modifies the index for appending.
  void pushData(const std::byte bytesToPush);
  void pushData(const std::array<std::byte, 2>& bytesToPush);
  void pushData(const std::vector<std::byte>& bytesToPush);

private:
  // main header
  std::byte versionNumber_;
  std::byte type_; // always 0, only one packet type
  std::byte dataFieldHeader_;
  std::byte appIdSource_;
  std::byte appIdDestination_;
  std::byte sequenceControlFlags_;
  std::array<std::byte, 2> sequenceControlCount_;
  std::array<std::byte, 2> length_; // amount of octets within app. data

  // data field header (optional)
  std::byte ccsds_;
  std::byte pusVersion_;
  std::byte ack_;
  std::byte serviceType_;
  std::byte serviceSubtype_;

  // data
  std::array<std::byte, APP_DATA_SIZE> appData_;
  uint8_t appDataIndex_; // for app. data appending

  // special field at the end of the packet
  std::array<std::byte, 2> packetErrorControl_;
};
