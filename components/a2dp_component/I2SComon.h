#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h" 
#include "driver/i2s_types.h"
#include "driver/i2s_common.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "soc/soc.h"
#include "soc/io_mux_reg.h"
#include "sdkconfig.h"
#include "BluetoothA2DPCommon.h"
#include "II2Sinterface.h"

#define BT_I2S "BT_I2S"
#define BT_SPP "BT_SPP"

struct pin_configuration{
    int mclk;
    int bclk;
    int ws; //known as LRCK
    int dout; //data out to external i2s device 
};// pin_configuration_t;


class I2SCommon
{

protected:

    i2s_config_t i2s_config;
    i2s_pin_config_t pin_config;
    //pin_configuration pin_config; 
  
    i2s_output_type i2s_output = EXTERNAL;

    i2s_channel_t i2s_channels = I2S_CHANNEL_STEREO;
    i2s_port_t i2s_port = I2S_NUM_0; 
    i2s_chan_handle_t tx_chan;

    esp_spp_cfg_t spp_config;
public:
    I2SCommon();
    ~I2SCommon();

    //Initialise I2S config structure
    virtual void init_i2s();

    //load i2s driver and start it
    virtual esp_err_t init_i2s_driver();

    /// writes the data to i2s
    virtual size_t i2s_write_data(const uint8_t* data, size_t item_size, bool extended = false);
    
    //i2s_bits_per_sample_t bits_per_sample set in config structure
    virtual void set_bits_per_sample(i2s_bits_per_sample_t );

    //get bits_per_sample from config structure
    virtual i2s_bits_per_sample_t get_bits_per_sample();

    /// Define the pins
    virtual void set_pin_config(pin_configuration pin_config);
      
    /// Define an alternative i2s port other then 0 
    virtual void set_i2s_port(i2s_port_t i2s_num);
   
    /// Define the i2s configuration (ovewrite the one initiated during init_i2s()
    virtual void set_i2s_config(i2s_config_t i2s_config);
    
    //Set sample reate
    virtual void set_sample_rate(uint32_t);

    //get sample rate
    virtual uint16_t get_sample_rate();

    virtual i2s_config_t get_config(){return i2s_config;}
    
    /// Defines the pin for the master clock
    virtual esp_err_t i2s_mclk_pin_select(const uint8_t pin);

    virtual bool get_i2s_output();

    virtual void set_i2s_output(bool out);

    virtual esp_err_t uninstall_driver();

    virtual esp_err_t set_sample_channels();

    virtual esp_err_t clear_dma_buffer();

    virtual esp_err_t start();

    virtual esp_err_t stop();

    //init SSP stack with new approach
    virtual esp_err_t init_spp();

};