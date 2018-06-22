/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   read_file.cpp
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:30 PM
 */

#include <cstdlib>
#include <iostream>

#include "hipo/reader.h"
#include "hipo/writer.h"
#include "hipo/event.h"
#include "hipo/node.h"
#include "hipo/text.h"
#include "hipo/data.h"


using namespace std;
/*
 *
 */

vector<int> getRandomVector(){
   vector<int> vec;
   int nelements = rand()%25+5;
   for(int i = 0; i < nelements; i++) vec.push_back(rand()%16575);
   //printf(" random number = %d\n",nelements);
   return vec;
}

int main(int argc, char** argv) {

    int nevents = 1000;
    hipo::writer  writer;
    hipo::event   event;

    writer.open("test_file.hipo");
    printf("----> creating file : test_file.hipo\n");
    for(int n = 0; n < nevents; n++){
      vector<int> vec1 = getRandomVector();
      vector<int> vec2 = getRandomVector();
      event.reset();
      event.appendNode(34,1,vec1);
      event.appendNode(34,2,vec2);
      writer.writeEvent(event);
    }
    writer.close();
    printf("----> finished writing file with %d events\n\n",nevents);
    return 0;
}
