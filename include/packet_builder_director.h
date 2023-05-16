/**
 * @file packet_factory.h
 * @author Marcos Barrios
 * @brief Uses the packet builder to make certain packets. Each packet is wrapped
 *      in the corresponding Packet extended variant depending on the service it
 *      belongs to.
 * 
 * It handles type conversion to std::byte. An exception is the
 * makeMegaPackete function. 
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
