/**
 * @file packet_factory.h
 * @author Marcos Barrios
 * @brief Uses the packet builder to make certain packets. Wraps the correct
 *      Packet extended.
 * 
 */

#ifndef PACKET_BUILDER_DIRECTOR_H
#define PACKET_BUILDER_DIRECTOR_H

#include <vector>

#include "packet.h"

class PacketBuilderDirector {
public:
  PacketBuilderDirector();
  ~PacketBuilderDirector();

  // a megapacket is a packet whose app data is bigger than the maximum
  // for a single packet.
  std::vector<Packet> makeMegaPacket(std::vector<std::byte>& appData);

  // make verification packet acceptance success
  // make verification packet acceptance failure
  // etc
};

#endif
