/*
 * modbus_parser.c
 *
 *  Created on: 08-Aug-2026
 *      Author: shrey
 */

#include "modbus_parser.h"
#include "modbus_crc.h"
#include <string.h>

bool modbus_parse_request(const uint8_t *raw, uint16_t raw_len, ModbusRequest *out)
{
    // Minimum valid frame: addr(1) + func(1) + reg_addr(2) + value_or_count(2) + crc(2) = 8 bytes
    if (raw_len < 8) {
        return false;
    }

    // Verify CRC: last 2 bytes are CRC (low byte first, per Modbus spec)
    uint16_t received_crc = (uint16_t)raw[raw_len - 1] << 8 | raw[raw_len - 2];
    uint16_t calculated_crc = modbus_crc16(raw, raw_len - 2);

    // Modbus CRC is transmitted low-byte-first, so we need to compare carefully
    uint16_t calc_low = calculated_crc & 0xFF;
    uint16_t calc_high = (calculated_crc >> 8) & 0xFF;
    uint16_t recv_low = raw[raw_len - 2];
    uint16_t recv_high = raw[raw_len - 1];

    if (calc_low != recv_low || calc_high != recv_high) {
        return false;  // CRC mismatch, corrupted frame
    }

    out->slave_addr      = raw[0];
    out->function_code   = raw[1];
    out->register_addr   = ((uint16_t)raw[2] << 8) | raw[3];
    out->value_or_count  = ((uint16_t)raw[4] << 8) | raw[5];

    (void)received_crc; // silence unused variable warning, kept for clarity/debug

    return true;
}

uint16_t modbus_build_read_response(uint8_t slave_addr, const uint16_t *reg_values,
                                     uint8_t reg_count, uint8_t *out_buf)
{
    uint16_t idx = 0;
    out_buf[idx++] = slave_addr;
    out_buf[idx++] = MB_FUNC_READ_HOLDING_REG;
    out_buf[idx++] = reg_count * 2;  // byte count field

    for (uint8_t i = 0; i < reg_count; i++) {
        out_buf[idx++] = (reg_values[i] >> 8) & 0xFF;
        out_buf[idx++] = reg_values[i] & 0xFF;
    }

    uint16_t crc = modbus_crc16(out_buf, idx);
    out_buf[idx++] = crc & 0xFF;         // CRC low byte first
    out_buf[idx++] = (crc >> 8) & 0xFF;  // CRC high byte

    return idx;
}

uint16_t modbus_build_write_response(uint8_t slave_addr, uint16_t register_addr,
                                      uint16_t value, uint8_t *out_buf)
{
    uint16_t idx = 0;
    out_buf[idx++] = slave_addr;
    out_buf[idx++] = MB_FUNC_WRITE_SINGLE_REG;
    out_buf[idx++] = (register_addr >> 8) & 0xFF;
    out_buf[idx++] = register_addr & 0xFF;
    out_buf[idx++] = (value >> 8) & 0xFF;
    out_buf[idx++] = value & 0xFF;

    uint16_t crc = modbus_crc16(out_buf, idx);
    out_buf[idx++] = crc & 0xFF;
    out_buf[idx++] = (crc >> 8) & 0xFF;

    return idx;
}
