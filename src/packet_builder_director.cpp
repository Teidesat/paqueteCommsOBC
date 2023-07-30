#include "../include/packet_builder_director.h"

#include <cmath>

std::vector<Packet> PacketBuilderDirector::makeMegaPacket(
    const std::vector<uint8_t>& appData) {
  std::vector<Packet> output;
  const size_t amountOfFullChunks = appData.size() / Packet::APP_DATA_SIZE;

  // copy chunk into packet, push packet
  for (size_t i = 0; i < amountOfFullChunks; ++i) {
    std::array<uint8_t, Packet::APP_DATA_SIZE> chunk{};
    std::copy(
        appData.begin() + (i * Packet::APP_DATA_SIZE),
        appData.begin() + ((i + 1) * Packet::APP_DATA_SIZE),
        chunk.begin()
    );
    Packet newPacket;
    newPacket.setAppData(chunk);
    output.push_back(newPacket);
  }

  // last chunk is not full, so different implementation
  const int amountOfBytesPending = appData.size() % Packet::APP_DATA_SIZE;
  if (amountOfBytesPending > 0) {
    Packet newPacket;
    std::array<uint8_t, Packet::APP_DATA_SIZE> chunk{};
    std::copy(
      appData.end() - amountOfBytesPending - 1,
      appData.end(),
      chunk.begin()
    );
    newPacket.setAppData(chunk);
    output.push_back(newPacket);
  }
  return output;
}

PacketExtendedVerification1 PacketBuilderDirector::makeVerificationSuccess(
    const uint8_t appIdSource, const uint8_t appIdDestination,
    const uint16_t sequenceCount) {
  builder_.newPacket(0, appIdSource, appIdDestination,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount);
  
  // it swaps app id's
  builder_.addCommandVerificationAppData(appIdDestination, appIdSource,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount);
  return { builder_.getPacket() };
}

PacketExtendedVerification1 PacketBuilderDirector::makeVerificationFailure(
    const uint8_t appIdSource, const uint8_t appIdDestination,
    const uint16_t sequenceCount, const uint8_t code,
    const std::vector<uint8_t>& parameters) {
  builder_.newPacket(0, appIdSource, appIdDestination,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount);

  // it swaps app id's
  builder_.addCommandVerificationAppData(appIdDestination, appIdSource,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount, code, parameters);
  return { builder_.getPacket() };
}

// std::move parameters
PacketExtendedVerification1 PacketBuilderDirector::makeVerificationFailure(
    const uint8_t appIdSource, const uint8_t appIdDestination,
    const uint16_t sequenceCount, const uint8_t code,
    std::vector<uint8_t>&& parameters) {
  builder_.newPacket(0, appIdSource, appIdDestination,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount);
  
  // it swaps app id's.
  builder_.addCommandVerificationAppData(appIdDestination, appIdSource,
      Packet::SequenceFlags::STAND_ALONE, sequenceCount, code,
      parameters); // No need for std::move here since we are using a reference
  return { builder_.getPacket() };
}
