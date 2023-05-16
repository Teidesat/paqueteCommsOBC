#include "../../include/packet_extended/packet_extended_verification.h"

#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet) :
  packetBasic_(packet),
  errorCode_(0),
  hasErrorCode_(false)
{}

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet,
    uint8_t errorCode, const std::vector<uint8_t>& parameters) :
  packetBasic_(packet),
  errorCode_(errorCode),
  hasErrorCode_(true),
  parameters_(parameters)
{}

PacketExtendedVerification::~PacketExtendedVerification() {}

PacketExtendedVerification PacketExtendedVerification::swapApplicationIdFields() {
  Packet newPacket = packetBasic_.getPacket();
  newPacket.setAppIdSource(packetBasic_.getPacket().getAppIdDestination());
  newPacket.setAppIdDestination(packetBasic_.getPacket().getAppIdSource());
  return PacketExtendedVerification(newPacket);
}

bool PacketExtendedVerification::hasErrorCode() {
  return hasErrorCode_;
}

int PacketExtendedVerification::getErrorCode() {
  return errorCode_;
}

const std::vector<uint8_t>& PacketExtendedVerification::getParameters() {
  return parameters_;
}

