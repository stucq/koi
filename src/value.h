#ifndef VALUE_H
#define VALUE_H

typedef union {
	int ival;
} InternalValue;

InternalValue ival(int i);

#endif
