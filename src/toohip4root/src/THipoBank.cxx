#include "THipoBank.h"

using std::cout;

namespace hipo {

  ////////////////////////////////////////////////THipoBank
  THipoBank::~THipoBank() {
    for (UInt_t i = 0; i < fVecItems.size(); i++)
      delete fVecItems.at(i);
    fVecItems.clear();
  }
  void THipoBank::InitTree(TTree* tree) {
    // Make a branch for each item
    TString branchname0 = fName;
    branchname0.ReplaceAll("::", "_");
    branchname0.ReplaceAll(" ", "");
    for (UInt_t in = 0; in < fVecItems.size(); in++) {
      fVecItems[in]->SetIsVector(fIsVector);
      fVecItems[in]->TreeBranch(branchname0, tree);
    }
  }
  void THipoBank::Print(int verbose) {
    cout << "Printing Hipo Bank\n";
    cout << "Name = " << fName << "\n";
    cout << "Group = " << fGroup << "\n";
    cout << "Entry :" << fEntry << "\n";
    cout << "Items :" << fVecItems.size() << "\n";
    if(verbose>0) {
      if(verbose==1){
        for (UInt_t i = 0; i < fVecItems.size(); i++){ 
          cout << "  " << fVecItems[i]->GetName() << ", ";
        }
        cout << "\n";
      } else {
        for (UInt_t i = 0; i < fVecItems.size(); i++){ 
          cout << "  " << fVecItems[i]->GetName() << " with type " << fVecItems[i]->Type() << "\n";
        }
      }
    }
  }

  THipoItem* THipoBank::GetItem(TString name) { 

    for (UInt_t i = 0; i < fVecItems.size(); i++)
      if (fVecItems[i]->GetName() == name) { 
        // remember to read this item
        // we will disgard items that are not "Got"
        // fVecReadItems.push_back(fVecItems[i]);
        return fVecItems[i];
      }

    return nullptr;
  }
  
}

