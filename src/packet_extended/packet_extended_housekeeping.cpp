#include "../../include/packet_extended/packet_extended_housekeeping.h"

std::unordered_map<uint16_t, std::vector<uint8_t>> PacketExtendedHousekeeping::structureIdDefinitions;

PacketExtendedHousekeeping::PacketExtendedHousekeeping(
    const Packet& packet, uint16_t structureId,
    GenerationMode mode, const std::vector<std::byte>& parameters) :
      packetBasic_(packet),
      structureId_(structureId),
      mode_(mode) {
  const auto& relevantDefinition = structureIdDefinitions.find(structureId)->second;
  
  // The definition is a vector structured as follows:
  // length first sequence, first sequence parameters, length second sequence,
  // second sequence arrays
  const uint8_t lengthFirstSequence = relevantDefinition[0];
  for (uint8_t i = 1; i < 1 + lengthFirstSequence; ++i) {
    sampledParameters_.push_back(relevantDefinition[i]);
  }
  const uint8_t lengthSecondSequence = relevantDefinition[1 + lengthFirstSequence];

  // For each array extract it's content and then update current position
  uint8_t currentPosition = 1 + lengthFirstSequence;
  for (uint8_t i = 0; i < lengthSecondSequence; ++i) {
    std::vector<uint8_t> arrayContent;
    uint8_t sampleAmount = currentPosition;
    uint8_t parameterAmount = currentPosition + 1;
    for (uint8_t j = currentPosition;
        j < currentPosition + sampleAmount * parameterAmount; ++j) {
      arrayContent.push_back(relevantDefinition[j]);
    }
    sampledArrays_.push_back(arrayContent);
    currentPosition += currentPosition + sampleAmount * parameterAmount;
  }
}

PacketExtendedHousekeeping::~PacketExtendedHousekeeping() {}

std::vector<uint8_t> PacketExtendedHousekeeping::getSampledParameters() {
  return sampledParameters_;
}

std::vector<std::vector<uint8_t>> PacketExtendedHousekeeping::getSampledArrays() {
  return sampledArrays_;
}

PacketExtendedHousekeeping::GenerationMode PacketExtendedHousekeeping::getGenerationMode() {
  return mode_;
}
