# cubesat TEIDESAT-I's packet library

## Description

Packet representation and buffer operations based on the ECSS-70-E-41A standard. Check the [packet design document](docs/articles/PaqueteRF_TEIDESAT_I-2.pdf) for the packet's design history.

## Big-endian is required

TEIDESAT-I'S OBC is based on an ARM cortex m7, and ARM typically provides a pin to select little-endian or big-endian. However, the library assumes it is big-endian, so it is important to make sure that the microcontroller is set to big-endian. This is due to the library sometimes converting two <code>uint8_t</code> into a <code>uint16_t</code> when reading a packet from buffer by assuming that [0] is the most significant part, while [1] is less significant, which is incorrect if the system is little endian.

## References

 - Tesis de Upsat [https://nchronas.github.io/upsat_msc_thesis/](https://nchronas.github.io/upsat_msc_thesis/) 
 - ECSS-70-E-41A [https://ecss.nl/standard/ecss-e-70-41a-ground-systems-and-operations-telemetry-and-telecommand-packet-utilization/](https://ecss.nl/standard/ecss-e-70-41a-ground-systems-and-operations-telemetry-and-telecommand-packet-utilization/)
