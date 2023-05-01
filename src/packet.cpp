#include "../include/packet.h"

Packet::Packet(std::bitset<VERSION_NUMBER_SIZE> const& versionNumber,
    std::bitset<DATA_FIELD_HEADER_SIZE> const& dataFieldHeader,
    std::bitset<APP_ID_SOURCE_SIZE> const& appIdSource,
    std::bitset<APP_ID_DESTINATION_SIZE> const& appIdDestination,
    std::bitset<SEQUENCE_CONTROL_SIZE> const& sequenceControl,
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
        sequenceControl_(sequenceControl),
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

std::bitset<Packet::SEQUENCE_CONTROL_SIZE> Packet::getSequenceControl() const {
  return sequenceControl_;
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
