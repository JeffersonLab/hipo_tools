/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   node.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <cstdio>

//#include "event.h"
//#include "reader.h"

namespace hipo {

class generic_node {
  private:
    int __group_id;
    int __item_id;
    int __type;
    int __length;
    std::string __node_name;

  public:
    generic_node();
    virtual ~generic_node();

    virtual void setAddress(char *address){}

    int  type();
    int  length();
    int  group();
    int  item();
    const char *name();

    void name(const char *name);
    void group(int group);
    void item(int item);
    void type(int type);
    void length(int length);
};

template <class T>
class node : public generic_node {

    private:
        T   *ptr;
    public:

        node();
        //node(int group, int item,hipo::event &event);
        node(int __group, int __item);
        //node(hipo::reader &reader, int group, int item);

        virtual ~node();

        T getValue(int index);
        //void setNode(int group, int item, hipo::event &event);
        void   reset();
        void   show();
        int    getLength();
        char  *getAddress();
        int    getBytesLength();
        void   setLength(int l);
        void   setAddress(char *address);
};

}

namespace hipo {

    template <class T> node<T>::node(){

    }

    template <class T> node<T>::node(int __group, int __item){
      group(__group);
      item(__item);
    }

  /*  template <class T> node<T>::node(int group, int item,hipo::event &event){
      int address = event.getNodeAddress(group,item);
      if(address<0){
          length ( 0 );
          ptr    = NULL;
      } else {
          length (event.getNodeSize(address));
          ptr    = reinterpret_cast<T*>(event.getNodePtr(address));
      }
    }
*/
/*
    template <class T> node<T>(hipo::reader &reader, int group, int item){
       group(group);
       item(item);
       //reader.addNode(shared_ptr<generic_node>(this));
    }*/

    template <class T> node<T>::~node(){
        length (0);
        ptr    = 0L;
    }

    template <class T> void node<T>::reset(){
        length(0);
    }

    template <class T> void node<T>::setLength(int l){
        length(l);
    }

    template <class T> void node<T>::setAddress(char* address){
        ptr = reinterpret_cast<T*>(address);
    }

    template <class T> int node<T>::getLength(){ return length();}

    template <class T> int node<T>::getBytesLength(){ return sizeof(*ptr)*length();}
    template <class T> T node<T>::getValue(int index){
        return ptr[index];
    }

    template <class T> void node<T>::show(){
      int nl = length();
        std::cout << " NODE LENGTH = " << length() << '\n';
        for(int i = 0; i < nl; i++){
          std::cout << getValue(i) << ",";
        }
        std::cout << std::endl;
    }

    template <class T> char  *node<T>::getAddress(){
        return reinterpret_cast<char *> (ptr);
    }
}
#endif /* NODE_H */
