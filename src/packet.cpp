#include "../include/packet.h"

Packet::Packet(std::bitset<VERSION_NUMBER_SIZE> const& versionNumber,
    std::bitset<DATA_FIELD_HEADER_SIZE> const& dataFieldHeader,
    std::bitset<APP_ID_SOURCE_SIZE> const& appIdSource,
    std::bitset<APP_ID_DESTINATION_SIZE> const& appIdDestination,
    std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE> const& sequenceControlFlags,
    std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> const& sequenceControlCount,
    std::bitset<LENGTH_SIZE> const& length,
    std::bitset<ACK_SIZE> const& ack,
    std::bitset<SERVICE_TYPE_SIZE> const& serviceType,
    std::bitset<SERVICE_SUBTYPE_SIZE> const& serviceSubtype,
    std::vector<std::byte> const& appData,
    std::bitset<PACKET_ERROR_CONTROL_SIZE> const& packetErrorControl) :
        versionNumber_(versionNumber),
        dataFieldHeader_(dataFieldHeader),
        appIdSource_(appIdSource),
        appIdDestination_(appIdDestination),
        sequenceControlFlags_(sequenceControlFlags),
        sequenceControlCount_(sequenceControlCount),
        length_(length),
        ack_(ack),
        serviceType_(serviceType),
        serviceSubtype_(serviceSubtype),
        appData_(appData.begin(), appData.end()),
        packetErrorControl_(packetErrorControl)
{}

std::bitset<Packet::VERSION_NUMBER_SIZE> Packet::getVersionNumber() const {
  return versionNumber_;
}

std::bitset<Packet::DATA_FIELD_HEADER_SIZE> Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

std::bitset<Packet::APP_ID_SOURCE_SIZE> Packet::getAppIdSource() const {
  return appIdSource_;
}

std::bitset<Packet::APP_ID_DESTINATION_SIZE> Packet::getAppIdDestination() const {
  return appIdDestination_;
}

std::bitset<Packet::SEQUENCE_CONTROL_FLAGS_SIZE> Packet::getSequenceControlFlags() const {
  return sequenceControlFlags_;
}

std::bitset<Packet::SEQUENCE_CONTROL_COUNT_SIZE> Packet::getSequenceControlCount() const {
  return sequenceControlCount_;
}

std::bitset<Packet::LENGTH_SIZE> Packet::getLength() const {
  return length_;
}

std::bitset<Packet::ACK_SIZE> Packet::getAck() const {
  return ack_;
}

std::bitset<Packet::SERVICE_TYPE_SIZE> Packet::getServiceType() const {
  return serviceType_;
}

std::bitset<Packet::SERVICE_SUBTYPE_SIZE> Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

std::vector<std::byte> Packet::getAppData() const {
  return appData_;
}

std::bitset<Packet::PACKET_ERROR_CONTROL_SIZE> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}

void Packet::setVersionNumber(std::bitset<VERSION_NUMBER_SIZE> const& versionNumber) {
  versionNumber_ = versionNumber;
}

void Packet::setACK(const bool newValue) {
  ack_.set(ACK_SIZE - 1, newValue);
}

void Packet::setDataFieldHeader(const bool newValue) {
  dataFieldHeader_.set(DATA_FIELD_HEADER_SIZE - 1, newValue);
}

void Packet::setAppIdSource(const std::byte newAddress) {
  std::bitset<APP_ID_SOURCE_SIZE> addressBits(std::to_integer<int>(newAddress));
  appIdSource_ = addressBits;
}

void Packet::setAppIdDestination(const std::byte newAddress) {
  std::bitset<APP_ID_DESTINATION_SIZE> addressBits(std::to_integer<int>(newAddress));
  appIdDestination_ = addressBits;
}

void Packet::setLength(const uint16_t amountOfBytes) {
  std::bitset<LENGTH_SIZE> lengthBits(amountOfBytes);
  length_ = lengthBits;
}

void Packet::setSequenceControlFlags(SequenceFlags const newFlags) {
  sequenceControlFlags_ = std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE>(static_cast<uint8_t>(newFlags));
  switch (newFlags) {
    case SequenceFlags::INITIAL:
      sequenceControlFlags_.set(0, 0);
      sequenceControlFlags_.set(1, 1);
      break;
    case SequenceFlags::INBETWEEN:
      sequenceControlFlags_.set(0, 1);
      sequenceControlFlags_.set(1, 0);
      break;
    case SequenceFlags::FINAL:
      sequenceControlFlags_.set(0, 1);
      sequenceControlFlags_.set(1, 1);
      break;
    case SequenceFlags::STAND_ALONE:
      sequenceControlFlags_.reset();
      break;
  }
}

void Packet::setSequenceControlCount(const uint16_t newCount) {
  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> countBits(newCount);
  sequenceControlCount_ = countBits;
}

void Packet::setServiceType(const uint8_t typeId) {
  std::bitset<SERVICE_TYPE_SIZE> typeBits(typeId);
  serviceType_ = typeBits;
}

void Packet::setServiceSubtype(const uint8_t subtypeId) {
  std::bitset<SERVICE_SUBTYPE_SIZE> subtypeBits(subtypeId);
  serviceSubtype_ = subtypeBits;
}

void Packet::setPacketErrorControl(const uint8_t crc) {
  std::bitset<PACKET_ERROR_CONTROL_SIZE> crcBits(crc);
  packetErrorControl_ = crcBits;
}

void Packet::setAppData(const std::vector<std::byte>& newAppData) {
    appData_.assign(newAppData.begin(), newAppData.end());
}