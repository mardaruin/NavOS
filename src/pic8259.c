#include "hardwareio.h"
#include "pic8259.h"
                               
void setup8259(bool auto_eoi) {
    // Сначала запрещаем все устройства
    write_to_port(MASTER_DATA_PORT, 0xff);
    write_to_port(SLAVE_DATA_PORT, 0xff);

    // Отправляем первую команду ICW1
    u8 icw1_master = 0b10001;
    u8 icw1_slave = 0b10001;
    write_to_port(MASTER_COMMAND_PORT, icw1_master);
    write_to_port(SLAVE_COMMAND_PORT, icw1_slave);

    // Вторая команда ICW2 - задаём начало диапазона векторов
    write_to_port(MASTER_DATA_PORT, 0x20);  // Диапазон для Master начинается с 0x20
    write_to_port(SLAVE_DATA_PORT, 0x28);   // Диапазон для Slave начинается с 0x28

    // Третья команда ICW3 - настройка каскадов
    write_to_port(MASTER_DATA_PORT, 0b100); // Маска подключения Slave к Master
    write_to_port(SLAVE_DATA_PORT, 2);      // Slave подключён ко второму контакту Master

    // Четвёртая команда ICW4 - вторичные флаги
    u8 icw4 = auto_eoi ? 0b11 : 0b01;  // Автоматический EOI включен?
    write_to_port(MASTER_DATA_PORT, icw4);
    write_to_port(SLAVE_DATA_PORT, icw4);

    // Запрещаем все устройства снова
    write_to_port(MASTER_DATA_PORT, 0xff);
    write_to_port(SLAVE_DATA_PORT, 0xff);
}

// Посылаем сигнал EOI устройству
void send_eoi(u8 device) {
    if (device == MASTER_COMMAND_PORT || device == SLAVE_COMMAND_PORT) {
        write_to_port(device, 0x20); // Сигнал EOI
    }
}

// Получаем текущую маску устройств
u8 get_master_device_mask() {
    return ~read_from_port(MASTER_DATA_PORT);
}

u8 get_slave_device_mask() {
    return ~read_from_port(SLAVE_DATA_PORT);
}

// Устанавливаем маску устройств
void set_master_device_mask(u8 mask) {
    write_to_port(MASTER_DATA_PORT, ~mask);
}

void set_slave_device_mask(u8 mask) {
    write_to_port(SLAVE_DATA_PORT, ~mask);
}