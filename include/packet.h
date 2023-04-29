#pragma once

#include <vector>
#include <cstddef>

class Packet {
public:
  Packet(const std::byte* buffer, std::size_t size);
  ~Packet();


private:
  // main header
  std::byte versionNumber_[3];
  std::byte dataFieldHeader_[1];
  std::byte appIdSource_[5];
  std::byte appIdDestination_[5];
  std::byte sequenceControl_[16];
  std::byte length_[16];

  // data field header
  std::byte ack_[4];
  std::byte serviceType_[8];
  std::byte serviceSubtype_[8];

  // application data
  std::vector<std::byte> appData_;

  // last field
  std::byte packetErrorControl_[16];
};
