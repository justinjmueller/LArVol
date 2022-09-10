#include <sstream>
#include <string>
#include <vector>
#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArVoxTrack.cc"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4GenericMessenger.hh"
#include "G4AccumulableManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

LArRunAction::LArRunAction()
: G4UserRunAction(),
  vox_track(),
  output_name("output.root")
{
  messenger = new G4GenericMessenger(this, "/larvol/", "Base for LArVol parameters.");
  messenger->DeclareMethod("output", &LArRunAction::SetOutputName);
}

LArRunAction::~LArRunAction()
{ }

void LArRunAction::SetOutputName(G4String n)
{
  output_name = n;
}

void LArRunAction::BeginOfRunAction(const G4Run*)
{ 
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Open the analysis output file.
  auto analysis_manager = G4AnalysisManager::Instance();
  #ifdef G4MULTITHREADED
  analysis_manager->SetNtupleMerging(true);
  #endif
  analysis_manager->SetDefaultFileType("root");
  analysis_manager->OpenFile(output_name);

  analysis_manager->CreateNtuple("voxels", "Voxel information");
  analysis_manager->CreateNtupleIColumn(0, "track_id");
  analysis_manager->CreateNtupleIColumn(0, "event_id");
  analysis_manager->CreateNtupleIColumn(0, "pdg");
  analysis_manager->CreateNtupleIColumn(0, "parent_id");
  analysis_manager->CreateNtupleIColumn(0, "parent_pdg");
  analysis_manager->CreateNtupleSColumn(0, "creator_process");
  analysis_manager->CreateNtupleFColumn(0, "vertex_energy");
  analysis_manager->CreateNtupleFColumn(0, "start_energy");
  analysis_manager->CreateNtupleFColumn(0, "destruction_energy");
  analysis_manager->CreateNtupleFColumn(0, "vertex_x");
  analysis_manager->CreateNtupleFColumn(0, "vertex_y");
  analysis_manager->CreateNtupleFColumn(0, "vertex_z");
  analysis_manager->CreateNtupleFColumn(0, "vertex_px");
  analysis_manager->CreateNtupleFColumn(0, "vertex_py");
  analysis_manager->CreateNtupleFColumn(0, "vertex_pz");
  analysis_manager->CreateNtupleIColumn(0, "vox_x", vox_track.vox_x);
  analysis_manager->CreateNtupleIColumn(0, "vox_y", vox_track.vox_y);
  analysis_manager->CreateNtupleIColumn(0, "vox_z", vox_track.vox_z);
  analysis_manager->CreateNtupleFColumn(0, "energy", vox_track.energy);
  analysis_manager->FinishNtuple();

  /*analysis_manager->CreateNtuple("statistics", "Statistics");
  analysis_manager->CreateNtupleIColumn(1, "event_id");
  analysis_manager->CreateNtupleIColumn(1, "pdg");
  analysis_manager->CreateNtupleFColumn(1, "energy");
  analysis_manager->FinishNtuple();*/
}

void LArRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nevents = run->GetNumberOfEvent();
  if (nevents == 0) return;

  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
  
  const LArPrimaryGeneratorAction* generator_action
    = static_cast<const LArPrimaryGeneratorAction*>
    (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

  G4String run_condition;
  if(generator_action)
  {
    const G4ParticleGun* particle_gun = generator_action->GetParticleGun();
    G4double particle_energy = particle_gun->GetParticleEnergy();
    run_condition += particle_gun->GetParticleDefinition()->GetParticleName();
    run_condition += " of ";
    run_condition += G4BestUnit(particle_energy, "Energy");
  }

  if (IsMaster()) {
    G4cout
      << G4endl
      << "--------------------End of Global Run-----------------------"
      << G4endl;
  }
  else {
    G4cout
      << G4endl
      << "--------------------End of Local Run------------------------"
      << G4endl;
  }
}
