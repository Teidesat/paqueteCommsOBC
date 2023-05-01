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
#include <string>
#include <bitset>

class Packet {
public:
  Packet(const std::byte* buffer, std::size_t size);
  ~Packet();

  // getters and setters
  std::bitset<3> getVersionNumber();
  std::bitset<1> getDataFieldHeader();
  std::bitset<5> getAppIdSource();
  std::bitset<5> getAppIdDestination();
  std::bitset<16> getSequenceControl();
  std::bitset<16> getLength();

  std::bitset<4> getAck();
  std::bitset<8> getServiceType();
  std::bitset<8> getServiceSubtype();

  std::bitset<3> getPacketErrorControl();
private:
  // main header
  std::bitset<3> versionNumber_;
  std::bitset<1> dataFieldHeader_;
  std::bitset<5> appIdSource_;
  std::bitset<5> appIdDestination_;
  std::bitset<16> sequenceControl_;
  std::bitset<16> length_;

  // data field header
  std::bitset<4> ack_;
  std::bitset<8> serviceType_;
  std::bitset<8> serviceSubtype_;

  // application data
  std::vector<std::byte> appData_;

  // last field
  std::bitset<3> packetErrorControl_;
};
