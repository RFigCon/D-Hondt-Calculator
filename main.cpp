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
            std::cout << "File is badly formated." << std::endl;
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

bool is_whitespace(char c){
    if(c == ' ' ||
       c == '\t' )
    {
        return true;
    }
    return false;
}

ErrorStatus get_party(Party &p, std::ifstream &file){

    char c = file.get();

    if(c == '\n') return NONE;

    char name[PARTY_NAME_CAPACITY];
    size_t name_len = 0;

    while ( !file.eof() && c!= '\n' && !is_whitespace(c) && name_len<PARTY_NAME_CAPACITY ) {

        name[name_len] = c;

        name_len++;
        c = file.get();
    }

    if( file.eof()  || c == '\n' || name_len == 0 ){
        return FILE_FORMAT;
    }

    if( name_len==PARTY_NAME_CAPACITY ){

        name[PARTY_NAME_CAPACITY-1] = '\0';

        while ( !file.eof() && c != '\n' && !is_whitespace(c) ) {
            c = file.get();
        }
    }else{
        name[name_len] = '\0';
    }

    if( file.eof() || c == '\n' ){
        return FILE_FORMAT;
    }

    while( is_whitespace(c) && !file.eof() ) {
        c = file.get();
    }
    if( file.eof() || c=='\n'){
        return FILE_FORMAT;
    }

    double number = 0.0; 

    //TODO: Aceitar floats (ex. "27.3" ou "22,8")
    while( !file.eof() && c != '\n' && !is_whitespace(c) ) {
        if(c=='_') goto next;
        if(c<'0' || c>'9') return INVALID_NUMBER;

        number *= 10;
        number += (double)(c-'0');

        next:
        c = file.get();
    }

    set_party(p, name, number);
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
    if(file.eof()){
        file.close();
        return INCOMPLETE;
    }

    //-----------Ignore Second Line For Formatting/Readibility Reasons----------
    c = file.get();
    while(c != '\n' && !file.eof()){
        c = file.get();
    }
    if(file.eof()){
        file.close();
        return INCOMPLETE;
    }

    //-----------Read Party Name & Votes-----------------------------------
    ErrorStatus stat;
    while( !file.eof() && len < MAX_PARTIES ){
        stat = get_party(parties[len], file);
        if( stat != NONE ){
            file.close();
            return stat;
        }
        len++;
    }

    file.close();
    return NONE;
}

void show_results(const Party * const parties, const uint8_t len, const uint16_t total_seats){
    std::cout << "For a total of " << total_seats << " seats:"<< std::endl;
    for(uint8_t i = 0; i<len; i++){
        if(parties[i].seats == 0) continue;
        std::cout << "   -" << parties[i].name << " gets " << parties[i].seats;
        
        if(parties[i].seats == 1) std::cout << " seat."<< std::endl;
        else std::cout << " seats."<< std::endl;
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