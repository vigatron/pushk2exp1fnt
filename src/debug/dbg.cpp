#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>

void dbg_nl() { std::cout << "\n"; }

void dbg_msg(const std::string & msg) {
    std::cout << msg; dbg_nl(); }

void dbg_i1t(const std::string & msg, int v1) {
    printf(msg.c_str(), v1); dbg_nl(); }

void dbg_i2t(const std::string & msg, int v1, int v2) {
    printf(msg.c_str(), v1, v2); dbg_nl(); }

void dbg_8x8(std::vector<uint8_t> arr) {
    printf("\n");
    for(int j=0;j<8;j++) {
        for(int i=0;i<8;i++) {
            uint8_t v = arr[j*8+i];
            printf("%c", v ? '*' : '.' ); }
        printf("\n"); } }