// # Editor    :Jiang from DFRobot, Jordan Lyn 2
// # Data      :18.09.2012

// # Product name:ultrasonic scanner
// # Product SKU:SEN0001
// # Version :  0.2

// # Description:
// # The Sketch for scanning 180 degree area 4-500cm detecting range

// # Connection:
// #       Pin 1 VCC (URM V3.2) -> VCC (Arduino)
// #       Pin 2 GND (URM V3.2) -> GND (Arduino)
// #       Pin 4 PWM (URM V3.2) -> Pin 3 (Arduino)
// #       Pin 6 COMP/TRIG (URM V3.2) -> Pin 5 (Arduino)
// #
//int URPWM = 3; // PWM Output 0－25000US，Every 50US represent 1cm
//int URTRIG=5; // PWM trigger pin


#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10

//int distance=0;
boolean checker= 1;
boolean checker2 =1;

long duration, distance;

//unsigned int Distance=0;
//unsigned int Distance2=0;
//uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(270, PIN, NEO_GRB + NEO_KHZ800);
int stripDim[] = {0, 72, 131, 178, 210};

//int step1FirstPixel=0; //a1
//int step1EndPixel=5;//a2
//int step2FirstPixel=5;//b1
//int step2EndPixel=17;//b2
//int step3FirstPixel=17;//c1
//int step3EndPixel=32;//c2



void setup()
{                                 // Serial initialization
  Serial.begin(9600);                         // Sets the baud rate to 9600
  //PWM_Mode_Setup();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //brighten(e1,e2);
  delay(50);
}

void loop()
{
//PWM_Mode();
//delay(20);
calcDis();
int brightenCount=0;
  while(checker){
    int stripArraySize = sizeof(stripDim) / sizeof(int);
    if (brightenCount==0){
      brighten(stripDim[stripArraySize - 2], stripDim[stripArraySize - 1]);
    }
    /*bothdim(a1,a2,b1, b2);
    delay(50);
    bothdim(b1,b2,c1,c2);
    delay(50);
    bothdim(c1,c2,d1,d2);
    delay(50);
    bothdim(d1,d2,e1,e2);
    delay(50);
    bothdim(e1,e2,d1,d2);
    delay(50);
    bothdim(d1,d2,c1,c2);
    delay(50);
    bothdim(c1,c2,b1,b2);
    delay(50);
    bothdim(b1,b2,a1,a2);
    delay(50);*/
    for (int i = stripArraySize - 1; i >= 2; i--) {
      bothdim(stripDim[i - 1], stripDim[i], stripDim[i - 2], stripDim[i - 1]);
      delay(100);
    }

    for (int i = 0; i < stripArraySize - 2; i++) {
      bothdim(stripDim[i], stripDim[i + 1], stripDim[i + 1], stripDim[i + 2]);
      delay(100);
    }

    brightenCount++;
  }

  colorWipe(strip.Color(0, 0, 0), 20);

}
//PWM mode setup function
// 0 to 255
void brighten(int stripStart, int stripEnd ) {
  uint16_t i, j;

  for (j = 0; j < 100; j=j+3) {
    for (i = stripStart; i < stripEnd; i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
  }
  //delay(1500);
}

// 255 to 0
void darken(int stripStart, int stripEnd ) {
  Serial.begin(9600);
  uint16_t i, j;

  for (j = 50; j > 0; j--) {
    for (i = stripStart; i < stripEnd; i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
    Serial.println(j);
  }
  delay(1500);
}

void bothdim(int strip1Start, int strip1End, int strip2Start, int strip2End){

  uint16_t i, j,i2,j2;
  for (j = 0, j2=100; j < 100; j=j+3, j2=j2-3) {
    for (i = strip1Start, i2 =strip2Start; (i < strip1End)||(i2<strip2End); i++,i2++) {

      if(i>=strip1End){
        i=strip1End-1;
      }
      if(i2>=strip2End){
        i2=strip2End-1;
      }
      strip.setPixelColor(i, j2, j2, j2);
      strip.setPixelColor(i2, j, j, j);
    }
    strip.show();
    delay(10);
  }
  //PWM_Mode();//NEWWW
  calcDis();
}





void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

      void colorWipe(uint32_t c, uint8_t wait) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);

    //delay(wait);
  }
  strip.show();
}


void calcDis(){

  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  //Serial.println(distance);

  if (distance > 200) {  // This is where the LED On/Off happens
    //Serial.print("NEUTRAL ");
    checker=0;
  }
  else if(7 < distance < 40) {
    //Serial.print("DETECTED ");
    checker=1;
  }
  delay(20);
}


/* void PWM_Mode_Setup()
{
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH

  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command

  for(int i=0;i<4;i++)
  {
      Serial.write(EnPwmCmd[i]);
  }
}

/*
void PWM_Mode()
{                              // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses

    unsigned long DistanceMeasured=pulseIn(URPWM,LOW);


    if(DistanceMeasured>=10200)
    {              // the reading is invalid.
      Serial.println("Invalid");
      checker=0;
    }
    else
    {
      Distance2=DistanceMeasured;
      Distance=DistanceMeasured/50;           // every 50us low level stands for 1cm
      distance=int(Distance);
      Serial.print("Distance=");
      Serial.print(distance);
      Serial.println("cm");
      if (10<distance<33){
      checker=1;

    }

}}*/
