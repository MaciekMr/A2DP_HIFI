#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "driver/i2s.h"
#include "esp_spp_api.h"
#include "BluetoothA2DPSink32.h"


extern "C" {
	void app_main(void);
}


BluetoothA2DPSink32 a2dp_sink; // Subclass of BluetoothA2DPSink

void setup() {
  a2dp_sink.set_bits_per_sample(32);  
  a2dp_sink.start("Hifi32bit");  
}




void app_main(void)
{
  setup();
}