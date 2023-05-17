/**
 * @file packet_extended_housekeeping_25.h
 * @author Marcos Barrios
 * @brief Higher level packet for housekeeping and diagnostic data
 *    service. Packet of subtypes 25 and 26. 25 is of housekeeping
 *    subservice, while 26 is of diagnostics data subservice. Since
 *    both are equal I simply use a 25 class for both. 25 is for
 *    parameter report.
 * 
 *  Generation mode is specified but whether it is finally included
 *    or not is unknown to me at the moment of writing this. It can
 *    be periodic or filtered. Filtered allows some
 *    parameters not to be included in the report, although a maximum
 *    timeout period exists for that parameter, after which the
 *    parameter will be sampled even if it is filtered. Periodic is just
 *    periodically sending all of the SID's parameters. Each mode has
 *    a value associated, 0 being periodic. For filtered, 1 when a the
 *    filtered parameter exceeded threshold, 2 when the timeout took place.
 *    So on filtered mode, if no parameter has changed beyond the threshold
 *    or no timeout has activated for that SID then no report is generated.
 * 
 *  Sending a report is part of the minimum capability set for the
 *    housekeeping subservice. The report contains the structure
 *    identifier (SID), the mode, and then the parameters field,
 *    which is a sequence of parameters and then a sequence of fixed
 *    length arrays as the specific SID defines.
 *
 */

#ifndef PACKET_EXTENDED_HOUSEKEEPING_25_H
#define PACKET_EXTENDED_HOUSEKEEPING_25_H

#include <vector>
#include <unordered_map>

#include "../packet.h"
#include "packet_extended_basic.h"
#include "packet_extended_housekeeping_basic.h"

class PacketExtendedHousekeeping25 {
public:
  enum class GenerationMode : uint8_t {
    PERIODIC = 0,
    FILTERED_THRESHOLD = 1,
    FILTERED_TIMEOUT = 2,
  };

  // Extraction of the samples in the parameters field is done here
  PacketExtendedHousekeeping25(const Packet& packet, uint16_t structureId,
      GenerationMode mode, const std::vector<std::byte>& parameters);

  ~PacketExtendedHousekeeping25();

  /****** Methods from composited classes ******/

  /**
   * @brief Calls the packet extended basic implementation of this method.
   * 
   * @return const Packet& 
   */
  const Packet& getPacket();

  /**
   * @brief Calls the packet extended basic implementation of this method.
   * 
   * @return PacketExtendedHousekeeping25 
   */
  PacketExtendedHousekeeping25 swapApplicationIdFields();

  /**
   * @brief Calls the packet extended basic implementation of this method.
   * 
   * @return std::vector<uint8_t> 
   */
  std::vector<uint8_t> getSampledParameters();

  /**
   * @brief Calls the packet extended housekeeping implementation of this
   *    method.
   */
  uint16_t getStructureIdentifier();

  /**
   * @brief Calls the packet extended basic implementation of this method.
   * 
   * @return std::vector<std::vector<uint8_t>> 
   */
  std::vector<std::vector<uint8_t>> getSampledArrays();

  /****** Methods from this class ******/

  /**
   * @brief 0 = periodic, 1 = filtered threshold, 2 = filtered timeout
   * 
   * See ECSS-E-70-41A.
   * 
   * @return GenerationMode 
   */
  GenerationMode getGenerationMode();
  void setGenerationMode(const GenerationMode newMode);

private:
  PacketExtendedHousekeeping packetHousekeepingBasic_;
  GenerationMode mode_;

  /*
   * Parameters is a sequence of sampled parameters followed by a
   *  sequence of fixed-length arrays that also each contain parameters
   *  but that may be sampled more than once.
   * 
   * See PacketExtendedKeeping2 class description, which is the definition
   *    of a new diagnostic parameter report.
  */
  std::vector<uint8_t> sampledParameters_;
  std::vector<std::vector<uint8_t>> sampledArrays_;
};

#endif
