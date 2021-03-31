#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define CLIENT_ADDRESS 2
#define SERVER_ADDRESS 1

// Singleton instance of the radio driver
RH_RF95 driver(6, 2);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

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

  //Serial.println("Setup complete");

}

void loop()
{
  if (manager.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      uint16_t buf_merged= (buf[1] << 8) | buf[0];
      //Serial.print("got data from: ");
      Serial.print(from, HEX);
      Serial.print(":");
      Serial.println(buf_merged);     
    } 
  }
}