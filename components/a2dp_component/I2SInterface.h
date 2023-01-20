#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_types.h"
#include "driver/i2s_common.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_spp_api.h"
#include "sdkconfig.h"
#include "II2Sinterface.h"

/*TODO: Replace with data from sdkconfig.h*/

#define EXAMPLE_BUFF_SIZE               2048



#define I2S_TAG "I2S_IF"
/*
I2S has to be used in standard mode to send data to external 
foraudio we can use only audio clock: i2s_clock_src_t::I2S_CLK_SRC_APLL
*/
/*
typedef enum {
    I2S_BITS_PER_CHAN_DEFAULT        = (0),      // *!< channel bit-width equals to data bit-width 
    I2S_BITS_PER_CHAN_8BIT           = (8),      // *!< channel bit-width: 8 
    I2S_BITS_PER_CHAN_16BIT          = (16),     // *!< channel bit-width: 16 
    I2S_BITS_PER_CHAN_24BIT          = (24),     // *!< channel bit-width: 24 
    I2S_BITS_PER_CHAN_32BIT          = (32),     // *!< channel bit-width: 32 
} i2s_bits_per_chan_t;

*/


struct pin_configuration{
    int mclk;
    int bclk;
    int ws; //known as LRCK
    int dout; //data out to external i2s device 
};// pin_configuration_t;


struct i2s_config{
    uint16_t port;
};

struct i2s_handler_t{
    i2s_chan_handle_t tx_handler;
    i2s_chan_config_t channel_configuration;
    i2s_std_config_t  i2s_configuration;
};

struct sound_quality_t{
    uint32_t    sample_rate; //16000, 32000, 44100, 48000
    i2s_data_bit_width_t data_width;
    i2s_slot_mode_t  mode;
};

class I2SInterface
    //:public II2Sinterface
{
protected:
    i2s_handler_t    i2s_handler;
    sound_quality_t  sound_quality;
    esp_spp_cfg_t    spp_config;
    i2s_output_type  i2s_output = EXTERNAL;
    bool             handler_enabled;

public:
    I2SInterface();
    ~I2SInterface();

    void setvolume();
    void reconfigI2S();

    //Override the virtual methods
    virtual void init_i2s();

    //load i2s driver and start it
    virtual esp_err_t init_i2s_driver();

    virtual size_t i2s_write_data(const uint8_t* data, size_t item_size, bool extended = false);

    virtual void set_sample_rate(uint32_t freq);

    virtual void set_bits_per_sample(i2s_data_bit_width_t bps);

    virtual i2s_data_bit_width_t get_bits_per_sample();

    virtual esp_err_t uninstall_driver();

    virtual esp_err_t set_sample_channels();

    virtual esp_err_t clear_dma_buffer();

    virtual esp_err_t start();

    virtual esp_err_t stop();

    void set_pin_config(pin_configuration pin_config);

    void set_i2s_config(i2s_config i2s_config);

    bool get_i2s_output();

    void set_i2s_output(bool);

    esp_err_t init_spp();

    uint16_t get_sample_rate();

    uint8_t get_role();

};