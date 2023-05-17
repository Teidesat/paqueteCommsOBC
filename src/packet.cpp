#include "../include/packet.h"

#include <cassert>

Packet::Packet() :
  sequenceControlFlags_(SequenceFlags::STAND_ALONE), // standalone by default
  pusVersion_(1), // currentStandard requires =1
  appDataIndex_(0)
{}

Packet::Packet(
  const uint8_t versionNumber,
  const bool dataFieldHeader,
  const uint8_t appIdSource,
  const uint8_t appIdDestination,
  const SequenceFlags sequenceControlFlags,
  const uint16_t sequenceControlCount,
  const uint16_t length,
  const bool ccsds,
  const uint8_t pusVersion,
  const bool ack,
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

Packet::~Packet() {}

uint8_t Packet::getVersionNumber() const {
  return versionNumber_;
}

void Packet::setVersionNumber(uint8_t versionNumber) {
  versionNumber_ = versionNumber;
}

bool Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

void Packet::setDataFieldHeader(const bool newValue) {
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

bool Packet::getCCSDS() const {
  return ccsds_;
}

void Packet::setCCSDS(const bool ccsds) {
  ccsds_ = ccsds;
}

uint8_t Packet::getPUSVersion() const {
  return pusVersion_;
}

void Packet::setPUSVersion(const uint8_t pusVersion) {
  pusVersion_ = pusVersion;
}

bool Packet::getAck() const {
  return ack_;
}

void Packet::setACK(const bool newValue) {
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

void Packet::setAppData(const std::array<std::byte, APP_DATA_SIZE>& newAppData) {
  appData_ = newAppData;
}

void Packet::pushData(const std::byte byteToPush) {
  appData_[appDataIndex_] = byteToPush;
  ++appDataIndex_;
}

void Packet::pushData(const uint16_t bytesToPush) {
  std::memcpy(&appData_[appDataIndex_], &bytesToPush, sizeof(bytesToPush));
  appDataIndex_ += sizeof(bytesToPush);
}

void Packet::pushData(const std::array<std::byte, 2>& bytesToPush) {
  pushData(bytesToPush[0]);
  pushData(bytesToPush[1]);
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
