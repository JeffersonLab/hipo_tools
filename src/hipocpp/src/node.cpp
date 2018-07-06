/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo/node.h"

namespace hipo {

  generic_node::generic_node(){

  }

  generic_node::~generic_node(){

  }

  void generic_node::group(int group){ __group_id = group;}
  int  generic_node::group(){ return __group_id;}

  void generic_node::type(int type){ __type = type;}
  int  generic_node::type(){ return __type;}

  void generic_node::item(int item){ __item_id = item;}
  int  generic_node::item(){ return __item_id;}

  void generic_node::name(const char *name) { __node_name = name;}
  const char *generic_node::name(){ return __node_name.c_str();}

  void generic_node::length(int length) { __length = length;}
  int  generic_node::length() const { return __length;}

  template class node<int8_t>;
  template class node<int16_t>;
  template class node<int32_t>;
  template class node<int64_t>;
  template class node<float>;
  template class node<double>;

}
