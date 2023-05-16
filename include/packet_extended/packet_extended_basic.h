/**
 * @file packet_extended_basic.h
 * @author Marcos Barrios
 * @brief Higher level packet for the services.
 * 
 */

#ifndef PACKET_EXTENDED_BASIC_H
#define PACKET_EXTENDED_BASIC_H

#include "../packet.h"

class PacketExtendedBasic {
public:
  PacketExtendedBasic(const Packet& packet);
  ~PacketExtendedBasic();

  /**
   * @brief Get low level packet 
   * 
   * To avoid having to define all the getters here.
   * 
   * @return const Packet& 
   */
  const Packet& getPacket();

  /**
   * @brief When replying to something, source becomes destination and viceversa.
   * 
   * @return PacketExtendedBasic 
   */
  PacketExtendedBasic swapApplicationIdFields();

private:
  Packet packet_;
};

#endif
