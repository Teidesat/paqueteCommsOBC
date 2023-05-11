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

class Packet {
public:
  // *** constants ***
  static constexpr uint8_t VERSION_NUMBER_SIZE = 3;
  static constexpr uint8_t TYPE_SIZE = 1;
  static constexpr uint8_t DATA_FIELD_HEADER_SIZE = 1;
  static constexpr uint8_t APP_ID_SOURCE_SIZE = 5;
  static constexpr uint8_t APP_ID_DESTINATION_SIZE = 5;
  static constexpr uint8_t SEQUENCE_CONTROL_FLAGS_SIZE = 2;
  static constexpr uint8_t SEQUENCE_CONTROL_COUNT_SIZE = 14;
  static constexpr uint8_t LENGTH_SIZE = 16;
  static constexpr uint8_t CCSDS_SIZE = 1;
  static constexpr uint8_t PUS_VERSION_SIZE = 3;
  static constexpr uint8_t ACK_SIZE = 4;
  static constexpr uint8_t SERVICE_TYPE_SIZE = 8;
  static constexpr uint8_t SERVICE_SUBTYPE_SIZE = 8;
  static constexpr uint8_t PACKET_ERROR_CONTROL_SIZE = 3;
  inline static constexpr uint16_t APP_DATA_SIZE = 256; // inline for appdata array size

  static constexpr uint8_t PACKET_HEADER_SIZE =
      VERSION_NUMBER_SIZE +
      TYPE_SIZE +
      DATA_FIELD_HEADER_SIZE +
      APP_ID_SOURCE_SIZE +
      APP_ID_DESTINATION_SIZE +
      SEQUENCE_CONTROL_FLAGS_SIZE +
      SEQUENCE_CONTROL_COUNT_SIZE +
      LENGTH_SIZE +
      PACKET_ERROR_CONTROL_SIZE;

  // static constexpr uint8_t PACKET_DATA_HEADER_SIZE =
  //     CCSDS_SIZE
  //     PUS_VERSION_SIZE
  //     ACK_SIZE +
  //     SERVICE_TYPE_SIZE +
  //     SERVICE_SUBTYPE_SIZE;

  enum class SequenceFlags : uint8_t {
    INITIAL = 0b01,
    INBETWEEN = 0b00,
    FINAL = 0b10,
    STAND_ALONE = 0b11
  };

  Packet();
  Packet(
    const uint8_t versionNumber,
    const uint8_t dataFieldHeader,
    const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const uint8_t sequenceControlFlags,
    const uint8_t sequenceControlCount,
    const uint8_t length,
    const uint8_t ccsds,
    const uint8_t pusVersion,
    const uint8_t ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype,
    const std::array<uint8_t, APP_DATA_SIZE>& appData,
    const uint8_t packetErrorControl
  );

  ~Packet();

  uint8_t getVersionNumber() const;
  void setVersionNumber(const uint8_t versionNumber);

  uint8_t getDataFieldHeader() const;
  void setDataFieldHeader(const uint8_t newValue);

  uint8_t getAppIdSource() const;
  void setAppIdSource(const uint8_t newAddress);

  uint8_t getAppIdDestination() const;
  void setAppIdDestination(const uint8_t newAddress);

  uint8_t getSequenceControlFlags() const;
  void setSequenceControlFlags(const SequenceFlags newFlags);

  uint16_t getSequenceControlCount() const;
  void setSequenceControlCount(const uint16_t newCount);

  uint16_t getLength() const;
  void setLength(const uint16_t amountOfBytes);

  uint8_t getCCSDS() const;
  void setCCSDS(const uint8_t ccsds);

  uint8_t getPUSVersion() const;
  void setPUSVersion(const uint8_t pusVersion);

  uint8_t getAck() const;
  void setACK(const bool newValue);

  uint8_t getServiceType() const;
  void setServiceType(const uint8_t typeId);

  uint8_t getServiceSubtype() const;
  void setServiceSubtype(const uint8_t subtypeId);

  uint16_t getPacketErrorControl() const;
  void setPacketErrorControl(const uint16_t crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<uint8_t, APP_DATA_SIZE> getAppData();
  void setAppData(const std::array<uint8_t, APP_DATA_SIZE>& ptrNewAppData);

private:
  // main header
  uint8_t versionNumber_;
  uint8_t type_; // always 0, only one packet type
  uint8_t dataFieldHeader_;
  uint8_t appIdSource_;
  uint8_t appIdDestination_;
  uint8_t sequenceControlFlags_;
  uint16_t sequenceControlCount_;
  uint16_t length_; // amount of octets within app. data

  // data field header (optional)
  uint8_t ccsds_;
  uint8_t pusVersion_;
  uint8_t ack_;
  uint8_t serviceType_;
  uint8_t serviceSubtype_;

  // data
  std::array<uint8_t, APP_DATA_SIZE> appData_;

  // special field at the end of the packet
  uint16_t packetErrorControl_;
};
