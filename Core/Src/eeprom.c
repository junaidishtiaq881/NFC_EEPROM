/*
 * eeprom.c
 *
 *  Created on: Dec 23, 2024
 *      Author: Admin
 */
#include "eeprom.h"
#include "main.h"
#include "math.h"
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
// Functions definitions

struct EEPROM
{
	int paddrposition;
	uint16_t start_page;
	uint16_t end_page;
	uint16_t num_of_pages;
	uint16_t position;

};
// function to determine the remaining bytes
uint16_t bytes_to_write(uint16_t size)
{
	if((size ) < PAGE_SIZE)
	{
		return size;
	}else
	{
		return (PAGE_SIZE);
	}
}

void EEPROM_Read(uint16_t page, uint8_t *data, uint16_t size)
{
	struct EEPROM Read;
	// finding out the number of bits where page addressing starts
	Read.paddrposition = log2(PAGE_SIZE);

	// calculating the start and end of the page
	Read.start_page = page;
	Read.end_page = page + (size)/PAGE_SIZE;

	// number of pages to be written
	Read.num_of_pages = (Read.end_page - Read.start_page) + 1; // number of pages to be written
	Read.position = 0;

	// writing the data
	for(int i = 0; i < Read.num_of_pages; i++)
	{
		/*calculating the address of the memory location
		* Here we add the page address with the byte address
		*/
		uint16_t memaddr = Read.start_page << Read.paddrposition;
		uint16_t bytes_rem = bytes_to_write(size); // calculating the remaining bytes to be written

		HAL_I2C_Mem_Read(&hi2c1, I2C_ADDRESS, memaddr, 2, &data[Read.position], bytes_rem, 1000); // write the data to the device
		Read.start_page = Read.start_page + 1; // incrementing the page so that a new page address can be selected for further write

		size = size - bytes_rem; //reducing the size of the bytes
		Read.position = bytes_rem; // update the position for the data buffer
	}
}

void EEPROM_Write(uint16_t page, uint8_t *data, uint16_t size)
{
	struct EEPROM Write;
	// finding out the number of bits where page addressing starts
	Write.paddrposition = log2(PAGE_SIZE);

	// calculating the start and end of the page
	Write.start_page = page;
	Write.end_page = page + ((size)/PAGE_SIZE);

	// number of pages to be written
	Write.num_of_pages = (Write.end_page - Write.start_page) + 1;
	Write.position  = 0;

	// writing the data
	for (int i = 0; i < Write.num_of_pages; i++)
	{
		/* calculating the address of the memory location
		 * Here we add the page address with the byte address
		 */
		uint16_t memaddr = Write.start_page << Write.paddrposition;
		uint16_t bytes_rem = bytes_to_write(size); // calculating the remaining bytes to be written

		HAL_I2C_Mem_Write(&hi2c1, I2C_ADDRESS, memaddr, 2, &data[Write.position], bytes_rem, 1000); // write the data to the device

		Write.start_page = Write.start_page + 1; // incrementing the page so that a new page address can be selected for further write
		 // It will be 0 because we will be writing to a new page
		size = size - bytes_rem; //reducing the size of the bytes
		Write.position = bytes_rem; // update the position for the data buffer

		HAL_Delay(5);
	}
}
void EEPROM_PageErase (uint16_t page)
{
	struct EEPROM Erase;
	// calculating mem address based on the page number
	Erase.paddrposition = log2(PAGE_SIZE);
	uint16_t memaddr = page<<Erase.paddrposition;

	// creating buffer to store the reset values
	uint8_t data[PAGE_SIZE];
	memset(data, 0xff, PAGE_SIZE);

	// writing data to the EEPROM
	HAL_I2C_Mem_Write(&hi2c1, I2C_ADDRESS, memaddr, 2, data, PAGE_SIZE, 1000);

	HAL_Delay(5);

}

