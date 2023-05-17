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
    const uint8_t versionNumber,
    const bool dataFieldHeader,
    const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const SequenceFlags sequenceControlFlags,
    const uint16_t sequenceControlCount,
    const uint16_t length,
    const bool ccsds,
    const uint8_t pusVersion,
    const bool ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype,
    const std::array<std::byte, APP_DATA_SIZE>& appData,
    const std::array<std::byte, 2> packetErrorControl
  );
  Packet(const Packet& other);

  ~Packet();

  uint8_t getVersionNumber() const;
  void setVersionNumber(const uint8_t versionNumber);

  bool getDataFieldHeader() const;
  void setDataFieldHeader(const bool newValue);

  uint8_t getAppIdSource() const;
  void setAppIdSource(const uint8_t newAddress);

  uint8_t getAppIdDestination() const;
  void setAppIdDestination(const uint8_t newAddress);

  SequenceFlags getSequenceControlFlags() const;
  void setSequenceControlFlags(const SequenceFlags newFlags);

  uint16_t getSequenceControlCount() const;
  void setSequenceControlCount(const uint16_t newCount);

  uint16_t getLength() const;
  void setLength(const uint16_t amountOfBytes);

  bool getCCSDS() const;
  void setCCSDS(const bool ccsds);

  uint8_t getPUSVersion() const;
  void setPUSVersion(const uint8_t pusVersion);

  bool getAck() const;
  void setACK(const bool newValue);

  uint8_t getServiceType() const;
  void setServiceType(const uint8_t typeId);

  uint8_t getServiceSubtype() const;
  void setServiceSubtype(const uint8_t subtypeId);

  std::array<std::byte, 2> getPacketErrorControl() const;
  void setPacketErrorControl(const std::array<std::byte, 2> crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<std::byte, APP_DATA_SIZE> getAppData();
  void setAppData(const std::array<std::byte, APP_DATA_SIZE>& ptrNewAppData);

  // To avoid the client making copies of app data to substitute old app data.
  // It modifies the index for appending.
  void pushData(const std::byte bytesToPush);
  void pushData(const uint16_t bytesToPush);
  void pushData(const std::array<std::byte, 2>& bytesToPush);
  void pushData(const std::vector<std::byte>& bytesToPush);

private:
  // main header
  uint8_t versionNumber_;
  bool dataFieldHeader_;
  uint8_t appIdSource_;
  uint8_t appIdDestination_;
  SequenceFlags sequenceControlFlags_;
  uint16_t sequenceControlCount_;
  uint16_t length_; // amount of octets within app. data

  // data field header (optional)
  bool ccsds_;
  uint8_t pusVersion_;
  bool ack_;
  uint8_t serviceType_;
  uint8_t serviceSubtype_;

  // data
  std::array<std::byte, APP_DATA_SIZE> appData_;
  uint8_t appDataIndex_; // for app. data appending

  // special field at the end of the packet
  std::array<std::byte, 2> packetErrorControl_;
};
