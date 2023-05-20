#include <gtest/gtest.h>

#include <array>

#include "../include/packet.h"
#include "../include/packet_buffer_io.h"

TEST(PacketBufferIO, PacketBufferReadWorks) {
  /**
   * Define a packet of verification service acceptance success
   *    (subtype 1) that is as follows:
   * 
   *  version type dfh   appid length sequence_control ccsds pus_version ack type subtype  error
   *    0      0    1     1, 2    8      (0b11, 2)        0       1        0   13     1      0
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
  std::byte packetBuffer[Packet::PACKET_SIZE];
  for (size_t i = 0; i < Packet::PACKET_SIZE; ++i) {
    packetBuffer[i] = std::byte{0};
  }
  packetBuffer[0] = std::byte{0b00001000};	// XXX X X 0 XX... | std::byte = version, type, dfh, 0, source...
  packetBuffer[1] = std::byte{0b00100010};	// ...XXX XXXXX    | std::byte = ...source, destination
  packetBuffer[2] = std::byte{0b00000000};	// XXXXXXXX...     | std::byte = length...
  packetBuffer[3] = std::byte{0b00001000};	// ...XXXXXXXX     | std::byte = ...length
  packetBuffer[4] = std::byte{0b11000000};	// XX XXXXXX...    | std::byte = seq_ctrl_flags, seq_ctrl_count...
  packetBuffer[5] = std::byte{0b00000010};	// ...XXXXXXXX     | std::byte = ...seq_ctrl_count
  packetBuffer[6] = std::byte{0b00010000};	// X XXX XXXX      | std::byte = ccsds, pus, ack
  packetBuffer[7] = std::byte{0b00001101};	// XXXXXXXX        | std::byte = type
  packetBuffer[8] = std::byte{0b00000001};	// .XXXXXXX...     | std::byte = subtype

  // app. id copy of the telecommand this packet is confirming as
  // acceptance success since this test is not precedent to a telecommand,
  // I will suppose the telecommand was from ground (app id 2) to an
  // imaginary cubesat module of app id 1.
  packetBuffer[9] = std::byte{0b00000000};	//
  packetBuffer[10] = std::byte{0b01000001};	// copy of app. id. of telecommand

  // sequence control copy of the telecommand this packet is confirming.
  // I suppose it's standalone with a value of 1 for the telecommand
  // being the first ever sent by this service, with this verification
  // being the second ever.
  packetBuffer[11] = std::byte{0b11000000}; //
  packetBuffer[12] = std::byte{0b00000010}; // copy of count of telecommand

  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer, Packet::PACKET_SIZE);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), Packet::Bool8Enum::TRUE);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getLength(), 8);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 2);
  EXPECT_EQ(packet.getCCSDS(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], std::byte{0});
  EXPECT_EQ(packet.getPacketErrorControl()[1], std::byte{0});
}
