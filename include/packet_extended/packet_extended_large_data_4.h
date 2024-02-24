/**
 * @file packet_extended_large_data_4.h
 * @author Marcos Barrios
 * @brief Higher level packet for large data transfer service.
 * 
 * This class can be used for the following subtypes:
 *    4, 8, 13, 16
 * 
 */

#ifndef PACKET_EXTENDED_LARGE_DATA_4_H
#define PACKET_EXTENDED_LARGE_DATA_4_H

#include <vector>
#include <cstdint>

#include "../packet.h"
#include "packet_extended_basic.h"


class PacketExtendedLargeData4 {
public:
  PacketExtendedLargeData4(const Packet& packet, uint8_t largeDataUnitId,
      uint8_t reasonCode);

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
   * @return PacketExtendedLargeData4 
   */
  PacketExtendedLargeData4 swapApplicationIdFields();

  /**
   * @brief Calls the packet extended large data implementation of this
   *    method.
   * 
   * @return uint8_t 
   */
  uint8_t getLargeDataUnitId();

  /****** Methods from this class ******/

  uint8_t getReasonCode();
  void setReasonCode(const uint8_t newReasonCode);
private:
  PacketExtendedBasic packetBasic_;
  uint8_t largeDataUnitId_;
  uint8_t reasonCode_;
};

#endif
