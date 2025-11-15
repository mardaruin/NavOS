#include "experiments.h"
#include "pic8259.h"
#include "interrupt_handlers.h"
#include "idt.h"

// Эксперимент №1: минимальный тест
void run_experiment_1() {
    // Установка типа дескрипторов
    setup_idt(Gate_Interrupt);
    // Настройка PIC8259
    setup8259(false); // Ручной EOI
    // Нет подключенных устройств
    set_master_device_mask(DEVICE_MASK_NONE);
    set_slave_device_mask(DEVICE_MASK_NONE);

    // Включаем обработку прерываний
    sti();
    // Бесконечный цикл
    while(true);
}

// Эксперимент №2: обработка таймера
void run_experiment_2() {
    // Установка типа дескрипторов
    setup_idt(Gate_Interrupt);
    // Настройка PIC8259
    setup8259(false); // Ручной EOI
    // Разрешаем таймер
    set_master_device_mask(DEVICE_MASK_TIMER);
    set_slave_device_mask(DEVICE_MASK_NONE);

    // Регистрируем обработчик таймера
    register_interrupt_handler(SYSTEM_TIMER, timer_handler);

    // Включаем обработку прерываний
    sti();
    // Бесконечный цикл
    while(true);
}
