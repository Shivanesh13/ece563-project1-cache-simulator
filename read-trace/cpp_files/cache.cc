#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;
#include "sim.h"
#include "cache.h"
#include <bits/stdc++.h>

cache :: cache (cache_config_t input_params, cache_config_t nxt_level_params = {0,0,0}) {
    config.INDEX = input_params.SIZE / (input_params.BLOCKSIZE * input_params.ASSOC);
    config.BLOCKSIZE = input_params.BLOCKSIZE;
    config.ASSOC = input_params.ASSOC;    
    config.SIZE = input_params.SIZE;
    config.PREF_M = input_params.PREF_M;
    config.PREF_N = input_params.PREF_N;
    cache_stats.reads = 0;
    cache_stats.read_misses = 0;
    cache_stats.writes = 0;
    cache_stats.writebacks = 0;
    cache_stats.prefetch = 0;

    bit_info.INDEX_BITS = log2(config.INDEX);
    bit_info.OFFSET_BITS = log2(config.BLOCKSIZE);
    bit_info.TAG_BITS = ADDRESS_LENGTH - bit_info.INDEX_BITS - bit_info.OFFSET_BITS;

    if(nxt_level_params.SIZE >= input_params.SIZE){
        nxt_cache = new cache(nxt_level_params);
        nxt_cache->cache_level = this->cache_level + 1;
    }

    for(uint32_t i =0;i< config.INDEX;i++){
        sets[i] = new cache_set(i,config.ASSOC);
    }

    if(nxt_cache == nullptr && config.PREF_M > 0 && config.PREF_N > 0){
        stream_buff = new stream_vector(config.PREF_N,config.PREF_M);
    }
} 

void cache :: cache_write(uint32_t addr){
    uint32_t index = (addr >> bit_info.OFFSET_BITS) & ((1 << bit_info.INDEX_BITS) - 1);
    uint32_t tag = addr >> (bit_info.OFFSET_BITS + bit_info.INDEX_BITS);

    cache_stats.writes++;
    
    if(sets.find(index) != sets.end() && sets[index]->check_tag(tag)){
            cache_block* tempblock = sets[index]->get_block(tag);
            sets[index]->update_order(tempblock->get_counter());
            tempblock->mark_dirty();
            if(stream_buff != nullptr)
                stream_buff->check_tag(((tag << bit_info.INDEX_BITS) | (index)),true);
    } else {
            cache_stats.write_misses++;

            dirty_state temp = sets[index]->add_line(tag);
            if(temp.is_dirty){
                cache_stats.writebacks++;
                if(nxt_cache != nullptr)
                    nxt_cache->cache_write(((temp.tag << (bit_info.OFFSET_BITS + bit_info.INDEX_BITS)) | (index << bit_info.OFFSET_BITS)));
            }
            
            if(stream_buff != nullptr){
                if(stream_buff->check_tag(((tag << bit_info.INDEX_BITS) | (index)),false)){
                    cache_stats.write_misses--;
                    cache_stats.prefetch++;
                }
            }
            else if(nxt_cache != nullptr)
                nxt_cache->cache_read(addr);
            
            sets[index]->mark_dirty(tag);
        }
}

void cache :: cache_print(){
    cout << "\n===== L"<<cache_level<< " contents =====" << endl;
    for(uint32_t i = 0;i < sets.size();i++){
        sets[i]->cache_print();
    }

    if(nxt_cache != nullptr)
        nxt_cache->cache_print();

}

void cache :: stream_print(){
        if(nxt_cache == nullptr){
            if(stream_buff != nullptr){
                stream_buff->stream_print();
            }
        } else {
            nxt_cache->stream_print();
        }
}

void cache :: print_stats(){
    cout << "\n===== Measurements ===== " << endl;
    cout << "a. L" << cache_level << " read:              \t" << cache_stats.reads <<endl;
    cout << "b. L" << cache_level << " read misses:       \t" << cache_stats.read_misses << endl;
    cout << "c. L" << cache_level << " writes:            \t" << cache_stats.writes << endl;
    cout << "d. L" << cache_level << " write misses:      \t" << cache_stats.write_misses << endl;
    cout << "e. L" << cache_level << " " << endl;
    cout << "f. L" << cache_level << " writebacks:        \t" << cache_stats.writebacks << endl;
    cout << "f. L" << cache_level << " prefetches:        \t" << cache_stats.prefetch << endl;

    if(nxt_cache != nullptr)
        nxt_cache->print_stats();
}


void cache :: cache_read(uint32_t addr){
    uint32_t index = (addr >> bit_info.OFFSET_BITS) & ((1 << bit_info.INDEX_BITS) - 1);
    uint32_t tag = addr >> (bit_info.OFFSET_BITS + bit_info.INDEX_BITS);

    cache_stats.reads++;
    if(sets.find(index) != sets.end() && sets[index]->check_tag(tag)){
            cache_block* tempblock = sets[index]->get_block(tag);
            sets[index]->update_order(tempblock->get_counter());
            if(stream_buff != nullptr)
                stream_buff->check_tag(((tag << bit_info.INDEX_BITS) | (index)),true);
        } else {

            dirty_state temp = sets[index]->add_line(tag);
            if(temp.is_dirty){
                cache_stats.writebacks++;
                if(nxt_cache != nullptr)
                    nxt_cache->cache_write(((temp.tag << (bit_info.OFFSET_BITS + bit_info.INDEX_BITS)) | (index << bit_info.OFFSET_BITS)));
            }
            
            if(stream_buff != nullptr){
                if(stream_buff->check_tag(((tag << bit_info.INDEX_BITS) | (index)),false)){
                    cache_stats.read_misses--;
                    cache_stats.prefetch++;
                }
            }
            else if(nxt_cache != nullptr)
                nxt_cache->cache_read(addr);

            cache_stats.read_misses++;
        }
}

cache :: ~cache(){
    for(auto& set : sets){
        delete set.second;
    }
    delete nxt_cache;
    delete stream_buff;
}