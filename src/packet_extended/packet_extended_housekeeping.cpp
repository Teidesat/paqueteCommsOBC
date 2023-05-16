#include "../../include/packet_extended/packet_extended_housekeeping.h"

PacketExtendedHousekeeping::PacketExtendedHousekeeping(
    const Packet& packet, int structureId,
    GenerationMode mode, const std::vector<std::byte>& parameters) :
      packetBasic_(packet),
      structureId_(structureId),
      mode_(mode) {
  
}

PacketExtendedHousekeeping::~PacketExtendedHousekeeping() {}

std::vector<uint8_t> PacketExtendedHousekeeping::getSampledParameters() {

}

std::vector<std::vector<uint8_t>> PacketExtendedHousekeeping::getSampledArrays() {

}

PacketExtendedHousekeeping::GenerationMode PacketExtendedHousekeeping::getGenerationMode() {
  return mode_;
}
