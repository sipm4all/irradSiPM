#include "EventAction.hh"
#include "G4Event.hh"

/******************************************************************************/

void
EventAction::BeginOfEventAction(const G4Event *aEvent)
{

  //  std::cout << "--- start of event: " << aEvent->GetEventID() << std::endl;

}

/******************************************************************************/

void
EventAction::EndOfEventAction(const G4Event *aEvent)
{

  //  std::cout << "--- end of event: " << aEvent->GetEventID() << std::endl;

}

/******************************************************************************/
