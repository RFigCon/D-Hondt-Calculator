#include "dhondt.h"

#include <iostream>
#include <fstream>

#define FILE_NAME "input.txt"
#define MAX_PARTIES 25 

enum ErrorStatus{
    NONE,
    OPEN,
    READ,
    FILE_FORMAT,
    INVALID_NUMBER,
    INCOMPLETE
};

void handle_error(ErrorStatus status){
    switch (status) {
        case OPEN:
            std::cout << "Failed to open file." << std::endl;
            break;
        case READ:
            std::cout << "Failed to read file." << std::endl;
            break;
        case FILE_FORMAT:
            std::cout << "Badly format." << std::endl;
            break;
        case INVALID_NUMBER:
            std::cout << "Invalid number." << std::endl;
            break;
        case INCOMPLETE:
            std::cout << "Incomplete file." << std::endl;
            break;
        case NONE:
            //Should not be reachable
            break;
    }
}

ErrorStatus get_party(Party &p, std::ifstream &file){
    return NONE;
}

ErrorStatus fill_from_file(Party *parties, uint8_t &len, uint16_t &total_seats){
    std::ifstream file(FILE_NAME);

    if(!file.is_open()) return OPEN;

    len = 0;
    total_seats = 0;

    //-----------Read Total Seats-----------------------------------
    char c = file.get();
    while( c != '\n' && !file.eof()){
        if( c<'0' || c>'9' ) return INVALID_NUMBER;
        total_seats *= 10;
        total_seats += (c-'0');
        c = file.get();
    }
    if(file.eof()) return INCOMPLETE;

    //-----------Ignore Second Line For Formatting/Readibility Reasons----------
    c = file.get();
    while(c != '\n' && !file.eof()){
        c = file.get();
    }
    if(file.eof()) return INCOMPLETE;

    //-----------Read Party Name & Votes-----------------------------------
    c = file.get();
    ErrorStatus stat;
    while( !file.eof() && len < MAX_PARTIES ){
        stat = get_party(parties[len], file);
        if( stat != NONE ){
            return stat;
        }
        len++;
    }

    return NONE;
}

void show_results(const Party * const parties, const uint8_t len, const uint16_t total_seats){
    std::cout << "For a total of " << total_seats << " seats:"<< std::endl;
    for(uint8_t i = 0; i<len; i++){
        if(parties[i].seats == 0) continue;
        std::cout << "   -" << parties[i].name << " gets " << parties[i].seats << " seats."<< std::endl;
    }
}

int main(){
    
    uint8_t len;
    uint16_t total_seats;
    Party parties[MAX_PARTIES];

    ErrorStatus status = fill_from_file(parties, len, total_seats);

    if(status!=ErrorStatus::NONE){
        handle_error(status);
        return 1;
    }

    calc(parties, len, total_seats);

    show_results(parties, len, total_seats);
}