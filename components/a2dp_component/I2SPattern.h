

class I2SPattern
{
protected:

public:
    //Initialise I2S config structure
    virtual void init_i2s();

    //load i2s driver and start it
    virtual esp_err_t init_i2s_driver();

    virtual esp_err_t uninstall_driver();

    virtual esp_err_t set_sample_channels();

    virtual esp_err_t clear_dma_buffer();

    virtual esp_err_t start();

    virtual esp_err_t stop();

    //init SSP stack with new approach
    virtual esp_err_t init_spp();
}