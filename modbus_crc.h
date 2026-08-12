/*
 * modbus_crc.h
 *
 *  Created on: 08-Aug-2026
 *      Author: shreya
 */
#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include <stdint.h>
#include <stddef.h>

uint16_t modbus_crc16(const uint8_t *buf, uint16_t len);

#endif
