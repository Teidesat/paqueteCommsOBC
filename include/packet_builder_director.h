/**
 * @file packet_factory.h
 * @author Marcos Barrios
 * @brief Uses the packet builder to make certain packets. Each packet is wrapped
 *      in the corresponding Packet extended variant depending on the service it
 *      belongs to.
 * 
 * It handles type conversion to uint8_t. An exception is the
 * makeMegaPackete function. 
 * 
 * I general, it allows creation of specific packets by passing less
 *    arguments because it assumes for example, that version is 0,
 *    that the packet is stand_alone and that it contains a
 *    datafieldheader.
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
  PacketBuilderDirector() = default;

  /**
   * @brief A megapacket is a packet whose app data is bigger than
   *    Packet::APP_DATA_SIZE
   * @param appData 
   * @return * std::vector<Packet> 
   */
  std::vector<Packet> makeMegaPacket(const std::vector<uint8_t>& appData);

  /**
   * @brief Make verification packet acceptance success
   * 
   * It swaps that source and destination by default  on the
   *    app. Id. field that is going into the app data because
   *    this method assumes that the packet that is being created
   *    is a verification for a packet that was for the same module
   *    that is creating this verification packet.
   * 
   * @param appIdSource 
   * @param appIdDestination 
   * @param sequenceCount Sequence control count
   * @return PacketExtendedVerification1 
   */
  PacketExtendedVerification1 makeVerificationSuccess(
      uint8_t appIdSource, uint8_t appIdDestination,
      uint16_t sequenceCount);

  /**
   * @brief Make verification packet acceptance failure.
   * 
   * It swaps that source and destination by default  on the
   *    app. Id. field that is going into the app data because
   *    this method assumes that the packet that is being created
   *    is a verification for a packet that was for the same module
   *    that is creating this verification packet.
   * 
   * @param appIdSource 
   * @param appIdDestination 
   * @param sequenceCount 
   * @param code 
   * @param parameters 
   * @return PacketExtendedVerification1 
   */
  PacketExtendedVerification1 makeVerificationFailure(
      uint8_t appIdSource, uint8_t appIdDestination,
      uint16_t sequenceCount, uint8_t code,
      const std::vector<uint8_t>& parameters);

  /**
   * @brief Make verification packet acceptance failure.
   * 
   * It swaps that source and destination by default  on the
   *    app. Id. field that is going into the app data because
   *    this method assumes that the packet that is being created
   *    is a verification for a packet that was for the same module
   *    that is creating this verification packet.
   *
   * @param appIdSource 
   * @param appIdDestination 
   * @param sequenceCount 
   * @param code 
   * @param parameters 
   * @return PacketExtendedVerification1 
   */
  PacketExtendedVerification1 makeVerificationFailure(
      uint8_t appIdSource, uint8_t appIdDestination,
      uint16_t sequenceCount, uint8_t code,
      std::vector<uint8_t>&& parameters);

private:
  PacketBuilder builder_;
};

#endif
