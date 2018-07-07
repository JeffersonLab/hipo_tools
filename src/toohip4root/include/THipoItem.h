#ifndef hipo_tools_THipoItem_HH
#define hipo_tools_THipoItem_HH

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

namespace hipo {

  /** Hipo Item.
   *
   */
  class THipoItem {
  public:
    THipoItem(Int_t* entry) : fBankEntry(*entry) {}

    virtual ~THipoItem(){};

    UInt_t Size() { return fNode->length(); }
    // virtual Bool_t FindEntry(Float_t val){return kFALSE;};
    //  virtual Bool_t FindEntry(Float_t val){return kFALSE;};
    void    Reset() { fNode->length(0); }
    Int_t   GetBankEntry() { return fBankEntry; }
    void    SetBankEntry(Int_t* entry) { fBankEntry = *entry; }
    TString GetName() { return fName; }
    Int_t   Type() { return fNode->type(); }

    //  char* GetAddress(){return fNode->getAddress();}

    void SetName(TString name) {
      fNode->name(name.Data());
      fName = name;
    }
    void SetType(Int_t type) { fNode->type(type); }
    void SetNEntries() { fNEntries = fNode->length(); }

    virtual void TreeBranch(TTree* outtree){};
    Bool_t       TreeBranch(TString bankname0, TTree* outtree);
    virtual void VectorBranch(TTree* outtree){};

    // void SetBranchAddress(){if(fBranch)fBranch->SetAddress(GetAddress());}
    virtual void CopyToVector(){};
    void         SetIsVector(Bool_t vect) { fIsVector = vect; }
    // template<class T>   T Val(){return 0;};
  protected:
    TBranch* fBranch = nullptr;
    TString  fBranchname;
    TString  fNBranchname;

    hipo::generic_node* fNode = nullptr;

    Int_t&  fBankEntry; // Points to THipoBank fEntry
    Int_t   fPos      = 0;
    Int_t   fNEntries = 0;
    Bool_t  fIsVector = kTRUE;
    TString fName;
  };
  

  /** Hipo Item for int8.
   *  Item for int8
   */
  class THipoItemB : public THipoItem {

  public:
    THipoItemB(hipo::node<int8_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeB = node;
      fNode  = node;//dynamic_cast<hipo::generic_node*>(fNodeB);
      fNode->type(1);
    };
    virtual ~THipoItemB() {}

    virtual void ResetNode(hipo::node<int8_t>* node) {
      fNodeB = nullptr;
      fNode  = nullptr;
      fNodeB = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeB);
      fNode->type(1);
    };

    // virtual void TreeBranch(TTree* outtree){
    //  fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/B");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecB); }

    virtual void CopyToVector() {
      fVecB.assign((int8_t*)fNodeB->getAddress(),
                   (int8_t*)fNodeB->getAddress() + fNodeB->getLength());
      //fVecB.clear();
      //fVecB.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecB[i] = fNodeB->getValue(i);
    }

    virtual Bool_t FindEntry(Int_t val) {
      for (UInt_t i = 0; i < Size(); i++) {
        if (val == fNodeB->getValue(i)) {
          fBankEntry = i;
          return kTRUE;
        }
      }
      return kFALSE;
    }
    int8_t Val() { return fNodeB->getValue(fBankEntry); }

  private:
    hipo::node<int8_t>* fNodeB = nullptr;
    std::vector<int8_t> fVecB;
  };


  /** Item for int16.
   */
  class THipoItemS : public THipoItem {

  public:
    THipoItemS(hipo::node<int16_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeS = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeS);
      fNode->type(2);
    };
    virtual ~THipoItemS() {}
    virtual void ResetNode(hipo::node<int16_t>* node) {
      fNodeS = nullptr;
      fNode  = nullptr;
      fNodeS = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeS);
      fNode->type(2);
    };

    // virtual void TreeBranch(TTree* outtree){
    // fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/S");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecS); }

    virtual void CopyToVector() {
      //fVecS.clear();
      //fVecS.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecS[i] = fNodeS->getValue(i);
      fVecS.assign((int16_t*)fNodeS->getAddress(),
                   (int16_t*)fNodeS->getAddress() + fNodeS->getLength());
    }
    virtual Bool_t FindEntry(Int_t val) {
      for (UInt_t i = fBankEntry + 1; i < Size(); i++) {
        if (val == fNodeS->getValue(i)) {
          fBankEntry = i;
          return kTRUE;
        }
      }
      return kFALSE;
    }
    int16_t Val() { return fNodeS->getValue(fBankEntry); }

  private:
    hipo::node<int16_t>* fNodeS = nullptr;
    std::vector<int16_t> fVecS;
  };
  
  /** Item for int32
   */
  class THipoItemI : public THipoItem {

  public:
    THipoItemI(hipo::node<int32_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeI = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeI);
      fNode->type(3);
    };
    virtual ~THipoItemI() {}
    virtual void ResetNode(hipo::node<int32_t>* node) {
      fNodeI = nullptr;
      fNode  = nullptr;
      fNodeI = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeI);
      fNode->type(3);
    };

    // virtual void TreeBranch(TTree* outtree){
    // fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/I");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecI); }

    virtual void CopyToVector() {
      //fVecI.clear();
      //fVecI.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecI[i] = fNodeI->getValue(i);
      fVecI.assign((int32_t*)fNodeI->getAddress(),
                   (int32_t*)fNodeI->getAddress() + fNodeI->getLength());
    }
    virtual Bool_t FindEntry(Int_t val) {
      for (UInt_t i = 0; i < Size(); i++) {
        if (val == fNodeI->getValue(i)) {
          fBankEntry = i;
          return kTRUE;
        }
      }
      return kFALSE;
    }
    int32_t Val() { return fNodeI->getValue(fBankEntry); }

  private:
    hipo::node<int32_t>* fNodeI = nullptr;
    std::vector<int32_t> fVecI;
  };

  /** Item for float.
   */
  class THipoItemF : public THipoItem {

  public:
    THipoItemF(hipo::node<float_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeF = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeF);
      fNode->type(4);
    };
    virtual ~THipoItemF() {}
    virtual void ResetNode(hipo::node<float_t>* node) {
      fNodeF = nullptr;
      fNode  = nullptr;
      fNodeF = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeF);
      fNode->type(4);
    };

    // virtual void TreeBranch(TTree* outtree){
    //  fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/F");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecF); }

    virtual void CopyToVector() {
      //fVecF.clear();
      //fVecF.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecF[i] = fNodeF->getValue(i);
      fVecF.assign((int32_t*)fNodeF->getAddress(),
                   (int32_t*)fNodeF->getAddress() + fNodeF->getLength());
    }
    float_t Val() { return fNodeF->getValue(fBankEntry); }

  private:
    hipo::node<float_t>* fNodeF = nullptr;
    std::vector<float_t> fVecF;
  };


  /** Item for double.
   */
  class THipoItemD : public THipoItem {

  public:
    THipoItemD(hipo::node<double_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeD = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeD);
      fNode->type(5);
    };
    virtual ~THipoItemD() {}
    virtual void ResetNode(hipo::node<double_t>* node) {
      fNodeD = nullptr;
      fNode  = nullptr;
      fNodeD = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeD);
      fNode->type(5);
    };

    // virtual void TreeBranch(TTree* outtree){
    // fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/D");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecD); }

    virtual void CopyToVector() {
      //fVecD.clear();
      //fVecD.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecD[i] = fNodeD->getValue(i);
      fVecD.assign((double_t*)fNodeD->getAddress(),
                   (double_t*)fNodeD->getAddress() + fNodeD->getLength());
    }
    double_t Val() { return fNodeD->getValue(fBankEntry); }

  private:
    hipo::node<double_t>* fNodeD = nullptr;
    std::vector<double_t> fVecD;
  };
  
  /** Item for long (64 bit int).
   */
  class THipoItemL : public THipoItem {

  public:
    THipoItemL(hipo::node<int64_t>* node, Int_t* entry) : THipoItem(entry) {
      fNodeL = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeL);
      fNode->type(8);
    };
    virtual ~THipoItemL(){};
    virtual void ResetNode(hipo::node<int64_t>* node) {
      fNodeL = nullptr;
      fNode  = nullptr;
      fNodeL = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeL);
      fNode->type(8);
    };

    // virtual void TreeBranch(TTree* outtree){
    // fBranch=outtree->Branch(fBranchname,GetAddress(),fBranchname+"["+fNBranchname+"]/L");}
    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecL); }

    virtual void CopyToVector() {
      //fVecL.clear();
      //fVecL.resize(Size());
      //for (UInt_t i = 0; i < Size(); i++)
      //  fVecL[i] = fNodeL->getValue(i);
      fVecL.assign((int64_t*)fNodeL->getAddress(),
                   (int64_t*)fNodeL->getAddress() + fNodeL->getLength());
    }
    virtual Bool_t FindEntry(Int_t val) {
      for (UInt_t i = 0; i < Size(); i++) {
        if (val == fNodeL->getValue(i)) {
          fBankEntry = i;
          return kTRUE;
        }
      }
      return kFALSE;
    }
    int64_t Val() { return fNodeL->getValue(fBankEntry); }

  private:
    hipo::node<int64_t>* fNodeL = nullptr;
    std::vector<int64_t> fVecL;
  };


  /** Templated Item.
   */
  template <typename T, int I_type>
  class Item : public THipoItem {
  public:
    Item(hipo::node<T>* node, Int_t* entry) : THipoItem(entry) {
      fNodeF = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeF);
      fNode->type(I_type);
    };
    virtual ~Item() {}

    virtual void ResetNode(hipo::node<T>* node) {
      fNodeF = nullptr;
      fNode  = nullptr;
      fNodeF = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeF);
      fNode->type(I_type);
    };

    virtual void VectorBranch(TTree* outtree) { fBranch = outtree->Branch(fBranchname, &fVecF); }

    virtual void CopyToVector() {
      fVecF.clear();
      fVecF.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecF[i] = fNodeF->getValue(i);
    }
    T Val() { return fNodeF->getValue(fBankEntry); }

  private:
    hipo::node<T>* fNodeF = nullptr;
    std::vector<T> fVecF;
  };
  /* inline Bool_t THipoItem::FindEntry(Float_t val){ */
  /*   for(UInt_t i=0;i<Size();i++){ */
  /*     if(val==fNodeF->getValue(i)) */
  /*       fBankEntry=i; */
  /*     break; */
  /*   } */
  /* } */
  /* inline Bool_t THipoItem::FindEntry(Float_t val){ */
  /*   if(!fNode->length()) return kFALSE; */

  /*   if(fNode->type()==1){ */

  /*   } */

  /*   if(fIsFloat){ */
  /*     if(fItemF->size()==0) return kFALSE; */
  /*     auto it = std::find(fItemF->begin()+fPos,fItemF->end(),val); */
  /*     fPos= it-fItemF->begin(); */
  /*     if(fPos>=(Int_t)fItemF->size())fPos=-1; */

  /*   } */
  /*   else{ */
  /*     if(fItemI->size()==0) return kFALSE; */
  /*     auto it = std::find(fItemI->begin()+fPos,fItemI->end(),(Int_t)val); */
  /*     fPos= it-fItemI->begin(); */
  /*     if(fPos>=(Int_t)fItemI->size()) fPos=-1; */
  /*   } */
  /*   if(fPos>-1) { */
  /*     fBankEntry=fPos; */
  /*     fPos++; */
  /*     return kTRUE; */
  /*   } */
  /*   else{ */
  /*     fPos=0; */
  /*     return kFALSE; */
  /*   } */
  /* } */



  /* inline  void THipoBank::ReadItemI(hipo::event *event,UInt_t ii){ */
  /*   // Int_t type= event->getNodeType(event->getNodeAddress(fGroup,IdxI(ii))); */
  /*   //cout<<"Type "<<type<<endl;  */
  /*   if(fTypeI[ii][0]=='L'){ */
  /*     vector<Long_t> vecl=event->getLong(fGroup,IdxI(ii)); */
  /*     //cout<<vecl.size()<<endl; */
  /*     for(UInt_t iv=0;iv<vecl.size();iv++){ */
  /*       (fVecI.at(ii))->push_back(vecl.at(iv)); */
  /*       (fWrVecL.at(ii))->push_back(vecl.at(iv)); */
  /*     } */
  /*     return; */
  /*   } */
  /*   vector<Int_t> vec=event->getInt(fGroup,IdxI(ii)); */
  /*   for(UInt_t iv=0;iv<vec.size();iv++){ */
  /*     (fVecI.at(ii))->push_back(vec.at(iv)); */
  /*     if(fTypeI[ii][0]=='I')(fWrVecI.at(ii))->push_back(vec.at(iv)); */
  /*     else if(fTypeI[ii][0]=='S')(fWrVecS.at(ii))->push_back(vec.at(iv)); */
  /*     else if(fTypeI[ii][0]=='B')(fWrVecB.at(ii))->push_back(vec.at(iv)); */
  /*   } */
  /* } */
  /* inline  void THipoBank::ReadItemF(hipo::event *event,UInt_t ii){ */
  /*   // Int_t type= event->getNodeType(event->getNodeAddress(fGroup,IdxI(ii))); */
  /*   //cout<<"Type "<<type<<endl;  */
  /*   vector<Float_t> vec=event->getFloat(fGroup,IdxF(ii)); */
  /*   for(UInt_t iv=0;iv<vec.size();iv++) */
  /*     (fVecF.at(ii))->push_back(vec.at(iv)); */
  /* } */

}
#endif
