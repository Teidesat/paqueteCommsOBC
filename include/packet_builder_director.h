/**
 * @file packet_factory.h
 * @author Marcos Barrios
 * @brief Uses the packet builder to make certain packets. Each packet is wrapped
 *      in the corresponding Packet extended variant depending on the service it
 *      belongs to.
 * 
 * It handles type conversion to std::byte. An exception is the
 * makeMegaPackete function. 
 * 
 */

#ifndef PACKET_BUILDER_DIRECTOR_H
#define PACKET_BUILDER_DIRECTOR_H

#include <vector>

#include "packet.h"

#include "../include/packet_builder.h"
#include "packet_extended/packet_extended_verification_1.h"

class PacketBuilderDirector {
public:
  PacketBuilderDirector();

  /**
   * @brief A megapacket is a packet whose app data is bigger than
   *    Packet::APP_DATA_SIZE
   * @param appData 
   * @return * std::vector<Packet> 
   */
  std::vector<Packet> makeMegaPacket(const std::vector<std::byte>& appData);

  /**
   * @brief Make verification packet acceptance success
   * 
   * SequenceFlag is stand-alone by default
   * 
   * @param appIdSource 
   * @param appIdDestination 
   * @param sequenceCount Sequence control count
   * @return PacketExtendedVerification1 
   */
  PacketExtendedVerification1 makeVerificationSuccess(
      const uint8_t appIdSource, const uint8_t appIdDestination,
      const uint16_t sequenceCount);

  PacketExtendedVerification1 makeVerificationFailure(
      const uint8_t appIdSource, const uint8_t appIdDestination,
      const uint16_t sequenceCount, const uint8_t code,
      const std::vector<std::byte>& parameters);

  PacketExtendedVerification1 makeVerificationFailure(
      const uint8_t appIdSource, const uint8_t appIdDestination,
      const uint16_t sequenceCount, const uint8_t code,
      std::vector<std::byte>&& parameters);

private:
  PacketBuilder builder_;
};

#endif
