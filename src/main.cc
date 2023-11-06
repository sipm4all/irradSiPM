#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "PrimaryGeneratorAction.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "DetectorConstruction.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "RootIO.hh"

int
main(int argc, char **argv)
{
  auto ui = new G4UIExecutive(argc, argv, "tcsh");

  auto run = new G4RunManager;
  auto physics = new QGSP_BERT_HP; // FTFP_BERT; // nein! QGSP_BERT_HP, QGSP_BIC_HP, QGSP_INCLXX, Shielding
  auto detector = new DetectorConstruction();
  run->SetUserInitialization(detector);
  run->SetUserInitialization(physics);
  
  //  run->Initialize();

  auto action_generator = new PrimaryGeneratorAction();
  auto action_run = new RunAction();
  auto action_event = new EventAction();

  run->SetUserAction(action_generator);
  run->SetUserAction(action_run);
  run->SetUserAction(action_event);
  
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // start interative session
  if (argc == 1) {
    RootIO::Instance()->setInteractiveMode(true);
    auto ui = new G4UIExecutive(argc, argv, "Qt");
    //    auto ui = new G4UIExecutive(argc, argv, "tcsh");
    ui->SessionStart();
    delete ui;  
    return 0;
  }

  auto uiManager = G4UImanager::GetUIpointer();
  std::string command = "/control/execute ";
  std::string fileName = argv[1];
  uiManager->ApplyCommand(command + fileName);
  
  return 0;
}
