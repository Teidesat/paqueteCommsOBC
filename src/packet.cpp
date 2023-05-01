#include "../include/packet.h"

#include <cstring>

// without data field header
Packet::Packet(std::bitset<VERSION_NUMBER_SIZE> versionNumber,
    std::bitset<DATA_FIELD_HEADER_SIZE> dataFieldHeader,
    std::bitset<APP_ID_SOURCE_SIZE> appIdSource,
    std::bitset<APP_ID_DESTINATION_SIZE> appIdDestination,
    std::bitset<SEQUENCE_CONTROL_SIZE> sequenceControl,
    std::bitset<LENGTH_SIZE> length,
    std::bitset<PACKET_ERROR_CONTROL_SIZE> packetErrorControl) :
        versionNumber_(versionNumber),
        dataFieldHeader_(dataFieldHeader),
        appIdSource_(appIdSource),
        appIdDestination_(appIdDestination),
        sequenceControl_(sequenceControl),
        length_(length),
        packetErrorControl_(packetErrorControl)
{}

// with data field header
Packet::Packet(std::bitset<VERSION_NUMBER_SIZE> versionNumber,
    std::bitset<DATA_FIELD_HEADER_SIZE> dataFieldHeader,
    std::bitset<APP_ID_SOURCE_SIZE> appIdSource,
    std::bitset<APP_ID_DESTINATION_SIZE> appIdDestination,
    std::bitset<SEQUENCE_CONTROL_SIZE> sequenceControl,
    std::bitset<LENGTH_SIZE> length,
    std::bitset<ACK_SIZE> ack,
    std::bitset<SERVICE_TYPE_SIZE> serviceType,
    std::bitset<SERVICE_SUBTYPE_SIZE> serviceSubtype,
    std::vector<std::byte> appData,
    std::bitset<PACKET_ERROR_CONTROL_SIZE> packetErrorControl) :
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
