// uncomment next line to activate debug messages on serial port
// #define DEBUG

#ifdef DEBUG

#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)

#else

#define DPRINT(...)
#define DPRINTLN(...)

#endif
