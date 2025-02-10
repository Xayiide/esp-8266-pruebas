#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
#include "driver/gpio.h"

#include "veml7700.h"




#define LUX_FC_COEFFICIENT 0.092903
#define FC_LUX_COEFFICIENT 10.7639

#define SDA_GPIO 4
#define SCL_GPIO 5



void app_main()
{

    i2c_config_t           i2c_conf;
    struct veml7700_config veml_conf;                                                 
    veml7700_handle_t      handle;

    i2c_conf.mode             = I2C_MODE_MASTER;
    i2c_conf.sda_io_num       = SDA_GPIO;
    i2c_conf.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    i2c_conf.scl_io_num       = SCL_GPIO;
    i2c_conf.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    i2c_conf.clk_stretch_tick = 400;
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER);
    i2c_param_config(I2C_NUM_0, &i2c_conf);


    veml7700_initialize(&handle, I2C_NUM_0);
                                                                                
    veml_conf.gain             = 0x02; /* 1/8 */                                      
    veml_conf.integration_time = 0x0C; /* 800 MS */                                   
    veml_conf.persistance      = 0x00; /* pers mode 1 */                              
    veml_conf.interrupt_enable = 0; /* false */                                       
    veml_conf.shutdown         = 0x00; /* powersave mode 1 */                         
                                                                                
    ESP_ERROR_CHECK(veml7700_set_config(handle, &veml_conf));



    double lux_als, lux_white, fc_als, fc_white;

    while (1) {
        veml7700_read_als_lux(handle, &lux_als);
        fc_als = lux_als * LUX_FC_COEFFICIENT;

        veml7700_read_white_lux(handle, &lux_white);
        fc_white = lux_white * FC_LUX_COEFFICIENT;

        printf("[+] lux als:   %f\n", lux_als);
        printf("         fc:   %f\n", fc_als);

        printf("[+] white lux: %f\n", lux_white);
        printf("           fc: %f\n", fc_white);
        printf("\n ====== \n");


        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
