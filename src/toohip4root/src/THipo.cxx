#include "THipo.h"
#include "TApplication.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"

using namespace std;

namespace hipo {

  THipo::THipo() {
    fParser = new THipoBankParser();
    // Decide whether input files or dirs from command line
    //Int_t  argc = gApplication->Argc();
    //char** argv = gApplication->Argv();
    //for (Int_t i = 1; i < argc; i++) {
    //  TString dirname = argv[i];
    //  if (dirname.Contains("--in=")) {
    //    fUseCLDirs = kTRUE;
    //    break;
    //  } else
    //    fUseCLDirs = kFALSE;
    //}
    //// Look for output dir
    //for (Int_t i = 1; i < argc; i++) {
    //  TString dirname = argv[i];
    //  if (dirname.Contains("--out=")) {
    //    dirname.Remove(0, 6);
    //    fOutDirName = dirname;
    //    cout << "THipo will write root files to " << fOutDirName << endl;
    //    if (gSystem->MakeDirectory(fOutDirName) == -1) {
    //      cout << "Error directory " << fOutDirName
    //           << " already exists or its parent directory does not exist" << endl;
    //      exit(1);
    //    }
    //  }
    //}
  }

  THipo::THipo(TString filename) { fParser = new THipoBankParser(); }

  THipo::~THipo() {
    if (fParser)
      delete fParser;
    for (UInt_t ib = 0; ib < fBanks.size(); ib++)
      delete fBanks.at(ib);
    fBanks.clear();
  }

  void THipo::InitFile(TString filename) {
    // if(fReader) delete fReader;
    // fReader=nullptr;
    if (!fReader)
      fReader = new hipo::reader();
    fParser->SetReader(fReader);
    fReader->open(filename.Data());
    fReader->showInfo();
    // If not the first file, banks already configured, just need to set new nodes
    // for(UInt_t ig=0;ig<fBanks.size();ig++){
    // fParser->ReConfigBank3(fBanks.at(ig));
    //}
  }

  void THipo::AddCommandLineFiles() {
    // If using command line dirs do that then return
    if (fUseCLDirs) // This is set in the constructor
      return AddCommandLineDirs();
    // look for files from command line
    cout << "THipo::AddCommandLineFiles() "
         << " using input files from commandline" << endl;
    //Int_t  argc = gApplication->Argc();
    //char** argv = gApplication->Argv();
    //for (Int_t i = 0; i < argc; i++) {
    //  if (TString(argv[i]).Contains(".hipo"))
    //    fInFiles.push_back(argv[i]);
    //}
  }

  void THipo::AddCommandLineDirs() {
    cout << "THipo::AddCommandLineDirs() "
         << " using input directory from commandline" << endl;
    // look for files from command line
    //Int_t  argc = gApplication->Argc();
    //char** argv = gApplication->Argv();
    //for (Int_t i = 1; i < argc; i++) {
    //  TString dirname = argv[i];
    //  if (dirname.Contains("--in=")) {
    //    dirname.Remove(0, 5);
    //    cout << "THipo::AddCommandLineDirs() checking for files in " << dirname << endl;
    //    void* dir = gSystem->OpenDirectory(dirname);
    //    cout << dir << endl;
    //    TString fileName;
    //    while ((fileName = (gSystem->GetDirEntry(dir)))) {
    //      cout << fileName << endl;
    //      if (fileName.Contains(TString(".hipo")))
    //        fInFiles.push_back(dirname + fileName);
    //      if (fileName == TString(""))
    //        break;
    //    }
    //  }
    //}

    cout << "THipo::AddCommandLineDirs() found " << fInFiles.size()
         << " hipo files in --in directories" << endl;
  }

  void THipo::ToRoot() {
    // Use file list
    for (UInt_t i = 0; i < fInFiles.size(); i++)
      ToRoot(fInFiles.at(i));
  }
  void  THipo::ToRoot(std::vector<std::string> files) {
    for(auto f : files ) {
      ToRoot(f);
    }
  }

  //void THipo::ToRoot(std::string filename) {
  //  ToRoot(filename);
  //}
  
  void THipo::ToRoot(TString filename, TString output_filename, int N_events) {
    cout << "THipo::ToRoot " << filename << endl;
    // Convert file called filename
    InitFile(filename);
    // cout<<"THipo::ToRoot Number of records "<<fNRecords<<endl;
    // if perviously defined by user configuration or previous hipo file
    if (fBanks.size() == 0){
      ConfigBanks();
    }
    //filename.ReplaceAll(".hipo", ".root");
    // Append the output directory name
    //if (fOutDirName.Sizeof() > 1) {
    //  TString newfilename = fOutDirName + "/" + gSystem->BaseName(filename);
    //  filename            = newfilename;
    //}
    InitOutput(output_filename);
    // loop over events
    //
    int i_event = 0;
    while (NextEvent()) {
      if( (N_events > 0) && (i_event >= N_events) ){
        break;
      }
      i_event++;
    }
    CloseOutput();
  }

  void THipo::ToRoot(TString filename) {
    cout << "THipo::ToRoot " << filename << endl;
    // Convert file called filename
    InitFile(filename);
    // cout<<"THipo::ToRoot Number of records "<<fNRecords<<endl;
    // if perviously defined by user configuration or previous hipo file
    if (fBanks.size() == 0)
      ConfigBanks();
    filename.ReplaceAll(".hipo", ".root");

    // Append the output directory name
    if (fOutDirName.Sizeof() > 1) {
      TString newfilename = fOutDirName + "/" + gSystem->BaseName(filename);
      filename            = newfilename;
    }
    InitOutput(filename);
    // loop over events
    while (NextEvent()) { }
    CloseOutput();
  }

  Bool_t THipo::NextFile() {
    if (fFilei >= fInFiles.size())
      return kFALSE;
    cout << " THipo::NextFile() Analyse file " << fInFiles.at(fFilei) << endl;
    InitFile(fInFiles.at(fFilei++));
    return kTRUE;
  }

  Long64_t THipo::ProcessFile(TString filename) {
    InitFile(filename);
    // cout<<"Number of records "<<fNRecords<<endl;

    // loop over records
    while (NextEvent()) { }
    if (fOutTree)
      CloseOutput();
    return fNEvents;
  }

  Bool_t THipo::NextEvent() { // Read next event in file
    if (!fReader->next()) {
      return kFALSE;
    }
    // Loop over groups if require output (put values in vectors)
    for (UInt_t ig = 0; ig < fBanks.size(); ig++) {
      fCurBank = fBanks.at(ig);
      fCurBank->ResetEntry();
      if (fOutTree)
        fCurBank->ReadEvent(); // read into vectors
    }
    if (fOutTree)
      fOutTree->Fill();

    fNEvents++;
    return kTRUE;
  }

  Bool_t THipo::ConfigBank(TString bankname) {
    fBanks.push_back(new THipoBank(bankname));
    fBanks.back()->SetIsVector(fIsVector);
    Bool_t isOK = fParser->ConfigBank3(fBanks.back());
    fBankMap[fBanks[fBanks.size() - 1]->GetName()] =
        fBanks.size() - 1; // map name after configure in case items removed
    return isOK;
  }

  Bool_t THipo::ConfigBanks() {
    // Use Hipo Dictionary to configure banks
    fDictionary = fReader->getSchemaDictionary();
    fParser->SetDict(fDictionary);
    //  Loop through dictionary and get the bank information
    hipo::event              schema;
    std::vector<std::string> banks = fDictionary->getSchemaList();
    for (UInt_t d = 0; d < banks.size(); d++) {
      if (!fDictionary->hasSchema(banks[d].c_str())) {
        cout << "Warning : THipo::ConfigBanks(), no bank " << banks[d].c_str() << endl;
        continue;
      }
      TString dBank = banks[d];
      if (fOnlyBanks.size() != 0) {
        for (UInt_t io = 0; io < fOnlyBanks.size(); io++) {
          if (dBank.Contains(fOnlyBanks[io]))
            ConfigBank(dBank);
        }
      } else
        ConfigBank(dBank);
    }
    return kTRUE;
  }

  Bool_t THipo::ConfigOnlyBank(TString bankname) {
    fOnlyBanks.push_back(bankname);
    return kTRUE;
  }

  void THipo::InitOutput(TString filename) {
    fOutFile = new TFile(filename, "recreate");
    fOutFile->SetCompressionLevel(fCompression);
    fOutTree = new TTree("hipo2root", "CLAS12 banks in ROOT");
    // Make branches from all the banks
    // fReader.next();
    for (UInt_t ib = 0; ib < fBanks.size(); ib++)
      fBanks.at(ib)->InitTree(fOutTree);
    fOutTree->Print("");
  }

  void THipo::CloseOutput() {
    fOutTree->Write();
    fOutFile->Close();
    delete fOutFile;
    fOutFile = nullptr;
    fOutTree = nullptr;
  }

  THipoBank* THipo::GetBank(TString name) {
    auto it = fBankMap.find(name);
    if (it == fBankMap.end()) {
      cout << "Error THipo* GetBank( No bank configured with name = " << name << endl;
      return nullptr;
    }
    return fBanks[fBankMap[name]];
  };

  /// Set fEntry back to -1 in all banks while staying on same event
  // void THipo::ResetEntries(){

  //   for(UInt_t i=0;i<fBanks.size();i++)
  //     fBanks[i]->ResetEntry();
  // }
  //////////////////////////////////////////////////THipoItem
  Bool_t THipoItem::TreeBranch(TString branchname0, TTree* outtree) {
    fBranchname  = branchname0 + "_" + GetName();
    fNBranchname = TString("N") + branchname0 + "_" + GetName();
    if (!fIsVector)
      outtree->Branch(fNBranchname, &fNEntries, fNBranchname + "/I");
    if (fIsVector)
      VectorBranch(outtree);
    else
      TreeBranch(outtree);
    return kTRUE;
  }
} // namespace hipo
