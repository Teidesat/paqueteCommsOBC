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
#include "packet_extended_basic.h"

class PacketExtendedVerification {
public:
  PacketExtendedVerification(const Packet& packet);
  PacketExtendedVerification(const Packet& packet, uint8_t errorCode,
      const std::vector<uint8_t>& parameters);

  ~PacketExtendedVerification();

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
  PacketExtendedVerification swapApplicationIdFields();

  // verification acceptance failure includes an error code
  bool hasErrorCode();
  int getErrorCode();

  // verification acceptance failure includes parameters
  const std::vector<uint8_t>& getParameters();
private:
  PacketExtendedBasic packetBasic_;
  uint8_t errorCode_;
  bool hasErrorCode_; // if it has error code then this is a failure packet
  std::vector<uint8_t> parameters_;
};

#endif
