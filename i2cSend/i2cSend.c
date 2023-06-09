/*
* This utility wraps the pico I2C hardware API to send commands and data to 
* the SSD1306 128x64 OLED Display module.
*/
#include "hardware/i2c.h"
#include "i2cSend.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//Send command to SSD1306
int sendCommand(unsigned char command){
    unsigned char buf[2];
    buf[0]=0x00;
    buf[1]=command;
    return i2c_write_blocking(i2c_default,0x3c,buf,2,false);
}

//Send a byte of data to SSD1306
int sendDataByte(unsigned char byte){
    unsigned char buf[2];
    buf[0]=0x40;
    buf[1]=byte;
    return i2c_write_blocking(i2c_default,0x3c,buf,2,false);
}

//Send a block of data to SSD1306
//Memory pointed by the block argument is to be freed by the callee.
int sendDataBlock(unsigned char *block,const unsigned int size){
    unsigned char *buf =malloc(size*(sizeof(unsigned char))+1);
    int ret;
    buf[0]=0x40;
    memcpy(buf+1,block,size);
    ret=i2c_write_blocking(i2c_default,0x3c,buf,size+1,false);
    free(buf);
    return ret;
}