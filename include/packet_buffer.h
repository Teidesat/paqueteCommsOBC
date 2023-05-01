/**
 * @file packet_buffer.h
 * @author Marcos Barrios
 * @brief 
 * @version 0.1
 * @date 2023-05-01
 * 
 * @copyright Copyright (c) 2023
 * 
 * To read and write packets from the buffer
 * 
 */
#pragma once

#include "packet.h"

class PacketBuffer {
public:
  PacketBuffer();
  ~PacketBuffer();

  Packet readPacket(std::byte* buffer, std::size_t size);
  void writePacket(Packet packet);
private:
};
