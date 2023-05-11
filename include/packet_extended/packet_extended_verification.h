/**
 * @file packet_extended.h
 * @author Marcos Barrios
 * @brief Higher level packet for command verification service. 
 * 
 */

#ifndef PACKET_EXTENDED_VERIFICATION_H
#define PACKET_EXTENDED_VERIFICATION_H

#include <vector>

#include "../packet.h"

class PacketExtendedVerification {
public:
  PacketExtendedVerification(const Packet& packet);
  ~PacketExtendedVerification();

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

  PacketExtendedVerification getInstance();
  PacketExtendedVerification getAcknowledgedVersion();

  void incrementSequenceCounter();
  
  // When replying to something, source becomes destination and viceversa.
  PacketExtendedVerification swapApplicationIdFields();
  
  // verification acceptance failure includes an error code
  bool hasErrorCode();
  int getErrorCode();

  const std::vector<uint8_t>& getParameters();
private:
  Packet packet_;
  uint8_t errorCode_;
  std::vector<uint8_t> parameters_;
};

#endif
