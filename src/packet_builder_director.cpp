#include "../include/packet_builder_director.h"

#include <cmath>

PacketBuilderDirector::PacketBuilderDirector() {}
PacketBuilderDirector::~PacketBuilderDirector() {}

std::vector<Packet> PacketBuilderDirector::makeMegaPacket(
    const std::vector<std::byte>& appData) {
  std::vector<Packet> output;
  const size_t amountOfFullChunks = appData.size() / Packet::APP_DATA_SIZE;

  // copy chunk into packet, push packet
  for (size_t i = 0; i < amountOfFullChunks; ++i) {
    std::array<std::byte, Packet::APP_DATA_SIZE> chunk;
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
    std::array<std::byte, Packet::APP_DATA_SIZE> chunk;
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
