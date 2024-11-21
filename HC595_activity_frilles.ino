
#include "hello_kitty.h"
#include "Sonic.h"
#include "flying_cat.h"
#include "alya.h"
// OLED display
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//global variable
//int i = 0; // this variable is used for loops


//Input
#define LDR A0
int LDRval;
const int LDR_threshold = 300;

#define Potentio A1
int Potval;
int Pot_state;

//IC pins
const int dataPin = 10;  // DS (Data Input) -- pin 12
const int clockPin = 9; // SH_CP (Shift Clock) -- pin 11
const int latchPin = 8;   // ST_CP (Latch Clock) -- pin 14

//timing
unsigned long currentT;
unsigned long imageT = 0;
unsigned long penguinT = 0;
unsigned long LDRT = 0;
unsigned long delayT = 600;

//pattern variables

//activate boolean
bool LDR_state = true ;

int LDR_pattern_num = 0;

bool MSB_LSB = true;

void centerText(const char *text ,int y,int TEXT_SIZE ) {
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(SSD1306_WHITE);
  int textWidth = strlen(text) * 6 * TEXT_SIZE; 
  int textHeight = 8 * TEXT_SIZE;            
  int x = (SCREEN_WIDTH - textWidth) / 2;
  //int y = (SCREEN_HEIGHT - textHeight) / 2;
  y -= textHeight;
  display.setCursor(x, y);
  display.println(text);
}


void setup(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  pinMode(Potentio,INPUT);
  pinMode(LDR, INPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  Serial.begin(9600);
}
void loop(){
  currentT = millis();
  activate();
  Potentiometer();
  //animate_1();
  //animate_2();
  //animate_3();
  //animate_4();
  }




void activate(){
if(LDR_state == true){
  display.clearDisplay();
  centerText("DEACTIVATED",32,1);
  display.display();
  while(LDR_state == true){
  currentT = millis();
  LDRval = analogRead(LDR);
  LDR_state = Light_Dependent_Resistor(LDRval);
    clear();
  }
  activate_animate();
  }

}
bool Light_Dependent_Resistor(int value){
  if (value <= LDR_threshold){
    return false;
  }else
    return true;
}

void clear(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
} 

void Potentiometer(){
  Potval = analogRead(Potentio);
  Pot_state = map(Potval, 0, 1023, 0, 5);
  switch(Pot_state){
    case 0:
      LDR_pattern();
      break;
    case 1:
      animate_1();
      shift_out_data_MSB(1);
      break;
    case 2:
      animate_2();
      shift_out_data_MSB(4);
      break;
    case 3:
      animate_3();
      shift_out_data_MSB(16);
      break;
    case 4:
      Display_reset();  
    default:
      break;
  }
}

void LDR_pattern(){
  LDRval = analogRead(LDR);

  if(LDRval <= LDR_threshold && LDR_pattern_num < 5 && currentT - LDRT >= 700){
    LDR_pattern_num++;
    LDRT = currentT;
  }
  if(LDR_pattern_num >= 5){
    LDR_pattern_num = 0;
  }
  switch(LDR_pattern_num){
    case 0:
    Running_light();
      break;
    case 1:
    expandContractPattern();
      break;
    case 2:
    binaryCounterPattern();
      break;
    case 3:
    BlinkPattern();
      break;
    case 4:
    alternatingPattern();
      break;
    default:
      break;
  }
}




void animate_1(){
  if(currentT - imageT >= 300){
    static int i = 0;
    display.clearDisplay();
    display.drawBitmap(32, 0, hello_kittyallArray[i], 64, 64, WHITE);
    display.display();
    i++;
    if(i >= hello_kittyallArray_LEN){
      i = 0;
    }
    imageT = currentT;
  }

}

void animate_2(){
    if(currentT - imageT >= 10){
    static int i = 0;
    display.clearDisplay();
    display.drawBitmap(19.5, 0, sonicallArray[i], 89, 64, WHITE);
    display.display();
    i++;
    if(i >= sonicallArray_LEN){
      i = 0;
    }
    imageT = currentT;
  }
}

void animate_3(){
  if(currentT - imageT >= 75){
    static int i = 0;
    display.clearDisplay();
    display.drawBitmap(32, 0, alyaallArray[i], 64, 64, WHITE);
    display.display();
    i++;
    if(i >= alyaallArray_LEN){
      i = 0;
    }
    imageT = currentT;
  }
}


void activate_animate(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int k = -80 - (strlen("ACTIVATED")*6);
  for(int j = -80; j < 128; j++){
    static int i = 0;
    display.clearDisplay();
    display.drawBitmap(j, 0, flying_catallArray[i], 80, 64, WHITE);
    i++;
    if(i >= flying_catallArray_LEN){
      i = 0;
    }
    if((128 - k )/ 2 > 48 ){
     k = j - (strlen("ACTIVATED")*6);
    }
    display.setCursor(k,28);
    display.println("ACTIVATED");
    display.display();
  }
}

void deactivate_animate(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int k = -80 - (strlen("ACTIVATED")*6);
  for(int j = -80; j < 128; j++){
    static int i = 0;
    display.clearDisplay();
    display.drawBitmap(j, 0, flying_catallArray[i], 80, 64, WHITE);
    i++;
    if(i >= flying_catallArray_LEN){
      i = 0;
    }
    if((128 - k )/ 2 > 48 ){
     k = j - (strlen("DEACTIVATING")*6);
    }
    display.setCursor(k,28);
    display.println("DEACTIVATING");
    display.display();
  }
}


void Running_light(){
  if(currentT - imageT >= delayT){
    static int i = 0;
    int pattern_display1 = 1 << i;
    display.clearDisplay();
    centerText("Running light",22,1);
    display.display();
    if(MSB_LSB == true){
      i++;
    }else{
      i--;
    }
    if(i > 7 ){
      MSB_LSB = !MSB_LSB;
      i = 6;
    }
    if(i < 0){
      MSB_LSB = !MSB_LSB;
      i = 1;
    }
    shift_out_data_MSB(pattern_display1);
  imageT = currentT;
  }
}

void expandContractPattern() {
  if(currentT - imageT >= delayT){
  static int step = 0;
  static bool expanding = true;
  int pattern;
  if (expanding) {
    pattern = (1 << step) | (1 << (7 - step)); 
  } else {
    pattern = ~((1 << step) | (1 << (7 - step))) & 0xFF; 
  }
  shift_out_data_MSB(pattern);
  step++;
  if (step > 3) {
    step = 0;
    expanding = !expanding; 
  }
  display.clearDisplay();

  centerText("Expand and Contract",22,1);
  display.display();
  imageT = currentT;
  }
}
void binaryCounterPattern() {
  if(currentT - imageT >= delayT){
  static int counter = 0;
  shift_out_data_MSB(counter);
  counter++;
  if (counter > 255) { 
    counter = 0;
  }
  display.clearDisplay();
  centerText("Binary Counter",22,1);
  display.display();
  imageT = currentT;
  }
}
void BlinkPattern() {
  if(currentT - imageT >= delayT){
    static int pattern;
    static bool blink = true;
    if(blink == true){
    pattern = 0b00000000;
    }else{
      pattern = ~0b00000000;
    }
  shift_out_data_MSB(pattern);
  display.clearDisplay();
  centerText("Blink Pattern",22,1);
  display.display();
  blink = !blink;
  imageT = currentT;
  }
}
void alternatingPattern() {
  if(currentT - imageT >= delayT){
  static bool toggle = false;
  int pattern = toggle ? 0b10101010 : 0b01010101;
  shift_out_data_MSB(pattern);
  toggle = !toggle;
  display.clearDisplay();
  centerText("Alternating Pattern",22,1);
  display.display();
  imageT = currentT;
  }
}

void Display_reset(){
  shift_out_data_MSB(128);
  display.clearDisplay();
  centerText("Block the LDR",28,1);
  centerText("To Turn Off",38,1);
  display.display();
  LDRval = analogRead(LDR);
  if(LDRval <= LDR_threshold){
    LDR_state = true;
    shift_out_data_MSB(0);
    deactivate_animate();
    }
}

void shift_out_data_MSB(int pattern){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, pattern);
  digitalWrite(latchPin, HIGH);
}
void shift_out_data_LSB(int pattern){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, pattern);
  digitalWrite(latchPin, HIGH);
}