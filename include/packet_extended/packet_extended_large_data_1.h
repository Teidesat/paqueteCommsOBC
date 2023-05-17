/**
 * @file packet_extended_large_data.h
 * @author Marcos Barrios
 * @brief Higher level packet for large data transfer service. Packet
 *    subtypes 1 and 9 (downlink and uplink respectively).
 * 
 * This represents the first part of a Large Data Unit Id.
 * 
 * Field large data unit Id. can eb skipped if the large data service
 *  only supports one simultaneous transference. In this case, I include
 *  it temporarilly as I don't know if we need it.
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

  /****** Methods from this class ******/

  uint8_t getLargeDataUnitId();
  uint16_t getSequenceNumber();
  std::vector<std::byte>& getData();
private:
  PacketExtendedBasic packetBasic_;
  uint8_t largeDataUnitId_;
  uint16_t sequenceNumber_;
  std::vector<std::byte> data_;
};

#endif
