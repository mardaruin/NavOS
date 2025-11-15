#ifndef HARDWARE_IO_H
#define HARDWARE_IO_H

#include "types.h"

// Макросы для записи и чтения из портов
#define WRITE_TO_PORT_RAW(port, val)                     \
    __asm__ volatile(                                   \
        "mov al, %0\n"                                  \
        "mov dx, %1\n"                                  \
        "out dx, al\n"                                  \
        :                                               \
        : "r"(val), "r"(port)                           \
        : "al", "dx")

#define READ_FROM_PORT(port) ({                          \
    u8 result;                                          \
    __asm__ volatile(                                   \
        "mov dx, %1\n"                                  \
        "in al, dx\n"                                   \
        "mov %0, al\n"                                  \
        : "=r"(result)                                  \
        : "r"(port)                                     \
        : "al", "dx");                                  \
    result;                                             \
})

// Задержка для синхронизации
inline void delay() {
    for (int i = 0; i < 1000; ++i) {
        WRITE_TO_PORT_RAW(0x80, 0);
    }
}

// Функции записи/чтения с задержкой
inline void write_to_port(u16 port, u8 payload) {
    WRITE_TO_PORT_RAW(port, payload);
    delay();
}

inline u8 read_from_port(u16 port) {
    return READ_FROM_PORT(port);
}

#endif