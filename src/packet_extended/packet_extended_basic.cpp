#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedBasic::PacketExtendedBasic(const Packet& packet) {}

PacketExtendedBasic::~PacketExtendedBasic() {}

// low level packet 
Packet& PacketExtendedBasic::getPacket() {
  return packet_;
}


int PacketExtendedBasic::getVersionNumber() {

}

int PacketExtendedBasic::getAppIdOrigin() {

}

int PacketExtendedBasic::getAppIdDestination() {

}

int PacketExtendedBasic::getSequenceCount() {

}

int PacketExtendedBasic::getLength() {

}


int PacketExtendedBasic::getPusVersion() {

}

int PacketExtendedBasic::getServiceType() {

}

int PacketExtendedBasic::getServiceSubtype() {

}

PacketExtendedBasic PacketExtendedBasic::getInstance() {
  
}

PacketExtendedBasic PacketExtendedBasic::getAcknowledgedVersion() {

}

void incrementSequenceCounter() {

}

// When replying to something, source becomes destination and viceversa.
PacketExtendedBasic swapApplicationIdFields() {

}
