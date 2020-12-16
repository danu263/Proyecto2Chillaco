#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};
bool sleep = 0;
bool confirmation = 0;
int dataReceived;
const char confirmationMessage[] = "Data Received, go to sleep mode";
const char sendText[] = "Send data";   

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  int rh;

  rh = readRH();
  Serial.println("Data in main is:");
  Serial.println(rh);
  
}

int readRH(){

  int humidity = 322;
  radio.stopListening();                                    // First, stop listening so we can talk.
  int n = 0;

  while(!confirmation && n < 3){
  Serial.println(sendText);
  radio.write(&sendText, sizeof(sendText));
      
  radio.startListening();                                    // Now, continue listening
  
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
  
  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 3000000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
    }      
  }
      
  if ( timeout ){                                             // Describe the results
      Serial.println("No data received after 3 seconds");
  }else{
                                                              // Grab the response, compare, and send to debugging spew
      unsigned long time = micros();
      radio.read(&dataReceived, sizeof(dataReceived));
        
      Serial.println("Data received is: ");
      Serial.println(dataReceived);
      Serial.println("Sending confirmation");
      humidity = dataReceived;
     
      radio.stopListening();
      radio.write(&confirmationMessage, sizeof(confirmationMessage));
      confirmation = 1;
  }
  n++;  
  }
  confirmation = 0;
  return humidity;
}
