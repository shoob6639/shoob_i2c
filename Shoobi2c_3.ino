#define SDA_HI PORTB = (PORTB & ~(1 << 3)) | 0x08 
#define SDA_LO PORTB = (PORTB & ~(1 << 3)) | 0x00 

#define SCL_HI PORTB = (PORTB & ~(1 << 4)) | 0x16 
#define SCL_LO PORTB = (PORTB & ~(1 << 4)) | 0x00 

void setup() {

  DDRB = 0xFF;

  SDA_HI;
  SCL_HI;
}

int X=100; //TIME INTERVAL

void I2C_START(int X){
  //I2C START 
  //Pull SCL / SDA HI
  SDA_HI;
  SCL_HI;
  delay(X);
  //Pull SDA LO, WAIT, PULL SCL LO
  SDA_LO;
  delay(X);
  SCL_LO;
  delay(X);

}

void I2C_STOP(int X){
  SDA_LO;
  delay(X);
  SCL_LO;
  delay(X);
  //Pull SCL / SDA HI
  SCL_HI;
  delay(X);
  SDA_HI;
  delay(X);
}

void I2C_SEND(byte SDA){
  //Transmit Byte
  delay(1);
  SDA_HI;
  for(int i = 7; i>= 0; i--) {
    delay(1);
    SCL_HI;
    (SDA & 0x80) ? SDA_HI : SDA_LO; 
    SDA <<= 1;
    delay(99);
    SCL_LO;
  }
  delay(1);
  SDA_LO;
  delay(1);
  SCL_HI;
  delay(1);
  SCL_LO;
  delay(1);
}



void loop(){

I2C_START(X);
I2C_SEND(0XAA);
I2C_SEND(0XFF);
I2C_SEND(0X13);
I2C_SEND(0X38);
I2C_SEND(0X4B);
I2C_STOP(X);
while(1) {
    //do nothing in here
  }
}
