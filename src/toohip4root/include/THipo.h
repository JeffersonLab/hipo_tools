#ifndef clasdigi_hpio_THIPO_h
#define clasdigi_hpio_THIPO_h

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

/** Hipo file stuff.
 *
 *  \todo HipoItem classes should make use of template
 *
 */
namespace hipo {

  class THipoBank;

  /** Hipo Bank parser.
   */
  class THipoBankParser {
  public:
    THipoBankParser();
    virtual ~THipoBankParser(){};

    Bool_t  ConfigBank3(THipoBank* bank);
    Bool_t  ReConfigBank3(THipoBank* bank);
    void    SetDict(hipo::dictionary* dict) { fDictionary = dict; };
    int     CodeToType(std::string code);
    TString CodeToItem(std::string code);
    void    SetReader(hipo::reader* reader) { fReader = reader; }

  private:
    hipo::dictionary* fDictionary = nullptr;
    hipo::reader*     fReader     = nullptr;
  };

  /** Hipo Item.
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
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeB);
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
      fVecB.clear();
      fVecB.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecB[i] = fNodeB->getValue(i);
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
      fVecS.clear();
      fVecS.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecS[i] = fNodeS->getValue(i);
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
      fVecI.clear();
      fVecI.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecI[i] = fNodeI->getValue(i);
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
      fVecF.clear();
      fVecF.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecF[i] = fNodeF->getValue(i);
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
      fVecD.clear();
      fVecD.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecD[i] = fNodeD->getValue(i);
    }
    double_t Val() { return fNodeD->getValue(fBankEntry); }

  private:
    hipo::node<double_t>* fNodeD = nullptr;
    std::vector<double_t> fVecD;
  };
  
  /** Item for long.
   */
  class THipoItemL : public THipoItem {

  public:
    THipoItemL(hipo::node<long>* node, Int_t* entry) : THipoItem(entry) {
      fNodeL = node;
      fNode  = dynamic_cast<hipo::generic_node*>(fNodeL);
      fNode->type(8);
    };
    virtual ~THipoItemL(){};
    virtual void ResetNode(hipo::node<long>* node) {
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
      fVecL.clear();
      fVecL.resize(Size());
      for (UInt_t i = 0; i < Size(); i++)
        fVecL[i] = fNodeL->getValue(i);
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
    long Val() { return fNodeL->getValue(fBankEntry); }

  private:
    hipo::node<long>* fNodeL = nullptr;
    std::vector<long> fVecL;
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

  /** THipo interface class
   */
  class THipo {

  public:
    THipo();
    THipo(TString filename);
    virtual ~THipo();

  protected:
    hipo::reader*     fReader = nullptr;
    hipo::record      fRecord;
    hipo::event       fHipoEvent;
    hipo::dictionary* fDictionary = nullptr;

    std::vector<TString> fInFiles;

    std::vector<TString> fOnlyBanks;

    TTree* fOutTree = nullptr;
    TFile* fOutFile = nullptr;

    std::vector<THipoBank*> fBanks;
    THipoBankParser*        fParser  = nullptr;
    THipoBank*              fCurBank = nullptr;

    /* Long64_t fNRecEvents=0; */
    /* Long64_t fRecEventCount=0; */
    /* Long64_t fNRecords=0; */
    /* Long64_t fRecordCount=0; */

    Long64_t fNEvents = 0;
    UInt_t   fFilei   = 0;

    Bool_t  fUseCLDirs = kFALSE;
    TString fOutDirName;
    Int_t   fCompression = 1;
    Bool_t  fIsVector    = kTRUE;

    std::map<TString, Int_t> fBankMap;

  public:
    Long64_t   ProcessFile(TString filename);
    Bool_t     NextFile();
    void       AddCommandLineFiles();
    void       AddCommandLineDirs();

    void       ToRoot();
    void       ToRoot(TString filename);
    void       ToRoot(std::vector<std::string> files);
    void       ToRoot(TString filename, TString output_filename, int N_events = 0);

    void       InitFile(TString filename);
    Bool_t     ConfigBanks();
    Bool_t     ConfigBank(TString bankname);
    Bool_t     ConfigOnlyBank(TString bankname);
    THipoBank* GetBank(TString name);
    // void ReadItemI(UInt_t ii){fCurBank->ReadItemI(&fHipoEvent,ii);}
    // void ReadItemF(UInt_t ii){fCurBank->ReadItemF(&fHipoEvent,ii);}

    Bool_t NextEvent();
    void   ClearEvent();

    void     SetCompression(Int_t com) { fCompression = com; }
    void     SetIsVector(Bool_t vect) { fIsVector = vect; }
    void     InitOutput(TString filename);
    TString  GetOutDirName() { return fOutDirName; }
    void     CloseOutput();
    TTree*   GetTree() { return fOutTree; }
    Long64_t GetNEvents() { return fNEvents; }
    // void ResetEntries();
  };

} // namespace hipo

#endif
