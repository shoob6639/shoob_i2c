#define SDA_HI DDRB = (DDRB & ~(1 << 3)) | 0x00 // Release SDA HI
#define SDA_LO DDRB = (DDRB & ~(1 << 3)) | 0x08 // Pull SDA LO

#define SCL_HI DDRB = (DDRB & ~(1 << 4)) | 0x00 // Release SDA HI
#define SCL_LO DDRB = (DDRB & ~(1 << 4)) | 0x10 // Pull SCL LO

byte ACK = 0x00;

void setup() {
  SDA_HI;
  SCL_HI;
  Serial.begin(9600);



}

void delayClocks( uint32_t clocks ){
    do
    {
      __asm( "nop" );
    }
    while ( --clocks );
  }



byte SDA_READ(){
  byte OUTPUTSDA = 0x00;
  SDA_HI;
  SCL_HI;
  delayClocks(500);
  OUTPUTSDA = ((PINB & 0x08) >> 3);
  SCL_LO;
  delayClocks(500);
  return(OUTPUTSDA);
  
  }

byte SCL_READ(){
  byte OUTPUTSCL = 0x00;
  SCL_HI;
  delayClocks(500);
  OUTPUTSCL = ((PINB & 0x10) >> 3);
  SCL_LO;
  delayClocks(500);
  return(OUTPUTSCL);
  }  

void I2C_START(){ 
  SDA_HI;
  SCL_HI;

  //Pull SDA LO
  SDA_LO;
   delayClocks(500);
  // Pull SCL LO
  SCL_LO; 
   delayClocks(500);
  //DONE
  }

void I2C_STOP(){
  SDA_LO;
  SCL_LO;
  //Release SCL HI
  SCL_HI;
   delayClocks(500);
  //Release SDA HI
  SDA_HI; 
   delayClocks(500);
  //DONE
  } 

void I2C_SEND(byte SDA){
  //Transmit Byte
  SCL_LO;
  for(int i = 7; i>= 0; i--) {
    (SDA & 0x80) ? SDA_HI : SDA_LO;
    SDA <<= 1;
     delayClocks(500);
    SCL_HI;  
     delayClocks(500);
    SCL_LO;
     delayClocks(500);
   }

  //RELEASE SDA
  bool ACK = !SDA_READ();
  delayClocks(500);
  
  }

byte I2C_READ(){
  delayClocks(500);
  //Transmit Byte
  byte DATA = 0x00;
  SCL_LO;
  for(int i = 7; i>= 0; i--) {
    DATA <<= 1;
    do{
        SCL_HI;
    } while(SCL_READ == 0);
     delayClocks(500);
    DATA |= SDA_READ();
     delayClocks(500);
    SCL_LO;
     delayClocks(500);
   }

  Serial.print(DATA,HEX);
  Serial.println(" ");
  
  ACK ? SDA_HI : SDA_LO;
  delayClocks(500);
  SCL_HI;
  delayClocks(500);
  SCL_LO;
  delayClocks(500);
  SDA_HI;
  return(DATA);
  
  }

void loop(){

  I2C_START();
  I2C_SEND(0x70);
  I2C_SEND(0xAC);
  I2C_SEND(0x33);
  I2C_SEND(0x01);
  I2C_STOP();

  delay(80);
  I2C_START();
  I2C_SEND(0x71);
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_READ();
  I2C_STOP();
  delay(2500);

}
