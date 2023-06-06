#include "ESP_Arduino_CC1101_NFS32002.h"
/****************************************************************/

CC1101_NFS32002::CC1101_NFS32002(int spi_sck, int spi_miso, int spi_mosi, int spi_cs, int radio_input_pin, int radio_output_pin){
    
    SPI_SCK = spi_sck;
    SPI_MISO = spi_miso;
    SPI_MOSI = spi_mosi;
    SPI_CS = spi_cs;
    RADIO_OUTPUT_PIN = radio_input_pin;
    RADIO_OUTPUT_PIN = radio_output_pin;
    
    id = 0;
    
    CC1101 cc1101(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS, RADIO_INPUT_PIN, RADIO_OUTPUT_PIN);    
    cc1101.init();
	cc1101.setMHZ(868.3);
	cc1101.setTXPwr(TX_PLUS_10_DBM);
	cc1101.setDataRate(4000);
	cc1101.setModulation(ASK_OOK);
	cc1101.setRx();
    // ready to listen !

}

void CC1101_NFS32002::radioHandlerOnChange(void (*callbackFunction)()) {

    int delta_micros = micros() - last_micros;
	
	bool input = digitalRead(RADIO_OUTPUT_PIN);
	if(input == 1){
    buff[id] = delta_micros;
    vals[id] = 0;
    id++;
	} else {
    buff[id] = delta_micros;
    vals[id] = 1;
    id++; 
	}
  if (id % 50==0){
    Serial.print(" .. ");
  }
	if (id >= BUFFER_LEN){
    cc1101.setIdle();
    id=0;
    Serial.println("checking...");
    detectFrame(vals, buff, (*callbackFunction)());
    cc1101.setRx();
  }
	last_micros = micros();
}

void CC1101_NFS32002::detectFrame(int values[], int timings[], void (*callbackFunction)()){
  bool detected_frame = false;
  int i = 0;
  int values_len = BUFFER_LEN;
  int values_len = sizeof(values)/sizeof(values[0]);
  
  while (i < values_len) {
    Check the presence of a syncword
    bool data = true;
    if (values[i] == 1){
      int j = i;
      
      for (float timing : data_timings){
        if (timings[j] >= (timing * ERROR_RATE_MIN) &&
          timings[j] <= (timing * ERROR_RATE_MAX) &&
          j < values_len){
          j += 1;
        }
        else{
          data = false;
          break;
        }
      }
      if (data) {
        detected_frame = true;
        break;
      }
    }
    i += 1;
  }
  if (detected_frame){
    Serial.println("Ouistici !");
    (*callbackFunction)();
  }
  else{
    Serial.println("Pas de trame détectée");
    i = 0;
  }
}

void CC1101_NFS32002::startDetection(void (*callbackFunction)()){
    
  cc1101.spiWriteReg(0x1B,0x03) ; // AGCCTRL2 ; gain 
  Serial.println("AGCCTRL2 : ");Serial.println(cc1101.spiReadReg(0x1B)); // check gain

  cc1101.spiWriteReg(0x1C,0x00) ; // AGCCTRL1
  Serial.println("AGCCTRL1 : ");Serial.println(cc1101.spiReadReg(0x1C)); // check

  cc1101.spiWriteReg(0x1D,0x91) ; // AGCCTRL0
  Serial.println("AGCCTRL0 : ");Serial.println(cc1101.spiReadReg(0x1D)); // check

  Serial.println("MDMCFG4 : ");
  Serial.print(cc1101.spiReadReg(0x10));
  
  attachInterrupt(RADIO_OUTPUT_PIN, radioHandlerOnChange, CHANGE, (*callbackFunction)());
    
}
