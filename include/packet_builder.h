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
#include "packet_extended/packet_extended_housekeeping.h"

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
   * 
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

  /**
   * @brief 
   * 
   * @param appIdSource 
   * @param sequenceFlags 
   * @param sequenceCount 
   * @param code error code
   */
  void addCommandVerificationHeader(const uint8_t appIdSource,
      const Packet::SequenceFlags sequenceFlags, const uint16_t sequenceCount,
      const uint8_t code);

  /**
   * @brief 
   * 
   * @param appIdSource 
   * @param sequenceFlags 
   * @param sequenceCount 
   * @param code error code
   * @param parameters
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

  /**
   * @brief Housekeeping report with an Structure Identifier (SID), a
   *    generation mode and then the content of the report.
   * 
   * Each SID defines an amount of parameters, an amount of fixed-length
   *  arrays, how many times a parameter is sampled in the array, and
   *  how many parameters the array contains.
   * 
   * Each parameter has an associated Parameter ID in the ground station.
   *  The ground station knows the parameters by the (SID, origin app. id,
   *    service type) tuple. 
   *
   * The generation mode can be periodic or filtered. Filtered allows some
   *  parameters not to be included in the report, although a maximum
   *  timeout period exists for that parameter, after which the
   *  parameter will be sampled even if it is filtered. Periodic is just
   *  periodically sending all of the SID's parameters. Each mode has
   *  a value associated, 0 being periodic. For filtered, 1 when a the
   *  filtered parameter exceeded threshold, 2 when the timeout took place.
   *  So on filtered mode, if no parameter has changed beyond the threshold
   *  or no timeout has activated for that SID then no report is generated.
   * 
   *  @param structureID int16_t because I don't know how many we need.
   *  @param mode
   */
  void addHousekeepingReportHeader(uint16_t structureId,
      PacketExtendedHousekeeping::GenerationMode mode,
      const std::vector<std::byte>& parameters);

private:
  Packet packet_;
};

#endif
