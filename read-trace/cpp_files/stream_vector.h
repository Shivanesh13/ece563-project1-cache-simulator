#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <inttypes.h>
#include "stream_buffer.h"
using namespace std;


class stream_vector{
    vector<stream_buffer*> vec;
    uint32_t total_stream, stream_size;
    uint32_t prefetch = 0;

    public:
        stream_vector(uint32_t , uint32_t);
        ~stream_vector();
        bool check_tag(uint32_t , bool);
        void update_counters(uint32_t );
        void stream_print();
        uint32_t get_prefetch();
};