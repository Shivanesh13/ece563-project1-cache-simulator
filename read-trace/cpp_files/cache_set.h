#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <inttypes.h>
#include "cache_block.h"
#include "sim.h"
using namespace std;

class cache_set{
  uint32_t index;
  uint32_t num_sets;
  uint32_t active_blocks;

  vector<cache_block*> blocks;
  public:
  cache_set(uint32_t , uint32_t);
  ~cache_set();
  bool check_tag(uint32_t);
  cache_block * get_block(uint32_t);
  cache_block* check_set(uint32_t);
  dirty_state add_line(uint32_t );  
  void update_order(uint32_t);
  void mark_dirty(uint32_t);
  void cache_print();
};