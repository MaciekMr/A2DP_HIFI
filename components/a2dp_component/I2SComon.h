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

class I2SCommon
{

protected:

    i2s_config_t i2s_config;
    //i2s_std_config_t i2s_config;
    i2s_pin_config_t pin_config; 
    //i2s_std_gpio_config_t pin_config;

    bool is_i2s_output = true;

    i2s_channel_t i2s_channels = I2S_CHANNEL_STEREO;
    //i2s_slot_mode_t i2s_channels = I2S_SLOT_MODE_STEREO;
    i2s_port_t i2s_port = I2S_NUM_0; 
    //i2s_chan_config_t i2s_channel_config = I2S_CHANNEL_DEFAULT_CONFIG(i2s_port, I2S_ROLE_MASTER);;
    i2s_chan_handle_t tx_chan;

    
    
    

public:
    I2SCommon();
    ~I2SCommon();

    //Initialise I2S 
    virtual void init_i2s();

    /// writes the data to i2s
    size_t i2s_write_data(const uint8_t* data, size_t item_size);
    
    //i2s_bits_per_sample_t bits_per_sample;
    void set_bits_per_sample(i2s_bits_per_sample_t );

    /// Define the pins
    virtual void set_pin_config(i2s_pin_config_t pin_config);
    //virtual void set_pin_config(i2s_std_gpio_config_t pin_config);
   
    /// Define an alternative i2s port other then 0 
    virtual void set_i2s_port(i2s_port_t i2s_num);
   
    /// Define the i2s configuration
    virtual void set_i2s_config(i2s_config_t i2s_config);
    //virtual void set_i2s_config(i2s_std_config_t i2s_config);

    uint16_t sample_rate();

    void set_sample_rate(uint32_t);
    /// ends the I2S bluetooth sink with the indicated name - if you release the memory a future start is not possible
    //virtual void end(bool release_memory=false);

    /// set output to I2S_CHANNEL_STEREO (default) or I2S_CHANNEL_MONO
   
    i2s_config_t get_config(){return i2s_config;}
    
    /// Defines the pin for the master clock
    virtual esp_err_t i2s_mclk_pin_select(const uint8_t pin);

    bool use_i2s_output();

    void set_i2s_output(bool out);

    esp_err_t uninstall_driver();

    esp_err_t init_driver();

    esp_err_t set_sample_channels();

};