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
  Packet(const std::byte* buffer, std::size_t size);
  ~Packet();

  // *** getters ***
  std::bitset<3> getVersionNumber() const;
  std::bitset<1> getDataFieldHeader() const;
  std::bitset<5> getAppIdSource() const;
  std::bitset<5> getAppIdDestination() const;
  std::bitset<16> getSequenceControl() const;
  std::bitset<16> getLength() const;

  std::bitset<4> getAck() const;
  std::bitset<8> getServiceType() const;
  std::bitset<8> getServiceSubtype() const;

  std::vector<std::byte> getAppData() const;

  std::bitset<3> getPacketErrorControl() const;

  // *** constants ***
  static constexpr std::size_t VERSION_NUMBER_SIZE = 3;
  static constexpr std::size_t DATA_FIELD_HEADER_SIZE = 1;
  static constexpr std::size_t APP_ID_SOURCE_SIZE = 5;
  static constexpr std::size_t APP_ID_DESTINATION_SIZE = 5;
  static constexpr std::size_t SEQUENCE_CONTROL_SIZE = 16;
  static constexpr std::size_t LENGTH_SIZE = 16;
  static constexpr std::size_t ACK_SIZE = 4;
  static constexpr std::size_t SERVICE_TYPE_SIZE = 8;
  static constexpr std::size_t SERVICE_SUBTYPE_SIZE = 8;
  static constexpr std::size_t PACKET_ERROR_CONTROL_SIZE = 3;

  static constexpr std::size_t PACKET_HEADER_SIZE =
      VERSION_NUMBER_SIZE +
      DATA_FIELD_HEADER_SIZE +
      APP_ID_SOURCE_SIZE +
      APP_ID_DESTINATION_SIZE +
      SEQUENCE_CONTROL_SIZE +
      LENGTH_SIZE +
      PACKET_ERROR_CONTROL_SIZE;

  static constexpr std::size_t PACKET_DATA_HEADER_SIZE =
      ACK_SIZE +
      SERVICE_TYPE_SIZE +
      SERVICE_SUBTYPE_SIZE;
private:
  // main header
  std::bitset<VERSION_NUMBER_SIZE> versionNumber_;
  std::bitset<DATA_FIELD_HEADER_SIZE> dataFieldHeader_;
  std::bitset<APP_ID_SOURCE_SIZE> appIdSource_;
  std::bitset<APP_ID_DESTINATION_SIZE> appIdDestination_;
  std::bitset<SEQUENCE_CONTROL_SIZE> sequenceControl_;
  std::bitset<LENGTH_SIZE> length_;

  // data field header
  std::bitset<ACK_SIZE> ack_;
  std::bitset<SERVICE_TYPE_SIZE> serviceType_;
  std::bitset<SERVICE_SUBTYPE_SIZE> serviceSubtype_;

  std::vector<std::byte> appData_;

  // main header
  std::bitset<PACKET_ERROR_CONTROL_SIZE> packetErrorControl_;
};
