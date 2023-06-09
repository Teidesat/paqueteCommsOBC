/**
 * @file packet_extended_large_data_1.h
 * @author Marcos Barrios
 * @brief Higher level packet for large data transfer service.
 * 
 * This class can be used for the following subtypes:
 *    1, 2, 3, 7, 9, 10, 11, 12
 * 
 */

#ifndef PACKET_EXTENDED_LARGE_DATA_1_H
#define PACKET_EXTENDED_LARGE_DATA_1_H

#include <vector>

#include "../packet.h"
#include "packet_extended_basic.h"

class PacketExtendedLargeData1 {
public:
  // Note: sequence nubmer is not the same field as the main header's
  // sequence control field. This one is relative to the large data unit,
  // so it starts from 1 for this large data transfer.
  PacketExtendedLargeData1(const Packet& packet, uint8_t largeDataUnitId,
      uint16_t sequenceNumber, const std::vector<std::byte>& data);
  PacketExtendedLargeData1(const Packet& packet, uint8_t largeDataUnitId,
      uint16_t sequenceNumber, std::vector<std::byte>&& data);

  /****** Methods from composited classes ******/

  /**
   * @brief Calls the packet extended basic implementation of this
   *    method.
   * 
   * To avoid having to define all the getters here.
   * 
   * @return const Packet& 
   */
  Packet& getPacket();

  /**
   * @brief Calls the packet extended basic implementation of this
   *    method.
   * 
   * @return PacketExtendedLargeData1 
   */
  PacketExtendedLargeData1 swapApplicationIdFields();

  /**
   * @brief Calls the packet extended large data implementation of this
   *    method.
   * 
   * @return uint8_t 
   */
  uint8_t getLargeDataUnitId();

  /****** Methods from this class ******/

  uint16_t getSequenceNumber();
  void setSequenceNumber(const uint16_t newSequenceNumber);

  std::vector<std::byte>& getData();
  void setData(const std::vector<std::byte>& newData);
  void setData(std::vector<std::byte>&& newData);
private:
  PacketExtendedBasic packetBasic_;
  uint8_t largeDataUnitId_;
  uint16_t sequenceNumber_;
  std::vector<std::byte> data_;
};

#endif
