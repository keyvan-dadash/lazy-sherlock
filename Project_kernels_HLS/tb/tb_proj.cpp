#include "krnl_proj.h"
#include <iostream>


int main(void) {

    hls::stream<pkt> in_stream;
    hls::stream<pkt> out_stream;


    ap_uint<32> *m_c = new ap_uint<32>();

    pkt test_pkt;
    test_pkt.data = 0x2e776d7a;
    in_stream.write(test_pkt);



    for (int i = 0; i < 1024; i++) {
        pkt p;
        ap_uint<512> data = 0x0;
        p.data = data;
        in_stream.write(p);
    }

    ap_uint<32> *out;
    
    krnl_proj(in_stream, out_stream, m_c, 1024);

    std::cout << "Test bench finished!\n";
    return 0;
}