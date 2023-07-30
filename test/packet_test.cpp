#include <gtest/gtest.h>

#include <array>

#include "../include/packet.h"

class PacketTest : public testing::Test {
protected:
  void SetUp() override {
    for (size_t i = 0; i < Packet::APP_DATA_SIZE; ++i) {
      appData[i] = uint8_t(0);
    }

    // [0] most significant, [1] least
    packetErrorControl = {uint8_t(0b00011010),
        uint8_t(0b00000011)};
    packet = Packet(
        0,                                  // version
        true,            // datafieldheader
        1,                                  // app id source
        2,                                  // app id destination
        Packet::SequenceFlags::STAND_ALONE, // seq. control flags
        2,                                  // seq. control count
        9,                                  // length
        true,            // ccsds
        1,                                  // PUS version
        false,           // ACK
        13,                                 // Service type
        1,                                  // Service subtype
        appData,
        packetErrorControl
    );
  }

  // ****** members of the class ******
  Packet packet;
  std::array<uint8_t, Packet::APP_DATA_SIZE> appData;
  std::array<uint8_t, 2> packetErrorControl;
};

TEST_F(PacketTest, SettersAndGettersWorkProperly) {
  packet.setVersionNumber(43);
  EXPECT_EQ(packet.getVersionNumber(), 43);

  packet.setDataFieldHeader(false);
  EXPECT_EQ(packet.getDataFieldHeader(), false);

  packet.setAppIdSource(5);
  EXPECT_EQ(packet.getAppIdSource(), 5);

  packet.setAppIdDestination(6);
  EXPECT_EQ(packet.getAppIdDestination(), 6);

  packet.setSequenceControlFlags(Packet::SequenceFlags::IN_BETWEEN);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::IN_BETWEEN);

  packet.setSequenceControlCount(3);
  EXPECT_EQ(packet.getSequenceControlCount(), 3);

  packet.setLength(10);
  EXPECT_EQ(packet.getLength(), 10);

  packet.setCCSDS(false);
  EXPECT_EQ(packet.getCCSDS(), false);

  packet.setPUSVersion(2);
  EXPECT_EQ(packet.getPUSVersion(), 2);

  packet.setAck(true);
  EXPECT_EQ(packet.getAck(), true);

  packet.setServiceType(14);
  EXPECT_EQ(packet.getServiceType(), 14);

  packet.setServiceSubtype(2);
  EXPECT_EQ(packet.getServiceSubtype(), 2);
  
  appData[0] = uint8_t{0x0A};
  packet.setAppData(appData);
  const auto newAppData = packet.getAppData();
  for (size_t i = 0; i < Packet::APP_DATA_SIZE; ++i) {
    EXPECT_EQ(newAppData[i], appData[i]);
  }

  packetErrorControl[1] = uint8_t{0xC2};
  packet.setPacketErrorControl(packetErrorControl);
  const auto newErrorControl = packet.getPacketErrorControl();
  for (size_t i = 0; i < 2; ++i) {
    EXPECT_EQ(newErrorControl[i], packetErrorControl[i]);
  }
}

TEST_F(PacketTest, PushDataPushByteCanPush) {
  packet.pushData(uint8_t{0x78}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
}

TEST_F(PacketTest, PushDataPushByteDoesNotEraseContent) {
  packet.pushData(uint8_t{0x78}); // arbitrary data
  packet.pushData(uint8_t{0x9A}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
  EXPECT_EQ(newAppData[1], uint8_t{0x9A});
}

TEST_F(PacketTest, PushDataPushByteArrayCanPush) {
  packet.pushData(std::array<uint8_t, 2>{uint8_t{0x78}, uint8_t{0XBB}}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
  EXPECT_EQ(newAppData[1], uint8_t{0xBB});
}

TEST_F(PacketTest, PushDataPushByteArrayDoesNotEraseContent) {
  packet.pushData(std::array<uint8_t, 2>{uint8_t{0x78}, uint8_t{0XBB}}); // arbitrary data
  packet.pushData(std::array<uint8_t, 2>{uint8_t{0x11}, uint8_t{0X22}}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
  EXPECT_EQ(newAppData[1], uint8_t{0xBB});
  EXPECT_EQ(newAppData[2], uint8_t{0x11});
  EXPECT_EQ(newAppData[3], uint8_t{0x22});
}

TEST_F(PacketTest, PushDataPushByteVectorCanPush) {
  packet.pushData(std::vector<uint8_t>{uint8_t{0x78}, uint8_t{0XBB}}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
  EXPECT_EQ(newAppData[1], uint8_t{0xBB});
}

TEST_F(PacketTest, PushDataPushByteVectorDoesNotEraseContent) {
  packet.pushData(std::vector<uint8_t>{uint8_t{0x78}, uint8_t{0XBB}}); // arbitrary data
  packet.pushData(std::vector<uint8_t>{uint8_t{0x11}, uint8_t{0X22}}); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x78});
  EXPECT_EQ(newAppData[1], uint8_t{0xBB});
  EXPECT_EQ(newAppData[2], uint8_t{0x11});
  EXPECT_EQ(newAppData[3], uint8_t{0x22});
}

TEST_F(PacketTest, PushDataPushuint16_tCanPush) {
  packet.pushData(static_cast<uint16_t>(90)); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x00});
  EXPECT_EQ(newAppData[1], uint8_t{0x5A});
}

TEST_F(PacketTest, PushDataPushuint16_tDoesNotEraseContent) {
  packet.pushData(static_cast<uint16_t>(90)); // arbitrary data
  packet.pushData(static_cast<uint16_t>(17)); // arbitrary data
  auto newAppData = packet.getAppData();
  EXPECT_EQ(newAppData[0], uint8_t{0x00});
  EXPECT_EQ(newAppData[1], uint8_t{0x5A});
  EXPECT_EQ(newAppData[2], uint8_t{0x00});
  EXPECT_EQ(newAppData[3], uint8_t{0x11});
}
