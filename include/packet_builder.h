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
#include "packet_extended/packet_extended_housekeeping_basic.h"
#include "packet_extended/packet_extended_housekeeping_25.h"

class PacketBuilder {
public:
  template<typename T1, typename T2>
  using pairs_t = std::vector<std::pair<T1, T2>>;

  PacketBuilder() = default;

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

  void newPacket(uint8_t versionNumber, uint8_t appIdSource,
      uint8_t appIdDestination,
      Packet::SequenceFlags sequenceControlFlags,
      uint16_t sequenceControlCount);

  void setDataFieldHeader(bool ack, uint8_t serviceType,
      uint8_t serviceSubtype);

  /**
   * ******** Telecommand Verification Service headers. ********
   *
  */
  void addCommandVerificationAppData(uint8_t appIdSource,
      uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
      uint16_t sequenceCount);

  /**
   * @brief 
   * 
   * @param appIdSource 
   * @param sequenceFlags 
   * @param sequenceCount 
   * @param code error code
   */
  void addCommandVerificationAppData(uint8_t appIdSource,
      uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
      uint16_t sequenceCount, uint8_t code);

  /**
   * @brief 
   * 
   * @param appIdSource 
   * @param sequenceFlags 
   * @param sequenceCount 
   * @param code error code
   * @param parameters
   */
  void addCommandVerificationAppData(uint8_t appIdSource,
      uint8_t appIdDestination, Packet::SequenceFlags sequenceFlags,
      uint16_t sequenceCount, uint8_t code,
      const std::vector<uint8_t>& parameters);

  /**
   * ******** Device command Distribution Service headers. ********
   * 
   */

  /**
   * on-off register
   *
   */
  void addCommandDistributionAppData(const std::vector<uint8_t>& addAppData);

  /** register load
   * It represents a setter; register[pair.first] = pair.second.
   *
   * Note: In the ECSS-70-E-41A standard, register load data has as type "any"
   * (as long as it is defined in clause 23), But because I have not read the
   * exact types allowed, I set the type as uint8_t but it may change after
   * I have read the clause 23.
   * 
   * @pre addressAAppData.size() <= Packet::APP_DATA_SIZE
   */
  template <typename T>
  void addCommandDistributionAppData(const pairs_t<uint8_t, T>& addressAndData);

  /** CPDU
   * It represents a sequence of pulses on certain output line.
   * 
   * Unline register load, this does not add a length as first byte if higher than
   *    one, thus a it is different method than the register load one.
   * 
   * I use uint8_t temporally because I am not sure about how many lines there will be
   * 
   * @pre lineIDAndDuration.size() <= Packet::APP_DATA_SIZE
   */
  void addCommandDistributionAppData(const pairs_t<uint8_t, uint8_t>& lineIDAndDuration);

  /**
   * ******** Housekeeping & diagnostic data reporting service headers ********
   * 
   */

  /**
   * @brief Housekeeping parameter report with an Structure Identifier (SID),
   *    a generation mode and then the content of the report.
   *
   *  @param structureID int16_t for not because I don't know how many we need.
   *  @param mode
   */
  void addHousekeepingReportAppData(uint16_t structureId,
      PacketExtendedHousekeeping25::GenerationMode mode,
      const std::vector<uint8_t>& parameters);

private:
  Packet packet_;
};

// first byte may be the amount of addresses
template <typename T>
void PacketBuilder::addCommandDistributionAppData(
    const pairs_t<uint8_t, T>& addressAndData) {
  if (addressAndData.size() > 1) {
    packet_.pushData(static_cast<uint8_t>(addressAndData.size()));
  }
  for (size_t i = 0; i < addressAndData.size(); ++i) {
    packet_.pushData(static_cast<uint8_t>(addressAndData[i].first));
    packet_.pushData(static_cast<uint8_t>(addressAndData[i].second));
  }
}

#endif
