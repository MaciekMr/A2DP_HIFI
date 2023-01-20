
#include "I2SInterface.h"



I2SInterface::I2SInterface()
{
    i2s_handler = {0};
    sound_quality = {
        .sample_rate = 44100,
        .data_width = I2S_DATA_BIT_WIDTH_16BIT,
        .mode = I2S_SLOT_MODE_STEREO
    };
    handler_enabled = false;

}

I2SInterface::~I2SInterface()
{
    /* Have to stop the channel before deleting it */
    i2s_channel_disable(i2s_handler.tx_handler);
    handler_enabled = false;
    /* If the handle is not needed any more, delete it to release the channel resources */
    i2s_del_channel(i2s_handler.tx_handler);
}

void I2SInterface::init_i2s()
{
    esp_err_t error = ESP_OK;
    /* Setp 1: Determine the I2S channel configuration and allocate two channels one by one
     * The default configuration can be generated by the helper macro,
     * it only requires the I2S controller id and I2S role
     * The tx and rx channels here are registered on different I2S controller,
     * Except ESP32 and ESP32-S2, others allow to register two separate tx & rx channels on a same controller */
    //i2s_handler.channel_configuration = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_handler.channel_configuration.id = I2S_NUM_0;  //= I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_handler.channel_configuration.role = I2S_ROLE_MASTER;
    i2s_handler.channel_configuration.dma_desc_num = 6;
    i2s_handler.channel_configuration.dma_frame_num = 240;
    i2s_handler.channel_configuration.auto_clear = true;
    //Create new channel only for TX, RX -> NULL
    ESP_ERROR_CHECK(error = i2s_new_channel(&i2s_handler.channel_configuration, &i2s_handler.tx_handler, NULL));
    
   

    /* Step 2: Setting the configurations of standard mode and initialize each channels one by one
     * The slot configuration and clock configuration can be generated by the macros
     * These two helper macros is defined in 'i2s_std.h' which can only be used in STD mode.
     * They can help to specify the slot and clock configurations for initialization or re-configuring */
    //i2s_handler.i2s_configuration = {
    
    i2s_handler.i2s_configuration.clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(sound_quality.sample_rate);
    i2s_handler.i2s_configuration.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(sound_quality.data_width, sound_quality.mode);
    
    i2s_handler.i2s_configuration.gpio_cfg.mclk = I2S_GPIO_UNUSED,    // some codecs may require mclk signal, this example doesn't need it
    i2s_handler.i2s_configuration.gpio_cfg.bclk = (gpio_num_t)I2S_CLK_PIN,
    i2s_handler.i2s_configuration.gpio_cfg.ws   = (gpio_num_t)I2S_LRCK_PIN,
    i2s_handler.i2s_configuration.gpio_cfg.dout = (gpio_num_t)I2S_DATA_PIN,
    i2s_handler.i2s_configuration.gpio_cfg.din  = I2S_GPIO_UNUSED,
    i2s_handler.i2s_configuration.gpio_cfg.invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            };
        //};
    //};
    ESP_ERROR_CHECK(error = i2s_channel_init_std_mode(i2s_handler.tx_handler, &i2s_handler.i2s_configuration));
    if(error != ESP_OK)
    {

    }else{

    }
}

void I2SInterface::reconfigI2S()
{
    esp_err_t error;
    ESP_LOGI(I2S_TAG, "Reconfiguration initiated");
    i2s_channel_disable(i2s_handler.tx_handler);
    handler_enabled = false;

    i2s_handler.i2s_configuration.clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sound_quality.sample_rate);
    i2s_handler.i2s_configuration.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, sound_quality.mode);
    error = i2s_channel_reconfig_std_clock(i2s_handler.tx_handler, &i2s_handler.i2s_configuration.clk_cfg);
    error = i2s_channel_reconfig_std_slot(i2s_handler.tx_handler, &i2s_handler.i2s_configuration.slot_cfg);

    if(error != ESP_OK) //one call for each channel (slot -> channel)
    {
        ESP_LOGE(I2S_TAG, "i2s_channel reconfig failed");
    }
    i2s_channel_enable(i2s_handler.tx_handler);
    handler_enabled = true;
}

esp_err_t I2SInterface::init_i2s_driver()
{
    esp_err_t error = ESP_OK;
    //if((error = i2s_channel_enable(i2s_handler.tx_handler)) != ESP_OK)
    //    ESP_LOGE(I2S_TAG, "Enable channel has failed!");
    ESP_LOGI(I2S_TAG, "TX handler status %s", handler_enabled ? "true" : "false");
    return (error);
}

size_t I2SInterface::i2s_write_data(const uint8_t* data, size_t item_size, bool extended)
{
    size_t bytes_written;
    esp_err_t error;
    if(!handler_enabled)
        ESP_LOGI(I2S_TAG, "TX handler status %s", handler_enabled ? "true" : "false");
    if((error = i2s_channel_write(i2s_handler.tx_handler, data, item_size, &bytes_written, portMAX_DELAY)) != ESP_OK)
    {
        ESP_LOGE(I2S_TAG, "i2s_write has failed ");
    //}else{
    //    ESP_LOGI(I2S_TAG, "i2s_write send %d bytes ", bytes_written);
    }
    return(error);
}

esp_err_t I2SInterface::uninstall_driver()
{
    esp_err_t error = ESP_OK;
    error = i2s_channel_disable(i2s_handler.tx_handler);
    handler_enabled = false;
    i2s_del_channel(i2s_handler.tx_handler);
    return(error);
}

esp_err_t I2SInterface::set_sample_channels()
{
    esp_err_t error = ESP_OK;
    reconfigI2S();
    return(error);
}

esp_err_t I2SInterface::clear_dma_buffer()
{
    esp_err_t error = ESP_OK;

    return(error);
}

esp_err_t I2SInterface::start()
{
    esp_err_t error = ESP_OK;
    if(!handler_enabled)
    {
        ESP_LOGI(I2S_TAG, "TX handler status %s", handler_enabled ? "true" : "false");
        error = i2s_channel_enable(i2s_handler.tx_handler);
        handler_enabled = true;
    }else{
        ESP_LOGI(I2S_TAG, "TX handler status %s", handler_enabled ? "true" : "false");
    }
    return(error);
}

esp_err_t I2SInterface::stop()
{
    esp_err_t error = ESP_OK;
    error = i2s_channel_disable(i2s_handler.tx_handler);
    handler_enabled = false;
    return(error);
}

void I2SInterface::set_sample_rate(uint32_t freq)
{
    sound_quality.sample_rate = freq;
}

void I2SInterface::set_bits_per_sample(i2s_data_bit_width_t bps)
{
    sound_quality.data_width = bps;
}

i2s_data_bit_width_t I2SInterface::get_bits_per_sample()
{
    return(sound_quality.data_width);
}

void I2SInterface::set_i2s_config(i2s_config i2s_config)
{

}

esp_err_t I2SInterface::init_spp()
{
    esp_err_t error = ESP_OK;
    spp_config = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true,
        .tx_buffer_size = ESP_SPP_MIN_TX_BUFFER_SIZE
    };
    if((error = esp_spp_enhanced_init(&spp_config))!= ESP_OK)
    {
        ESP_LOGW(I2S_TAG,"SPP failed to initialise");
    }else{
        ESP_LOGI(I2S_TAG,"SPP initialised sucessfully");
    }

    return(error);
}

uint16_t I2SInterface::get_sample_rate()
{
    return (sound_quality.sample_rate);
}


uint8_t I2SInterface::get_role()
{
    return(i2s_handler.channel_configuration.role);
}

void I2SInterface::set_i2s_output(bool out)
{
    //is_i2s_output = out;
    i2s_output = out?EXTERNAL:INTERNAL;
}

bool I2SInterface::get_i2s_output()
{
    return(i2s_output);
}