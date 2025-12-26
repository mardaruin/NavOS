#ifndef VARGS_H
#define VARGS_H

typedef struct {
    void* current_position;
} va_list;

#define va_start(ap, last_arg) ap.current_position = &(last_arg)+sizeof(last_arg)
#define va_arg(ap, type) *(type*)(ap.current_position+=sizeof(type)-sizeof(type))
#define va_copy(dest, src) dest.current_position = src.current_position
#define va_end(ap) (void)(ap.current_position=NULL)

#endif