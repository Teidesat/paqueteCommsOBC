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
    const uint8_t versionNumber,
    const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const Packet::SequenceFlags sequenceControlFlags,
    const uint16_t sequenceControlCount
  );

  void setDataFieldHeader(
    const bool ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype
  );

  // TODO: Declare headers in app data for the different ECSS services.

  /*
   * Telecommand Verification Service header.
   *    
  */
  void addCommandVerificationHeader(
    const uint8_t appIdSource,
    const uint8_t sequenceFlags,
    const uint16_t sequenceCount
  );

  void addCommandVerificationHeader(
    const uint8_t appIdSource,
    const uint8_t sequenceFlags,
    const uint8_t sequenceCount,
    const uint8_t code
  );

  void addCommandVerificationHeader(
    const uint8_t appIdSource,
    const uint8_t sequenceFlags,
    const uint8_t sequenceCount,
    const uint8_t code,
    const std::vector<uint8_t>& parameters
  );

private:
  Packet packet_;
};

#endif
