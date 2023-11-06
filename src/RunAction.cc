#include "RunAction.hh"
#include "G4Run.hh"

#include "RootIO.hh"

/******************************************************************************/

RunAction::~RunAction()
{
}

/******************************************************************************/

void
RunAction::BeginOfRunAction(const G4Run *aRun)
{
  /** begin of run action **/

  std::cout << "--- start of run: " << aRun->GetRunID() << std::endl;
  RootIO::Instance()->Open(std::string("irradSiPM." + std::to_string(aRun->GetRunID()) + ".root").c_str());

}

/******************************************************************************/

void
RunAction::EndOfRunAction(const G4Run *aRun)
{
  /** end of run action **/

  std::cout << "--- end of run: " << aRun->GetRunID() << std::endl;
  RootIO::Instance()->Close();
}

/******************************************************************************/
