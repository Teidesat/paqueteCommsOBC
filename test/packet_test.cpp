#include <gtest/gtest.h>

#include "../include/packet.h"

TEST(Packet, SettersWorkProperly) {
  /**
   * Define a packet of verification service acceptance success
   *    (subtype 1) that is as follows:
   * 
   *  version type dfh   appid length sequence_control ccsds pus_version ack type subtype  error
   *    0      0    1     1, 2    9      (0b11, 2)        0       1        0   13     1     794 
   * 
   */
    const uint8_t versionNumber,
    const Bool8Enum dataFieldHeader,
    const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const SequenceFlags sequenceControlFlags,
    const uint16_t sequenceControlCount,
    const uint16_t length,
    const Bool8Enum ccsds,
    const uint8_t pusVersion,
    const Bool8Enum ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype,
    const std::array<std::byte, APP_DATA_SIZE>& appData,
    const std::array<std::byte, 2> packetErrorControl
  Packet packet(0, Packet::Bool8Enum::TRUE, 1, 2, Packet::SequenceFlags::STAND_ALONE,
      );
}
