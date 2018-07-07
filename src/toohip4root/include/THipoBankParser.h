#ifndef hipo_tools_THipoBankParser_HH
#define hipo_tools_THipoBankParser_HH

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

#include "THipoBank.h"

namespace hipo {

  /** Hipo Bank parser.
   */
  class THipoBankParser {
  public:
    THipoBankParser();
    virtual ~THipoBankParser(){};

    /** Configure bank 3 does...
     */
    Bool_t  ConfigBank3(THipoBank* bank);

    /** Re-configure bank 3 does...
     */
    Bool_t  ReConfigBank3(THipoBank* bank);

    void    SetDict(hipo::dictionary* dict) { fDictionary = dict; };

    /** Converts the type given by a string, e.g., "int" to its corresponding
     * integer type.
     *
     * - 1 =  "int8_t"  
     * - 2 =  "int16_t" 
     * - 3 =  "int32_t" 
     * - 4 =  "float"   
     * - 5 =  "double"  
     * - 8 =  "int64_t" 
     */
    int     CodeToType(std::string code);
    TString CodeToItem(std::string code);

    void    SetReader(hipo::reader* reader) { fReader = reader; }

  private:
    hipo::dictionary* fDictionary = nullptr;
    hipo::reader*     fReader     = nullptr;
  };

}

#endif
