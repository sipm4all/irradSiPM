#ifndef _RunAction_h_
#define _RunAction_h_

#include "G4UserRunAction.hh"

class TFile;
class TTree;

class RunAction : public G4UserRunAction
{

public:
  
  RunAction() = default;
  ~RunAction() override;
  
  void BeginOfRunAction(const G4Run *run) override;
  void EndOfRunAction(const G4Run *run) override;
  
protected:

};

#endif /** _RunAction_h_ **/
