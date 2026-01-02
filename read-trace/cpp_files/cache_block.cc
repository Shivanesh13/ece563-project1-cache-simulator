#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;
#include "cache_block.h"

cache_block :: cache_block(uint32_t tag) {
    valid = false;
    dirty = false;
    this->tag = tag;
    counter = 0;
}

void cache_block :: set_tag(uint32_t tag) {
    this->tag = tag;
}

bool cache_block :: check_tag(uint32_t tag) {
    return (this->tag == tag );
}

uint32_t cache_block :: get_tag(){
    return this->tag;
}

void cache_block :: evict() {
    this->valid = false;
    this->dirty = false;
    this->tag = 0;
}

uint32_t cache_block :: get_counter(){ return counter; }

void cache_block :: set_counter(uint32_t c){ counter = c; }

void cache_block :: mark_clean(){ dirty = false; }

void cache_block :: mark_valid(){ valid = true; }

void cache_block :: mark_dirty() {
    this->dirty = true;
}

bool cache_block :: is_dirty() {
    return this->dirty;
}

bool cache_block :: is_valid() {
    return this->valid;
}

