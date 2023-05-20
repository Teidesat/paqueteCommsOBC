/**
 * @file packet_buffer.h
 * @author Marcos Barrios
 * @brief To read and write packets from the buffer
 * 
 */

#ifndef PACKET_BUFFER_IO_H
#define PACKET_BUFFER_IO_H

#include <type_traits>

#include "packet.h"

class PacketBufferIO {
public:
  PacketBufferIO();

  Packet readPacket(const std::byte* buffer);
  void writePacket(std::byte* buffer, Packet& packet);
private:
  /**
   * @brief I need to separate a word that has different fields into
   *    separate fields.
   * 
   * @param startIndex rightmost = 0
   * 
   * @return a word with the field and if there are other bits they are all
   *    0. For example: for input 0b00100110, startIndex = 5,
   *    lengthOfField = 3, then output is 0b00000001
   */
  uint8_t extractFieldFrom(std::byte inputByte, uint8_t startIndex,
      size_t lengthOfField);
};

#endif
