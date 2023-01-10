
#include "I2SComon.h"


I2SCommon::I2SCommon()
{

}

I2SCommon::~I2SCommon()
{

}

void I2SCommon::set_pin_config(i2s_pin_config_t pin_config)
{

}

void I2SCommon::set_i2s_port(i2s_port_t i2s_num)
{

}

void I2SCommon::set_i2s_config(i2s_config_t i2s_config)
{

}

void I2SCommon::init_i2s()
{
     i2s_port = (i2s_port_t) 0;
        //i2s_new_channel(&i2s_channel_config, &tx_handle, NULL);
    // setup default i2s config
        i2s_config = {
            .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
            .sample_rate = 44100,
            .bits_per_sample = (i2s_bits_per_sample_t)16,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = (i2s_comm_format_t) I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = 0, // default interrupt priority
            .dma_buf_count = 8,
            .dma_buf_len = 64,
            .use_apll = false,
            .tx_desc_auto_clear = true, // avoiding noise in case of data unavailability
            /*.gpio_cfg = {
                .mclk = I2S_GPIO_UNUSED,
                .bclk = I2S_CLK_PIN,
                .ws = I2S_DATA_PIN,
                .dout = I2S_LRCK_PIN,
                .din = I2S_GPIO_UNUSED,
                .invert_flags = {
                    .mclk_inv = false,
                    .bclk_inv = false,
                    .ws_inv = false,
                }
            }*/
        };

        /* Initialize the channel */
        //i2s_channel_init_std_mode(tx_chan, &i2s_config);
        //i2s_channel_enable(tx_chan);
        // setup default pins
        pin_config = {
            .bck_io_num = I2S_CLK_PIN,  //bck
            .ws_io_num = I2S_LRCK_PIN,  //lrck
            .data_out_num = I2S_DATA_PIN, //dout
            .data_in_num = I2S_PIN_NO_CHANGE
        };
}


esp_err_t I2SCommon::init_driver()
{
    esp_err_t error = ESP_OK;
    ESP_LOGI(BT_AV_TAG,"init_i2s");
    if (is_i2s_output){
        ESP_LOGI(BT_AV_TAG,"init_i2s is_i2s_output");
        // setup i2s
        if ((error = i2s_driver_install(i2s_port, &i2s_config, 0, NULL)) != ESP_OK) {
            ESP_LOGE(BT_AV_TAG,"i2s_driver_install failed");
        } else {
            
        }

        // pins are only relevant when music is not sent to internal DAC
        if (i2s_config.mode & I2S_MODE_DAC_BUILT_IN) {
            if ((error = i2s_set_pin(i2s_port, nullptr))!= ESP_OK) {
                ESP_LOGE(BT_AV_TAG,"i2s_set_pin failed");
            }      
            if ((error = i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN))!= ESP_OK){
                ESP_LOGE(BT_AV_TAG,"i2s_set_dac_mode failed");
            }
            ESP_LOGI(BT_AV_TAG, "Output will go to DAC pins");
        } else {
            if ((error = i2s_set_pin(i2s_port, &pin_config)) != ESP_OK) {
                ESP_LOGE(BT_AV_TAG,"i2s_set_pin failed");
            }
        }
    }
    return(error);
}

esp_err_t I2SCommon::i2s_mclk_pin_select(const uint8_t pin)
{
    if(pin != 0 && pin != 1 && pin != 3) {
        ESP_LOGE(BT_APP_TAG, "Only support GPIO0/GPIO1/GPIO3, gpio_num:%d", pin);
        return ESP_ERR_INVALID_ARG;
    }
    switch(pin){
        case 0:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
            WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
            break;
        case 1:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
            WRITE_PERI_REG(PIN_CTRL, 0xF0F0);
            break;
        case 3:
            PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
            WRITE_PERI_REG(PIN_CTRL, 0xFF00);
            break;
        default:
            break;
    }
    return ESP_OK;
}

uint16_t I2SCommon::sample_rate()
{
    return(i2s_config.sample_rate);
}

void I2SCommon::set_sample_rate(uint32_t freq)
{
    i2s_config.sample_rate = freq;
}

void I2SCommon::set_bits_per_sample(i2s_bits_per_sample_t bps)
{
    i2s_config.bits_per_sample = bps;
}

size_t I2SCommon::i2s_write_data(const uint8_t* data, size_t item_size)
{

    return 0;
}

void I2SCommon::set_i2s_output(bool out)
{
    is_i2s_output = out;
}

bool I2SCommon::use_i2s_output()
{
    return(is_i2s_output);
}

esp_err_t I2SCommon::uninstall_driver()
{
    return(i2s_driver_uninstall(i2s_port));
}

esp_err_t I2SCommon::set_sample_channels()
{
    // setup sample rate and channels
    if (i2s_set_clk(i2s_port, i2s_config.sample_rate, i2s_config.bits_per_sample, i2s_channels)!=ESP_OK){
        ESP_LOGE(BT_AV_TAG, "i2s_set_clk failed with samplerate=%d", (int)i2s_config.sample_rate);
    } else {
        ESP_LOGI(BT_AV_TAG, "audio player configured, samplerate=%d", (int) i2s_config.sample_rate);
        player_init = true; //init finished
    }
}