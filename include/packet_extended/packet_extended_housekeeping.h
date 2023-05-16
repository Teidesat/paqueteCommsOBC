/**
 * @file packet_extended_housekeeping.h
 * @author Marcos Barrios
 * @brief Higher level packet for housekeeping and diagnostic data
 *    service.
 * 
 *  Generation mode is specified but whether it is finally included
 *    or not is unknown to me at the moment of writing this.
 * 
 *  Sending a report is part of the minimum capability set for the
 *    housekeeping subservice. The report contains the structure
 *    identifier (SID), the mode, and then the parameters field,
 *    which is a sequence of parameters and then a sequence of fixed
 *    length arrays as the specific SID defines.
 *
 *  Minimum capability of diagnostics is to be implemented in this
 *    class.
 * 
 *  A data structure for each SID is stored as static to know
 *    how to interpret the parameters field. The content of that
 *    data structure is the same information that a packet from
 *    ground station sends for defining an SID. See ECSS-E-70-41A.
 * 
 */

#ifndef PACKET_EXTENDED_HOUSEKEEPING_H
#define PACKET_EXTENDED_HOUSEKEEPING_H

#include <vector>

#include "../packet.h"
#include "packet_extended_basic.h"

class PacketExtendedHousekeeping {
public:
  enum class GenerationMode : uint8_t {
    PERIODIC = 0,
    FILTERED_THRESHOLD = 1,
    FILTERED_TIMEOUT = 2,
  };

  // Extraction of the samples in the parameters field is done here
  PacketExtendedHousekeeping(const Packet& packet, int structureId,
      GenerationMode mode, const std::vector<std::byte>& parameters);

  ~PacketExtendedHousekeeping();

  /**
   * @brief Get low level packet 
   * 
   * To avoid having to define all the getters here.
   * 
   * @return const Packet& 
   */
  const Packet& getPacket();

  /**
   * @brief When replying to something, source becomes destination and
   *    viceversa.
   * 
   * @return PacketExtendedHousekeeping 
   */
  PacketExtendedHousekeeping swapApplicationIdFields();

  /**
   * @brief Sampled parameters (first sequence of the parameters field,
   *    before the fixed-length arrays)
   * 
   * @return std::vector<uint8_t> 
   */
  std::vector<uint8_t> getSampledParameters();

  /**
   * @brief Sampled arays (second sequence of the parameters field,
   *    after the sequence of parameter sampled)
   * 
   * @return std::vector<std::vector<uint8_t>> 
   */
  std::vector<std::vector<uint8_t>> getSampledArrays();

  GenerationMode getGenerationMode();
private:
  PacketExtendedBasic packetBasic_;
  int structureId_;
  GenerationMode mode_;

  /*
   * Parameters is a sequence of parameters followed by a sequence of
   *  fixed-length arrays.
  */
  std::vector<uint8_t> sampledParameters_;
  std::vector<std::vector<uint8_t>> sampledArrays_;

  /**
   * @brief SID definitions for interpretation of the parameters field.
   * 
   *  Each SID defines a collection interval (optional), an amount of
   *    parameters sampled once per collection interval, a sequence of
   *    Parameter Id. (depending on the previous amount), then an
   *    amount of fixed-length arrays, then a sequence of definitions
   *    for each fixed-length array with the amount of sampled values
   *    for each parameter within the fixed-length array and the amount
   *    of different parameters withint the fixed-length array.
   *  
   *  According to ECSS-70-E-41A.
   * 
   *  Thats why a a matrix of unsigned 8 bit integer follows.
   * 
   */
  static std::vector<std::vector<uint8_t>> structureIdDefinitions;
};

#endif
