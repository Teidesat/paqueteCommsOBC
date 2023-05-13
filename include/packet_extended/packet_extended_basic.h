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
  PacketExtendedBasic(const PacketExtendedBasic& other);
  ~PacketExtendedBasic();

  // low level packet 
  Packet& getPacket();

  int getVersionNumber();
  int getAppIdOrigin();
  int getAppIdDestination();
  int getSequenceCount();
  int getLength();

  int getPUSVersion();
  int getServiceType();
  int getServiceSubtype();

  /**
   * @brief Get a new instance of this very same object.
   * Copies are probably gonna be a frequent task so this method is for it.
   * 
   * @return PacketExtendedBasic duplicate of this
   */
  PacketExtendedBasic getInstance();

  /**
   * @brief Answer to a packet means getting the acknowledged byte on true (0b1)
   * 
   * @return PacketExtendedBasic 
   */
  PacketExtendedBasic getAcknowledgedVersion();

  /**
   * @brief Counter typically goes up until it reaches a limit and resets back
   *    to zero
   * 
   */
  void incrementSequenceCounter();
  
  // When replying to something, source becomes destination and viceversa.
  PacketExtendedBasic swapApplicationIdFields();
  
private:
  Packet packet_;
};

#endif
