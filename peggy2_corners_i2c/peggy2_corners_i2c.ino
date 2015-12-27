/* Simple test of I2C communications
 *  Peggy2 acts as receive only client
 *  changes which corner is lit depending on signal from master
 *  on Raspberry Pi
*/


#include <Peggy2.h>
#include <Wire.h>

Peggy2 frame1;     // Make a first frame buffer object
Peggy2 frame2;
Peggy2 frame3;
Peggy2 frame4;

constant byte MYADDRESS = 42;         // set I2C address
volatile byte signal = 0;             // volatile since used in interrupt
volatile boolean signalFlag = false;
byte pattern = 0;
   
void setup()                    
{
  frame0.HardwareInit();        // Only needed once, even if you've got lots of frames.
  Wire.begin(MYADDRESS);        // join i2c bus
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  frame1.SetPoint(0,0);
  frame1.SetPoint(0,1);
  frame1.SetPoint(0,2);
  frame1.SetPoint(1,0);
  frame1.SetPoint(1,1);
  frame1.SetPoint(1,2);
  frame1.SetPoint(2,0);
  frame1.SetPoint(2,1);
  frame1.SetPoint(2,2);
  
  frame2.SetPoint(22,0);
  frame2.SetPoint(22,1);
  frame2.SetPoint(22,2);
  frame2.SetPoint(23,0);
  frame2.SetPoint(23,1);
  frame2.SetPoint(23,2);
  frame2.SetPoint(24,0);
  frame2.SetPoint(24,1);
  frame2.SetPoint(24,2);
  
  frame3.SetPoint(0,22);
  frame3.SetPoint(0,23);
  frame3.SetPoint(0,24);
  frame3.SetPoint(1,22);
  frame3.SetPoint(1,23);
  frame3.SetPoint(1,24);
  frame3.SetPoint(2,22);
  frame3.SetPoint(2,23);
  frame3.SetPoint(2,24);
  
  frame4.SetPoint(22,22);
  frame4.SetPoint(22,23);
  frame4.SetPoint(22,24);
  frame4.SetPoint(23,22);
  frame4.SetPoint(23,23);
  frame4.SetPoint(23,24);
  frame4.SetPoint(24,22);
  frame4.SetPoint(24,23);
  frame4.SetPoint(24,24);  
  
} // end setup  


void loop()
{ 
  if (signalFlag){
    noInterrupts();
    signalFlag = false;
    pattern = signal;
    interrupts();
  } // end if
  // signal will be between 0 and 15 (4 bits)
  // use pattern to determine which frame is shown
  // loop should be fast enough between signals to show pattern
  if (signal & 1){        // bit 1 is set (i.e. 0001b)
    frame1.RefreshAll(1);
  }
  if (signal & 2){        // bit 2 is set (i.e. 0010b)
    frame2.RefreshAll(1);
  }
  if (signal & 1){        // bit 3 is set (i.e. 0100b)
    frame3.RefreshAll(1);
  }
  if (signal & 1){        // bit 4 is set (i.e. 1000b)
    frame4.RefreshAll(1);
  }
} // end loop

void receiveEvent(int bytesReceived) {
  while (1 < Wire.available()) { // loop through all but the last in case more than 1 byte received
    byte b = Wire.read();        // do nothing with bytes
  }
  pattern = Wire.read();        // capture in volatile variable so it doesn't get messed with during interrupts
  signalFlag = true;            // set flag for loop()
} // end receiveEvent

