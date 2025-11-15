#ifndef PIC8259_H
#define PIC8259_H

#include "types.h"

// Порты контроллеров
#define MASTER_COMMAND_PORT 0x20
#define MASTER_DATA_PORT 0x21
#define SLAVE_COMMAND_PORT 0xA0
#define SLAVE_DATA_PORT 0xA1

// Константы масок устройств
#define DEVICE_MASK_NONE 0xFF
#define DEVICE_MASK_TIMER 0xFE
#define DEVICE_MASK_KEYBOARD 0xFD
#define DEVICE_MASK_BOTH (DEVICE_MASK_TIMER & DEVICE_MASK_KEYBOARD)

// Интерфейс функций PIC8259
void setup8259(bool auto_eoi);
void send_eoi(u8 device);
u8 get_master_device_mask();
u8 get_slave_device_mask();
void set_master_device_mask(u8 mask);
void set_slave_device_mask(u8 mask);

#endif