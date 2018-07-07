#include <ROOT/RDFUtils.hxx>
#include <ROOT/RHipoDS.hxx>
#include <ROOT/TSeq.hxx>
#include <TClass.h>
#include <TError.h>
#include <TROOT.h>         // For the gROOTMutex
#include <TVirtualMutex.h> // For the R__LOCKGUARD
#include <ROOT/RMakeUnique.hxx>

#include <algorithm>
#include <vector>

namespace ROOT {

namespace RDF {

std::vector<void *> RHipoDS::GetColumnReadersImpl(std::string_view name, const std::type_info &id)
{
   const auto colTypeName = GetTypeName(name);
   const auto &colTypeId = ROOT::Internal::RDF::TypeName2TypeID(colTypeName);
   if (id != colTypeId) {
      std::string err = "The type of column \"";
      err += name;
      err += "\" is ";
      err += colTypeName;
      err += " but a different one has been selected.";
      throw std::runtime_error(err);
   }

   const auto index =
      std::distance(fListOfBranches.begin(), std::find(fListOfBranches.begin(), fListOfBranches.end(), name));
   std::vector<void *> ret(fNSlots);
   for (auto slot : ROOT::TSeqU(fNSlots)) {
      ret[slot] = (void *)&fBranchAddresses[index][slot];
   }
   return ret;
}

RHipoDS::RHipoDS(std::string_view treeName, std::string_view fileNameGlob)
   : fTreeName(treeName), fFileNameGlob(fileNameGlob), fModelChain(std::string(treeName).c_str())
{
   fModelChain.open(fFileNameGlob.c_str());
   
   hipo::dictionary *dict = hip_reader.getSchemaDictionary();

   fListOfBranches = dict->getSchemaList();

   //const TObjArray &lob = *fModelChain.GetListOfBranches();
   //fListOfBranches.resize(lob.GetEntries());

   //TIterCategory<TObjArray> iter(&lob);
   //std::transform(iter.Begin(), iter.End(), fListOfBranches.begin(), [](TObject *o) { return o->GetName(); });
}

RHipoDS::~RHipoDS()
{
   for (auto addr : fAddressesToFree) {
      delete addr;
   }
}

std::string RHipoDS::GetTypeName(std::string_view colName) const
{
   if (!HasColumn(colName)) {
      std::string e = "The dataset does not have column ";
      e += colName;
      throw std::runtime_error(e);
   }
   // TODO: we need to factor out the routine for the branch alone...
   // Maybe a cache for the names?
   auto typeName =
      ROOT::Internal::RDF::ColumnName2ColumnTypeName(std::string(colName), /*nsID=*/0, &fModelChain, /*ds=*/nullptr,
                                                     /*isCustomCol=*/false);
   // We may not have yet loaded the library where the dictionary of this type is
   TClass::GetClass(typeName.c_str());
   return typeName;
}

const std::vector<std::string> &RHipoDS::GetColumnNames() const
{
   return fListOfBranches;
}

bool RHipoDS::HasColumn(std::string_view colName) const
{
   //if (!fListOfBranches.empty())
   //   GetColumnNames();
   return fListOfBranches.end() != std::find(fListOfBranches.begin(), fListOfBranches.end(), colName);
}

void RHipoDS::InitSlot(unsigned int slot, ULong64_t firstEntry)
{
  hipo::reader*  chain = new hipo::reader();
  chain->open(fFileNameGlob);
  // //auto chain = new TChain(fTreeName.c_str());
  // //chain->ResetBit(kMustCleanup);
  // //chain->Add(fFileNameGlob.c_str());
  // //chain->GetEntry(firstEntry);
  // //
  // TString setBranches;
  // for (auto i : ROOT::TSeqU(fListOfBranches.size())) {
  //    auto colName = fListOfBranches[i].c_str();
  //    auto &addr = fBranchAddresses[i][slot];
  //    auto typeName = GetTypeName(colName);
  //    auto typeClass = TClass::GetClass(typeName.c_str());
  //    if (typeClass) {
  //       chain->SetBranchAddress(colName, &addr, nullptr, typeClass, EDataType(0), true);
  //    } else {
  //       if (!addr) {
  //          addr = new double();
  //          fAddressesToFree.emplace_back((double *)addr);
  //       }
  //       chain->SetBranchAddress(colName, addr);
  //    }
  // }
  fChains[slot].reset(chain);
}

void RHipoDS::FinaliseSlot(unsigned int slot)
{
   fChains[slot].reset(nullptr);
}

std::vector<std::pair<ULong64_t, ULong64_t>> RHipoDS::GetEntryRanges()
{
   auto entryRanges(std::move(fEntryRanges)); // empty fEntryRanges
   return entryRanges;
}

bool RHipoDS::SetEntry(unsigned int slot, ULong64_t entry)
{
  while(fChains[slot]->getCurrentEvent() > entry) {
    fChains[slot]->next();
  }
  //fChains[slot]->GetEntry(entry);
  return true;
}

void RHipoDS::SetNSlots(unsigned int nSlots)
{
   R__ASSERT(0U == fNSlots && "Setting the number of slots even if the number of slots is different from zero.");

   fNSlots = nSlots;

   const auto nColumns = fListOfBranches.size();
   // Initialise the entire set of addresses
   fBranchAddresses.resize(nColumns, std::vector<void *>(fNSlots, nullptr));

   fChains.resize(fNSlots);
}

void RHipoDS::Initialise()
{
   const auto nentries = fModelChain.GetEntries();
   const auto chunkSize = nentries / fNSlots;
   const auto reminder = 1U == fNSlots ? 0 : nentries % fNSlots;
   auto start = 0UL;
   auto end = 0UL;
   for (auto i : ROOT::TSeqU(fNSlots)) {
      start = end;
      end += chunkSize;
      fEntryRanges.emplace_back(start, end);
      (void)i;
   }
   fEntryRanges.back().second += reminder;
}

RDataFrame MakeRootDataFrame(std::string_view treeName, std::string_view fileNameGlob)
{
   ROOT::RDataFrame tdf(std::make_unique<RHipoDS>(treeName, fileNameGlob));
   return tdf;
}

} // ns RDF

} // ns ROOT
