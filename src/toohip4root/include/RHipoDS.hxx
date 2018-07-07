#ifndef HIPO_ROOT_RHIPODS
#define HIPO_ROOT_RHIPODS

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDataSource.hxx"
#include <TChain.h>

#include <memory>

#include "hipo/dictionary.h"
#include "hipo/event.h"
#include "hipo/node.h"
#include "hipo/reader.h"
#include "hipo/record.h"

namespace ROOT {

  namespace RDF {

    class RHipoDS final : public ROOT::RDF::RDataSource {
    private:
      unsigned int             fNSlots = 0U;
      std::string              fTreeName;
      std::string              fFileNameGlob;
      mutable hipo::reader           fModelChain; // Mutable needed for getting the column type name
      std::vector<double*>     fAddressesToFree;
      std::vector<std::string> fListOfBranches;
      std::vector<std::pair<ULong64_t, ULong64_t>> fEntryRanges;
      std::vector<std::vector<void*>> fBranchAddresses; // first container-> slot, second -> column;
      std::vector<std::unique_ptr<hipo::reader>> fChains;

      std::vector<void*> GetColumnReadersImpl(std::string_view, const std::type_info&);

    public:
      RHipoDS(std::string_view treeName, std::string_view fileNameGlob);
      ~RHipoDS();
      std::string                                  GetTypeName(std::string_view colName) const;
      const std::vector<std::string>&              GetColumnNames() const;
      bool                                         HasColumn(std::string_view colName) const;
      void                                         InitSlot(unsigned int slot, ULong64_t firstEntry);
      void                                         FinaliseSlot(unsigned int slot);
      std::vector<std::pair<ULong64_t, ULong64_t>> GetEntryRanges();
      bool                                         SetEntry(unsigned int slot, ULong64_t entry);
      void                                         SetNSlots(unsigned int nSlots);
      void                                         Initialise();
    };

    RDataFrame MakeRootDataFrame(std::string_view treeName, std::string_view fileNameGlob);

  } // ns RDF

} // ns ROOT

#endif
