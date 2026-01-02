#include <stdlib.h>
#include <stdio.h>
using namespace std;
#include "cache_set.h"
#include "sim.h"

cache_set :: cache_set(uint32_t index, uint32_t num_sets) : index(index), num_sets(num_sets) {
    active_blocks = 0;
    for (uint32_t i = 0; i < num_sets; i++)
    {
        cache_block* new_block = new cache_block(0);
        new_block->set_counter(i);
        blocks.push_back(new_block);
    }
}

cache_set :: ~cache_set(){
    for(auto& block : blocks){
        delete block;
    }
}

bool cache_set :: check_tag(uint32_t tag) {
    for (auto block : blocks) {
        if (block->check_tag(tag) && block->is_valid()) {
            return true;
        }
    }
    return false;
}


void cache_set :: update_order(uint32_t counter){
    for(auto& block : blocks){
        if(block->get_counter() == counter){
            block->set_counter(0);
        } else if(block->get_counter() < counter){
            block->set_counter(block->get_counter()+1);
        }
    }
}

void cache_set :: mark_dirty(uint32_t tag){
    for(auto& block : blocks){
        if(block->check_tag(tag)){
            block->mark_dirty();
            return;
        }
    }
}

cache_block* cache_set :: get_block(uint32_t tag){
    for (auto block : blocks) {
        if (block->check_tag(tag)) {
            return block;
        }
    }
    return nullptr;
}

dirty_state cache_set :: add_line(uint32_t tag) {
    dirty_state temp;
    for(auto& block : blocks){
        if(block->get_counter() == num_sets-1){
            if(block->is_dirty() & block->is_valid()){
                temp.tag = block->get_tag();
                temp.is_dirty = true;
            } else {
                temp.is_dirty = false;
            }
            block->set_tag(tag);
            block->mark_clean();
            block->mark_valid();
            update_order(num_sets-1);
            return temp;
        }
    }
    return temp;
}


void cache_set::cache_print() {
    cout << "set" << setw(7) << index << ": ";
    for (uint32_t i = 0; i < blocks.size(); i++) {
        for (auto block : blocks) {
            if (block->is_valid() && block->get_counter() == i) {
                cout << hex << setw(8) << block->get_tag();
                if (block->is_dirty())
                    cout << " D"; 
                else
                    cout << "  "; 
            }
        }
    }
    cout << dec << endl; 
}



