/**
 * @file packet_extended.h
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

  // low level packet 
  Packet& getPacket();

  int getVersionNumber();
  int getAppIdOrigin();
  int getAppIdDestination();
  int getSequenceCount();
  int getLength();

  int getPusVersion();
  int getServiceType();
  int getServiceSubtype();

  PacketExtendedBasic getInstance();
  PacketExtendedBasic getAcknowledgedVersion();

  void incrementSequenceCounter();
  
  // When replying to something, source becomes destination and viceversa.
  PacketExtendedBasic swapApplicationIdFields();
  
private:
  Packet packet_;
};

#endif
