/*
 * modbus_parser.h
 *
 *  Created on: 08-Aug-2026
 *      Author: shrey
 */

#ifndef MODBUS_PARSER_H
#define MODBUS_PARSER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MB_FUNC_READ_HOLDING_REG = 0x03,
    MB_FUNC_WRITE_SINGLE_REG = 0x06
} ModbusFunctionCode;

typedef struct {
    uint8_t  slave_addr;
    uint8_t  function_code;
    uint16_t register_addr;
    uint16_t value_or_count;  // for read: quantity of registers, for write: value to write
} ModbusRequest;

// Parses a raw Modbus RTU frame. Returns true if valid (CRC ok, length ok).
bool modbus_parse_request(const uint8_t *raw, uint16_t raw_len, ModbusRequest *out);

// Builds a Modbus RTU response frame for a Read Holding Registers reply.
// reg_values: array of register values to send back
// reg_count: how many registers
// out_buf: buffer to write the response into (caller must provide enough space)
// returns number of bytes written into out_buf
uint16_t modbus_build_read_response(uint8_t slave_addr, const uint16_t *reg_values,
                                     uint8_t reg_count, uint8_t *out_buf);

// Builds a Modbus RTU response frame for a Write Single Register reply (echo back).
uint16_t modbus_build_write_response(uint8_t slave_addr, uint16_t register_addr,
                                      uint16_t value, uint8_t *out_buf);

#endif
