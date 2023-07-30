/**
 * @file packet_extended_verification.h
 * @author Marcos Barrios
 * @brief Higher level packet for command verification service. 
 * 
 * In this case I used a single packet for both acceptance success
 *    (subtype 1) and acceptance failure (subtype 2), which is the
 *    minimum capability for the command verification service.
 * 
 * An app. id. is the first field in the app. data part, but since
 *  that is information that is already in the packet because it
 *  has both the app id. for source and for destination, then no
 *  member is declared for it in this class, because it can be accessed
 *  from the low level Packet class that this class has as member. It does
 *  get included when transmiting it, but since an address is only 5 bits,
 *  then out of the 11 that an app. id. field is made of, only 5 bits will
 *  be used, leaving the rest of bits at a 0 value.
 * 
 * The same for the sequence control field; it is included in the Packet
 *    class.
 * 
 */

#ifndef PACKET_EXTENDED_VERIFICATION_1_H
#define PACKET_EXTENDED_VERIFICATION_1_H

#include <vector>

#include "../packet.h"
#include "packet_extended_basic.h"

class PacketExtendedVerification1 {
public:
  PacketExtendedVerification1(const Packet& packet);
  PacketExtendedVerification1(const Packet& packet, uint8_t errorCode,
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
   * @return PacketExtendedVerification1 
   */
  PacketExtendedVerification1 swapApplicationIdFields();

  /****** Methods from this class ******/

  // verification acceptance failure includes an error code
  bool hasErrorCode();
  int getErrorCode();

  // verification acceptance failure includes parameters
  std::vector<uint8_t> getParameters();
private:
  PacketExtendedBasic packetBasic_;
  uint8_t errorCode_;
  bool hasErrorCode_; // if it has error code then this is a failure packet
  std::vector<uint8_t> parameters_;
};

#endif
