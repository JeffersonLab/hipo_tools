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

#include "THipoBankParser.h"

/** Hipo file stuff.
 *
 *  \todo HipoItem classes should make use of template
 *
 */
namespace hipo {


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
