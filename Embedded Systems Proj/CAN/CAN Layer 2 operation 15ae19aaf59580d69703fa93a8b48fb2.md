# CAN Layer 2 operation

# Principle of Operations

## Arbitration:

- Since bus access occurs by any Nodes, without centralized management, it is possible for several participants to simultaneously start sending a message, a selection phase (CAN arbitration phase) decides which Node may transmit on the bus based on ID of the CAN frame.
- The ID of CAN frame:
    - Has 11 bits or 29 bits, which decides the priority of the message and the CAN node.
    - Bit 0 has higher priority and is the dominant bit, always overwrites a recessive bit on a CAN bus. Higher priority message has lower value on ID field.
    - Each CAN node has a unique identifier on the CAN bus.
    - The nodes can simultaneously transmit data, starting with SOF flag and ID bits.  Arbitration resolves conflict bit by bit. If one Nodes has bit `0`  in its ID, it drives the bus line to dominant and win arbitration. Other nodes cannot drive the bus to recessive and then has to listen.
    - The node that wins arbitration can continue with the message. Nondestructive means that the node winning arbitration just continues on with the message, without the message being destroyed or corrupted by another node.

![image.png](image%205.png)

Example: Source [1]

![image.png](image%206.png)

 *The participants 1, 2, and 3 start an arbitration attempt at the same time (1). Participant 2 loses the bus access right at time (2), while participant 1 loses the bus access right at time (3). Both participants therefore go into the receiving state; at the end of the CAN arbitration phase (4), only participant 3 has the bus access right and switches its message to the bus.*

- Each Node can know if the Bus is dominant or recessive and thus know if it has to go to listening mode by “sensing” the voltage on the bus line. A transmitting node constantly monitors each bit of its own transmission.
- The following illustrates the circuitry and connection between each node and the bus that allows the Nodes to monitor its own transmission.

![image.png](image%207.png)

Inverted logic at connection between nodes and CAN bus. Source [2]

![image.png](image%208.png)

## Error checking & Error Correction

- five methods of error checking: three at the message level and two at the bit level.
- Error checking at the message level includes:
    - the CRC checksum: CRC contains the checksum of the preceding application data for error detection. The calculated checksum is appended to the original data. This combined data (original data + checksum) is then transmitted or stored. Upon receiving the data, the same CRC algorithm is applied. The data (including the checksum) is divided by the same generator polynomial. If the remainder is zero, the data is considered intact. If not, it indicates that the data has been corrupted. ⇒ Error type is *bit error, ACK error.*
    - form check. This check looks for fields in the message which must always be recessive bits. If a dominant bit is detected, an error is generated. The bits checked are the SOF, EOF, ACK delimiter, and the CRC delimiter bits. ⇒ Form error.
- Error checking at the bit level:
    - Each bit transmitted is monitored by the transmitter of the message. If a data bit (not arbitration bit) is written onto the bus and its opposite is read, an error is generated. ⇒ Bit error type, ACK error.
    - Bit-stuffing rule where after five consecutive bits of the same logic level, if the next bit is not a complement, an error is generated ⇒ Error type is *stuff error*.
- Error state: a CAN node has the following possible error states:
    - Active: A node having this error can fix itself.
    - Passive: A node goes into an error passive state when it detects errors but has not exceeded the bus-off threshold. In this state, the node continues to monitor the bus but does not actively participate in  [communication](https://www.rfwireless-world.com/Terminology/CAN-bus-working-errors-frame-and-types.html#) until it recovers from the error condition
    - Bus Off: occurs when an node repeatedly fails to transmit messages correctly and exceeds a predefined error threshold. In response to a bus-off error, the malfunctioning node is temporarily disconnected from the bus to prevent further disruption.
- Error correction:
    - when a node found error, it will send error to notify all other nodes of the error on the bus.
    - the first node will resend the message until the message is successfully sent or the error node is change to `bus off` state.
- The following illustrates how a receiving node can acknowledge if there is no message in the preceding data:

![image.png](image%209.png)

# CAN Frames:

### Data Frame

- Standard CAN:

![image.png](image%2010.png)

- Extended CAN:

![image.png](image%2011.png)

- The data frame is the most common message type, and comprises the Arbitration Field, the Data Field, the CRC Field, and the Acknowledgment Field.
- The Arbitration field includes `SOF` flag, the `identifier`, `RTC` flag
- `ACK` flag: when transmitting, it is always recessive (Logic 1), and when receving, if there is no error, ACK will be dominant (Logic 0). If a receiving node detect an error, it will this bit recessive (logic 1). It discards the message and the sending node repeats the message after re-arbitration.
- `ACK Delimiter`  1 bit and is always recessive.

### Remote Frame

- used when a Node is requesting data from another frame. The other frame should return a date frame.
- `RTR`  flag identifies the frame as data frame or remote frame.
- `Identifier`  field represents the Node that is requested data. The Node has to reply with a data frame whose ID is the same as the Remote frame’s.

![image.png](image%2012.png)

### Error Frame

- The error frame is a special message that violates the formatting rules of a CAN message. It is transmitted when a node detects an error in a message, and causes all other nodes in the network to send an error frame as well. The original transmitter then automatically retransmits the message.
- An elaborate system of error counters in the CAN controller ensures that a node cannot tie up a bus by repeatedly transmitting
error frames. [2]

![image.png](image%2013.png)

### Overloaded Frame

- indicates that the bus is overloaded with messages. Overload frame errors can occur if the bus is overwhelmed with traffic.

# Reference:

[1] [What is CAN arbitration and how does this work?](https://www.hms-networks.com/industrial-iot-blog/blogpost/hms-blog/2024/06/18/what-is-can-arbitration-and-how-does-this-work) 

[2] [Introduction to the Controller Area Network (CAN) (Rev. B)](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf)