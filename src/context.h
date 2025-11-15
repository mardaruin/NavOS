#indef CONTEXT_H
#define CONTEXT_H

#include "types.h"

// контекст хранит состояние цпу при входе в обработчик
struct context {
	uint32_t eax, ebx, ecx, edx;  // общие регистры
	uint32_t esi, edi, ebp, esp;  // индексные и базовые регистры
	uint32_t cs, ds, fs, gs;      // сегментные регистры
	uint32_t eflags;              // флаги состояния
	uint32_t vector;              // номер вектора прерывания
	uint32_t error_code;          // код ошибки (если есть)
};

#endif