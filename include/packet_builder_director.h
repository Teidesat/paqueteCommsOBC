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

  /**
   * @brief A megapacket is a packet whose app data is bigger than Packet::APP_DATA_SIZE
   * @param appData 
   * @return * std::vector<Packet> 
   */
  std::vector<Packet> makeMegaPacket(const std::vector<std::byte>& appData);

  // make verification packet acceptance success
  // make verification packet acceptance failure
  // etc
};

#endif
