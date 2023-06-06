#include <ESP_Arduino_CC1101_NFS32002.h>

const int SPI_SCK = 18; 			//board or mcu specific
const int SPI_MISO = 16; 			//board or mcu specific
const int SPI_MOSI = 19; 			//board or mcu specific
const int SPI_CS = 17;
const int RADIO_INPUT_PIN = 7;
const int RADIO_OUTPUT_PIN = 4;

// Créer une fonction qui sera appelée lors de la détection d'une télécommande
void detect(){
  Serial.println("Ouistici !");
}

void setup(){
  Serial.begin(9600);
  
  // Initialiser le module radio
  CC1101_NFS32002 radio(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS, RADIO_INPUT_PIN, RADIO_OUTPUT_PIN);
  
  // Appeler la méthode startDetection en lui indiquant en paramètre la fonction à appeler lors de la détection
  startDetection(detect);
}


void loop(){

}
