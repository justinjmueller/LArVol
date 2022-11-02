#include <sstream>
#include <string>
#include <vector>

#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArAnalysisTools.hh"
#include "LArConfiguration.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4GenericMessenger.hh"
#include "G4AccumulableManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

LArRunAction::LArRunAction(G4String on, bool wvt, bool wtt, bool wh)
: G4UserRunAction(),
  dparticle(),
  output_name(on),
  write_voxel_tuple(wvt),
  write_target_tuple(wtt),
  write_histograms(wh)
{ }

LArRunAction::~LArRunAction()
{ }

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

  //if(write_histograms) analysis::create_histograms();
  //if(write_voxel_tuple)
  analysis::create_voxel_tuple(dparticle);
  //  if(write_target_tuple)
  analysis::create_target_tuple();
  analysis::create_intv_tuple();
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
