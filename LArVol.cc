#include "LArVoxelizedDetectorConstruction.hh"
#include "LArActionInitialization.hh"
#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include <time.h>

int main(int argc,char** argv)
{
  G4UIExecutive* ui = 0;
  if ( argc == 1 )
    ui = new G4UIExecutive(argc, argv);

  G4int seed(time(0));
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(seed);
  
  auto* run_manager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  run_manager->SetUserInitialization(new LArVoxelizedDetectorConstruction());

  G4VModularPhysicsList* physics_list = new QGSP_BERT_HP;
  physics_list->SetVerboseLevel(0);
  run_manager->SetUserInitialization(physics_list);
 
  run_manager->SetUserInitialization(new LArActionInitialization());
  
  G4VisManager* vis_manager = new G4VisExecutive;
  vis_manager->Initialize();
  G4UImanager* ui_manager = G4UImanager::GetUIpointer();
  
  if(!ui)
  { 
    G4String command = "/control/execute ";
    G4String file_name = argv[1];
    ui_manager->ApplyCommand(command+file_name);
  }
  else
  { 
    ui_manager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  delete vis_manager;
  delete run_manager;
}
