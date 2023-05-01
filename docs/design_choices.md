

# Construction of the packet

A buffer and a size was passed as argument to the packet constructor initially. But those are two responsibilities: buffer handling to get the information and then passing that information to the object, I don't want the packet to be related to the buffer.


