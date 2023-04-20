/**
 * A simple Raspberry Pi Pico program to display the onboard temperature sensor value on
 * SSD1306 OLED Display using I2C.
 * 
 */

#include<stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "i2cSend/i2cSend.h"
#include "digitRenderer/digitRenderer.h"

symbol scale=DEGREE_CELSIUS;
/*Display Temperature Value on SSD1306*/
/*Temperature value parsing is specific to Raspberry Pi Pico Device.If on any other device, check the documentation and make changes accordingly.*/
void parseAndDisplayTemp(float value){
    int x=4,y=80;
    renderSymbol(NULLSYM,x,96); //Bugfix: clear unit symbol if it is shifted to right.
    int temp=(int)value; //round up to integer value. To display actual flaoting point value, you can add a "." symbol and parse accordingly :)
    bool negative=false;
    if(temp<0){
        negative=true;
        temp*=-1;
    }
    if(temp>=100){
        y=96;
    }
    renderSymbol(scale,x,y);
    while(temp>0){
        y-=16;
        renderDigit(temp%10,x,y);
        temp/=10;
    }
    if(negative){
        y-=16;
        renderSymbol(MINUS,x,y);
    }

    while(y>=0){    //Clear Stale higher significance digits if number of digits decrease.
        y-=16;
        renderSymbol(NULLSYM,x,y);
    }
}

int main() {
    stdio_init_all(); //Initialize stdio for printf

    adc_init(); //ADC initialization
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); //Select input for temperature sensor.
    sleep_ms(200);
    i2c_init(i2c_default, 400 * 1000); // Initialize I2C with clock fequency 400kHz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    sleep_ms(200);
    initDisplay();
    renderSymbol(RPI,1,56); // Render the Rpi Symbol
    while(1){
        const float convFactor = 3.3f / (1 << 12);
        float adc = (float)adc_read() * convFactor;
        float temp = 27.0f - (adc - 0.706f) / 0.001721f;
        // printf("Temp:%f",temp);
        parseAndDisplayTemp(temp);
        sleep_ms(1000);
    }

    return 0;
}
