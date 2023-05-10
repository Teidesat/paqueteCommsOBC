# Design choices

Justification of the arquitecture of the system.

## Construction of the packet

A buffer and a size was passed as argument to the packet constructor initially. But those are two responsibilities: buffer handling to get the information and then passing that information to the object, I don't want the packet to be related to the buffer.

## Only one packet class for all packets

Since the packet may not have a data field header, two different classes for <code>Packet</code> were considered, with and without. But since the application data field has a fixed size anyways then it would not make much difference to have or not have some extra small fields.
