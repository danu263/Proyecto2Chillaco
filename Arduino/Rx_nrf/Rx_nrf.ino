#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};
bool sleep = 0;
char myData[] = "X humedad relativa";
char confirmationMessage[32] = "";
char sendText[32] = "";

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  if (!sleep == 1) {
    
    radio.startListening();

    while (!radio.available());
    radio.read(&sendText, sizeof(sendText));
    Serial.print(sendText);
    
    if( sendText == "Send data") {
      radio.stopListening();
      Serial.print("Sending data");
      radio.write( &myData, sizeof(myData) );
      
      radio.startListening();
      while (!radio.available());
      radio.read(&confirmationMessage, sizeof(confirmationMessage));
      Serial.print(confirmationMessage);
      if (confirmationMessage == "Data Received, go to sleep mode") {
        sleep == 1;
        }
      Serial.print("Estado de sleep: ");
      Serial.print(sleep) ;     
      }
    }
}