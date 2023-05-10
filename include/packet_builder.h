/**
 * @file packet_builder.h
 * @author Marcos Barrios
 * @brief Builder for different kinds of packets
 * @version 0.1
 * @date 2023-05-09
 * 
 * A packet can be either:
 *    - main header only + app data
 *    - main header + data field header + app data
 * 
 * Then, the app data can contain many different units depending on
 *    the service.
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

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
    const std::bitset<Packet::VERSION_NUMBER_SIZE>& versionNumber,
    const std::bitset<Packet::APP_ID_SOURCE_SIZE>& appIdSource,
    const std::bitset<Packet::APP_ID_DESTINATION_SIZE>& appIdDestination,
    const Packet::SequenceFlags sequenceControlFlags,
    const std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE>& sequenceControlCount
  );

  void setDataFieldHeader(
    const bool ack,
    const std::bitset<Packet::SERVICE_TYPE_SIZE>& serviceType,
    const std::bitset<Packet::SERVICE_SUBTYPE_SIZE>& serviceSubtype
  );

  // TODO: Headers in app data for the different ECSS services.

  /*
   * Telecommand Verification Service header.
   *    
  */
  void addVerificationHeader();

private:
  Packet packet_;
};

#endif
