#include "../include/packet.h"

#include <cassert>

Packet::Packet() :
  sequenceControlFlags_(std::byte{0x36}), // standalone by default
  pusVersion_(std::byte{0b1}), // currentStandard requires =1
  appDataIndex_(0)
{}

Packet::Packet(
  const std::byte versionNumber,
  const std::byte dataFieldHeader,
  const std::byte appIdSource,
  const std::byte appIdDestination,
  const std::byte sequenceControlFlags,
  const std::array<std::byte, 2> sequenceControlCount,
  const std::array<std::byte, 2> length,
  const std::byte ccsds,
  const std::byte pusVersion,
  const std::byte ack,
  const std::byte serviceType,
  const std::byte serviceSubtype,
  const std::array<std::byte, Packet::APP_DATA_SIZE>& appData,
  const std::array<std::byte, 2> packetErrorControl
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

std::byte Packet::getVersionNumber() const {
  return versionNumber_;
}

void Packet::setVersionNumber(std::byte versionNumber) {
  versionNumber_ = versionNumber;
}

std::byte Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

void Packet::setDataFieldHeader(const std::byte newValue) {
  dataFieldHeader_ = newValue;
}

std::byte Packet::getAppIdSource() const {
  return appIdSource_;
}

void Packet::setAppIdSource(std::byte newAddress) {
  appIdSource_ = newAddress;
}

std::byte Packet::getAppIdDestination() const {
  return appIdDestination_;
}

void Packet::setAppIdDestination(std::byte newAddress) {
  appIdDestination_ = newAddress;
}

std::byte Packet::getSequenceControlFlags() const {
  return sequenceControlFlags_;
}

void Packet::setSequenceControlFlags(SequenceFlags const newFlags) {
  switch (newFlags) {
    case SequenceFlags::INITIAL:
      sequenceControlFlags_ = std::byte{0b01};
      break;
    case SequenceFlags::INBETWEEN:
      sequenceControlFlags_ = std::byte{0b00};
      break;
    case SequenceFlags::FINAL:
      sequenceControlFlags_ = std::byte{0b10};
      break;
    case SequenceFlags::STAND_ALONE:
      sequenceControlFlags_ = std::byte{0b11};
      break;
  }
}

std::array<std::byte, 2> Packet::getSequenceControlCount() const {
  return sequenceControlCount_;
}

void Packet::setSequenceControlCount(std::array<std::byte, 2> newCount) {
  sequenceControlCount_ = newCount;
}

std::array<std::byte, 2> Packet::getLength() const {
  return length_;
}

void Packet::setLength(std::array<std::byte, 2> amountOfBytes) {
  length_ = amountOfBytes;
}

std::byte Packet::getCCSDS() const {
  return ccsds_;
}

void Packet::setCCSDS(std::byte ccsds) {
  ccsds_ = ccsds;
}

std::byte Packet::getPUSVersion() const {
  return pusVersion_;
}

void Packet::setPUSVersion(std::byte pusVersion) {
  pusVersion_ = pusVersion;
}

std::byte Packet::getAck() const {
  return ack_;
}

void Packet::setACK(const std::byte newValue) {
  ack_ = newValue;
}

std::byte Packet::getServiceType() const {
  return serviceType_;
}

void Packet::setServiceType(std::byte typeId) {
  serviceType_ = typeId;
}

std::byte Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

void Packet::setServiceSubtype(std::byte subtypeId) {
  serviceSubtype_ = subtypeId;
}

std::array<std::byte, Packet::APP_DATA_SIZE> Packet::getAppData() {
  return appData_;
}

void Packet::setAppData(const std::array<std::byte, APP_DATA_SIZE>& newAppData) {
  appData_ = newAppData;
}

std::array<std::byte, 2> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}

void Packet::setPacketErrorControl(std::array<std::byte, 2> crc) {
  packetErrorControl_ = crc;
}
