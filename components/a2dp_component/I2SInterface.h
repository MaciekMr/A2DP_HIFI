#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_types.h"
#include "driver/i2s_common.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "sdkconfig.h"
#include "I2SComon.h"

/*TODO: Replace with data from sdkconfig.h*/

#if CONFIG_IDF_TARGET_ESP32
    #define EXAMPLE_STD_BCLK_IO1        GPIO_NUM_4      // I2S bit clock io number
    #define EXAMPLE_STD_WS_IO1          GPIO_NUM_5      // I2S word select io number
    #define EXAMPLE_STD_DOUT_IO1        GPIO_NUM_18     // I2S data out io number
    #define EXAMPLE_STD_DIN_IO1         GPIO_NUM_19     // I2S data in io number
#else
    #define EXAMPLE_STD_BCLK_IO2    GPIO_NUM_6      // I2S bit clock io number
    #define EXAMPLE_STD_WS_IO2      GPIO_NUM_7      // I2S word select io number
    #define EXAMPLE_STD_DOUT_IO2    GPIO_NUM_8      // I2S data out io number
    #define EXAMPLE_STD_DIN_IO2     GPIO_NUM_9      // I2S data in io number
#endif

#define EXAMPLE_BUFF_SIZE               2048


/*
I2S has to be used in standard mode to send data to external 
foraudio we can use only audio clock: i2s_clock_src_t::I2S_CLK_SRC_APLL
*/

struct{
    int mclk;
    int bclk;
    int ws; //known as LRCK
    int dout; //data out to external i2s device 
}pin_configuration;

class I2SInterface
    :public I2SCommon
{
protected:
    i2s_chan_handle_t  tx_chan;        // I2S tx channel handler
    i2s_std_config_t tx_std_cfg;
    i2s_chan_config_t tx_chan_cfg;
    
    I2SInterface();
    ~I2SInterface();
public:
    void initI2S();
    void senddata();
    void setvolume();
    void reconfigI2S();


};