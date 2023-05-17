# cubesat TEIDESAT-I's packet library

Marcos Barrios Lorenzo

## Description

Packet representation and buffer operations based on the ECSS-70-E-41A standard. See [the packet design document](docs/packet_design_teidesat_I.pdf) pdf.

## Important note on endianess

TEIDESAT-I'S OBC is based on an ARM cortex m7, and ARM typically provides a pin to select little-endian or big-endian. However, the library assumes it is little-endian, so it is important to make sure that the microcontroller is set to little-endian. Operations like int to std::byte array will fail otherwise because the order of the bytes for the same int is different depending on the endianess.

