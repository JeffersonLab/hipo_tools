#ifndef hipo_tools_THipoBank_HH
#define hipo_tools_THipoBank_HH


#include "TBenchmark.h"
#include "TFile.h"
#include "TMacro.h"
#include "TString.h"
#include "TTree.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "hipo/dictionary.h"
#include "hipo/event.h"
#include "hipo/node.h"
#include "hipo/reader.h"
#include "hipo/record.h"

#include "THipoItem.h"

namespace hipo {
  /** Hipo Bank.
   */
  class THipoBank {

  public:
    THipoBank(){};
    THipoBank(TString name) : fName(name){};
    virtual ~THipoBank();

    void SetGroup(Int_t group) { fGroup = group; }
    void AddItem(THipoItem* item, TString name) {
      fVecItems.push_back(item);
      fVecItems.back()->SetName(name);
    }

    void    SetIsVector(Bool_t vect) { fIsVector = vect; }
    void    SetName(TString name) { fName = name; }
    TString GetName() { return fName; }
    Int_t   Group() { return fGroup; }

    void ReadEvent();

    void ClearEvent();
    void Print();
    void InitTree(TTree* tree);
    // Iterate over entries for each event in item name
    Bool_t     NextEntry();
    Int_t      GetEntry() { return fEntry; }
    void       SetEntry(Int_t ent) { fEntry = ent; }
    Int_t*     GetEntryAddress() { return &fEntry; }
    void       ResetEntry() { fEntry = -1; }
    Int_t      GetEntries();
    THipoItem* GetItem(TString name);

    Bool_t IsConfigured() { return fConfigured; }
    void   SetConfigured() { fConfigured = kTRUE; }

  protected:
    TString                 fName;
    Int_t                   fGroup = 0;
    std::vector<THipoItem*> fVecItems;
    std::vector<THipoItem*> fVecReadItems;
    Bool_t                  fIsVector   = kTRUE;
    Bool_t                  fConfigured = kFALSE;
    Int_t                   fEntry      = -1;
  };

  inline void THipoBank::ReadEvent() {
    if (fIsVector) {
      for (UInt_t i = 0; i < fVecItems.size(); i++) {
        fVecItems.at(i)->CopyToVector();
      }
      return;
    }
    return;
    /* for(UInt_t i=0;i<fVecItems.size();i++){ */
    /*    fVecItems.at(i)->SetNEntries(); */
    /*    fVecItems.at(i)->SetBranchAddress(); */
    /* } */
  }
  inline void THipoBank::ClearEvent() {
    // for(UInt_t i=0;i<fVecItems.size();i++) fVecItems.at(i)->Reset();
    fEntry = -1;
  }
  inline Int_t  THipoBank::GetEntries() { return fVecItems[0]->Size(); }
  inline Bool_t THipoBank::NextEntry() {
    if (GetEntries() > fEntry + 1) {
      fEntry++;
      return kTRUE;
    } else
      return kFALSE;

    /* //  cout<<"NEXt Entry for "<<GetName()<<" "<<fEntry<<" "<<fVecI[0]->size()<<endl; */
    /* //try to iterate using first integer vector */
    /* if(fVecI.size()){ */
    /*   if((Int_t)fVecI[0]->size()>fEntry+1){ */
    /*     fEntry++; */
    /*     return kTRUE; */
    /*   } */
    /* } */
    /* if(fEntry>-1) return kFALSE; */
    /* //No integer vectors perhaps float */
    /* if(fVecF.size()){ */
    /*   if((Int_t)fVecF[0]->size()>fEntry+1){ */
    /*     fEntry++; */
    /*     return kTRUE; */
    /*   } */
    /* } */

    /* return kFALSE; */
  }

}
#endif
