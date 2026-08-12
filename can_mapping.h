/*
 * can_mapping.h
 *
 *  Created on: 08-Aug-2026
 *      Author: shrey
 */

#ifndef CAN_MAPPING_H
#define CAN_MAPPING_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t modbus_register;   // Modbus holding register address
    uint32_t can_id;            // corresponding CAN identifier
    const char *description;    // human-readable label, for debug logging
} RegisterCanMapping;

// Looks up the CAN ID for a given Modbus register.
// Returns true if found, and writes the CAN ID into *out_can_id.
bool can_mapping_lookup(uint16_t modbus_register, uint32_t *out_can_id);

// Reverse lookup: given a CAN ID, find the Modbus register it maps to.
// Used when a CAN response arrives and needs to be sent back as Modbus.
bool can_mapping_reverse_lookup(uint32_t can_id, uint16_t *out_modbus_register);

#endif
