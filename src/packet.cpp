#include "../include/packet.h"

Packet::Packet() :
  sequenceControlFlags_(Packet::SequenceFlags::STAND_ALONE), // standalone by default
  pusVersion_(1), // currentStandard requires =1
  appDataIndex_(0)
{}

Packet::Packet(
  const uint8_t versionNumber,
  const Packet::Bool8Enum dataFieldHeader,
  const uint8_t appIdSource,
  const uint8_t appIdDestination,
  const Packet::SequenceFlags sequenceControlFlags,
  const uint16_t sequenceControlCount,
  const uint16_t length,
  const Packet::Bool8Enum ccsds,
  const uint8_t pusVersion,
  const Packet::Bool8Enum ack,
  const uint8_t serviceType,
  const uint8_t serviceSubtype,
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

Packet::Packet(const Packet& other) :
  versionNumber_(other.versionNumber_),
  dataFieldHeader_(other.dataFieldHeader_),
  appIdSource_(other.appIdSource_),
  appIdDestination_(other.appIdDestination_),
  sequenceControlFlags_(other.sequenceControlFlags_),
  sequenceControlCount_(other.sequenceControlCount_),
  length_(other.length_),
  ccsds_(other.ccsds_),
  pusVersion_(other.pusVersion_),
  ack_(other.ack_),
  serviceType_(other.serviceType_),
  serviceSubtype_(other.serviceSubtype_),
  appData_(other.appData_),
  packetErrorControl_(other.packetErrorControl_),
  appDataIndex_(other.appDataIndex_) {}

uint8_t Packet::getVersionNumber() const {
  return versionNumber_;
}

void Packet::setVersionNumber(uint8_t versionNumber) {
  versionNumber_ = versionNumber;
}

Packet::Bool8Enum Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

void Packet::setDataFieldHeader(const Packet::Bool8Enum newValue) {
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

Packet::SequenceFlags Packet::getSequenceControlFlags() const {
  return sequenceControlFlags_;
}

void Packet::setSequenceControlFlags(Packet::SequenceFlags const newFlags) {
  sequenceControlFlags_ = newFlags;
}

uint16_t Packet::getSequenceControlCount() const {
  return sequenceControlCount_;
}

void Packet::setSequenceControlCount(const uint16_t newCount) {
  sequenceControlCount_ = newCount;
}

uint16_t Packet::getLength() const {
  return length_;
}

void Packet::setLength(const uint16_t amountOfBytes) {
  length_ = amountOfBytes;
}

Packet::Bool8Enum Packet::getCCSDS() const {
  return ccsds_;
}

void Packet::setCCSDS(const Packet::Bool8Enum ccsds) {
  ccsds_ = ccsds;
}

uint8_t Packet::getPUSVersion() const {
  return pusVersion_;
}

void Packet::setPUSVersion(const uint8_t pusVersion) {
  pusVersion_ = pusVersion;
}

Packet::Bool8Enum Packet::getAck() const {
  return ack_;
}

void Packet::setAck(const Packet::Bool8Enum newValue) {
  ack_ = newValue;
}

uint8_t Packet::getServiceType() const {
  return serviceType_;
}

void Packet::setServiceType(const uint8_t typeId) {
  serviceType_ = typeId;
}

uint8_t Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

void Packet::setServiceSubtype(const uint8_t subtypeId) {
  serviceSubtype_ = subtypeId;
}

std::array<std::byte, Packet::APP_DATA_SIZE> Packet::getAppData() {
  return appData_;
}

void Packet::setAppData(const std::array<std::byte, Packet::APP_DATA_SIZE>& newAppData) {
  appData_ = newAppData;
}

void Packet::pushData(const std::byte byteToPush) {
  appData_[appDataIndex_] = byteToPush;
  ++appDataIndex_;
}

void Packet::pushData(const uint16_t bytesToPush) {
  appData_[appDataIndex_] = std::byte(bytesToPush >> 8);
  appData_[appDataIndex_ + 1] = std::byte(bytesToPush);
  appDataIndex_ += sizeof(bytesToPush);
}

void Packet::pushData(const std::array<std::byte, 2> bytesToPush) {
  appData_[appDataIndex_] = bytesToPush[0];
  ++appDataIndex_;
  appData_[appDataIndex_] = bytesToPush[1];
  ++appDataIndex_;
}

void Packet::pushData(const std::vector<std::byte>& bytesToPush) {
  std::copy(bytesToPush.begin(), bytesToPush.end(), appData_.begin() + appDataIndex_);
  appDataIndex_ += bytesToPush.size();
}

std::array<std::byte, 2> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}

void Packet::setPacketErrorControl(std::array<std::byte, 2> crc) {
  packetErrorControl_ = crc;
}
