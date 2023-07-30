#include "../include/packet.h"

// Make sure all values are initialized here or in the class declaration
Packet::Packet() :
  versionNumber_(0),
  dataFieldHeader_(false),
  appIdSource_(0),
  appIdDestination_(0),
  sequenceControlFlags_(Packet::SequenceFlags::STAND_ALONE),
  sequenceControlCount_(0),
  length_(0),
  ccsds_(false),
  pusVersion_(1), // ECSS-70-E-41A requires =1
  ack_(false),
  serviceType_(0),
  serviceSubtype_(0),
  appDataIndex_(0)
{
  for (size_t i = 0; i < Packet::APP_DATA_SIZE; ++i) {
    appData_[i] = 0b0;
  }
  packetErrorControl_[0] = 0b0;
  packetErrorControl_[1] = 0b0;
}

Packet::Packet(
  uint8_t versionNumber,
  bool dataFieldHeader,
  uint8_t appIdSource,
  uint8_t appIdDestination,
  Packet::SequenceFlags sequenceControlFlags,
  uint16_t sequenceControlCount,
  uint16_t length,
  bool ccsds,
  uint8_t pusVersion,
  bool ack,
  uint8_t serviceType,
  uint8_t serviceSubtype,
  std::array<uint8_t, Packet::APP_DATA_SIZE>& appData,
  std::array<uint8_t, 2> packetErrorControl
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

[[nodiscard]] uint8_t Packet::getVersionNumber() {
  return versionNumber_;
}

void Packet::setVersionNumber(uint8_t versionNumber) {
  versionNumber_ = versionNumber;
}

[[nodiscard]] bool Packet::getDataFieldHeader() {
  return dataFieldHeader_;
}

void Packet::setDataFieldHeader(bool newValue) {
  dataFieldHeader_ = newValue;
}

[[nodiscard]] uint8_t Packet::getAppIdSource() {
  return appIdSource_;
}

void Packet::setAppIdSource(uint8_t newAddress) {
  appIdSource_ = newAddress;
}

[[nodiscard]] uint8_t Packet::getAppIdDestination() {
  return appIdDestination_;
}

void Packet::setAppIdDestination(uint8_t newAddress) {
  appIdDestination_ = newAddress;
}

[[nodiscard]] Packet::SequenceFlags Packet::getSequenceControlFlags() {
  return sequenceControlFlags_;
}

void Packet::setSequenceControlFlags(Packet::SequenceFlags newFlags) {
  sequenceControlFlags_ = newFlags;
}

[[nodiscard]] uint16_t Packet::getSequenceControlCount() {
  return sequenceControlCount_;
}

void Packet::setSequenceControlCount(uint16_t newCount) {
  sequenceControlCount_ = newCount;
}

[[nodiscard]] uint16_t Packet::getLength() {
  return length_;
}

void Packet::setLength(uint16_t amountOfBytes) {
  length_ = amountOfBytes;
}

[[nodiscard]] bool Packet::getCCSDS() {
  return ccsds_;
}

void Packet::setCCSDS(bool ccsds) {
  ccsds_ = ccsds;
}

[[nodiscard]] uint8_t Packet::getPUSVersion() {
  return pusVersion_;
}

void Packet::setPUSVersion(uint8_t pusVersion) {
  pusVersion_ = pusVersion;
}

[[nodiscard]] bool Packet::getAck() {
  return ack_;
}

void Packet::setAck(bool newValue) {
  ack_ = newValue;
}

[[nodiscard]] uint8_t Packet::getServiceType() {
  return serviceType_;
}

void Packet::setServiceType(uint8_t typeId) {
  serviceType_ = typeId;
}

[[nodiscard]] uint8_t Packet::getServiceSubtype() {
  return serviceSubtype_;
}

void Packet::setServiceSubtype(uint8_t subtypeId) {
  serviceSubtype_ = subtypeId;
}

[[nodiscard]] std::array<uint8_t, Packet::APP_DATA_SIZE> Packet::getAppData() {
  return appData_;
}

void Packet::setAppData(std::array<uint8_t, Packet::APP_DATA_SIZE> newAppData) {
  appData_ = newAppData;
}

void Packet::pushData(uint8_t byteToPush) {
  appData_[appDataIndex_] = byteToPush;
  ++appDataIndex_;
}

void Packet::pushData(uint16_t bytesToPush) {
  appData_[appDataIndex_] = static_cast<uint8_t>(bytesToPush >> 8);
  appData_[appDataIndex_ + 1] = static_cast<uint8_t>(bytesToPush);
  appDataIndex_ += sizeof(bytesToPush);
}

void Packet::pushData(std::array<uint8_t, 2> bytesToPush) {
  appData_[appDataIndex_] = bytesToPush[0];
  ++appDataIndex_;
  appData_[appDataIndex_] = bytesToPush[1];
  ++appDataIndex_;
}

void Packet::pushData(std::vector<uint8_t> bytesToPush) {
  std::copy(bytesToPush.begin(), bytesToPush.end(), appData_.begin() + appDataIndex_);
  appDataIndex_ += bytesToPush.size();
}

[[nodiscard]] std::array<uint8_t, 2> Packet::getPacketErrorControl() {
  return packetErrorControl_;
}

void Packet::setPacketErrorControl(std::array<uint8_t, 2> crc) {
  packetErrorControl_ = crc;
}
