/**
 * @file packet_extended_verification.h
 * @author Marcos Barrios
 * @brief Higher level packet for command verification service. 
 * 
 * In this case I used a single packet for both acceptance success
 *    (subtype 1) and acceptance failure (subtype 2), which is the
 *    minimum capability for the command verification service.
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
   * @return PacketExtendedVerification 
   */
  PacketExtendedVerification swapApplicationIdFields();

  /****** Methods from this class ******/

  // verification acceptance failure includes an error code
  Packet::Bool8Enum hasErrorCode();
  int getErrorCode();

  // verification acceptance failure includes parameters
  std::vector<uint8_t> getParameters();
private:
  PacketExtendedBasic packetBasic_;
  uint8_t errorCode_;
  Packet::Bool8Enum hasErrorCode_; // if it has error code then this is a failure packet
  std::vector<uint8_t> parameters_;
};

#endif
