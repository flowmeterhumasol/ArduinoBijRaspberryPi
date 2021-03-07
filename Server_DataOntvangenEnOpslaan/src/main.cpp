
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with the RFM95W 

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>


// Singleton instance of the radio driver
RH_RF95 rf95(6, 2);
int verbruikPerKraan[20]; 

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

void setup() 
{
  // Dramco uno - enable 3v3 voltage regulator
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  Serial.begin(9600);
  while(!Serial) ; // Wait for serial port to be available
  if(!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    rf95.setFrequency(868);

}


#warning "compiling lora ptp server code"
void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      Serial.print("got request: ");
      Serial.println((char*)buf);
      //buf van het formaat: int kraannumer : int aantalliters
      //String s1= buf.substring(0,2);
      //int index =  s1.toInt(); 
      //int aantalLiters= ; 
      //verbruikPerKraan[index] += aantalLiters ; 
      //HIER WAS IK AAN HET PRUTSEN 
      //  String s1= waarde.substring(0,2); 
               // int int1= - (s1.toInt()); 
      
      // Send a reply
      uint8_t data[] = "Succes";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }

}


