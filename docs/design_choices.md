# Design choices

Justification of the arquitecture of the system.

## Construction of the packet

A buffer and a size was passed as argument to the packet constructor initially. But those are two responsibilities: buffer handling to get the information and then passing that information to the object, I don't want the packet to be related to the buffer.

## Only one packet class for all packets

Since the packet may not have a data field header, two different classes for <code>Packet</code> were considered, with and without. But since the application data field has a fixed size anyways then it would not make much difference to have or not have some extra small fields.

## About mega packets

Sometimes I would want to separate app. data content into multiple packets. But how to implement it?

I could have the Packet or PacketExtended class do it; receive a bigger appData value as constructor and then somehow make it distribute the data into various instances. But it would probably be too much responsability for a Packet class that should only hold the data and it's setters/getters.

An alternative is to have the builder or another class handle the packet generation for the different parts of the app. data. Since it is already in charge of configuring a single new packet, it makes sense that it provides a way to "configure" a packet and split it into different packets if the configuration is too big.

At last, there is the builder director class, which knows the steps on how to build specific packets for specific services. It would know that to create two packets that belong to the same sequence you would have to configure them both the same, but distributing the same app. data between the two. In other words, **it tells the builder to build to specific packets that are related**.

I go with that last alternative.
