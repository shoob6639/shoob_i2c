//Shoob I2C V6 11/22/25
//Designed on ATMEGA328P

//Define LO/HI commands
//HI sets DDRB bus to READ for SDA/SCL pin (PB3/PB4)
//LO sets DDRB bus to WRITE for SDA/SCL pins
#define SDA_HI DDRB = (DDRB & ~(1 << 3)) | 0x00 // Release SDA HI
#define SDA_LO DDRB = (DDRB & ~(1 << 3)) | 0x08 // Pull SDA LO

#define SCL_HI DDRB = (DDRB & ~(1 << 4)) | 0x00 // Release SDA HI
#define SCL_LO DDRB = (DDRB & ~(1 << 4)) | 0x10 // Pull SCL LO

//Set ACK byte to 0x00
byte ACK = 0x00;

void setup() {
}

//Define delay function (Based on clock cycles)
void tick (){
    //Define CPU cycles in a single tick()
    //Breaks at i < 35 - 2.5KHz on Arduino Nano
    int i = 35;
    do
    {
      __asm( "nop" );
    }
    while ( --i );
  }

//Function to read SDA (PB3)
byte SDA_READ(){
  // Release SCL/SDA
  SDA_HI;
  SCL_HI;
   tick();
  // Return PB3 state
  return((PINB & 0x08) >> 3);
  }

//Function to read SCL (PB4)
byte SCL_READ(){
  // Release SCL
  SCL_HI;
   tick();
  // Return PB4 state
  return((PINB & 0x10) >> 4);
  }  

void I2C_START(){
  // Release SDA/SCL 
  SDA_HI;
  SCL_HI;
  // Pull SDA LO
  SDA_LO;
   tick();
  // Pull SCL LO
  SCL_LO; 
   tick();
  // Done
  }

void I2C_STOP(){
  // Pull SDA/SCL Lo
  SDA_LO;
  SCL_LO;
  // Release SCL HI
  SCL_HI;
   tick();
  // Release SDA HI
  SDA_HI; 
   tick();
  // Done
  } 

//Function to send a frame
void I2C_SEND(byte SDA){
  //Begin byte loop
  for(int i = 7; i>= 0; i--) {
    //Release SDA HI or pull LO based on the MSB of byte SDA
    (SDA & 0x80) ? SDA_HI : SDA_LO;
    //Bitshift MSB out
    SDA <<= 1;
    //Advance the clock
     tick();
    SCL_HI;  
     tick();
    SCL_LO;
     tick();
   }

  //Listen for ACK bit from slave
  bool ACK = !SDA_READ();
  tick();
  //Send SCL Lo, command is finished
  SCL_LO;
  tick();
  
  }

//Function to listen for a frame, note if it's the last frame or not
void I2C_READ(bool END = false){

  //Clear the data buffer
  byte DATA = 0x00;

  // Begin byte loop
  for(int i = 7; i>= 0; i--) {
    //Bitshift data buffer left one
    DATA <<= 1;
    //if SCL_READ tells us the SCL clock is in use, keep the clock released
      do{
          SCL_HI;
      } while(SCL_READ == 0);
     tick();
    //Set LSB of DATA to output of SDA_READ function
    DATA |= SDA_READ();
     tick();
    //Send SCL Lo, iteration is finished
    SCL_LO;
     tick();
   }
  //Check if this is the last read in the sequennce
  //If it isn't, send an ACK by sending the bus LO
    if (!END) {
     ACK ? SDA_HI : SDA_LO;
     } else {
      SDA_HI;
     }
  //Tick the clock and release SDA
   tick();
  SCL_HI;
   tick();
  SCL_LO;
   tick();
  SDA_HI;

  //Return read byte
  return(DATA);
  }

//Commands to read temperature from a DHT20
void loop(){

  I2C_START();
  I2C_SEND(0x70);
  I2C_SEND(0xAC);
  I2C_SEND(0x33);
  I2C_SEND(0x01);
  I2C_STOP();
  tick();
  I2C_START();
  I2C_SEND(0x71);
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ(true);
  I2C_STOP();
  tick();

}
