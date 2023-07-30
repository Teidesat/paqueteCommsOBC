#include <gtest/gtest.h>

#include <array>
#include <iostream>

#include "../include/packet.h"
#include "../include/packet_buffer_io.h"

// I want to have a common setup for a few tests so I do test fixture
class PacketBufferIOWithDataFieldHeaderTest : public testing::Test {
protected:
  void SetUp() override {
    /**
     * Define a packet of verification service acceptance success
     *    (subtype 1) that is as follows:
     * 
     * (service type 13 is not verification service, but keep it for now)
     *  version type dfh   appid sequence_control length  ccsds pus_version ack type subtype  error
     *    0      0    1     1, 2    (0b11, 2)       9       0       1        0   13     1     794 
     * 
     * length = 9 - 1 = (ccsds, pus, ack, type, subtype)(3 octet) +
     *  (app. id)(2 octets according to ECSS-70-E-41A) +
     *  (sequence control)(2 octets) + (error control)(2 octets) + (-1)
     * 
     * Sequence control contains flags and count. flags = 0b11 means
     *    stand_alone. Then count = 1.
     * 
     * error control = 0 because I won't use a CRC8 algorithm during unit
     *  testing.
     */
    for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
      packetBuffer[i] = std::byte{0};
    }
    packetBuffer[0] = std::byte{0b00001000};	// XXX X X 0 XX... | std::byte = version, type, dfh, 0, source...
    packetBuffer[1] = std::byte{0b00100010};	// ...XXX XXXXX    | std::byte = ...source, destination
    packetBuffer[2] = std::byte{0b11000000};	// XX XXXXXX...    | std::byte = seq_ctrl_flags, seq_ctrl_count...
    packetBuffer[3] = std::byte{0b00000010};	// ...XXXXXXXX     | std::byte = ...seq_ctrl_count
    packetBuffer[4] = std::byte{0b00000000};	// XXXXXXXX...     | std::byte = length...
    packetBuffer[5] = std::byte{0b00001001};	// ...XXXXXXXX     | std::byte = ...length
    packetBuffer[6] = std::byte{0b00010000};	// X XXX XXXX      | std::byte = ccsds, pus, ack
    packetBuffer[7] = std::byte{0b00001101};	// XXXXXXXX        | std::byte = type
    packetBuffer[8] = std::byte{0b00000001};	// XXXXXXXX        | std::byte = subtype

    // app. id copy of the telecommand this packet is confirming as
    // acceptance success since this test is not precedent to a telecommand,
    // I choose arbitrarily that the telecommand was from ground (app id 2) to an
    // imaginary cubesat module of app id 1.
    packetBuffer[9] = std::byte{0b00000000};	//
    packetBuffer[10] = std::byte{0b01000001};	// copy of app. id. of telecommand

    // sequence control copy of the telecommand this packet is confirming.
    // I set it arbitrarily it's standalone with a value of 1 for the telecommand
    // being the first ever sent by this service, with this verification
    // being the second ever.
    packetBuffer[11] = std::byte{0b11000000}; //
    packetBuffer[12] = std::byte{0b00000010}; // copy of count of telecommand
    // sequence control
    packetBuffer[13] = std::byte{0b00011010};
    packetBuffer[14] = std::byte{0b00000011};
  }

  // ****** members of the class ******
  std::byte packetBuffer[Packet::PACKET_SIZE];
};

//
// Because first argument is the fixture class, the protected members are
// available on the test (packetBuffer):
TEST_F(PacketBufferIOWithDataFieldHeaderTest, ReadsProperly) {
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), true);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getLength(), 9);
  EXPECT_EQ(packet.getCCSDS(), false);
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), false);
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], 0b00000011);
  EXPECT_EQ(packet.getPacketErrorControl()[1], 0b00011010);
}

 // other CCSDS, ACK variations:

TEST_F(PacketBufferIOWithDataFieldHeaderTest, ReadsProperlyCCSDSTRUEACKFALSE) {
  packetBuffer[6] = std::byte{0b10010000};
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), true);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getLength(), 9);
  EXPECT_EQ(packet.getCCSDS(), true); // changed
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), false);
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], 0b00000011);
  EXPECT_EQ(packet.getPacketErrorControl()[1], 0b00011010);
}

TEST_F(PacketBufferIOWithDataFieldHeaderTest, ReadsProperlyCCSDSTRUEACKTRUE) {
  packetBuffer[6] = std::byte{0b10010001};
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), true);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getLength(), 9);
  EXPECT_EQ(packet.getCCSDS(), true); // changed
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), true); // changed
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], 0b00000011);
  EXPECT_EQ(packet.getPacketErrorControl()[1], 0b00011010);
}

TEST_F(PacketBufferIOWithDataFieldHeaderTest, ReadsProperlyCCSDSFALSEACKTRUE) {
  packetBuffer[6] = std::byte{0b00010001};
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), true);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getLength(), 9);
  EXPECT_EQ(packet.getCCSDS(), false);
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), true); // changed
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], 0b00000011);
  EXPECT_EQ(packet.getPacketErrorControl()[1], 0b00011010);
}

// I want to test the write correctly with different ack and ccsds parameters
// too, so I write a parametrized version of the test:

// Because I want to test for ack = true|false and ccsds = true|false,
// I can then use a TestWithParam. I just need to invoke GetParam() to get the
// std::byte that will be at packetBuffer[6], which is the word that contains
// both ccsds and ack. Then I use INSTANTIATE_TEST_SUITE_P to create the
// different tests for the different ack, ccsds combinations.
class PacketBufferIOWithDataFieldHeaderParametrizedTest : public testing::TestWithParam<std::byte> {
protected:
  void SetUp() override {
    /**
     * Define a packet of verification service acceptance success
     *    (subtype 1) that is as follows:
     *
     * (service type 13 is not verification service, but keep it for now)
     *  version type dfh   appid sequence_control length  ccsds pus_version ack type subtype  error
     *    0      0    1     1, 2    (0b11, 2)       9       0       1        0   13     1     794 
     * 
     * length = 9 - 1 = (ccsds, pus, ack, type, subtype)(3 octet) +
     *  (app. id)(2 octets according to ECSS-70-E-41A) +
     *  (sequence control)(2 octets) + (error control)(2 octets) + (-1)
     * 
     * Sequence control contains flags and count. flags = 0b11 means
     *    stand_alone. Then count = 1.
     * 
     * error control = 0 because I won't use a CRC8 algorithm during unit
     *  testing.
     */
    for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
      packetBuffer[i] = std::byte{0};
    }
    packetBuffer[0] = std::byte{0b00001000};	// XXX X X 0 XX... | std::byte = version, type, dfh, 0, source...
    packetBuffer[1] = std::byte{0b00100010};	// ...XXX XXXXX    | std::byte = ...source, destination
    packetBuffer[2] = std::byte{0b11000000};	// XX XXXXXX...    | std::byte = seq_ctrl_flags, seq_ctrl_count...
    packetBuffer[3] = std::byte{0b00000010};	// ...XXXXXXXX     | std::byte = ...seq_ctrl_count
    packetBuffer[4] = std::byte{0b00000000};	// XXXXXXXX...     | std::byte = length...
    packetBuffer[5] = std::byte{0b00001001};	// ...XXXXXXXX     | std::byte = ...length
    packetBuffer[6] = GetParam();	// X XXX XXXX      | std::byte = ccsds, pus, ack
    packetBuffer[7] = std::byte{0b00001101};	// XXXXXXXX        | std::byte = type
    packetBuffer[8] = std::byte{0b00000001};	// XXXXXXXX        | std::byte = subtype

    // app. id copy of the telecommand this packet is confirming as
    // acceptance success since this test is not precedent to a telecommand,
    // I choose arbitrarily that the telecommand was from ground (app id 2) to an
    // imaginary cubesat module of app id 1.
    packetBuffer[9] = std::byte{0b00000000};	//
    packetBuffer[10] = std::byte{0b01000001};	// copy of app. id. of telecommand

    // sequence control copy of the telecommand this packet is confirming.
    // I set it arbitrarily it's standalone with a value of 1 for the telecommand
    // being the first ever sent by this service, with this verification
    // being the second ever.
    packetBuffer[11] = std::byte{0b11000000}; //
    packetBuffer[12] = std::byte{0b00000010}; // copy of count of telecommand
    // sequence control
    packetBuffer[13] = std::byte{0b00011010};
    packetBuffer[14] = std::byte{0b00000011};
  }

  // ****** members of the class ******
  std::byte packetBuffer[Packet::PACKET_SIZE];
};

TEST_P(PacketBufferIOWithDataFieldHeaderParametrizedTest, WritesCorrectly) {
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);

  std::byte packetBuffer2[Packet::PACKET_SIZE];
  for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
    packetBuffer2[i] = std::byte{0};
  }
  io.writePacket(packetBuffer2, packet);
  
  for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
    EXPECT_EQ(packetBuffer2[i], packetBuffer[i]);
  };
}

// WriteSuite is an arbitrary name, doesn't matter, the suit is not important.
INSTANTIATE_TEST_SUITE_P(WriteSuite, PacketBufferIOWithDataFieldHeaderParametrizedTest,
    testing::Values(
      0b00010000, // ccsds = FALSE, ack = FALSE
      0b00010001, // ccsds = FALSE, ack = TRUE
      0b10010000, // ccsds = TRUE, ack = FALSE
      0b00010001  // ccsds = TRUE, ack = TRUE
    ));

// *** Now testing without data field header ***

class PacketBufferIOWithoutDataFieldHeaderTest : public testing::Test {
protected:
  void SetUp() override {
    /**
     * Define a packet of verification service acceptance success
     *    (subtype 1) that is as follows:
     * 
     *  version type dfh   appid sequence_control length  error
     *    0      0    0     1, 2    (0b11, 2)        9     794 
     * 
     * length = 9 - 1 = (ccsds, pus, ack, type, subtype)(3 octet) +
     *  (app. id)(2 octets according to ECSS-70-E-41A) +
     *  (sequence control)(2 octets) + (error control)(2 octets) + (-1)
     * 
     * Sequence control contains flags and count. flags = 0b11 means
     *    stand_alone. Then count = 1.
     * 
     * error control = 0 because I won't use a CRC8 algorithm during unit
     *  testing.
     */
    for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
      packetBuffer[i] = std::byte{0};
    }
    packetBuffer[0] = std::byte{0b00001000};	// XXX X X 0 XX... | std::byte = version, type, dfh, 0, source...
    packetBuffer[1] = std::byte{0b00100010};	// ...XXX XXXXX    | std::byte = ...source, destination
    packetBuffer[2] = std::byte{0b11000000};	// XX XXXXXX...    | std::byte = seq_ctrl_flags, seq_ctrl_count...
    packetBuffer[3] = std::byte{0b00000010};	// ...XXXXXXXX     | std::byte = ...seq_ctrl_count
    packetBuffer[4] = std::byte{0b00000000};	// XXXXXXXX...     | std::byte = length...
    packetBuffer[5] = std::byte{0b00001001};	// ...XXXXXXXX     | std::byte = ...length

    // app. id copy of the telecommand this packet is confirming as
    // acceptance success since this test is not precedent to a telecommand,
    // I will choose arbitrarily the telecommand was from ground (app id 2) to an
    // imaginary cubesat module of app id 1.
    packetBuffer[9] = std::byte{0b00000000};	//
    packetBuffer[10] = std::byte{0b01000001};	// copy of app. id. of telecommand

    // sequence control copy of the telecommand this packet is confirming.
    // I choose arbitrarily it's standalone with a value of 1 for the telecommand
    // being the first ever sent by this service, with this verification
    // being the second ever.
    packetBuffer[11] = std::byte{0b11000000}; //
    packetBuffer[12] = std::byte{0b00000010}; // copy of count of telecommand
    // sequence control
    packetBuffer[13] = std::byte{0b00011010};
    packetBuffer[14] = std::byte{0b00000011};
  }

  // ****** members of the class ******
  std::byte packetBuffer[Packet::PACKET_SIZE];
};

TEST_F(PacketBufferIOWithoutDataFieldHeaderTest, ReadsCorrectly) {
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), true);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getLength(), 9);

  // default values for data field header fields
  EXPECT_EQ(packet.getCCSDS(), false);
  EXPECT_EQ(packet.getPUSVersion(), 0);
  EXPECT_EQ(packet.getAck(), false);
  EXPECT_EQ(packet.getServiceType(), 0);
  EXPECT_EQ(packet.getServiceSubtype(), 0);

  EXPECT_EQ(packet.getPacketErrorControl()[0], 0b00000011);
  EXPECT_EQ(packet.getPacketErrorControl()[1], 0b00011010);
}

TEST_F(PacketBufferIOWithoutDataFieldHeaderTest, WritesCorrectly) {
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer);

  std::byte packetBuffer2[Packet::PACKET_SIZE];
  for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
    packetBuffer2[i] = std::byte{0};
  }
  io.writePacket(packetBuffer2, packet);
  
  for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
    EXPECT_EQ(packetBuffer2[i], packetBuffer[i]);
  };
}
