#include "../include/packet.h"

#include <cstring>

Packet::Packet(const std::byte* buffer, std::size_t size) {
  std::memcpy(&versionNumber_, buffer, 3);
  buffer += 3;
  size -= 3;

  std::memcpy(&dataFieldHeader_, buffer, 1);
  buffer += 1;
  size -= 1;

  std::memcpy(&appIdSource_, buffer, 5);
  buffer += 5;
  size -= 5;

  std::memcpy(&appIdDestination_, buffer, 5);
  buffer += 5;
  size -= 5;

  std::memcpy(&length_, buffer, 16);
  buffer += 16;
  size -= 16;
  
  /* 0x01u (unsigned) is required due to c++ promoting 0x01 to int by default */
  if (dataFieldHeader_[0] == std::byte(0x01u)) {
    std::memcpy(&ack_, buffer, 4);
    buffer += 4;
    size -= 4;

    std::memcpy(&serviceType_, buffer, 8);
    buffer += 8;
    size -= 8;

    std::memcpy(&serviceType_, buffer, 8);
    buffer += 8;
    size -= 8;

    appData_.reserve(size);
    // insert the rest of the buffer but not the last 16
    // due to it being packetErrorControl
    appData_.insert(appData_.begin(), buffer, buffer + size - 16);
  }
  std::memcpy(&packetErrorControl_, buffer, 16);
  buffer += 16; // invalid pointer address from now on
  size -= 16;
}

Packet::~Packet() {}
