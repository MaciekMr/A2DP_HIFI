set(srcs "main.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES bt esp_common freertos hal log nvs_flash driver esp_adc)

target_compile_options(${COMPONENT_LIB} PUBLIC "-Wno-unused-variable" "-Wno-missing-field-initializers" "-fpermissive" -std=gnu++2b)
