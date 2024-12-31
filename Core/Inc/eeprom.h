/*
 * eeprom.h
 *
 *  Created on: Dec 23, 2024
 *      Author: Admin
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"

#define  I2C_ADDRESS    (0x53<<1)

//page size and number of pages
#define PAGE_SIZE 4 // in bytes
#define PAGE_NUM 256 // pages

uint16_t bytes_to_write(uint16_t size);
void EEPROM_Read(uint16_t page, uint8_t *data, uint16_t size);
void EEPROM_Write(uint16_t page, uint8_t *data, uint16_t size);
void EEPROM_PageErase (uint16_t page);

#endif /* INC_EEPROM_H_ */
