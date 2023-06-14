/**
 * @file packet.h
 * @author Marcos Barrios
 * @brief High level packet representation; does not care as much about size.
 * 
 * Based on ECSS-E-70-41A packet structure but with changes.
 *
 * The type field is not added because it will always be 0 because only one packet
 *    is handled, so there is no point in having a member for it.
 * 
 * Source comes first before destination (from left to right looking at the
 *    packet figure) on the app. id. field.
 * 
 * Length = Packet size (bytes) - 6 (packet header) - 1
 *  Substract 1 because it can be a value between 0 to (2^16 - 1) and not
 *  between 1 and 2^16.
 * 
 */

#ifndef Packet_H
#define Packet_H

#include <array>
#include <vector>

// Either use std::byte almost everywhere or uint8_t almost everywhere, but not both at like a 50/50 chance

class Packet {
public:
  // There is no need for comments indicating these are constants or inline
  // Also, datasize can be uint32_t instead of uint16_t since we are working with 32bit systems, it will cost the same
  // or even be faster while being more flexible. (uint16_t -> uint32_t)
  // *** constants ***
  inline static constexpr uint16_t APP_DATA_SIZE = 256; // inline for appdata array size

  // Make the comments in "code" instead of adding a comment, like this (same here, make them uint16_t -> uint32_t):
  inline static constexpr uint16_t MAIN_HEADER_SIZE = 6;
  inline static constexpr uint16_t DATA_HEADER_SIZE = 3;
  inline static constexpr uint16_t DATA_SIZE = 256;
  inline static constexpr uint16_t ERROR_CONTROL_SIZE = 2;
  inline static constexpr uint16_t PACKET_SIZE = MAIN_HEADER_SIZE + DATA_HEADER_SIZE + DATA_SIZE + ERROR_CONTROL_SIZE;

  // Here it's okay the uint8_t but often it's written like "IN_BETWEEN = 0x00" (hexadecimal) instead of "IN_BETWEEN = 0b00" (binary)
  // Unless each flag is just a bit, then it's okay to use binary.
  enum class SequenceFlags : uint8_t {
    INITIAL = 0b01,
    INBETWEEN = 0b00,
    FINAL = 0b10,
    STAND_ALONE = 0b11
  };

  // There is no need for this class, compiler will just pick the correct number of bytes. Just use "true" or "false" to
  // initialize e.g. dataFieldHeader_(false),
  // Because bool can be 32 bits but i need it to always be 1 byte.
  enum class Bool8Enum : uint8_t {
    TRUE = 1,
    FALSE = 0
  };

  Packet();
  Packet(
    // Unless some special use case, there is really no need for these values to be marked const, since they are not references
    const uint8_t versionNumber,
    // use bool here -> bool dataFieldHeader,
    const Bool8Enum dataFieldHeader,
    const uint8_t appIdSource,
    const uint8_t appIdDestination,
    const SequenceFlags sequenceControlFlags,
    const uint16_t sequenceControlCount,
    const uint16_t length,
    const Bool8Enum ccsds,
    const uint8_t pusVersion,
    const Bool8Enum ack,
    const uint8_t serviceType,
    const uint8_t serviceSubtype,
    const std::array<std::byte, APP_DATA_SIZE>& appData,
    const std::array<std::byte, 2> packetErrorControl // This shouldnt be const as well
  );
  Packet(const Packet& other);

  // We can add the label [[nodiscard]] in all these functions to make sure the return value is used
  [[nodiscard]] uint8_t getVersionNumber() const;
  // Same here, no need for const
  void setVersionNumber(const uint8_t versionNumber);

  Bool8Enum getDataFieldHeader() const;
  void setDataFieldHeader(const Bool8Enum newValue);

  uint8_t getAppIdSource() const;
  void setAppIdSource(const uint8_t newAddress);

  uint8_t getAppIdDestination() const;
  void setAppIdDestination(const uint8_t newAddress);

  SequenceFlags getSequenceControlFlags() const;
  void setSequenceControlFlags(const SequenceFlags newFlags);

  uint16_t getSequenceControlCount() const;
  void setSequenceControlCount(const uint16_t newCount);

  uint16_t getLength() const;
  void setLength(const uint16_t amountOfBytes);

  Bool8Enum getCCSDS() const;
  void setCCSDS(const Bool8Enum ccsds);

  uint8_t getPUSVersion() const;
  void setPUSVersion(const uint8_t pusVersion);

  Bool8Enum getAck() const;
  void setAck(const Bool8Enum newValue);

  uint8_t getServiceType() const;
  void setServiceType(const uint8_t typeId);

  uint8_t getServiceSubtype() const;
  void setServiceSubtype(const uint8_t subtypeId);

  std::array<std::byte, 2> getPacketErrorControl() const;
  void setPacketErrorControl(const std::array<std::byte, 2> crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<std::byte, APP_DATA_SIZE> getAppData();
  void setAppData(const std::array<std::byte, APP_DATA_SIZE>& ptrNewAppData);

  // To avoid the client making copies of app data to substitute old app data.
  // It modifies the index for appending.
  void pushData(const std::byte bytesToPush);
  void pushData(const uint16_t bytesToPush);
  void pushData(const std::array<std::byte, 2> bytesToPush);
  void pushData(const std::vector<std::byte>& bytesToPush);

private:
  // main header
  uint8_t versionNumber_;
  Bool8Enum dataFieldHeader_;
  uint8_t appIdSource_;
  uint8_t appIdDestination_;
  SequenceFlags sequenceControlFlags_;
  uint16_t sequenceControlCount_;
  uint16_t length_; // amount of octets within app. data

  // data field header (optional)
  Bool8Enum ccsds_;
  uint8_t pusVersion_;
  Bool8Enum ack_;
  uint8_t serviceType_;
  uint8_t serviceSubtype_;

  // data
  std::array<std::byte, APP_DATA_SIZE> appData_;
  uint8_t appDataIndex_; // for app. data appending

  // special field at the end of the packet
  std::array<std::byte, 2> packetErrorControl_;
};

#endif
