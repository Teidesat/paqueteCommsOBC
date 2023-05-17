/**
 * @file packet_extended_housekeeping_2.h
 * @author Marcos Barrios
 * @brief Higher level packet for housekeeping and diagnostic data
 *    service. Packet of subtype 2: Define new diagnostic parameter
 *    report.
 * 
 *  A diagnostic parameter report consists of a collection interval,
 *    an amount of parameters to sample, the sequence of parameter ID
 *    to be sampled for this SID according to the previous amount, an
 *    amount of fixed-length arrays, the sequence of arrays. The
 *    parameter Identifiers are defined at ground station.
 * 
 * The collection interval defines how many <DIAG_MIN_INTERV> to wait
 *    before sampling again, which is the time it takes to sample a
 *    parameter. So if I understood it properly, if it is 1 then the
 *    parameter will me continuously sampled. It can be sistematically
 *    skipped but I decided it is better to keep it around as I am
 *    not sure whether we will actually need it or not.
 * 
 * The fixed-length arrays have the amount of samples per parameter
 *    and the amount of parameters, so it will contain a sequence of
 *    samples per parameter. By using the amount of samples per
 *    parameter one can know which sample belongs to what parameter.
 * 
 */

#ifndef PACKET_EXTENDED_HOUSEKEEPING_2_H
#define PACKET_EXTENDED_HOUSEKEEPING_2_H

#include <vector>
#include <unordered_map>

#include "../packet.h"
#include "packet_extended_basic.h"
#include "packet_extended_housekeeping_basic.h"

class PacketExtendedHousekeeping2 {
public:
  PacketExtendedHousekeeping2(const Packet& packet, uint16_t structureId,
      uint8_t collectionInterval, uint8_t amountOfSamples,
      const std::vector<uint8_t>& samples, uint8_t amountOfArrays,
      const std::vector<uint8_t>& arrays);
       // arrays are fixed-length, so leave the interpretation for later and
       // just receive a vector of bytes.

  ~PacketExtendedHousekeeping2();

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
   * @return PacketExtendedHousekeeping2 
   */
  PacketExtendedHousekeeping2 swapApplicationIdFields();

  /**
   * @brief Calls the packet extended housekeeping implementation of this
   *    method.
   */
  uint16_t getStructureIdentifier();

  /****** Methods from this class ******/

  uint8_t getCollectionInterval();
  void setCollectionInterval(const uint8_t newCollectionInterval);

  uint8_t getAmountOfSamples();

  /**
   * @brief Set new amount of samples. When changing this make sure that
   *    samples.size() == newAmountOfSamples or the packet wrapper will
   *    be in an incorrect state and errors will follow.
   * 
   * @param newAmountOfSamples
   */
  void setAmountOfSamples(const uint8_t newAmountOfSamples);

  const std::vector<uint8_t>& getSamples();

  /**
   * @pre newSamples.size() == amountOfSamples
   * 
   * @param newSamples 
   */
  void setSamples(const std::vector<uint8_t>& newSamples);

  uint8_t getAmountOfArrays();

  /**
   * @brief Set new amount of arrays. When changing this make sure that
   *    arrays.size() == newAmountOfArrays or the packet wrapper will
   *    be in an incorrect state and errors will follow.
   * 
   * @param newAmountOfArrays 
   */
  void setAmountOfArrays(const uint8_t newAmountOfArrays);

  const std::vector<uint8_t>& getArrays();

  /** @pre arrays.size() == newAmountOfArrays or the packet wrapper will
   *    be in an incorrect state and errors will follow.
   */
  void setArrays(const std::vector<uint8_t>& newArrays);

private:
  PacketExtendedHousekeeping packetHousekeepingBasic_;
  uint8_t collectionInterval_;
  uint8_t amountOfSamples_;
  const std::vector<uint8_t>& samples_;
  uint8_t amountOfArrays_;
  const std::vector<uint8_t>& arrays_;
};

#endif
