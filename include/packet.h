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
      const std::bitset<VERSION_NUMBER_SIZE>&  versionNumber,
      const std::bitset<DATA_FIELD_HEADER_SIZE>& dataFieldHeader,
      const std::bitset<APP_ID_SOURCE_SIZE>& appIdSource,
      const std::bitset<APP_ID_DESTINATION_SIZE>& appIdDestination,
      const std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE>& sequenceControlFlags,
      const std::bitset<SEQUENCE_CONTROL_COUNT_SIZE>& sequenceControlCount,
      const std::bitset<LENGTH_SIZE>& length,
      const std::bitset<CCSDS_SIZE>& ccsds,
      const std::bitset<PUS_VERSION_SIZE>& pusVersion,
      const std::bitset<ACK_SIZE>& ack,
      const std::bitset<SERVICE_TYPE_SIZE>& serviceType,
      const std::bitset<SERVICE_SUBTYPE_SIZE>& serviceSubtype,
      const std::array<std::byte, APP_DATA_SIZE>& appData,
      const std::bitset<PACKET_ERROR_CONTROL_SIZE>& packetErrorControl);

  ~Packet();

  std::bitset<VERSION_NUMBER_SIZE> getVersionNumber() const;
  void setVersionNumber(const std::bitset<VERSION_NUMBER_SIZE>& versionNumber);

  std::bitset<DATA_FIELD_HEADER_SIZE> getDataFieldHeader() const;
  void setDataFieldHeader(const bool newValue);

  std::bitset<APP_ID_SOURCE_SIZE> getAppIdSource() const;
  void setAppIdSource(const std::bitset<APP_ID_SOURCE_SIZE>&  newAddress);

  std::bitset<APP_ID_DESTINATION_SIZE> getAppIdDestination() const;
  void setAppIdDestination(const std::bitset<APP_ID_DESTINATION_SIZE>& newAddress);

  std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> getSequenceControlFlags() const;
  void setSequenceControlFlags(const SequenceFlags newFlags);

  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> getSequenceControlCount() const;
  void setSequenceControlCount(const std::bitset<SEQUENCE_CONTROL_COUNT_SIZE>& newCount);

  std::bitset<LENGTH_SIZE> getLength() const;
  void setLength(const std::bitset<LENGTH_SIZE>& amountOfBytes);

  std::bitset<CCSDS_SIZE> getCCSDS() const;
  void setCCSDS(const std::bitset<CCSDS_SIZE> ccsds);

  std::bitset<PUS_VERSION_SIZE> getPUSVersion() const;
  void setPUSVersion(const std::bitset<PUS_VERSION_SIZE> pusVersion);

  std::bitset<ACK_SIZE> getAck() const;
  void setACK(const bool newValue);

  std::bitset<SERVICE_TYPE_SIZE> getServiceType() const;
  void setServiceType(const std::bitset<SERVICE_TYPE_SIZE>& typeId);

  std::bitset<SERVICE_SUBTYPE_SIZE> getServiceSubtype() const;
  void setServiceSubtype(const std::bitset<SERVICE_SUBTYPE_SIZE>& subtypeId);

  std::bitset<PACKET_ERROR_CONTROL_SIZE> getPacketErrorControl() const;
  void setPacketErrorControl(const std::bitset<PACKET_ERROR_CONTROL_SIZE>& crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<std::byte, APP_DATA_SIZE> getAppData();
  void setAppData(const std::array<std::byte, APP_DATA_SIZE>& ptrNewAppData);

private:
  // main header
  std::bitset<VERSION_NUMBER_SIZE> versionNumber_;
  std::bitset<TYPE_SIZE> type_; // always 0, only one packet type
  std::bitset<DATA_FIELD_HEADER_SIZE> dataFieldHeader_;
  std::bitset<1> empty;
  std::bitset<APP_ID_SOURCE_SIZE> appIdSource_;
  std::bitset<APP_ID_DESTINATION_SIZE> appIdDestination_;
  std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> sequenceControlFlags_;
  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> sequenceControlCount_;
  std::bitset<LENGTH_SIZE> length_; // amount of octets within app. data

  // data field header (optional)
  std::bitset<CCSDS_SIZE> ccsds_;
  std::bitset<PUS_VERSION_SIZE> pusVersion_;
  std::bitset<ACK_SIZE> ack_;
  std::bitset<SERVICE_TYPE_SIZE> serviceType_;
  std::bitset<SERVICE_SUBTYPE_SIZE> serviceSubtype_;

  // data
  std::array<std::byte, APP_DATA_SIZE> appData_;

  // special field at the end of the packet
  std::bitset<PACKET_ERROR_CONTROL_SIZE> packetErrorControl_;
};
