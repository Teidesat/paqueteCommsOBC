/**
 * @file packet_buffer.h
 * @author Marcos Barrios
 * @brief To read and write packets from the buffer
 * 
 */

#ifndef PACKET_BUFFER_IO_H
#define PACKET_BUFFER_IO_H

#include "packet.h"

class PacketBufferIO {
public:
  PacketBufferIO();

  Packet readPacket(const std::byte* buffer, std::size_t size);
  void writePacket(std::byte* buffer, Packet& packet);
private:
};

#endif
