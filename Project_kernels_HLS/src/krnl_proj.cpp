#include "krnl_proj.h"
#include "patternsMINI.h"
#include <cstddef>
#include <iostream>
#include "hls_stream.h"
#include <ap_shift_reg.h>
#include <sys/types.h>

/*
bool match_char(char c, bool shift_regs[ASCII_LEN][MAX_PATTERN_LEN]) {

    // shift
    for (size_t i = 0; i < ASCII_LEN; i++) {
        for (size_t j = 0; j < MAX_PATTERN_LEN-1; j++) {
            shift_regs[i][j] = shift_regs[i][j+1];
        }

        shift_regs[i][MAX_PATTERN_LEN-1] = c == i;
    }

    // check patterns
    bool found = false;

    for (int p = 0; p < NUM_PATTERNS; p++) {
        bool ok = true;

        for (int j = 0; j < PATTERNS[p].length; j++) {

            uint8_t pc = PATTERNS[p].bytes[j];

            ok &= shift_regs[pc][j];

            
            if (!shift_regs[pc][j]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            //std::cout << "MATCH!\n";
            found = true;
        }
    }
    return found;
}

extern "C" {
void krnl_proj(hls::stream<pkt> &in_stream,
                hls::stream<pkt> &out_stream,
                ap_uint<32>     *final_match_count,
                unsigned int size)
{
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE m_axi     port=final_match_count offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=final_match_count bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    bool shift_regs[ASCII_LEN][MAX_PATTERN_LEN] = {0}; 
    //#pragma HLS ARRAY_PARTITION variable=shift_regs complete dim=1
    //#pragma HLS ARRAY_PARTITION variable=PATTERNS type=complete dim=1 // Test different dim?


    unsigned int match_count = 0;

    unsigned int beats = size / 64;   // 512 bits = 64 bytes

    pkt in;

    for (unsigned int i = 0; i < beats; i++) {
        // Read incoming packet
        in_stream.read(in);

        // Extract data from packet
        ap_uint<512> raw = in.data;

        // Unpack all 64 bytes
        for (int b = 0; b < 64; b++) {
            #pragma HLS PIPELINE II=1
            char c = raw.range((b+1)*8-1, b*8);
            bool m = match_char(c, shift_regs);
            if (m) match_count++;
        }
    }

    out_stream.write(in);

    final_match_count[0] = match_count;
}
}
*/

bool match_char(char c, char input_shift_reg[]) {
    // shift
    for (int i = 0; i < MAX_PATTERN_LEN - 1; i++) {
        input_shift_reg[i] = input_shift_reg[i+1];
    }
    input_shift_reg[MAX_PATTERN_LEN-1] = c;

    // compare with patterns
    for (size_t p = 0; p < NUM_PATTERNS; p++) {
        bool ok = true;
        for (int j = 0; j < PATTERNS[p].length; j++) {
            if (PATTERNS[p].bytes[j] != input_shift_reg[j]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            //std::cout << "MATCH!\n";
            return true;
        }
    }
    return false;
}


extern "C" {
void krnl_proj(hls::stream<pkt> &in_stream,
                hls::stream<pkt> &out_stream,
                ap_uint<32>     *final_match_count,
                unsigned int size)
{
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE m_axi     port=final_match_count offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=final_match_count bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    char input_shift_reg[MAX_PATTERN_LEN] = {0}; // Stores incoming characters

    unsigned int match_count = 0;

    unsigned int beats = size / 64;   // 512 bits = 64 bytes

    pkt in;

    for (unsigned int i = 0; i < beats; i++) {

        // Read incoming packet
        in_stream.read(in);

        // Extract data from packet
        ap_uint<512> raw = in.data;

        // Unpack all 64 bytes
        for (int b = 0; b < 64; b++) {
            #pragma HLS PIPELINE II=1
            
            char c = raw.range((b+1)*8-1, b*8);
            if (match_char(c, input_shift_reg)) {
                match_count++;
            }
        }
    }

    out_stream.write(in);

    final_match_count[0] = match_count;
}
}
