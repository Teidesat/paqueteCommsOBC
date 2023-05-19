#include "../../include/packet_extended/packet_extended_verification_1.h"

PacketExtendedVerification1::PacketExtendedVerification1(const Packet& packet) :
  packetBasic_(packet),
  errorCode_(0),
  hasErrorCode_(Packet::Bool8Enum::FALSE)
{}

PacketExtendedVerification1::PacketExtendedVerification1(const Packet& packet,
    uint8_t errorCode, const std::vector<uint8_t>& parameters) :
  packetBasic_(packet),
  errorCode_(errorCode),
  hasErrorCode_(Packet::Bool8Enum::FALSE),
  parameters_(parameters)
{}

// low level packet 
Packet& PacketExtendedVerification1::getPacket() {
  return packetBasic_.getPacket();
}

PacketExtendedVerification1 PacketExtendedVerification1::swapApplicationIdFields() {
  PacketExtendedVerification1 copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}

Packet::Bool8Enum PacketExtendedVerification1::hasErrorCode() {
  return hasErrorCode_;
}

int PacketExtendedVerification1::getErrorCode() {
  return errorCode_;
}

std::vector<uint8_t> PacketExtendedVerification1::getParameters() {
  return parameters_;
}

