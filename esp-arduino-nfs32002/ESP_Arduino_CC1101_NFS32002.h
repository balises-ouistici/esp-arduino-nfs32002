#ifndef ESP_Arduino_CC1101_NFS32002.h
#define ESP_Arduino_CC1101_NFS32002.h

#include <CC1101_ESP_Arduino.h>
#include <Arduino.h>
#include <SPI.h>


class CC1101_NFS32002 {
	
	private:
        volatile long last_micros;

        const float ERROR_RATE = 0.3;
        const float ERROR_RATE_MIN = 1 - ERROR_RATE;
        const float ERROR_RATE_MAX = 1 + ERROR_RATE;
        const float data_timings[46] = {625, 312.5, 312.5, 207.5, 207.5, 500, 500, 250, 250, 250, 250, 500, 500, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 500, 250, 250, 500, 250, 250, 500, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250};

        const int BUFFER_LEN = 800;
        int buff[BUFFER_LEN];
        int vals[BUFFER_LEN];
        
        void radioHandlerOnChange(void (*callbackFunction)());
        void detectFrame(int values[], int timings[], void (*callbackFunction)());
        
        int SPI_SCK;
        int SPI_MISO;
        int SPI_MOSI;
        int SPI_CS;
        int RADIO_OUTPUT_PIN;
        int RADIO_OUTPUT_PIN;
        
        int id = 0;
        
    public:
        CC1101_NFS32002(int spi_sck, int spi_miso, int spi_mosi, int spi_cs, int radio_input_pin, int radio_output_pin);
        void startDetection(void (*callbackFunction)());
}


#endif
