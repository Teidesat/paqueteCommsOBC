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
#include <cstdint>

class Packet {
public:
  inline static constexpr uint32_t APP_DATA_SIZE = 256; // inline for appdata array size

  inline static constexpr uint32_t MAIN_HEADER_SIZE = 6;
  inline static constexpr uint32_t DATA_HEADER_SIZE = 3;
  inline static constexpr uint32_t DATA_SIZE = 256;
  inline static constexpr uint32_t ERROR_CONTROL_SIZE = 2;
  inline static constexpr uint32_t PACKET_SIZE = MAIN_HEADER_SIZE + DATA_HEADER_SIZE + DATA_SIZE + ERROR_CONTROL_SIZE;

  enum class SequenceFlags : uint8_t {
    INITIAL = 0x01,
    IN_BETWEEN = 0x00,
    FINAL = 0x02,
    STAND_ALONE = 0x03
  };

  Packet();
  Packet(
    uint8_t versionNumber,
    bool dataFieldHeader,
    uint8_t appIdSource,
    uint8_t appIdDestination,
    SequenceFlags sequenceControlFlags,
    uint16_t sequenceControlCount,
    uint16_t length,
    bool ccsds,
    uint8_t pusVersion,
    bool ack,
    uint8_t serviceType,
    uint8_t serviceSubtype,
    std::array<uint8_t, APP_DATA_SIZE>& appData,
    std::array<uint8_t, 2> packetErrorControl
  );
  Packet(const Packet& other) = default;

  // We can add the label [[nodiscard]] in all these functions to make sure the return value is used
  [[nodiscard]] uint8_t getVersionNumber();
  // Same here, no need for const
  void setVersionNumber(uint8_t versionNumber);

  [[nodiscard]] bool getDataFieldHeader();
  void setDataFieldHeader(bool newValue);

  [[nodiscard]] uint8_t getAppIdSource();
  void setAppIdSource(uint8_t newAddress);

  [[nodiscard]] uint8_t getAppIdDestination();
  void setAppIdDestination(uint8_t newAddress);

  [[nodiscard]] SequenceFlags getSequenceControlFlags();
  void setSequenceControlFlags(SequenceFlags newFlags);

  [[nodiscard]] uint16_t getSequenceControlCount();
  void setSequenceControlCount(uint16_t newCount);

  [[nodiscard]] uint16_t getLength();
  void setLength(uint16_t amountOfBytes);

  [[nodiscard]] bool getCCSDS();
  void setCCSDS(bool ccsds);

  [[nodiscard]] uint8_t getPUSVersion();
  void setPUSVersion(uint8_t pusVersion);

  [[nodiscard]] bool getAck();
  void setAck(bool newValue);

  [[nodiscard]] uint8_t getServiceType();
  void setServiceType(uint8_t typeId);

  [[nodiscard]] uint8_t getServiceSubtype();
  void setServiceSubtype(uint8_t subtypeId);

  [[nodiscard]] std::array<uint8_t, 2> getPacketErrorControl();
  void setPacketErrorControl(std::array<uint8_t, 2> crc);

  // It sets the object pointed to by ptrAppData to the value of appData_ .
  // The receptor array must be of size APP_DATA_SIZE
  std::array<uint8_t, APP_DATA_SIZE> getAppData();
  void setAppData(std::array<uint8_t, APP_DATA_SIZE> ptrNewAppData);

  // To avoid the client making copies of app data to substitute old app data.
  // It modifies the index for appending.
  void pushData(uint8_t bytesToPush);
  void pushData(uint16_t bytesToPush);
  void pushData(std::array<uint8_t, 2> bytesToPush);
  void pushData(std::vector<uint8_t> bytesToPush);

private:
  // main header
  uint8_t versionNumber_;
  bool dataFieldHeader_;
  uint8_t appIdSource_;
  uint8_t appIdDestination_;
  SequenceFlags sequenceControlFlags_;
  uint16_t sequenceControlCount_;
  uint16_t length_; // amount of octets within app. data

  // data field header (optional)
  bool ccsds_;
  uint8_t pusVersion_;
  bool ack_;
  uint8_t serviceType_;
  uint8_t serviceSubtype_;

  // data
  std::array<uint8_t, APP_DATA_SIZE> appData_;
  uint8_t appDataIndex_; // for app. data appending

  // special field at the end of the packet
  std::array<uint8_t, 2> packetErrorControl_;
};

#endif
