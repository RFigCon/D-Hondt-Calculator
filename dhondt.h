#ifndef DHONDT
#define DHONDT
#include <stdint.h>
#include <cstring>
#endif

#define NAME_CAPACITY 15

struct Party{
    const char name[NAME_CAPACITY] = {};
    double votes;
    double quoc;
    uint16_t seats;
};

void set_party( Party &party, const char * const name, const double votes );

void calc(Party* parties, uint8_t len, uint16_t total_seats );