#include "../../include/packet_extended/packet_extended_verification.h"

#include "../../include/packet_extended/packet_extended_basic.h"

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet) :
  packetBasic_(packet),
  errorCode_(-1)
{}

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet,
    uint8_t errorCode, const std::vector<uint8_t>& parameters) :
  packetBasic_(packet),
  errorCode_(errorCode),
  parameters_(parameters)
{}

PacketExtendedVerification::PacketExtendedVerification(const Packet& packet,
    uint8_t errorCode, std::vector<uint8_t>&& parameters) :
  packetBasic_(packet),
  errorCode_(errorCode),
  parameters_(std::move(parameters))
{}

PacketExtendedVerification::PacketExtendedVerification(
    const PacketExtendedVerification& other) :
  packetBasic_(other.packetBasic_),
  errorCode_(other.errorCode_),
  parameters_(other.parameters_)
{}

PacketExtendedVerification::PacketExtendedVerification(
    PacketExtendedVerification&& other) : 
  packetBasic_(std::move(other.packetBasic_)),
  errorCode_(std::move(other.errorCode_)),
  parameters_(std::move(other.parameters_))
{}

PacketExtendedVerification::~PacketExtendedVerification() {}

// low level packet 
Packet& PacketExtendedVerification::getPacket() {
  return packetBasic_.getPacket();
}

int PacketExtendedVerification::getVersionNumber() {
  return packetBasic_.getVersionNumber();
}

int PacketExtendedVerification::getAppIdOrigin() {
  return packetBasic_.getAppIdOrigin();
}

int PacketExtendedVerification::getAppIdDestination() {
  return packetBasic_.getAppIdDestination();
}

int PacketExtendedVerification::getSequenceCount() {
  return packetBasic_.getSequenceCount();
}

int PacketExtendedVerification::getLength() {
  return packetBasic_.getLength();
}

int PacketExtendedVerification::getPUSVersion() {
  return packetBasic_.getPUSVersion();
}

int PacketExtendedVerification::getServiceType() {
  return packetBasic_.getServiceType();
}

int PacketExtendedVerification::getServiceSubtype() {
  return packetBasic_.getServiceSubtype();
}

PacketExtendedVerification PacketExtendedVerification::getInstance() {
  return PacketExtendedVerification(packetBasic_.getPacket());
}

PacketExtendedVerification PacketExtendedVerification::getAcknowledgedVersion() {
  return PacketExtendedVerification(*this);
}

void PacketExtendedVerification::incrementSequenceCounter() {
  packetBasic_.incrementSequenceCounter();
}

PacketExtendedVerification PacketExtendedVerification::swapApplicationIdFields() {
  Packet packetCopy = packetBasic_.getPacket();
  return PacketExtendedVerification(std::move(packetCopy));
}

bool PacketExtendedVerification::hasErrorCode() {
  return errorCode_;
}

int PacketExtendedVerification::getErrorCode() {
  return errorCode_;
}

const std::vector<uint8_t>& PacketExtendedVerification::getParameters() {
  return parameters_;
}

