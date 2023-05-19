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

  Packet readPacket(const std::byte* buffer, size_t size);
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

  /**
   * @brief Because sometimes a field is split between different words,
   *    it is necessary to have a mechanism to put them back together.
   *
   * For example, when a 16 bits field has 1 bit in a word, 8 in another
   *    and 7 in the next. The result is 3 different bytes, but it should
   *    be just 2.
   * 
   */
  std::array<std::byte, 2> combineFields(const std::byte* inputArray, size_t sizeOfArray);
};

#endif
