#include <gtest/gtest.h>

#include <array>

#include "../include/packet.h"
// TEST(Packet, PacketBufferReadWorks) {
//   /**
//    * Define a packet of verification service acceptance success
//    *    (subtype 1) that is as follows:
//    * 
//    * 
//    *  version dfh   appid length sequence control ccsds pus version ack type subtype  error
//    *    0      1    1, 2    
//    */
//   std::array<std::byte, 256> packetBuffer;
//   for (uint8_t i = 0; i < 256; ++i) {
//     packetBuffer[i] = std::byte{0};
//   }
//   packetBuffer[0] = std::byte{0b01};
//   packetBuffer[1] = std::byte{1};
//   packetBuffer[2] = std::byte{0b01111100};
//   packetBuffer[3] = std::byte{}
// }
