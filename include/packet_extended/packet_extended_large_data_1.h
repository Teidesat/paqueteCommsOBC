/**
 * @file packet_extended_large_data_1.h
 * @author Marcos Barrios
 * @brief Higher level packet for large data transfer service.
 * @class PacketExtendedLargeData1
 *
 * This class can be used for the following subtypes:
 *    1, 2, 3, 7, 9, 10, 11, 12
 * 
 * Some packets have different meaning depending on
 * if it is being received on the ground station (donwlink) or if it
 * is being sent on the cubesat (downlink), but the packet structure
 * is the same for both.
 *
 * Subtypes 1, 2, 3 define the first, intermediate and last packets to
 * be transfered. 7 is used when re-transfering a packet.
 * 
 * 1 and 9 packet structures are the same, but 1 is sent-by-sender(downlink
 *    from now on) while 9 is interpreted on the ground station as 
 * 
 * 
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
      uint16_t sequenceNumber, const std::vector<uint8_t>& data);
  PacketExtendedLargeData1(const Packet& packet, uint8_t largeDataUnitId,
      uint16_t sequenceNumber, std::vector<uint8_t>&& data);

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

  std::vector<uint8_t>& getData();
  void setData(const std::vector<uint8_t>& newData);
  void setData(std::vector<uint8_t>&& newData);
private:
  PacketExtendedBasic packetBasic_;
  uint8_t largeDataUnitId_;
  uint16_t sequenceNumber_;
  std::vector<uint8_t> data_;
};

#endif
