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

  Packet readPacket(const std::byte* buffer, size_t size);
  void writePacket(std::byte* buffer, Packet& packet);
private:
  /**
   * @brief I need to separate a word that has different fields into
   *    separate fields.
   * 
   * @param startIndex rightmost = 0
   * @param lengthOfField
   */
  template<typename T>
  T extractFieldFrom(T inputWord, uint8_t startIndex, size_t lengthOfField);
};

#endif
