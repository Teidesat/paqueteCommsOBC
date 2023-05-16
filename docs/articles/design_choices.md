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

## Packet architecture

Initially I thought that having a low level packet included in a high level packet, and the later providing the operations that are most frequently used would be the best solution. But the problem it has is that the high level packet should have the high level data as a member so that it can be used without having to coarse the <code>std::byte</code> into <code>int</code> on each method call, which is what happens currently with the high level packet.

An alternative would be to have two independent classes, like now, but both having all the felds, one on low level (<code>std::cyte</code>) and the other on high level (<code>uint8_t</code>), but two representations is not be better because there is no difference in size between a <code>std::byte</code> and <code>uint8_t</code> so it makes no sense to have one for transmissions and the other for normal use.

The main reason I decided to have low level and high level is to about semantic errors like doing arithmetic on something that is not a number. But I have come to notice that most fields are actually numbers, and if I wanted to have a string then I could just use <code>std::string</code>. It would be simpler and thus most likely to have less bugs. So I now change the packet representation to use <code>uint8_t</code> rather than <code>std::byte</code> and to have one class less.

## Use of size_t for size and index representation

I decided not so use size_t because it is not standarized and it's implementation depends on the different compilrs. It is true that it could avoid overflow errors but with more careful programming it should not be a problem.

## About coarcing everything into std::byte to push into Packet's App. data

I decided to coarce everything to <code>std::byte</code> before pushing to app. data to keep the app data handling code as simple as possible. An alternative I thought of is using <code>std::variant</code>, but not knowing what's the type on compile time would bring some if else's that can be avoided by using type coercion.

## To implement Packet extended for handling packets of different services or not

I can define one class for handling the fields of the packets of each different service, or I could leave that part for the services to handle the application data field on their own. Taking into account that the app. data field will typically be associated with only one service, then I can easily implement the first. In other words: a packet will not be multipurpose, to keep things simple. For example, If I wanted to send information about housekeeping but also about the command verification service, then two packets would have to be sent. Otherwise, the code would have to check the app. data. in detail and somehow try to split the fields depending on what service they are from. So having multiple classes for representing packets of different services is viable.


