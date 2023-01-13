#include "config.h"



enum i2s_output_type { INTERNAL = 0, EXTERNAL = 1};

class II2Sinterface
{
protected:

public:
    //Initialise I2S config structure
    virtual void init_i2s() = 0;

    //load i2s driver and start it
    virtual esp_err_t init_i2s_driver() = 0;

    virtual esp_err_t uninstall_driver() = 0;

    virtual esp_err_t set_sample_channels() = 0;

    virtual esp_err_t clear_dma_buffer() = 0;

    virtual esp_err_t start() = 0;

    virtual esp_err_t stop() = 0;

    /// writes the data to i2s
    virtual size_t i2s_write_data(const uint8_t* data, size_t item_size, bool extended = false) = 0;
    
    //i2s_bits_per_sample_t bits_per_sample set in config structure
    virtual void set_bits_per_sample(uint8_t) = 0;

    //get bits_per_sample from config structure
    virtual uint8_t get_bits_per_sample() = 0;

    //init SSP stack with new approach
    virtual esp_err_t init_spp() = 0;

    //Set sample reate
    virtual void set_sample_rate(uint32_t) = 0;

    //get sample rate
    virtual uint16_t get_sample_rate() = 0;

    virtual bool get_i2s_output() = 0;

    virtual void set_i2s_output(bool out) = 0;

};