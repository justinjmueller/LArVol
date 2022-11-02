#include "LArActionInitialization.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArTrackingAction.hh"
#include "LArSteppingAction.hh"

#include "G4String.hh"

LArActionInitialization::LArActionInitialization()
  : G4VUserActionInitialization(),
    output_name("output.root"),
    write_voxel_tuple(false),
    write_target_tuple(false),
    write_histograms(true)
{
  messenger = new G4GenericMessenger(this, "/larvol/", "Base for LArVol parameters.");
  messenger->DeclareMethod("output", &LArActionInitialization::SetOutputName);
  messenger->DeclareMethod("write_voxel_tuple", &LArActionInitialization::SetWriteVoxelTuple);
  messenger->DeclareMethod("write_target_tuple", &LArActionInitialization::SetWriteTargetTuple);
  messenger->DeclareMethod("write_histograms", &LArActionInitialization::SetWriteHistograms);
}

LArActionInitialization::~LArActionInitialization()
{
  delete messenger;
}

void LArActionInitialization::SetOutputName(G4String n) { output_name = n; }
void LArActionInitialization::SetWriteVoxelTuple(G4String n) { write_voxel_tuple = (n == "true"); }
void LArActionInitialization::SetWriteTargetTuple(G4String n) { write_target_tuple = (n == "true"); }
void LArActionInitialization::SetWriteHistograms(G4String n) { n+'d'; write_histograms = (n == "true"); }

void LArActionInitialization::BuildForMaster() const
{
  LArRunAction* run_action = new LArRunAction(output_name,
					      write_voxel_tuple,
					      write_target_tuple,
					      write_histograms);
  SetUserAction(run_action);
}

void LArActionInitialization::Build() const
{
  SetUserAction(new LArPrimaryGeneratorAction);
  LArRunAction* run_action = new LArRunAction(output_name,
					      write_voxel_tuple,
					      write_target_tuple,
					      write_histograms);
  SetUserAction(run_action);
  
  LArEventAction* event_action = new LArEventAction(run_action,
						    write_voxel_tuple,
						    write_histograms);
  SetUserAction(event_action);

  SetUserAction(new LArTrackingAction(event_action));
  
  SetUserAction(new LArSteppingAction(event_action,
				      write_target_tuple));
}
