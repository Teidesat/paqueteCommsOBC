#include "../../include/packet_extended/packet_extended_verification.h"

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet) :
  packetBasic_(packet),
  errorCode_(0),
  hasErrorCode_(Packet::Bool8Enum::FALSE)
{}

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet,
    uint8_t errorCode, const std::vector<uint8_t>& parameters) :
  packetBasic_(packet),
  errorCode_(errorCode),
  hasErrorCode_(Packet::Bool8Enum::FALSE),
  parameters_(parameters)
{}

// low level packet 
Packet& PacketExtendedVerification::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedVerification PacketExtendedVerification::swapApplicationIdFields() {
  PacketExtendedVerification copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

Packet::Bool8Enum PacketExtendedVerification::hasErrorCode() {
  return hasErrorCode_;
}

int PacketExtendedVerification::getErrorCode() {
  return errorCode_;
}

std::vector<uint8_t> PacketExtendedVerification::getParameters() {
  return parameters_;
}

