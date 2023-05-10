/**
 * @file packet_extended.h
 * @author Marcos Barrios
 * @brief Higher level packet for the services.
 * @version 0.1
 * @date 2023-05-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PACKET_EXTENDED_H
#define PACKET_EXTENDED_H

#include "packet.h"

class PacketExtended {
public:
  PacketExtended(const Packet& packet);
  ~PacketExtended();
  
  Packet& getPacket();

  PacketExtended getAcknowledgedVersion();
  void incrementSequenceCounter();

  // When replying to something, source becomes destination and viceversa.
  PacketExtended swapApplicationIdFields();

  
private:
  Packet packet_;
};

#endif
