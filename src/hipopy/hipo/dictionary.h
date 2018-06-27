/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dictionary.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <string>
#include <vector>


//#include "reader.h"

namespace hipo {

class schema {
  private:

    std::map<std::string, std::pair<int,int> > schemaEntries;
    int groupid;
    std::string schemaName;
    std::string getTypeString(int type);
    int         getMaxStringLength();
    std::string getBranchVariable(const char *var, int max);

  public:

    schema(){ groupid = 0;}
    schema(const char *name){ schemaName = name;}

    schema(const schema &s) {
      schemaName    = s.schemaName;
      schemaEntries = s.schemaEntries;
      groupid       = s.groupid;
    }

    virtual ~schema(){}

    void  setName(const char* name) { schemaName = name;}
    std::string getName() { return schemaName;}

    void  setGroup( int grp){ groupid = grp;}
    bool  hasEntry(const char* entry){ return schemaEntries.count(entry);}
    void  addEntry( const char* name, int id, int type)
      { schemaEntries[name] = std::make_pair(id, type);}


    int   getGroup( ){ return groupid;};
    int   getItem(  const char* entry);
    int   getType(  const char* entry);
    int   getTypeByString(const char *typestring);
    void  ls();

    std::vector<std::string> getEntryList();
    std::vector<std::string> branchesCode();
    std::vector<std::string> branchesAccessCode();

    void operator = (const schema &D ) {
         schemaName = D.schemaName;
         groupid    = D.groupid;
         schemaEntries = D.schemaEntries;
      }
};

class dictionary {

    private:
        std::map<std::string, hipo::schema> mapDict;

    public:

        dictionary(){}
        virtual ~dictionary(){}
        //node(hipo::reader &reader, int group, int item);
        void          ls(int mode = 0);
        bool          hasSchema(const char* name){ return mapDict.count(name)>0;}
        hipo::schema  getSchema(const char *name){ return mapDict[name];}
        void          parse(std::string dictString);
        std::vector<std::string> getSchemaList();
};

}

#endif /* NODE_H */
