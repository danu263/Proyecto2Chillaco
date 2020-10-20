#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};
bool sleep = 0;
bool confirmation = 0;
char dataReceived[32] = "";
const char confirmationMessage = "Data Received, go to sleep mode";
const char sendText[] = "Send data";   

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
Serial.println("ffffirst print");
if (!sleep == 1)  {

    Serial.println("first print");
    radio.stopListening();                                    // First, stop listening so we can talk.
     
    while(!confirmation){
    Serial.println(sendText);
    radio.write(&sendText, sizeof(sendText));

    if (!radio.write( &sendText, sizeof(sendText) )){
       Serial.println("failed");
    }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 5000000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println("No data received after 5 seconds");
    }else{
                                                                // Grab the response, compare, and send to debugging spew
        unsigned long time = micros();
        radio.read(&dataReceived, sizeof(dataReceived));
          
        Serial.println("Data received is: ");
        Serial.println(dataReceived);
        Serial.println("Sending confirmation");
        
        radio.stopListening();
        radio.write(&confirmationMessage, sizeof(confirmationMessage));
        confirmation = 1;
    }

    Serial.println("2 seconds of delay, printing state of confirmation: ");
    Serial.println(confirmation);
    delay(2000);
    }
    confirmation = 0;
    Serial.println("Successful request");
    Serial.println("Changing confirmation to send a request again");
    delay(3000);
  }
  
}
