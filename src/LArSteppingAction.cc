#include "LArSteppingAction.hh"
#include "LArEventAction.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"

LArSteppingAction::LArSteppingAction()
  : G4UserSteppingAction()
{ }

LArSteppingAction::~LArSteppingAction()
{ }

void LArSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = (G4Track*)(step->GetTrack());  
  G4double ekin = track->GetKineticEnergy();

  /*if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "sv_target0" &&
     step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "sv_world")
  {
    G4AnalysisManager* mgr = G4AnalysisManager::Instance();
    G4int evt(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    mgr->FillNtupleIColumn(1, 0, evt);
    mgr->FillNtupleIColumn(1, 1, track->GetDynamicParticle()->GetPDGcode());
    mgr->FillNtupleFColumn(1, 2, track->GetKineticEnergy());
    mgr->AddNtupleRow(1);
    }*/
  
  if(ekin < 10.0*CLHEP::MeV && track->GetVolume()->GetName() != "rp_active_voxels")
    track->SetTrackStatus(fStopAndKill);
}
