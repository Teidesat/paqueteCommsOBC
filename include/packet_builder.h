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

  // TODO: Headers in app data for the different ECSS services.

  /*
   * Telecommand Verification Service header.
   *    
  */
  // void addVerificationHeader(
  //   const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_FLAGS_SIZE>& sequenceFlags,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceCount
  // );

  // void addVerificationHeader(
  //   const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_FLAGS_SIZE>& sequenceFlags,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceCount,
  //   const std::bitset<4>& code
  // );

  // void addVerificationHeader(
  //   const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_FLAGS_SIZE>& sequenceFlags,
  //   const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceCount,
  //   const std::bitset<4>& code,
  //   const std::vector<std::byte>& parameters
  // );

private:
  Packet packet_;
};

#endif
