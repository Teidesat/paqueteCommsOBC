#include "../include/packet.h"

#include <cassert>

Packet::Packet() :
  sequenceControlFlags_(0b11), // standalone by default
  pusVersion_(0b1), // currentStandard requires =1
  appDataIndex_(0)
{}

Packet::Packet(
  const uint8_t versionNumber,
  const uint8_t dataFieldHeader,
  const uint8_t appIdSource,
  const uint8_t appIdDestination,
  const uint8_t sequenceControlFlags,
  const uint8_t sequenceControlCount,
  const uint8_t length,
  const uint8_t ccsds,
  const uint8_t pusVersion,
  const uint8_t ack,
  const uint8_t serviceType,
  const uint8_t serviceSubtype,
  const std::array<uint8_t, Packet::APP_DATA_SIZE>& appData,
  const uint8_t packetErrorControl
) :
  versionNumber_(versionNumber),
  dataFieldHeader_(dataFieldHeader),
  appIdSource_(appIdSource),
  appIdDestination_(appIdDestination),
  sequenceControlFlags_(sequenceControlFlags),
  sequenceControlCount_(sequenceControlCount),
  length_(length),
  ccsds_(ccsds),
  pusVersion_(pusVersion),
  ack_(ack),
  serviceType_(serviceType),
  serviceSubtype_(serviceSubtype),
  appData_(appData),
  packetErrorControl_(packetErrorControl),
  appDataIndex_(0)
{}

Packet::~Packet() {}

uint8_t Packet::getVersionNumber() const {
  return versionNumber_;
}

void Packet::setVersionNumber(uint8_t versionNumber) {
  versionNumber_ = versionNumber;
}

uint8_t Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

void Packet::setDataFieldHeader(const uint8_t newValue) {
  dataFieldHeader_ = newValue;
}

uint8_t Packet::getAppIdSource() const {
  return appIdSource_;
}

void Packet::setAppIdSource(uint8_t newAddress) {
  appIdSource_ = newAddress;
}

uint8_t Packet::getAppIdDestination() const {
  return appIdDestination_;
}

void Packet::setAppIdDestination(uint8_t newAddress) {
  appIdDestination_ = newAddress;
}

uint8_t Packet::getSequenceControlFlags() const {
  return sequenceControlFlags_;
}

void Packet::setSequenceControlFlags(SequenceFlags const newFlags) {
  switch (newFlags) {
    case SequenceFlags::INITIAL:
      sequenceControlFlags_ = 0b01;
      break;
    case SequenceFlags::INBETWEEN:
      sequenceControlFlags_ = 0b00;
      break;
    case SequenceFlags::FINAL:
      sequenceControlFlags_ = 0b10;
      break;
    case SequenceFlags::STAND_ALONE:
      sequenceControlFlags_ = 0b11;
      break;
  }
}

uint16_t Packet::getSequenceControlCount() const {
  return sequenceControlCount_;
}

void Packet::setSequenceControlCount(uint16_t newCount) {
  sequenceControlCount_ = newCount;
}

uint16_t Packet::getLength() const {
  return length_;
}

void Packet::setLength(uint16_t amountOfBytes) {
  length_ = amountOfBytes;
}

uint8_t Packet::getCCSDS() const {
  return ccsds_;
}

void Packet::setCCSDS(uint8_t ccsds) {
  ccsds_ = ccsds;
}

uint8_t Packet::getPUSVersion() const {
  return pusVersion_;
}

void Packet::setPUSVersion(uint8_t pusVersion) {
  pusVersion_ = pusVersion;
}

uint8_t Packet::getAck() const {
  return ack_;
}

void Packet::setACK(const bool newValue) {
  ack_ = newValue;
}

uint8_t Packet::getServiceType() const {
  return serviceType_;
}

void Packet::setServiceType(uint8_t typeId) {
  serviceType_ = typeId;
}

uint8_t Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

void Packet::setServiceSubtype(uint8_t subtypeId) {
  serviceSubtype_ = subtypeId;
}

std::array<uint8_t, Packet::APP_DATA_SIZE> Packet::getAppData() {
  return appData_;
}

void Packet::setAppData(const std::array<uint8_t, APP_DATA_SIZE>& newAppData) {
  appData_ = newAppData;
}

uint16_t Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}

void Packet::setPacketErrorControl(uint16_t crc) {
  packetErrorControl_ = crc;
}
