#define SDA_HI PORTB = (PORTB & ~(1 << 3)) | 0x08 
#define SDA_LO PORTB = (PORTB & ~(1 << 3)) | 0x00

#define SCL_HI PORTB = (PORTB & ~(1 << 4)) | 0x16 
#define SCL_LO PORTB = (PORTB & ~(1 << 4)) | 0x00

void setup() {
  DDRB = 0xFF;
  PORTB = 0x00;
  SDA_HI;
  SCL_HI;
}



void I2C_START(int X){
  //I2C START 
  //Pull SCL / SDA HI
  SDA_HI;
  delay(X);
  SCL_HI;
  delay(X);
  //Pull SDA LO, WAIT, PULL SCL LO
  SDA_LO;
  delay(X);
  SCL_LO;
  delay(X);
}

void I2C_STOP(int X){
  //I2C START 
  //Pull SCL / SDA HI
  SDA_LO;
  delay(X);
  SCL_LO;
  delay(X);

  //Pull SCL HI
  //WAIT
  //PULL SDA HI

  SCL_HI;
  delay(X);
  SDA_HI;
  delay(X);
}

void I2C_SEND(byte SDA, int X){
  //Transmit Byte
  for(int i = 8; i>= 0; i--) {
    (SDA & 0x80) ? SDA_LO : SDA_HI; 
    SDA <<= 1;
    delay(X);
    SCL_LO;
    delay(X);
    SCL_HI;   
    delay(X);
  }

  SDA_HI;
  SCL_HI;
}

void loop(){

SDA_HI;
SCL_HI;

delay(2000);

I2C_START(20);
I2C_SEND(0x48, 20); //H
I2C_SEND(0x45, 20); //E
I2C_SEND(0x4c, 20); //L
I2C_SEND(0x4c, 20); //L
I2C_SEND(0x4f, 20); //O
I2C_STOP(20);

delay(1000);

I2C_START(20);
I2C_SEND(0x57, 20); //W
I2C_SEND(0x4f, 20); //O
I2C_SEND(0x52, 20); //R
I2C_SEND(0x4c, 20); //L
I2C_SEND(0x44, 20); //D
I2C_SEND(0x21, 20); //!
I2C_STOP(20);

delay(5000);

}
