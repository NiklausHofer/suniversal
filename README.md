
# *suniversal*

A USB adapter for *SUN Type 5* keyboards, based on *Arduino Pro Micro*. All keys working!


## Background
A couple of years back, I bought a *SUN Type 5* keyboard at the *VCFe* flea market, put it in the attic and forgot about it. A few days ago, it fell into my hands again while looking for something, and I thought it would be nice to make use of it. After cleaning it thoroughly, I considered the available options for connecting it to my Linux box. There are a few older projects, such as [here](http://ezhid.sourceforge.net/sunkbd.html), [here](http://snafu.priv.at/mystuff/sunkbd.html), and [here](http://kentie.net/article/sunkbd/), but they seemed too involved on the hardware side (require creating a PCB), compared to how cheap, small, and complete for example an *Arduino Pro Micro* board is. So, for the *Arduino* world, the best solution out there seemed to be Ben Rockwood's [SunType5_ArduinoAdapter](https://github.com/benr/SunType5_ArduinoAdapter), the only problem being that it does not make all keys available. Still good enough as a starting point.


## Hardware
I decided to put the *Arduino* into the keyboard case (had to open it anyway for cleaning). The original cable can be unplugged from the keyboard's PCB, so it's easy to revert the modification should I ever desire to do so. The only thing to fabricate was a very simple harness to connect the *Arduino* with the PCB:

| PCB pin | wire color* | function   | *Arduino* pin  |
|---------|-------------|------------|----------------|
|     1   |    black    |   GND      |      GND       |
|     2   |    red      |   +5V      |      Vcc       |
|     4   |    brown    |  serial TX (to KBD)   | D9  |
|     5   |    blue     |  serial RX (from KBD) | D10 |

*as found on a Type 5c keyboard, may differ depending on model & year

The most challenging part may be finding the right plug to connect to the PCB. I fabricated something out of a connector that had the right pitch.

![connector](connector.jpg)


## Software

Analyzing *SunType5_ArduinoAdapter*, I realized that the limitations were rooted in the use of the *Arduino* Keyboard library for the conversion to USB. Not that the library itself is in any way limited, it's just that is designed for a different use case - turning `Print`ed characters into key strokes. But what we need here is actually much simpler - just a plain scan code converter. So I merged *SunType5_ArduinoAdapter* and the Keyboard library and started refactoring the code. The result is this project.

#### Debug Mode
To see debug messages on the *Arduino*'s serial port, uncomment the `// #define DEBUG` line in `suniversal.h`.


## Gotchas

- The translation table is based on the international scan set. I have the German version of the keyboard, so I currently only test with that. You may have to adapt the table if your keyboard is a US version.

- Special keys such as *Stop*, *Again*, or *Undo*, are mapped to their closest USB code. Whether that has the desired result depends on your OS. You may have to configure it accordingly. On my *Ubuntu* box for example, I configured keyboard shortcuts for the audio and power keys in the keyboard settings.


## Further Development

Here a few ideas I may implement in the future:

- add second table with US scan set
- support key macros
- actual compose mode
