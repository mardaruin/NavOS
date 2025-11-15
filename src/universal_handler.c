#include "types.h"
#include "universal_handler.h"
#include "printer.h"
#include "panic.h"

void universal_handler(context* ctx) {
	kernel_panic("unhandled interrupt: ");
}                   
