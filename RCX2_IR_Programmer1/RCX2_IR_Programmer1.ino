#include <SoftwareSerial.h>
/**
 * Hardware layout for the RCX Programmer. Version 1.1.0
 *
 * Pin usage IR LED and TSOP for 950nm and 38kHz carrier:
 *
 *  IR-LED:SFH-409                 TSOP 31236
 *  cathode  anode
 *     -       +          Data           VCC   GND
 *     |       |            |             |     |
 *     |       |            |             |     |
 *     |       |            |             |     |
 *     |       |            |             |     |
 *             |            |             |     |
 *     9      10           11     12     13    GND
 *
 * ARDUINO
 *
 * Precondition
 * Arduino drivers are installed.
 *
 *
 * Description
 *
 * Producing the carrier 38khz when sending
 * a data byte over IR is done busy waiting and not using interrupts.
 * All timing values in function send(data) are manualy optimized for
 * a 16MHz ATmega32u4. So this (not using interupts) is a more 
 * generic solution which should work on many Arduino flavours.
 *
 * A future version might use interrupts.
 * 
 */
#define RX_PIN    11
#define RX_ON_OFF 13

#define TX_PIN    10
#define TX_ON_OFF  9

SoftwareSerial mySerial(RX_PIN, 12); // RX, TX

/**
 * to calculate parity
 */
int CountSetBits (int x)
{
  int count = 0;
  for (int n=0; n<16; ++n) {
      if (x & (1<<n)) {
          ++count;
      }
  }
  return count;
}

#define STOPBIT   0x400
#define PARITYBIT 0x200

/**
 * send 11 bits like this:
 * 1 start bit 0
 * 8 data bits
 * 1 odd parity bit 1
 * 1 stop bit 1
 */ 
void send(int data) {
  data= (data<<1) | ((CountSetBits(data)&1)?0:PARITYBIT) | STOPBIT;
  noInterrupts();
  for(uint8_t b=11; b; b--) {
    if(data&1){
      for(uint8_t i=0; i<14; i++) {
        digitalWrite(TX_PIN, LOW);
        delayMicroseconds(7);
        digitalWrite(TX_PIN, LOW);
        delayMicroseconds(7);
      }
    } else {
      for(uint8_t i=0; i<14; i++) {
        digitalWrite(TX_PIN, HIGH);
        delayMicroseconds(7);
        digitalWrite(TX_PIN, LOW);
        delayMicroseconds(7);
      }
    }
    data>>=1;
  }
  interrupts();
}

void powerRX(int mode) {
  pinMode(RX_ON_OFF, OUTPUT);
  digitalWrite(RX_ON_OFF, mode);
}

void powerTX() {
  pinMode(TX_PIN, OUTPUT);
  pinMode(TX_ON_OFF, OUTPUT);
  digitalWrite(TX_ON_OFF, LOW);
}

void setup() {  
  Serial.begin(2400);
  while (!Serial){
    ; // wait for a Serial to connect to save TSOP power
  }

  powerRX(HIGH);
  powerTX();
  mySerial.begin(2400);
}

void loop() { // run over and over
  static int r,s;
  static long t0;
  static uint8_t state=0;
  
  if (mySerial.available()) {
    r=mySerial.read();
    if (state!=1 || r!=255) {
      Serial.write(r);
      state=2;
    } else {
      state=3;
    }
  }
  
  if (Serial.available()) {
    s=Serial.read();
    // fake a (full duplex) IR echo to make lejosfirmdl happy
    Serial.write(s);
    send(s);
    powerRX(HIGH);
    state=1;
    t0=millis();
  }
  
  if (millis()-t0>10000) {
    if (state==0) {
    } else {
      // shut down TSOP after 10secs Serial silence to save power
      powerRX(LOW);
      state=0;
    }
    t0=millis();
  }
}

