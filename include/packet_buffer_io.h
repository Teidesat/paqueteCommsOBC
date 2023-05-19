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

// I use this for extract field auxiliary function.
// but since concepts have to be defined at namespace level
// then I have to define this here.
template <typename T>
concept IsOneByte = (sizeof(T) == 1);

// I want the combine function to work with array of 2 bytes or a vector
// of any amount of bytes.
template <typename T, typename T2>
concept IsArrayOrVector = std::is_same_v<T, std::vector<T2>> && IsOneByte<T2> ||
                          std::is_same_v<T, std::array<T2, 2>> && IsOneByte<T2>;

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
   */
  template<IsOneByte T>
  std::byte extractFieldFrom(T inputByte, uint8_t startIndex,
      size_t lengthOfField);

  /**
   * @brief Because sometimes a field is split between different words,
   *    it is necessary to have a mechanism to put them back together.
   *
   * For example, when a 16 bits field has 1 bit in a word, 8 in another
   *    and 7 in the next. The result is 3 different bytes, but it should
   *    be just 2.
   *
   * @tparam Toutvector the type of the array or vector.
   * @tparam Tinvector the type of the array or vector.
   * 
   * @return the combination of the different byte words into fewer.
   */
  template <typename Tout, typename Toutvector, typename Tin, typename Tinvector>
    requires IsArrayOrVector<Tout, Toutvector> && IsArrayOrVector<Tin, Tinvector>
  Tout combineFields(const Tin* inputArray, size_t sizeOfArray);
  uint16_t combineFields(const uint8_t* inputArray, size_t sizeOfArray);
};

#endif
