# STM32 CAN–Modbus Industrial Gateway

A 4-layer STM32-based industrial protocol gateway that bridges RS-485/Modbus-RTU
networks with CAN bus networks — designed end-to-end from schematic to PCB layout
in KiCad, with C firmware handling real-time protocol translation.

## Why this exists

Industrial factory floors often run a mix of legacy RS-485/Modbus-RTU equipment
(sensors, PLCs, HMIs) and newer CAN-based nodes (motor controllers, automotive-grade
modules). This gateway lets a Modbus master (PC, PLC, SCADA system) read and write
values on CAN-based devices without either side needing to natively support the
other's protocol — the same role commercial gateways (e.g. Moxa, Advantech) fill
in real deployments.

## How it works

1. A Modbus master sends a request (Read Holding Registers / Write Single Register)
   over RS-485
2. The STM32 receives it via UART, validates the frame (CRC16), and parses the
   register address and value
3. A lookup table maps the Modbus register to a CAN ID
4. The STM32 sends the corresponding CAN frame onto the bus
5. A CAN node responds; the STM32 receives it, maps it back to a Modbus register,
   and replies to the master over RS-485

USB is reserved for local configuration/debug access.

## Hardware

- **MCU**: STM32F407VET6 (LQFP100)
- **Power**: 24V DC input → TPS54233 buck (5V) → AP1117-33 LDO (3.3V)
- **RS-485**: MAX485E transceiver, jumper-selectable termination, fail-safe biasing
- **CAN**: SN65HVD230 transceiver, jumper-selectable 120Ω termination
- **Board**: 4-layer stackup (Signal / GND plane / 3V3 plane / Signal), designed
  in KiCad — schematic, footprint assignment, and layout completed with dedicated
  ground and power planes

See `/hardware` for the full KiCad project.

## Firmware

Written in C using the STM32 HAL, structured so the protocol logic is
hardware-independent and separately testable:

- `modbus_crc.c` — Modbus CRC16 checksum
- `modbus_parser.c` — Modbus RTU frame parsing and response building
- `can_mapping.c` — Modbus register ↔ CAN ID lookup table
- `main.c` — HAL integration: UART receive interrupt, CAN filter/notification
  setup, and the gateway's main translation loop

Clock tree: 8MHz HSE crystal → PLL → 168MHz SYSCLK, with CAN bus timing configured
for 500kbps on the 42MHz APB1 peripheral clock.

See `/firmware` for the full STM32CubeIDE project.

## Status

- Schematic: complete, ERC-clean
- PCB layout: components placed, 4-layer stackup and plane pours complete,
  routing in progress
- Firmware: compiles cleanly against the current pin mapping; logic modules
  (CRC, frame parsing, ID mapping) are hardware-independent and unit-testable
  on a PC
- Hardware validation: pending PCB fabrication and assembly — no physical
  board has been tested yet

## Build instructions

**Hardware**: open `hardware/*.kicad_pro` in KiCad 8+

**Firmware**: open `firmware/` as an existing project in STM32CubeIDE,
or import via *File → Open Projects from File System*

## License

MIT
