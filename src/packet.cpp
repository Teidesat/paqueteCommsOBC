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

std::bitset<3> Packet::getVersionNumber() const {
  return versionNumber_;
}

std::bitset<1> Packet::getDataFieldHeader() const {
  return dataFieldHeader_;
}

std::bitset<5> Packet::getAppIdSource() const {
  return appIdSource_;
}

std::bitset<5> Packet::getAppIdDestination() const {
  return appIdDestination_;
}

std::bitset<16> Packet::getSequenceControl() const {
  return sequenceControl_;
}

std::bitset<16> Packet::getLength() const {
  return length_;
}

std::bitset<4> Packet::getAck() const {
  return ack_;
}

std::bitset<8> Packet::getServiceType() const {
  return serviceType_;
}

std::bitset<8> Packet::getServiceSubtype() const {
  return serviceSubtype_;
}

std::vector<std::byte> Packet::getAppData() const {
  return appData_;
}

std::bitset<3> Packet::getPacketErrorControl() const {
  return packetErrorControl_;
}
