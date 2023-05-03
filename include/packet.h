/**
 * @file packet.h
 * @author Marcos Barrios
 * @brief 
 * @version 0.1
 * @date 2023-05-01
 * 
 * @copyright Copyright (c) 2023
 * 
 *  Packet representation, holds the data a packet has.
 *
 */

#pragma once

#include <vector>
#include <bitset>

class Packet {
public:
  // *** constants ***
  static constexpr uint8_t VERSION_NUMBER_SIZE = 3;
  static constexpr uint8_t DATA_FIELD_HEADER_SIZE = 1;
  static constexpr uint8_t APP_ID_SOURCE_SIZE = 5;
  static constexpr uint8_t APP_ID_DESTINATION_SIZE = 5;
  static constexpr uint8_t SEQUENCE_CONTROL_FLAGS_SIZE = 2;
  static constexpr uint8_t SEQUENCE_CONTROL_COUNT_SIZE = 14;
  static constexpr uint8_t LENGTH_SIZE = 16;
  static constexpr uint8_t ACK_SIZE = 4;
  static constexpr uint8_t SERVICE_TYPE_SIZE = 8;
  static constexpr uint8_t SERVICE_SUBTYPE_SIZE = 8;
  static constexpr uint8_t PACKET_ERROR_CONTROL_SIZE = 3;

  static constexpr uint8_t PACKET_HEADER_SIZE =
      VERSION_NUMBER_SIZE +
      DATA_FIELD_HEADER_SIZE +
      APP_ID_SOURCE_SIZE +
      APP_ID_DESTINATION_SIZE +
      SEQUENCE_CONTROL_FLAGS_SIZE +
      SEQUENCE_CONTROL_COUNT_SIZE +
      LENGTH_SIZE +
      PACKET_ERROR_CONTROL_SIZE;

  static constexpr uint8_t PACKET_DATA_HEADER_SIZE =
      ACK_SIZE +
      SERVICE_TYPE_SIZE +
      SERVICE_SUBTYPE_SIZE;

  enum class AckState : uint8_t {
    PACKET_ACCEPTED = 1,
    EXECUTION_STARTED = 2,
    EXECUTION_PROGRESSED = 3,
    EXECUTION_COMPLETED = 4
  };

  // *** constructor ***
  Packet(std::bitset<VERSION_NUMBER_SIZE>  const& versionNumber,
      std::bitset<DATA_FIELD_HEADER_SIZE> const& dataFieldHeader,
      std::bitset<APP_ID_SOURCE_SIZE> const& appIdSource,
      std::bitset<APP_ID_DESTINATION_SIZE> const& appIdDestination,
      std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> const& sequenceControlFlags,
      std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> const& sequenceControlCount,
      std::bitset<LENGTH_SIZE> const& length,
      std::bitset<ACK_SIZE> const& ack,
      std::bitset<SERVICE_TYPE_SIZE> const& serviceType,
      std::bitset<SERVICE_SUBTYPE_SIZE> const& serviceSubtype,
      std::vector<std::byte> const& appData,
      std::bitset<PACKET_ERROR_CONTROL_SIZE> const& packetErrorControl);

  ~Packet();

  // *** getters ***
  std::bitset<VERSION_NUMBER_SIZE> getVersionNumber() const;
  std::bitset<DATA_FIELD_HEADER_SIZE> getDataFieldHeader() const;
  std::bitset<APP_ID_SOURCE_SIZE> getAppIdSource() const;
  std::bitset<APP_ID_DESTINATION_SIZE> getAppIdDestination() const;
  std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> getSequenceControlFlags() const;
  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> getSequenceControlCount() const;
  std::bitset<LENGTH_SIZE> getLength() const;

  std::bitset<ACK_SIZE> getAck() const;
  std::bitset<SERVICE_TYPE_SIZE> getServiceType() const;
  std::bitset<SERVICE_SUBTYPE_SIZE> getServiceSubtype() const;

  std::vector<std::byte> getAppData() const;

  std::bitset<PACKET_ERROR_CONTROL_SIZE> getPacketErrorControl() const;

  // *** setters ***
  void setACK(const AckState newState);
  

private:
  // main header
  std::bitset<VERSION_NUMBER_SIZE> versionNumber_;
  std::bitset<DATA_FIELD_HEADER_SIZE> dataFieldHeader_;
  std::bitset<APP_ID_SOURCE_SIZE> appIdSource_;
  std::bitset<APP_ID_DESTINATION_SIZE> appIdDestination_;
  std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> sequenceControlFlags_;
  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> sequenceControlCount_;
  std::bitset<LENGTH_SIZE> length_;

  // data field header (optional)
  std::bitset<ACK_SIZE> ack_;
  std::bitset<SERVICE_TYPE_SIZE> serviceType_;
  std::bitset<SERVICE_SUBTYPE_SIZE> serviceSubtype_;

  // data
  std::vector<std::byte> appData_;

  // main header too
  std::bitset<PACKET_ERROR_CONTROL_SIZE> packetErrorControl_;
};
