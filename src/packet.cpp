#include "../include/packet.h"

#include <cassert>

Packet::Packet(const std::bitset<VERSION_NUMBER_SIZE>& versionNumber,
    const std::bitset<DATA_FIELD_HEADER_SIZE>& dataFieldHeader,
    const std::bitset<APP_ID_SOURCE_SIZE>& appIdSource,
    const std::bitset<APP_ID_DESTINATION_SIZE>& appIdDestination,
    const std::bitset<SEQUENCE_CONTROL_FLAGS_SIZE>& sequenceControlFlags,
    const std::bitset<SEQUENCE_CONTROL_COUNT_SIZE>& sequenceControlCount,
    const std::bitset<LENGTH_SIZE>& length,
    const std::bitset<ACK_SIZE>& ack,
    const std::bitset<SERVICE_TYPE_SIZE>& serviceType,
    const std::bitset<SERVICE_SUBTYPE_SIZE>& serviceSubtype,
    const std::array<std::byte, APP_DATA_SIZE>& appData,
    const std::bitset<PACKET_ERROR_CONTROL_SIZE>& packetErrorControl) :
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
        appData_(appData),
        packetErrorControl_(packetErrorControl)
{}

Packet::~Packet() {}

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

std::array<std::byte, Packet::APP_DATA_SIZE> Packet::getAppData() {
  return appData_;
}

void Packet::setAppData(const std::array<std::byte, APP_DATA_SIZE>& newAppData) {
  appData_ = newAppData;
}

std::bitset<Packet::PACKET_ERROR_CONTROL_SIZE> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}

void Packet::setVersionNumber(const std::bitset<VERSION_NUMBER_SIZE>& versionNumber) {
  versionNumber_ = versionNumber;
}

void Packet::setACK(const bool newValue) {
  ack_.set(ACK_SIZE - 1, newValue);
}

void Packet::setDataFieldHeader(const bool newValue) {
  dataFieldHeader_.set(DATA_FIELD_HEADER_SIZE - 1, newValue);
}

void Packet::setAppIdSource(const std::bitset<APP_ID_SOURCE_SIZE>& newAddress) {
  appIdSource_ = newAddress;
}

void Packet::setAppIdDestination(const std::bitset<APP_ID_DESTINATION_SIZE>& newAddress) {
  appIdDestination_ = newAddress;
}

void Packet::setLength(const std::bitset<LENGTH_SIZE>& amountOfBytes) {
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

void Packet::setSequenceControlCount(const std::bitset<SEQUENCE_CONTROL_COUNT_SIZE>& newCount) {
  std::bitset<SEQUENCE_CONTROL_COUNT_SIZE> countBits(newCount);
  sequenceControlCount_ = countBits;
}

void Packet::setServiceType(const std::bitset<SERVICE_TYPE_SIZE>& typeId) {
  std::bitset<SERVICE_TYPE_SIZE> typeBits(typeId);
  serviceType_ = typeBits;
}

void Packet::setServiceSubtype(const std::bitset<SERVICE_SUBTYPE_SIZE>& subtypeId) {
  std::bitset<SERVICE_SUBTYPE_SIZE> subtypeBits(subtypeId);
  serviceSubtype_ = subtypeBits;
}

void Packet::setPacketErrorControl(const std::bitset<PACKET_ERROR_CONTROL_SIZE>& crc) {
  std::bitset<PACKET_ERROR_CONTROL_SIZE> crcBits(crc);
  packetErrorControl_ = crcBits;
}
