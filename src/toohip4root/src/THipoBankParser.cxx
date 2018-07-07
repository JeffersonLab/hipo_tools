#include "THipoBankParser.h"

using std::cout;
using std::string;

namespace hipo {

  THipoBankParser::THipoBankParser() {}

  Bool_t THipoBankParser::ConfigBank3(THipoBank* bank) {
    // Use the code string to parse the reader::getBranch
    std::vector<std::string> code = fDictionary->getSchema(bank->GetName()).branchesCode();
    bank->SetGroup(fDictionary->getSchema(bank->GetName()).getGroup());
    cout << "CONF " << fReader << "\n";
    for (uint v = 0; v < code.size(); v++) {
      std::string vcode = code[v].c_str();
      switch (CodeToType(vcode)) {
      case 1:
        bank->AddItem(
            new THipoItemB(fReader->getBranch<int8_t>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      case 2:
        bank->AddItem(
            new THipoItemS(fReader->getBranch<int16_t>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      case 3:
        bank->AddItem(
            new THipoItemI(fReader->getBranch<int32_t>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      case 4:
        bank->AddItem(
            new THipoItemF(fReader->getBranch<float>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      case 5:
        bank->AddItem(
            new THipoItemD(fReader->getBranch<double>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      case 8:
        bank->AddItem(
            new THipoItemL(fReader->getBranch<int64_t>(bank->GetName(), CodeToItem(vcode).Data()),
                           bank->GetEntryAddress()),
            CodeToItem(vcode).Data());
        break;
      default:
        break;
      }
    }

    bank->SetConfigured();
    bank->Print();
    return kTRUE;
  }

  Bool_t THipoBankParser::ReConfigBank3(THipoBank* bank) { 
    if (!bank->IsConfigured())
      cout << "Warning THipoBankParser::ReConfigBank3 bankk not yet configured"
           << " " << bank->GetName() << "\n";
    // Use the code string to parse the reader::getBranch
    fDictionary                   = fReader->getSchemaDictionary();
    std::vector<std::string> code = fDictionary->getSchema(bank->GetName()).branchesCode();
    // bank->SetGroup(fDictionary->getSchema(bank->GetName()).getGroup());
    cout << " ReConfigBank3 " << fReader << "\n";
    for (uint v = 0; v < code.size(); v++) {
      std::string vcode = code[v].c_str();
      switch (CodeToType(vcode)) {
      case 1:
        dynamic_cast<THipoItemB*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<int8_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      case 2:
        dynamic_cast<THipoItemS*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<int16_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      case 3:
        dynamic_cast<THipoItemI*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<int32_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      case 4:
        dynamic_cast<THipoItemF*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<float_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      case 5:
        dynamic_cast<THipoItemD*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<double_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      case 8:
        dynamic_cast<THipoItemL*>(bank->GetItem(CodeToItem(vcode)))
            ->ResetNode(fReader->getBranch<int64_t>(bank->GetName(), CodeToItem(vcode).Data()));
        break;
      default:
        break;
      }
    }

    bank->Print();
    return kTRUE;
  }
  int THipoBankParser::CodeToType(string code) { 
    string type_str = hipo::utils::substring(code, "<", ">", 0);
    if (type_str == string("int8_t"))
      return 1;
    if (type_str == string("int16_t"))
      return 2;
    if (type_str == string("int32_t"))
      return 3;
    if (type_str == string("float"))
      return 4;
    if (type_str == string("double"))
      return 5;
    if (type_str == string("int64_t"))
      return 8;
    return 0;
  }
  TString THipoBankParser::CodeToItem(string code) { 
    string  item_str = hipo::utils::substring(code, ",", ")", 0);
    TString sub(item_str);
    sub.ReplaceAll("\"", "");
    return sub;
  }
} // namespace hipo
