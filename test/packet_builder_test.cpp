#include <gtest/gtest.h>

#include <vector>

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
  EXPECT_EQ(packetWithout.getDataFieldHeader(), false);
  EXPECT_EQ(packetWithout.getAck(), false);
  EXPECT_EQ(packetWithout.getServiceType(), 0);
  EXPECT_EQ(packetWithout.getServiceSubtype(), 0);

  builder.setDataFieldHeader(
    true,           // ACK
    13,                                 // Service type
    1                                   // Service subtype
  );
  auto packetWith = builder.getPacket();
  EXPECT_EQ(packetWith.getDataFieldHeader(), true);
  EXPECT_EQ(packetWith.getAck(), true);
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
      true,                               // ACK
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
  EXPECT_EQ(appData[0], uint8_t(0x02));
  EXPECT_EQ(appData[1], uint8_t(0x01));
  EXPECT_EQ(appData[2], uint8_t(Packet::SequenceFlags::STAND_ALONE));
  EXPECT_EQ(appData[3], uint8_t(0x00));
  EXPECT_EQ(appData[4], uint8_t(0x01));
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
  EXPECT_EQ(appData[5], uint8_t(0x04));
  EXPECT_EQ(appData[6], uint8_t(0x08));
  EXPECT_EQ(appData[7], uint8_t(Packet::SequenceFlags::INITIAL));
  EXPECT_EQ(appData[8], uint8_t(0x00));
  EXPECT_EQ(appData[9], uint8_t(0x02));
}

// Register on/off

TEST_F(PacketBuilderTest, CanMakeCommandDistributionOnOffFromScratch) {
  std::vector<uint8_t> addresses;

  // add 7 arbitrary addresses
  for (size_t i = 0; i < 7; ++i) {
    addresses.push_back(static_cast<uint8_t>(i + 1));
  }
  builder.addCommandDistributionAppData(addresses);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[0], uint8_t(0x07)); // amount of addresses
  EXPECT_EQ(appData[1], uint8_t(0x01));
  EXPECT_EQ(appData[2], uint8_t(0x02));
  EXPECT_EQ(appData[3], uint8_t(0x03));
  EXPECT_EQ(appData[4], uint8_t(0x04));
  EXPECT_EQ(appData[5], uint8_t(0x05));
  EXPECT_EQ(appData[6], uint8_t(0x06));
  EXPECT_EQ(appData[7], uint8_t(0x07));
}

TEST_F(PacketBuilderTest, CanMakeCommandDistributionOnOffTwice) {
  std::vector<uint8_t> addresses;

  // add 7 arbitrary addresses
  for (size_t i = 0; i < 7; ++i) {
    addresses.push_back(static_cast<uint8_t>(i + 1));
  }
  builder.addCommandDistributionAppData(addresses);
  builder.addCommandDistributionAppData(addresses);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[8], uint8_t(0x07)); // amount of addresses
  EXPECT_EQ(appData[9], uint8_t(0x01));
  EXPECT_EQ(appData[10], uint8_t(0x02));
  EXPECT_EQ(appData[11], uint8_t(0x03));
  EXPECT_EQ(appData[12], uint8_t(0x04));
  EXPECT_EQ(appData[13], uint8_t(0x05));
  EXPECT_EQ(appData[14], uint8_t(0x06));
  EXPECT_EQ(appData[15], uint8_t(0x07));
}

// If only one address then a length byte should not be pushed because there
// is no need for it.
TEST_F(PacketBuilderTest, CanMakeCommandDistributionOneAddressDoesNotIncludeLengthByte) {
  std::vector<uint8_t> addresses;
  addresses.push_back(14);
  builder.addCommandDistributionAppData(addresses);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[0], uint8_t(0xE));
}

// Register load

TEST_F(PacketBuilderTest, CanMakeCommandDistributionLoadFromScratch) {
  std::vector<std::pair<uint8_t, uint8_t>> addressesAndDataToLoad;

  // add 7 arbitrary addresses with 7 arbitrary datas
  for (size_t i = 0; i < 2; ++i) {
    addressesAndDataToLoad.push_back(std::pair<uint8_t, uint8_t>{i + 1, (7 - i + 1) + 3});
  }
  builder.addCommandDistributionAppData(addressesAndDataToLoad);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[0], uint8_t(0x02)); // amount of addresses
  EXPECT_EQ(appData[1], uint8_t(0x01)); // address1
  EXPECT_EQ(appData[2], uint8_t(0x0B)); // data1
  EXPECT_EQ(appData[3], uint8_t(0x02)); // address2
  EXPECT_EQ(appData[4], uint8_t(0x0A)); // data2
}

TEST_F(PacketBuilderTest, CanMakeCommandDistributionLoadTwice) {
  std::vector<std::pair<uint8_t, uint8_t>> addressesAndDataToLoad;

  // add 7 arbitrary addresses with 7 arbitrary datas
  for (size_t i = 0; i < 2; ++i) {
    addressesAndDataToLoad.push_back(std::pair<uint8_t, uint8_t>{i + 1, (7 - i + 1) + 3});
  }
  builder.addCommandDistributionAppData(addressesAndDataToLoad);
  builder.addCommandDistributionAppData(addressesAndDataToLoad);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[5], uint8_t(0x02)); // amount of addresses
  EXPECT_EQ(appData[6], uint8_t(0x01)); // address1
  EXPECT_EQ(appData[7], uint8_t(0x0B)); // data1
  EXPECT_EQ(appData[8], uint8_t(0x02)); // address2
  EXPECT_EQ(appData[9], uint8_t(0x0A)); // data2
}

// If only one address then a length byte should not be pushed because there
// is no need for it.
TEST_F(PacketBuilderTest, CanMakeCommandDistributionLoadOneAdressDoesNotIncludeLengthByte) {
  std::vector<std::pair<uint8_t, uint8_t>> addressesAndDataToLoad;

  // add 7 arbitrary addresses with 7 arbitrary datas
  for (size_t i = 0; i < 1; ++i) {
    addressesAndDataToLoad.push_back(std::pair<uint8_t, uint8_t>{i + 1, (7 - i + 1) + 3});
  }
  builder.addCommandDistributionAppData(addressesAndDataToLoad);
  auto& packet = builder.getPacket();
  const auto appData = packet.getAppData();
  EXPECT_EQ(appData[0], uint8_t(0x01)); // address1
  EXPECT_EQ(appData[1], uint8_t(0x0B)); // data1
  EXPECT_EQ(appData[2], uint8_t(0x00)); // should be 0
}
