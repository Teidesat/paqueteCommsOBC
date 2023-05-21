#include <gtest/gtest.h>

#include "../include/packet.h"
#include "../include/packet_builder.h"

TEST(PacketBuilder, NewPacketErasesPrevious) {
  PacketBuilder builder;
  builder.newPacket(
    0,                                  // version
    1,                                  // app id source
    2,                                  // app id destination
    Packet::SequenceFlags::STAND_ALONE, // seq. control flags
    2                                   // seq. control count
  );
  Packet previousPacket = builder.getPacket();
  builder.newPacket(
    0,                                  // version
    1,                                  // app id source
    2,                                  // app id destination
    Packet::SequenceFlags::STAND_ALONE, // seq. control flags
    3                                   // seq. control count << Different
  );
  Packet finalPacket = builder.getPacket();
  EXPECT_EQ(finalPacket.getSequenceControlCount(), 3);
}

TEST(PacketBuilder, CanGetPacketBeforeCallingNewPacket) {
  PacketBuilder builder;
  Packet initialPacket = builder.getPacket();
  Packet defaultConstructorPacket;
  EXPECT_EQ(initialPacket.getVersionNumber(), defaultConstructorPacket.getVersionNumber());
  EXPECT_EQ(initialPacket.getDataFieldHeader(), defaultConstructorPacket.getDataFieldHeader());
  EXPECT_EQ(initialPacket.getAppIdSource(), defaultConstructorPacket.getAppIdSource());
  EXPECT_EQ(initialPacket.getAppIdDestination(), defaultConstructorPacket.getAppIdDestination());
  EXPECT_EQ(initialPacket.getSequenceControlFlags(), defaultConstructorPacket.getSequenceControlFlags());
  EXPECT_EQ(initialPacket.getSequenceControlCount(), defaultConstructorPacket.getSequenceControlCount());
  EXPECT_EQ(initialPacket.getLength(), defaultConstructorPacket.getLength());
  EXPECT_EQ(initialPacket.getCCSDS(), defaultConstructorPacket.getCCSDS());
  EXPECT_EQ(initialPacket.getPUSVersion(), defaultConstructorPacket.getPUSVersion());
  EXPECT_EQ(initialPacket.getAck(), defaultConstructorPacket.getAck());
  EXPECT_EQ(initialPacket.getServiceType(), defaultConstructorPacket.getServiceType());
  EXPECT_EQ(initialPacket.getServiceSubtype(), defaultConstructorPacket.getServiceSubtype());

  // Compare app data
  auto initialAppData = initialPacket.getAppData();
  auto defaultAppData = defaultConstructorPacket.getAppData();
  EXPECT_EQ(initialAppData, defaultAppData);

  // Compare packet error control
  auto initialPacketErrorControl = initialPacket.getPacketErrorControl();
  auto defaultPacketErrorControl = defaultConstructorPacket.getPacketErrorControl();
  EXPECT_EQ(initialPacketErrorControl, defaultPacketErrorControl);
}

TEST(PacketBuilder, DataFieldHeaderGetsAddedProperly) {
  PacketBuilder builder;
  builder.newPacket(
    0,                                  // version
    1,                                  // app id source
    2,                                  // app id destination
    Packet::SequenceFlags::STAND_ALONE, // seq. control flags
    3                                   // seq. control count
  );
  auto packetWithout = builder.getPacket();

  // check if data field header has default values
  EXPECT_EQ(packetWithout.getDataFieldHeader(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packetWithout.getAck(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packetWithout.getServiceType(), 0);
  EXPECT_EQ(packetWithout.getServiceSubtype(), 0);

  builder.setDataFieldHeader(
    Packet::Bool8Enum::TRUE,           // ACK
    13,                                 // Service type
    1                                   // Service subtype
  );
  auto packetWith = builder.getPacket();
  EXPECT_EQ(packetWith.getDataFieldHeader(), Packet::Bool8Enum::TRUE);
  EXPECT_EQ(packetWith.getAck(), Packet::Bool8Enum::TRUE);
  EXPECT_EQ(packetWith.getServiceType(), 13);
  EXPECT_EQ(packetWith.getServiceSubtype(), 1);
}

class PacketBuilderTest : public testing::Test {
protected:
  void SetUp() override {
    builder.newPacket(
      0,                                  // version
      1,                                  // app id source
      2,                                  // app id destination
      Packet::SequenceFlags::STAND_ALONE, // seq. control flags
      1                                   // seq. control count
    );
    builder.setDataFieldHeader(
      Packet::Bool8Enum::TRUE,            // ACK
      1,                                  // Service type
      1                                   // Service subtype
    );
    packet = builder.getPacket();
  }

  // ***** Member variables *****
  Packet packet;
  PacketBuilder builder;
};

TEST_F(PacketBuilderTest, CanMakeVerificationAcceptanceSuccessFromScratch) {
  // information that was in the packet that is being acceptance success verified
  builder.addCommandVerificationAppData(
    2,                                  // source
    1,                                  // destination
    Packet::SequenceFlags::STAND_ALONE, // flags
    1                                   // count
  );
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[0], std::byte(0x02));
  EXPECT_EQ(appData[1], std::byte(0x01));
  EXPECT_EQ(appData[2], std::byte(Packet::SequenceFlags::STAND_ALONE));
  EXPECT_EQ(appData[3], std::byte(0x00));
  EXPECT_EQ(appData[4], std::byte(0x01));
}

TEST_F(PacketBuilderTest, CanMakeVerificationAcceptanceSuccessTwice) {
  // information that was in the packet that is being acceptance success verified
  builder.addCommandVerificationAppData(
    2,                                  // source
    1,                                  // destination
    Packet::SequenceFlags::STAND_ALONE, // flags
    1                                   // count
  );
  // add add another command verification app data
  builder.addCommandVerificationAppData(
    4,                                  // source
    8,                                  // destination
    Packet::SequenceFlags::INITIAL,     // flags
    2                                   // count
  );
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[5], std::byte(0x04));
  EXPECT_EQ(appData[6], std::byte(0x08));
  EXPECT_EQ(appData[7], std::byte(Packet::SequenceFlags::INITIAL));
  EXPECT_EQ(appData[8], std::byte(0x00));
  EXPECT_EQ(appData[9], std::byte(0x02));
}