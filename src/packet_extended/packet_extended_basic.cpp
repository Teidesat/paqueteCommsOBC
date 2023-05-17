#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedBasic::PacketExtendedBasic(const Packet& packet) :
    packet_(packet) {}

// low level packet 
Packet& PacketExtendedBasic::getPacket() {
  return packet_;
}

PacketExtendedBasic PacketExtendedBasic::swapApplicationIdFields() {
  PacketExtendedBasic copy = *this;
  copy.getPacket().setAppIdSource(
      copy.getPacket().getAppIdDestination());
  copy.getPacket().setAppIdDestination(
      copy.getPacket().getAppIdSource());
  return std::move(copy);
}
