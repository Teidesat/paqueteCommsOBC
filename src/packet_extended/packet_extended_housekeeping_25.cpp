#include "../../include/packet_extended/packet_extended_housekeeping_25.h"

PacketExtendedHousekeeping25::PacketExtendedHousekeeping25(
    const Packet& packet, uint16_t structureId,
    GenerationMode mode, const std::vector<uint8_t>& parameters) :
      packetHousekeepingBasic_(packet, structureId),
      mode_(mode) {
  interpretInputParameters(structureId);
}

Packet& PacketExtendedHousekeeping25::getPacket() {
  return packetHousekeepingBasic_.getPacket();
}

PacketExtendedHousekeeping25 PacketExtendedHousekeeping25::swapApplicationIdFields() {
  PacketExtendedHousekeeping25 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint16_t PacketExtendedHousekeeping25::getStructureIdentifier() {
  return packetHousekeepingBasic_.getStructureIdentifier();
}

std::vector<uint8_t> PacketExtendedHousekeeping25::getSampledParameters() {
  return sampledParameters_;
}

void PacketExtendedHousekeeping25::setSampledParameters(
   const std::vector<uint8_t> newSampledParameters) {
  sampledParameters_ = newSampledParameters;
}

std::vector<std::vector<uint8_t>> PacketExtendedHousekeeping25::getSampledArrays() {
  return sampledArrays_;
}

void PacketExtendedHousekeeping25::setSampledArrays(
   const std::vector<std::vector<uint8_t>> newSampledArrays) {
  sampledArrays_ = newSampledArrays;
}

PacketExtendedHousekeeping25::GenerationMode PacketExtendedHousekeeping25::getGenerationMode() {
  return mode_;
}

void PacketExtendedHousekeeping25::setGenerationMode(const GenerationMode newMode) {
  mode_ = newMode;
}

// used in constructor
void PacketExtendedHousekeeping25::interpretInputParameters(uint16_t structureId) {
  const auto& relevantDefinition =
      PacketExtendedHousekeeping::structureIdDefinitions.find(structureId)->second;

  // The definition is a vector structured as follows:
  // length first sequence, first sequence parameters, length second
  // sequence, second sequence arrays
  const uint8_t lengthFirstSequence = relevantDefinition[0];
  for (size_t i = 1; i < 1 + lengthFirstSequence; ++i) {
    sampledParameters_.push_back(relevantDefinition[i]);
  }
  const uint8_t lengthSecondSequence =
      relevantDefinition[1 + lengthFirstSequence];

  // For each array extract it's content and then update current position
  uint8_t currentPosition = 1 + lengthFirstSequence;
  for (size_t i = 0; i < lengthSecondSequence; ++i) {
    std::vector<uint8_t> arrayContent;
    uint8_t sampleAmount = currentPosition;
    uint8_t parameterAmount = currentPosition + 1;
    for (size_t j = currentPosition;
        j < currentPosition + sampleAmount * parameterAmount; ++j) {
      arrayContent.push_back(relevantDefinition[j]);
    }
    sampledArrays_.push_back(arrayContent);
    currentPosition += currentPosition + sampleAmount * parameterAmount;
  }
}