#ifndef DHONDT
#define DHONDT
#include <stdint.h>
#include <cstring>
#endif

#define PARTY_NAME_CAPACITY (15+1)

struct Party{
    const char name[PARTY_NAME_CAPACITY] = {};
    double votes;
    double quoc;
    uint16_t seats;
};

void set_party( Party &party, const char name[PARTY_NAME_CAPACITY], const double votes );

void calc(Party* parties, uint8_t len, uint16_t total_seats );