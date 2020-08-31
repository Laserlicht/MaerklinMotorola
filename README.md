This is a Arduino library for decoding the signals from the Märklin-Motorola-protocol.

DCC may not filtered correct at the moment.

Signal polarity from rail isn't relevant. Library is working with timings only.

Pin on Arduino must be Pinchange-interrupt-capable.

**Example-Circuit to get required signal to Arduino:**
<img width="100%" src="https://raw.githubusercontent.com/Laserlicht/MaerklinMotorola/master/circuit.svg">
													  

**Additional informations about the protocol**

Protocol:

- http://home.mnet-online.de/modelleisenbahn-digital/Dig-tutorial-start.html

- http://www.drkoenig.de/digital/motorola.htm

- http://www.skrauss.de/modellbahn/Schienenformat.pdf (MFX)
