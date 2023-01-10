set(a2dp "I2SInterface.cpp"
         "BluetoothA2DPSink.cpp" 
         "BluetoothA2DPSinkQueued.cpp" 
         "BluetoothA2DPSource.cpp" 
         "SoundData.cpp" 
         "BluetoothA2DPCommon.cpp")

idf_component_register(SRCS "I2SCommon.cpp" "${a2dp}"
         INCLUDE_DIRS "."
         REQUIRES bt esp_common freertos hal log nvs_flash driver esp_adc)

target_compile_options(${COMPONENT_LIB} PUBLIC "-Wno-unused-variable" "-Wno-missing-field-initializers" "-fpermissive" -std=gnu++2b)