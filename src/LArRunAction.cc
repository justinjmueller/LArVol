#include <sstream>
#include <string>
#include <vector>

#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArAnalysisTools.hh"

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
  dparticle(),
  output_name("output.root"),
  write_voxel_tuple(false),
  write_target_tuple(false),
  write_histograms(true)
{
  messenger = new G4GenericMessenger(this, "/larvol/", "Base for LArVol parameters.");
  messenger->DeclareMethod("output", &LArRunAction::SetOutputName);
  messenger->DeclareMethod("write_voxel_tuple", &LArRunAction::SetWriteVoxelTuple);
  messenger->DeclareMethod("write_target_tuple", &LArRunAction::SetWriteTargetTuple);
  messenger->DeclareMethod("write_histograms", &LArRunAction::SetWriteHistograms);
}

LArRunAction::~LArRunAction()
{ }

void LArRunAction::SetOutputName(G4String n) { output_name = n; }
void LArRunAction::SetWriteVoxelTuple(G4String n) { write_voxel_tuple = (n == "true"); }
void LArRunAction::SetWriteTargetTuple(G4String n) { write_target_tuple = (n == "true"); }
void LArRunAction::SetWriteHistograms(G4String n) { write_histograms = (n == "true"); }

void LArRunAction::BeginOfRunAction(const G4Run*)
{ 
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Open the analysis output file.
  auto mgr = G4AnalysisManager::Instance();
  
#ifdef G4MULTITHREADED
  mgr->SetNtupleMerging(true);
#endif
  
  mgr->SetDefaultFileType("root");
  mgr->OpenFile(output_name);

  if(write_voxel_tuple) analysis::create_voxel_tuple(dparticle);
  if(write_target_tuple) analysis::create_target_tuple();
  if(write_histograms) analysis::create_histograms();
}

void LArRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nevents = run->GetNumberOfEvent();
  if (nevents == 0) return;

  auto mgr = G4AnalysisManager::Instance();
  mgr->Write();
  mgr->CloseFile();

  if (IsMaster()) {
    G4cout
      << G4endl
      << "--------------------End of Global Run-----------------------"
      << G4endl;
  }
}
