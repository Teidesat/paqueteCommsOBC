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
  template<typename T1, typename T2>
  using pairs_t = std::vector<std::pair<T1, T2>>;

  PacketBuilder();
  ~PacketBuilder();

  /****** getters and setters ******/

  // reference to avoid an extra copy
  Packet& getPacket();

  /****** other ******/

  /**
   * @brief reset current construction with a new fresh packet
   * dataFieldHeader is true by default.
   * 
   * @param versionNumber 
   * @param appIdSource 
   * @param appIdDestination 
   * @param sequenceControlFlags 
   * @param sequenceControlCount 
   */
  void newPacket(const uint8_t versionNumber, const uint8_t appIdSource,
      const uint8_t appIdDestination,
      const Packet::SequenceFlags sequenceControlFlags,
      const uint16_t sequenceControlCount);

  void setDataFieldHeader(const bool ack, const uint8_t serviceType,
      const uint8_t serviceSubtype);

  /**
   * ******** Telecommand Verification Service headers. ********
   *
  */
  void addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount);

  void addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount,
      const uint8_t code);

  /**
   * @pre addressAndDataPair.size() <= Packet::APP_DATA_SIZE
   */
  void addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount,
      const uint8_t code, const std::vector<std::byte>& parameters);

  /**
   * ******** Device command Distribution Service headers. ********
   * 
   */

  /**
   * on-off register
   *
   */
  void addCommandDistributionHeader(const std::vector<uint8_t>& addresses);

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
  void addCommandDistributionHeader(const pairs_t<uint8_t, T>& addressAndData);

  /** CPDU
   * It represents a sequence of pulses on certain output line.
   *
   * I use int here because I am not sure about how many lines there will be
   * 
   * @pre lineIDAndDuration.size() <= Packet::APP_DATA_SIZE
   */
  void addCommandDistributionHeader(const pairs_t<int, std::byte>& lineIDAndDuration);

  /**
   * ******** Housekeeping & diagnostic data reporting service headers ********
   * 
   */

  

private:
  Packet packet_;
};

#endif
