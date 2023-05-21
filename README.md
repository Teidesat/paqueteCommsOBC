# cubesat TEIDESAT-I's packet library

Marcos Barrios Lorenzo

## Description

Packet representation and buffer operations based on the ECSS-70-E-41A standard. See [the packet design document](docs/packet_design_teidesat_I.pdf) pdf.

## Little-endian is required

TEIDESAT-I'S OBC is based on an ARM cortex m7, and ARM typically provides a pin to select little-endian or big-endian. However, the library assumes it is little-endian, so it is important to make sure that the microcontroller is set to little-endian. This is due to the library sometimes converting two <code>uint8_t</code> into a <code>uint16_t</code> by assuming that [0] is the least significant part, while [1] is more significant, which is incorrect if the system is little endian.

There are plans to make it endian independent.
