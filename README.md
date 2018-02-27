This is a Arduino library for decoding the signals from the Märklin-Motorola-protocol.

Märklin Motorola 2 Extensions are currently not supported. DCC may not filtered correct at the moment.

Signal polarity isn't relevant. Library is working with timings only.





Circuit for getting signal from rail, with optocoupler and signal-led:

                                                              6N137
                                                  +--------------------------+
                                                  | .                        |
                                            NC    |                          | VCC
                                          +-------+                          +-------/-------------/----------+   VCC
                                                 1|                          |8      |             |
                                                  |                          |       |             |
                                                  |                          |       |           +---+
        +--------+                          ANODE |                          | ENABLE|           |   |
     +--+        +--------/----------------------------+           +-----------------+           |   |
        +--------+     XXXXXXX                   2| XXXXXXX        |         |7                  |   |
 rail                    X X   +--->              |  X   X  +--->  |         |                   |   |
                        X   X  +--->              |   X X   +--->  |         |                   +---+
                       XXXXXXX             CATHODE| XXXXXXX        | +---+   | OUTPUT              |
     +--------------------/----------------------------+           +-+   +-------------------------/----------+   INT Arduino
                                                 3|                  +---+   |6
                                                  |                          |
                                                  |                          |
                                            NC    |                          | GND
                                          +-------+                          +--------------------------------+   GND
                                                 4|                          |5
                                                  |                          |
                                                  +--------------------------+


													  
													  
Protocol:
http://home.mnet-online.de/modelleisenbahn-digital/Dig-tutorial-start.html

http://www.drkoenig.de/digital/motorola.htm

http://www.skrauss.de/modellbahn/Schienenformat.pdf (MFX)
