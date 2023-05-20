# Design choices

Recopilation of design choices for the library.

## Construction of the packet

A buffer and a size was passed as argument to the packet constructor initially. But those are two responsibilities: buffer handling to get the information and then passing that information to the object, I don't want the packet to be related to the buffer.

## To create a new class or to extend the current class

Sometimes I have very similar classes, like a Packet with or without data field header. That's when I have to decide If i want to create a new class for each variant or just have a single class with extra boolean flags.

If I create a new class, it is organized better, but the amount of classes starts to grow fast, which makes the system harder to understand at a glance. But if I don't, then the logic gets more conditional overtime. 

For example, I mentioned that the Packet class always contains the data field header, but I could two classes instead; one with and another without. ¿How often does a packet come with a data field header? If 90% of the time then it is probably better to have a single class, because the logic is easier and the amount of memory wasted is not much. If it was more like 50%, then it would probably make sense to have two different classes. And I believe 90% of time it has a data field header, so I stay at a single class.

I'll the same class extended if it makes the logic simpler and the wasted memory is not much, otherwise a new class is probably better.

## About mega packets

Sometimes it is required fit a big app. array data into multiple packets rather than just one. How do I implement it?

I could have the Packet or PacketExtended class do it; receive a bigger appData value as constructor and then somehow make it distribute the data into various instances. But it would probably be too much responsability for a Packet class that should only hold the data and it's setters/getters.

An alternative is to have the builder or another class handle the packet generation for the different parts of the big app. data. Since it is already in charge of configuring a single new packet, it would make sense for it to provide a way to "configure" various packets and with different parts of a common app. data. because it adds headers to a current packet, which can result in a packet being too big, which follows a new packet call.

Another way is by using the builder director class, which knows the steps on how to build specific packets for specific services. It would know that to create two packets that belong to the same transmission sequence you would have to configure them both the same, but distributing the same app. data between the two. In other words, **it tells the builder to build specific packets that are related**. I go with this alternative.

## Packet architecture

Initially I thought that having a low level packet included in a high level packet, and the later providing the operations that are most frequently used would be the best solution. But the problem it has is that the high level packet should have the high level data as a member so that it can be used without having to coarse the <code>std::byte</code> into <code>int</code> on each method call, which is what happens currently with the high level packet.

An alternative would be to have two independent classes, like now, but both having all the fields, one on low level (<code>std::byte</code>) and the other on high level (<code>uint8_t</code>), but two representations is not be better because there is no difference in size between a <code>std::byte</code> and <code>uint8_t</code> so it makes no sense to have one for transmissions and the other for normal use.

The main reason I decided to have low level and high level is to avoid semantic errors like doing arithmetic on something that is not a number. But I have come to notice that most fields are actually numbers, and if I wanted to have a character then I could just use <code>char</code>. It would be simpler and thus most likely to have less bugs. So I now change the packet representation to use <code>uint8_t</code> rather than <code>std::byte</code> when the field is actually a number.

Back to two representations; <code>std::byte</code> is the same as <code>uint8_t</code>, but it is not right to use one of those types for a field that is only 1 bit. Also, the packet design mentions the concept of padding when sizing is not done in multiples of octets. That's why I should have a std::byte representation BUT that represents more than one field, leaving the methods to handle the specific fields. This lower level class should be only used for transmissions, as it's size is smaller but there is overhead when calling it's methods. This approach would end up with a <code>Packet</code> for high level and <code>PacketRaw</code> for low level.

However, having a low level representation is redundant because it means having a class that has as member the fields with the exact size, which means using a single byte for more than one field. But why have a representation of packet with the split fields but no semantic? I would want both the split fields AND the semantic (numbers and maybe chars, not just std::bytes), otherwise I could just use an <code>std::array<std::byte></code> for my low level packet implementation. This is the approach I take; <code>Packet</code> for high level, <code>PacketBufferIO</code> for handling low level packets, which are <code>std::array<std::byte></code>, the best representation for memory efficient transmissions.

## About coarcing everything into std::byte to push into Packet's App. data

I decided to coarce everything to <code>std::byte</code> before pushing to app. data in <code>Packet</code> to keep the app data handling code as simple as possible. An alternative I thought of is using <code>std::variant</code>, but not knowing what's the type on compile time would bring some if else's that can be avoided by using type coercion.

## To implement Packet extended for handling packets of different services or not

I can define one class for handling the fields of the packets of each different service, or I could leave that part for the services to handle the application data field on their own. Taking into account that the app. data field will typically be associated with only one service, then I can easily implement the first. In other words: a packet will not be multipurpose, to keep things simple. For example, If I wanted to send information about housekeeping but also about the command verification service, then two packets would have to be sent. Otherwise, the code would have to check the app. data. in detail and somehow try to split the fields depending on what service they are from. So having multiple classes for representing packets of different services is viable.

## To implement a polymorph method for wrapping Packets on corresponding Packet Extended

I could define on <code>PacketBuilderDirector</code> (since packet wrapping is it's responsibility) a method that returns at runtime the proper wrapper for the received <code>Packet</code>, but I'd rather not use dynamic memory since could be slower than the stack. Instead I will rely on giving the services the tools to instance the corresponding wrapper by defining a method for each wrapper.

## Packet wrapping for subtypes

I decided to implement a new class fore ach subtype packet of a service because the structure is different and it is simpler than having one class with all the methods.

## Array representation order of the bytes

Depending on the order of array storage, I could have at index 0 the bytes starting from version, or at index 0 the bytes starting from error control field. Both are equally valid, so I decide to have index 0 start from version. 