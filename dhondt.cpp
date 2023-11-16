#include "dhondt.h"

void set_party( Party &party, const char name[PARTY_NAME_CAPACITY], const double votes ){
    std::memcpy( (void *)party.name, name, PARTY_NAME_CAPACITY);
    party.votes = votes;
    party.quoc = votes;
    party.seats = 0;
}

void calc(Party* parties, uint8_t len, uint16_t total_seats ){

    for(; total_seats>0; total_seats--){
        double larg_val = 0.0;
        uint8_t larg_idx = 0;
        for(uint8_t i = 0; i<len; i++){

            if( parties[i].quoc>larg_val ||
                (parties[i].quoc==larg_val && parties[i].seats>parties[larg_idx].seats) ){

                larg_idx = i;
                larg_val = parties[i].quoc;
            }

        }

        parties[larg_idx].seats++;
        parties[larg_idx].quoc = parties[larg_idx].votes / (double)(parties[larg_idx].seats + 1);
    }

}