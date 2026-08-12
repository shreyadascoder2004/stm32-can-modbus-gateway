/*
 * can_mapping.c
 *
 *  Created on: 08-Aug-2026
 *      Author: shrey
 */

#include "can_mapping.h"
#include <string.h>

// This table is the heart of the gateway: edit this to add more sensors/actuators.
// Example scenario: register 0 = simulated temperature sensor node,
//                    register 1 = simulated motor RPM node,
//                    register 2 = actuator setpoint (write-only in practice)
static const RegisterCanMapping mapping_table[] = {
    { .modbus_register = 0, .can_id = 0x100, .description = "Temperature sensor node" },
    { .modbus_register = 1, .can_id = 0x101, .description = "Motor RPM node" },
    { .modbus_register = 2, .can_id = 0x102, .description = "Actuator setpoint" },
};

static const uint8_t mapping_table_size = sizeof(mapping_table) / sizeof(mapping_table[0]);

bool can_mapping_lookup(uint16_t modbus_register, uint32_t *out_can_id)
{
    for (uint8_t i = 0; i < mapping_table_size; i++) {
        if (mapping_table[i].modbus_register == modbus_register) {
            *out_can_id = mapping_table[i].can_id;
            return true;
        }
    }
    return false;  // register not in table, caller should respond with Modbus exception
}

bool can_mapping_reverse_lookup(uint32_t can_id, uint16_t *out_modbus_register)
{
    for (uint8_t i = 0; i < mapping_table_size; i++) {
        if (mapping_table[i].can_id == can_id) {
            *out_modbus_register = mapping_table[i].modbus_register;
            return true;
        }
    }
    return false;
}
