/*
 * keyboard.h
 *
 *  Created on: 2022年5月20日
 *      Author: admin
 */

#ifndef CODE_KEYBOARD_H_
#define CODE_KEYBOARD_H_

//定义按键引脚
#define KEY1    P20_6
#define KEY2    P20_7
#define KEY3    P11_2
#define KEY4    P11_3

//定义拨码开关引脚
#define SW1     P33_11
#define SW2     P33_12


#define EXAMPLE_EEPROM_SECTOR        (11)
extern uint8 image_copy[MT9V03X_H][MT9V03X_W];
extern int Ok;



void prompt(void);
void keyboard_init(void);

void  key_operate(void);
void  key1_function(void);
void  key2_function(void);
void  key3_function(void);
void  key4_function(void);
void interface_display(void);
void dispage2(void);

void dispage3(void);
void dispage4(void);
void number1(void);
void number2(void);
void number3(void);
void flash_read(void);
void flash_deal(void);

void fenglingqi_use(void);
void fengmingqi(void);
void wifi_spi(void);


#endif /* CODE_KEYBOARD_H_ */
