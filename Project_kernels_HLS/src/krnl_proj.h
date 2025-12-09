#include "hls_task.h"
#include <hls_stream.h>
#include "ap_axi_sdata.h"


typedef ap_axiu<512,1,1,16> pkt;
#define ASCII_LEN 0x7f

extern "C" {
    void krnl_proj(hls::stream<pkt> &in_stream, hls::stream<pkt> &out_stream, ap_uint<32> *final_match_count, unsigned int size);
}