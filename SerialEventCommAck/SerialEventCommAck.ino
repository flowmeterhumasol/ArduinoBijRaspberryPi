
String responseRPI = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

uint16_t liters1= 25;
uint8_t from1= 2; 
uint16_t liters2= 33;
uint8_t from2= 3; 
uint16_t liters3= 5;
uint8_t from3= 2; 

char buf[20][20];              
bool sending;
int index=0; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  responseRPI.reserve(200);
}

void loop() {
    sprintf(buf[0],"%u:%u",from1,liters1);  //format two ints into character array
    sprintf(buf[1],"%u:%u",from2,liters2);  //format two ints into character array      
    sprintf(buf[2],"%u:%u",from3,liters2);  //format two ints into character array      
  
  // print the string when a newline arrives:
  if (stringComplete) {
    if (responseRPI.compareTo("ACK")){
    index ++; 
      digitalWrite(LED_BUILTIN, HIGH);   
    delay(300);                       
    digitalWrite(LED_BUILTIN, LOW);
    }
    // clear the string:
    responseRPI = "";
    stringComplete = false;
  }

  if (index <= 2){
    Serial.print(index);
    Serial.print( "." );
    Serial.println(buf[index]);
    }
    
  delay(3500);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
