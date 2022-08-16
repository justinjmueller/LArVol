#include "LArVoxelizedDetectorConstruction.hh"
#include "LArActionInitialization.hh"
#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

int main(int argc,char** argv)
{
  G4UIExecutive* ui = 0;
  if ( argc == 1 )
    ui = new G4UIExecutive(argc, argv);

  auto* run_manager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  run_manager->SetUserInitialization(new LArVoxelizedDetectorConstruction(1));

  G4VModularPhysicsList* physics_list = new FTFP_BERT;
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
