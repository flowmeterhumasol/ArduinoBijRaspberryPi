#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define CLIENT_ADDRESS 2
#define SERVER_ADDRESS 1

//Verzend buffer
const int lengthSendingBuffer= 30;
char emptyChar[lengthSendingBuffer]= " "; 
char sendingBuf[lengthSendingBuffer][20];  
int indexBuf=0;
int indexBuf_empty=0; 
unsigned long time;

//Test voor in de buffer 
uint16_t liters1= 0;
uint8_t from1= 2;  

//Serial communication ISR
String responseRPI = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

// Singleton instance of the radio driver
RH_RF95 driver(6, 2);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

//ISR communication with RPI
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    responseRPI += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setup() 
{
  // Dramco uno - enable 3v3 voltage regulator
 pinMode(8, OUTPUT);
digitalWrite(8, HIGH);


  Serial.begin(9600);
  while(!Serial) ; // Wait for serial port to be available
  if(!manager.init())
    Serial.println("init failed");
    driver.setFrequency(868);
  
  // reserve 200 bytes for the inputString:
  responseRPI.reserve(200);
  //Serial.println("Setup complete");

  sprintf(sendingBuf[indexBuf_empty],"%u:%u",from1,liters1);  //format two ints into character array
  indexBuf_empty = (indexBuf_empty+1) % (lengthSendingBuffer); 
  time= millis(); 
}

void loop()
{
    // print the string when a newline arrives:
  if (stringComplete) {
    if (responseRPI.compareTo("ACK")){
      for(int i=0;i<lengthSendingBuffer;i++){
      sendingBuf[indexBuf][i]=' ';
      }
      indexBuf = (indexBuf+1) % (lengthSendingBuffer); 
    }
    // clear the string:
    responseRPI = "";
    stringComplete = false;
  }

  if ((indexBuf < indexBuf_empty ||(indexBuf==lengthSendingBuffer-1 && indexBuf != indexBuf_empty )) && millis()-time>1000){
    Serial.print(indexBuf);
    Serial.print( "." );
    Serial.println(sendingBuf[indexBuf]); 
    time= millis(); 
    }

    
    

  if (manager.available())
  {
    // Should be a message for us now   
    uint8_t buf_data[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf_data);
    uint8_t from;
    if (manager.recvfromAck(buf_data, &len, &from))
    {
      uint16_t buf_merged= (buf_data[1] << 8) | buf_data[0];
      sprintf(sendingBuf[indexBuf_empty],"%u:%u",from,buf_merged);  //format two ints into character array
      indexBuf_empty = (indexBuf_empty+1) % (lengthSendingBuffer);
    } 
  }
}