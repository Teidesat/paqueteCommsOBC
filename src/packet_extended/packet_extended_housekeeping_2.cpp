#include "../../include/packet_extended/packet_extended_housekeeping_2.h"

PacketExtendedHousekeeping2::PacketExtendedHousekeeping2(
    const Packet& packet, uint16_t structureId,
    uint8_t collectionInterval, uint8_t amountOfSamples,
    const std::vector<uint8_t>& samples, uint8_t amountOfArrays,
    const std::vector<std::vector<uint8_t>>& arrays) :
      packetHousekeepingBasic_(packet, structureId),
      collectionInterval_(collectionInterval),
      amountOfSamples_(amountOfSamples),
      samples_(samples),
      amountOfArrays_(amountOfArrays),
      arrays_(arrays)
{}

Packet& PacketExtendedHousekeeping2::getPacket() {
  return packetHousekeepingBasic_.getPacket();
}

PacketExtendedHousekeeping2 PacketExtendedHousekeeping2::swapApplicationIdFields() {
  PacketExtendedHousekeeping2 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

uint16_t PacketExtendedHousekeeping2::getStructureIdentifier() {
  return packetHousekeepingBasic_.getStructureIdentifier();
}

uint8_t PacketExtendedHousekeeping2::getCollectionInterval() {
  return collectionInterval_;
}

void PacketExtendedHousekeeping2::setCollectionInterval(
    const uint8_t newCollectionInterval) {
  collectionInterval_ = newCollectionInterval;
}

uint8_t PacketExtendedHousekeeping2::getAmountOfSamples() {
  return amountOfSamples_;
}

void PacketExtendedHousekeeping2::setAmountOfSamples(
    const uint8_t newAmountOfSamples) {
  amountOfSamples_ = newAmountOfSamples;
}

const std::vector<uint8_t>& PacketExtendedHousekeeping2::getSamples() {
  return samples_;
}

void PacketExtendedHousekeeping2::setSamples(
    const std::vector<uint8_t>& newSamples) {
  samples_ = newSamples;
}

void PacketExtendedHousekeeping2::setSamples(
    std::vector<uint8_t>&& newSamples) {
  samples_ = std::move(newSamples);
}

uint8_t PacketExtendedHousekeeping2::getAmountOfArrays() {
  return amountOfArrays_;
}

void PacketExtendedHousekeeping2::setAmountOfArrays(const uint8_t newAmountOfArrays) {
  amountOfArrays_ = newAmountOfArrays;
}