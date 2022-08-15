#include <sstream>
#include <string>
#include <vector>
#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArDetectorConstruction.hh"
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
  vox_track()
{ }

LArRunAction::~LArRunAction()
{ }

void LArRunAction::BeginOfRunAction(const G4Run*)
{ 
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Open the analysis output file.
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->SetNtupleMerging(true);
  analysis_manager->OpenFile("output_5GeV.root");

  analysis_manager->CreateNtuple("voxels", "Voxel information");
  analysis_manager->CreateNtupleIColumn(0, "track_id");
  analysis_manager->CreateNtupleIColumn(0, "event_id");
  analysis_manager->CreateNtupleIColumn(0, "pdg");
  analysis_manager->CreateNtupleIColumn(0, "parent_pid");
  analysis_manager->CreateNtupleSColumn(0, "creator_process");
  analysis_manager->CreateNtupleFColumn(0, "vertex_energy");
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
