#include <gtest/gtest.h>

#include "../include/packet.h"

TEST(Packet, SettersWorkProperly) {
  std::array<std::byte, Packet::APP_DATA_SIZE> appData;
  for (size_t i = 0; i < Packet::APP_DATA_SIZE; ++i) {
    appData[i] = std::byte(0);
  }

  // [0] most significant, [1] least
  std::array<std::byte, 2> packetErrorControl{std::byte(0b00011010),
      std::byte(0b00000011)};
  Packet packet(
      0,                                  // version
      Packet::Bool8Enum::TRUE,            // datafieldheader
      1,                                  // app id source
      2,                                  // app id destination
      Packet::SequenceFlags::STAND_ALONE, // seq. control flags
      2,                                  // seq. control count
      9,                                  // length
      Packet::Bool8Enum::TRUE,            // ccsds
      1,                                  // PUS version
      Packet::Bool8Enum::FALSE,           // ACK
      13,                                 // Service type
      1,                                  // Service subtype
      appData,
      packetErrorControl);

  packet.setVersionNumber(43);
  EXPECT_EQ(packet.getVersionNumber(), 43);

  packet.setDataFieldHeader(Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getDataFieldHeader(), Packet::Bool8Enum::FALSE);

  packet.setAppIdSource(5);
  EXPECT_EQ(packet.getAppIdSource(), 5);

  packet.setAppIdDestination(6);
  EXPECT_EQ(packet.getAppIdDestination(), 6);

  packet.setSequenceControlFlags(Packet::SequenceFlags::INBETWEEN);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::INBETWEEN);

  packet.setSequenceControlCount(3);
  EXPECT_EQ(packet.getSequenceControlCount(), 3);

  packet.setLength(10);
  EXPECT_EQ(packet.getLength(), 10);

  packet.setCCSDS(Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getCCSDS(), Packet::Bool8Enum::FALSE);

  packet.setPUSVersion(2);
  EXPECT_EQ(packet.getPUSVersion(), 2);

  packet.setAck(Packet::Bool8Enum::TRUE);
  EXPECT_EQ(packet.getAck(), Packet::Bool8Enum::TRUE);

  packet.setServiceType(14);
  EXPECT_EQ(packet.getServiceType(), 14);

  packet.setServiceSubtype(2);
  EXPECT_EQ(packet.getServiceSubtype(), 2);
  
  appData[0] = std::byte{0x0A};
  packet.setAppData(appData);
  const auto newAppData = packet.getAppData();
  for (size_t i = 0; i < Packet::APP_DATA_SIZE; ++i) {
    EXPECT_EQ(newAppData[i], appData[i]);
  }

  packetErrorControl[1] = std::byte{0xC2};
  packet.setPacketErrorControl(packetErrorControl);
  const auto newErrorControl = packet.getPacketErrorControl();
  for (size_t i = 0; i < 2; ++i) {
    EXPECT_EQ(newErrorControl[i], packetErrorControl[i]);
  }
}
