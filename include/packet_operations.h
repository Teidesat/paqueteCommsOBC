/**
 * @file packet_buffer.h
 * @author Marcos Barrios
 * @brief To read and write packets from the buffer
 * 
 */

#pragma once

#include "packet.h"

class PacketOperations {
public:
  PacketOperations();
  ~PacketOperations();

  Packet readPacket(const std::byte* buffer, std::size_t size);
  void writePacket(std::byte* buffer, Packet& packet);
private:
};
