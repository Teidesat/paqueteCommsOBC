/**
 * @file packet_builder.h
 * @author Marcos Barrios
 * @brief Builder for different kinds of packets
 * 
 * A packet can be either:
 *    - main header only + app data
 *    - main header + data field header + app data
 * 
 * Then, the app data can contain many different units depending on
 *    the service.
 * 
 */

#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include <vector>
#include <array>

#include "packet.h"

class PacketBuilder {
public:
  PacketBuilder();
  ~PacketBuilder();

  /****** getters and setters ******/

  Packet getPacket() const;

  /****** other ******/

  // reset current construction
  // dataFieldHeader is left implicit.
  // sequece control flags is set to default 11 independent
  void newPacket(
    const std::byte versionNumber,
    const std::byte appIdSource,
    const std::byte appIdDestination,
    const Packet::SequenceFlags sequenceControlFlags,
    const std::array<std::byte, 2> sequenceControlCount
  );

  void setDataFieldHeader(
    const std::byte ack,
    const std::byte serviceType,
    const std::byte serviceSubtype
  );

  // TODO: Declare headers in app data for the different ECSS services.

  /*
   * Telecommand Verification Service header.
   *    
  */
  void addCommandVerificationHeader(
    const std::byte appIdSource,
    const std::byte sequenceFlags,
    const std::array<std::byte, 2> sequenceCount
  );

  void addCommandVerificationHeader(
    const std::byte appIdSource,
    const std::byte sequenceFlags,
    const std::byte sequenceCount,
    const std::byte code
  );

  void addCommandVerificationHeader(
    const std::byte appIdSource,
    const std::byte sequenceFlags,
    const std::byte sequenceCount,
    const std::byte code,
    const std::vector<std::byte>& parameters
  );

  /**
   * Device command Distribution Service header
   * 
   */

  void addCommandDistributionHeader(
    const std::vector<std::byte> addresses,
    const std::byte amountOfAddresses = std::byte{0b1}
  );

private:
  Packet packet_;
};

#endif
