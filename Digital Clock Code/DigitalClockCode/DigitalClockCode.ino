#include<DS3231.h>

DS3231 rtc(SDA,SCL);
Time t;

int inputsForDecoder[10][4] = { { 0,0,0,0 } ,
                                { 0,0,0,1 } ,
                                { 0,0,1,0 } ,
                                { 0,0,1,1 } ,
                                { 0,1,0,0 } ,
                                { 0,1,0,1 } ,
                                { 0,1,1,0 } ,
                                { 0,1,1,1 } ,
                                { 1,0,0,0 } ,
                                { 1,0,0,1 } } ;

int DarkModeInputsForDecoder[4] = { 1,1,1,1 } ;

int decoderPinsForFirstSegmentOfHour[4] = { 22,23,24,25 } ;
int decoderPinsForSecondSegmentOfHour[4] = { 26,27,28,29 } ;
int decoderPinsForFirstSegmentOfMinute[4] = { 30,31,32,33 } ;
int decoderPinsForSecondSegmentOfMinute[4] = { 34,35,36,37 } ;
int decoderPinsForFirstSegmentOfSecond[4] = { 38,39,40,41 } ;
int decoderPinsForSecondSegmentOfSecond[4] = { 42,43,44,45 } ;

                                

void setup() {
  //Serial.begin(115200);
  rtc.begin();
  for(int i = 11; i<=13 ; i++){
    pinMode(i,INPUT); // Time Editing Switch
  }
  for(int i = 9 ; i<=10 ; i++){
    pinMode(i,OUTPUT); // 9 LED 10 Buzzer
  }
  for(int i = 22 ; i<=45 ; i++){
    pinMode(i,OUTPUT);
  }
  
}

void printTimeInSevenSegment(){
  int tempHour = t.hour ;
  if(tempHour>12)tempHour = tempHour - 12 ;
  if(tempHour==0)tempHour = tempHour + 12 ;
  int firstDigitOfHour = tempHour/10;
  int secondDigitOfHour = tempHour%10;
  int firstDigitOfMinute = t.min/10;
  int secondDigitOfMinute = t.min%10;
  int firstDigitOfSecond = t.sec/10;
  int secondDigitOfSecond = t.sec%10;

  if(firstDigitOfHour!=0){
    for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForFirstSegmentOfHour[i],inputsForDecoder[firstDigitOfHour][i]);
    }
  }
  else{
    for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForFirstSegmentOfHour[i],DarkModeInputsForDecoder[i]);
    }
  }
  for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForSecondSegmentOfHour[i],inputsForDecoder[secondDigitOfHour][i]);
  }

  //------------------------------------------------------
  for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForFirstSegmentOfMinute[i],inputsForDecoder[firstDigitOfMinute][i]);
  }
  for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForSecondSegmentOfMinute[i],inputsForDecoder[secondDigitOfMinute][i]);
  }
  
  //------------------------------------------------------
  for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForFirstSegmentOfSecond[i],inputsForDecoder[firstDigitOfSecond][i]);
  }
  for(int i = 0 ; i<4 ; i++){
      digitalWrite(decoderPinsForSecondSegmentOfSecond[i],inputsForDecoder[secondDigitOfSecond][i]);
  }
  
  
  
   
}

void giveSound(){
  for(int i = 0 ; i<5 ; i++){
    digitalWrite(10,HIGH);
    digitalWrite(9,HIGH);
    delay(1000);
    t = rtc.getTime();
    printTimeInSevenSegment();
    digitalWrite(10,LOW);
    digitalWrite(9,LOW);
    delay(1000);
    t = rtc.getTime();
    printTimeInSevenSegment();
  }
  
}

void printInSerialMonitor(){
  Serial.print(t.hour);
  Serial.print(" : ");
  Serial.print(t.min);
  Serial.print(" : ");
  Serial.print(t.sec);
  Serial.println("");
  
}

void loop() {
  digitalWrite(9,HIGH);
  t = rtc.getTime();
  //printInSerialMonitor();
  printTimeInSevenSegment();
  if(t.min==00 && t.sec==0){
      giveSound();
  }
  while(digitalRead(13)==HIGH){
    t = rtc.getTime();
    int tempHour = t.hour ;
    if(tempHour>12)tempHour = tempHour - 12 ;
    if(tempHour==0)tempHour = tempHour + 12 ;
    tempHour++;
    if(tempHour==13)tempHour = 1 ;
    rtc.setTime(tempHour , t.min , t.sec) ;
    t = rtc.getTime();
    printTimeInSevenSegment();
    //printInSerialMonitor();
    delay(1000);
  }
  while(digitalRead(12)==HIGH){
    t = rtc.getTime();
    int tempMin = t.min ;
    tempMin++ ;
    if(tempMin==60) tempMin = 0 ;
    rtc.setTime(t.hour , tempMin , t.sec);
    t = rtc.getTime();
    printTimeInSevenSegment();
    //printInSerialMonitor();
    delay(1000);
  }
  if(digitalRead(11)==HIGH){
    t = rtc.getTime();
    rtc.setTime(t.hour,t.min,0);
    t = rtc.getTime();
    printTimeInSevenSegment();
    //printInSerialMonitor();
  }
  delay(500);
  digitalWrite(9,LOW);
  delay(500);
}
