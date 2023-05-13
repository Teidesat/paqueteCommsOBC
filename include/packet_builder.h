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
  template<typename T>
  using pairs_t = std::vector<std::pair<T, T>>;

  PacketBuilder();
  ~PacketBuilder();

  /****** getters and setters ******/

  Packet getPacket() const;

  /****** other ******/

  // reset current construction
  // dataFieldHeader is left implicit.
  // sequece control flags is set to default 11 independent
  void newPacket(const std::byte versionNumber, const std::byte appIdSource,
      const std::byte appIdDestination,
      const Packet::SequenceFlags sequenceControlFlags,
      const std::array<std::byte, 2> sequenceControlCount);

  void setDataFieldHeader(const std::byte ack, const std::byte serviceType,
      const std::byte serviceSubtype);

  // TODO: Declare headers in app data for the different ECSS services.

  /*
   * Telecommand Verification Service header.
   *
  */
  void addCommandVerificationHeader(const std::byte appIdSource,
      const std::byte sequenceFlags, const std::array<std::byte, 2> sequenceCount);

  void addCommandVerificationHeader(const std::byte appIdSource,
      const std::byte sequenceFlags, const std::byte sequenceCount,
      const std::byte code);

  /**
   * @pre addressAndDataPair.size() <= Packet::APP_DATA_SIZE
   */
  void addCommandVerificationHeader(const std::byte appIdSource,
      const std::byte sequenceFlags, const std::byte sequenceCount,
      const std::byte code, const std::vector<std::byte>& parameters);

  /**
   * Device command Distribution Service header
   * 
   */

  /**
   * on-off register
   *
   */
  void addCommandDistributionHeader(const std::vector<std::byte>& addresses);

  /** register load
   * It represents a setter; register[pair.first] = pair.second.
   *
   * Note: In the ECSS-70-E-41A standard, register load data has as type "any"
   * (as long as it is defined in clause 23), But because I have not read the
   * exact types allowed, I set the type as std::byte but it may change after
   * I have read the clause 23.
   * 
   * @pre addressAndData.size() <= Packet::APP_DATA_SIZE
   */
  template<typename T>
  void addCommandDistributionHeader(const pairs_t<T>& addressAndData);

  /** CPDU
   * It represents a sequence of pulses on certain output line.
   *
   * @pre lineIDAndDuration.size() <= Packet::APP_DATA_SIZE
   */
  void addCommandDistributionHeader(const pairs_t<std::byte>& lineIDAndDuration);

private:
  Packet packet_;
};

#endif
