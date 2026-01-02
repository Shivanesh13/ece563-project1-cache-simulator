#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "cache_set.h"
#include "sim.h"
#include "stream_vector.h"
using namespace std;

class cache{
    private:
    unordered_map<uint32_t, cache_set*> sets;
    cache_bit_info_t bit_info;
    
    public:
    cache_config_t config;
    cache_stats_t cache_stats;
    //cache(cache_config_t);
    int cache_level = 1;
    cache* nxt_cache = nullptr;
    stream_vector* stream_buff = nullptr;
    cache(cache_config_t, cache_config_t);
    ~cache();
    void cache_read(uint32_t);
    void cache_write(uint32_t);
    void cache_print();
    void print_stats();
    void stream_print();
};  