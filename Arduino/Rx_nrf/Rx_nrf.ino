#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};
bool sleep = 0;
char myData[] = "X humedad relativa";
char sendData[] = "Send data";
char confMessage[] = "Data Received, go to sleep mode";
char confirmationMessage[32] = "";
char sendText[32] = "";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  
  if (!sleep == 1) {
    
    radio.startListening();

    while (!radio.available()){
      Serial.println(radio.available());
      delay(500);
      };
    radio.read(&sendText, sizeof(sendText));
    Serial.println("Value of sendText is: ");
    Serial.println(sendText);
    
    if( sendText[0] == 'S') {
      radio.stopListening();
      Serial.println("Sending data");
      radio.write( &myData, sizeof(myData) );
      
      radio.startListening();
      while (!radio.available()){
        Serial.println(radio.available());
        delay(500);
        };
      radio.read(&confirmationMessage, sizeof(confirmationMessage));
      Serial.println("Value of confirmation Message is: ");
      Serial.println(confirmationMessage);
      if (confirmationMessage[0] == 'D') {
        sleep == 1;
        }
      Serial.println("Estado de sleep: ");
      Serial.println(sleep) ;     
      }
    }
}
