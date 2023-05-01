#include "../include/packet_buffer.h"

PacketBuffer::PacketBuffer() {

}

PacketBuffer::~PacketBuffer() {}

Packet PacketBuffer::readPacket(std::byte* buffer, std::size_t size) {
  if (sizeof(buffer) )
  return Packet(buffer, size);
}

void PacketBuffer::writePacket(Packet packet) {

} 

