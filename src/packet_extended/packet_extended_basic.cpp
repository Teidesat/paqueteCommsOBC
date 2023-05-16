#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedBasic::PacketExtendedBasic(const Packet& packet) :
    packet_(packet) {}

PacketExtendedBasic::~PacketExtendedBasic() {}

// low level packet 
const Packet& PacketExtendedBasic::getPacket() {
  return packet_;
}

PacketExtendedBasic PacketExtendedBasic::swapApplicationIdFields() {
  Packet newPacket = packet_;
  newPacket.setAppIdSource(packet_.getAppIdDestination());
  newPacket.setAppIdDestination(packet_.getAppIdSource());
  return PacketExtendedBasic(newPacket);
}
