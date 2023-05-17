/**
 * @file packet_extended_housekeeping_basic.h
 * @author Marcos Barrios
 * @brief Higher level packet for housekeeping and diagnostic data
 *    service. Basic packet that all the subtypes use.
 *
 *  A data structure for each SID is stored as static to know
 *    how to interpret the parameters field. The content of that
 *    data structure is the same information that a packet from
 *    ground station sends for defining an SID. See ECSS-E-70-41A.
 *    Minimum capability doesn't include sending packets for
 *    defining a new SID, so the SID must be defined on the code,
 *    but I don't know what we need, so I just use it empty.
 * 
 * The packet for definition of a SID includes the sampling period,
 *    but it is not included in the data structure of the SID that
 *    is static to this class.
 */

#ifndef PACKET_EXTENDED_HOUSEKEEPING_BASIC_H
#define PACKET_EXTENDED_HOUSEKEEPING_BASIC_H

#include <vector>
#include <unordered_map>

#include "../packet.h"
#include "packet_extended_basic.h"

class PacketExtendedHousekeeping {
public:

  /**
   * @brief SID definitions for interpretation of the parameters field.
   * 
   *  Each SID defines a collection interval (optional), an amount of
   *    parameters sampled once per collection interval, a sequence of
   *    Parameter Id. (depending on the previous amount), then an
   *    amount of fixed-length arrays, then a sequence of definitions
   *    for each fixed-length array with the amount of sampled values
   *    for each parameter within the fixed-length array and the amount
   *    of different parameters withint the fixed-length array.
   *  
   *  According to ECSS-70-E-41A.
   * 
   *  Thats why a a matrix of unsigned 8 bit integer follows.
   * 
   *  Follows a map of SID to vector.
   */
  static std::unordered_map<uint16_t, std::vector<uint8_t>> structureIdDefinitions;

  PacketExtendedHousekeeping(const Packet& packet, uint16_t structureId);

  /****** Methods from composited classes ******/

  /**
   * @brief Calls the packet extended housekeeping implementation of this
   *    method.
   * 
   * To avoid having to define all the getters here.
   * 
   * @return const Packet& 
   */
  Packet& getPacket();

  /**
   * @brief Calls the packet extended housekeeping implementation of this
   *    method.
   * 
   * @return PacketExtendedHousekeeping 
   */
  PacketExtendedHousekeeping swapApplicationIdFields();

  /****** Methods from this class ******/

  /**
   * @brief Get the Structure Identifier
   * 
   * @return uint16_t 
   */
  uint16_t getStructureIdentifier();
private:
  PacketExtendedBasic packetBasic_;
  uint16_t structureId_;
};

#endif
