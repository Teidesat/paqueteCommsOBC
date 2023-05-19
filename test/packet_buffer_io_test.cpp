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
   *    0      0    1     1, 2    8      (0b11, 1)        0       1        0   13     1      0
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
  packetBuffer[0] = std::byte{0b00110000}; // 001 0 1 000...    | version, type, dfh, 0, source...
  packetBuffer[1] = std::byte{0b01000100}; // ...01 00010 0...  | ...source, destination, length...
  packetBuffer[2] = std::byte{0b00000000}; // ...00000000...    | ...length...
  packetBuffer[3] = std::byte{0b00010001}; // ...0001000 1...   | ...length, seq. ctrl. flags...
  packetBuffer[4] = std::byte{0b10000000}; // ...1 0000000...   | ...seq. ctrl. flags, seq. ctrl. count...
  packetBuffer[5] = std::byte{0b00000000}; // ...00000000...    | ...seq. ctrl. count...
  packetBuffer[6] = std::byte{0b10001000}; // ...1 0 001 000... | ...seq. ctrl. count, ccsds, pus, ack...
  packetBuffer[7] = std::byte{0b00000000}; // ...0 0000110...   | ...ack, type...
  packetBuffer[8] = std::byte{0b10000000}; // ...1 0000000...   | ...type, subtype...
  packetBuffer[9] = std::byte{0b10000000}; // ...1 0000000...   | ...subtype app data(...)
  PacketBufferIO io;
  Packet packet = io.readPacket(packetBuffer, Packet::PACKET_SIZE);
  EXPECT_EQ(packet.getVersionNumber(), 0);
  EXPECT_EQ(packet.getDataFieldHeader(), Packet::Bool8Enum::TRUE);
  EXPECT_EQ(packet.getAppIdSource(), 1);
  EXPECT_EQ(packet.getAppIdDestination(), 2);
  EXPECT_EQ(packet.getLength(), 8);
  EXPECT_EQ(packet.getSequenceControlFlags(), Packet::SequenceFlags::STAND_ALONE);
  EXPECT_EQ(packet.getSequenceControlCount(), 1);
  EXPECT_EQ(packet.getCCSDS(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getPUSVersion(), 1);
  EXPECT_EQ(packet.getAck(), Packet::Bool8Enum::FALSE);
  EXPECT_EQ(packet.getServiceType(), 13);
  EXPECT_EQ(packet.getServiceSubtype(), 1);

  EXPECT_EQ(packet.getPacketErrorControl()[0], std::byte{0});
  EXPECT_EQ(packet.getPacketErrorControl()[1], std::byte{0});
}
