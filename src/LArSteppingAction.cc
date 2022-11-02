#include "LArSteppingAction.hh"
#include "LArEventAction.hh"
#include "LArAnalysisTools.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"
#include "LArConfiguration.hh"

LArSteppingAction::LArSteppingAction(LArEventAction* eact, bool wtt)
  : G4UserSteppingAction(),
    event_action(eact),
    write_target_tuple(wtt)
{ }

LArSteppingAction::~LArSteppingAction()
{ }

void LArSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = (G4Track*)(step->GetTrack());  
  G4double ekin = track->GetKineticEnergy();

  if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "sv_target" &&
     step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "sv_world")
  {
    auto vtx = track->GetVertexPosition();
    event_action->AddExitingParticle(analysis::particle(track->GetDynamicParticle()->GetPDGcode(),
							track->GetTrackID(),
							track->GetParentID(),
							track->GetKineticEnergy(),
							vtx.x(), vtx.y(), vtx.z(), 0));

    G4AnalysisManager* mgr = G4AnalysisManager::Instance();
    G4int evt(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    mgr->FillNtupleIColumn(1, 0, track->GetTrackID());
    mgr->FillNtupleIColumn(1, 1, evt);
    mgr->FillNtupleIColumn(1, 2, track->GetDynamicParticle()->GetPDGcode());
    mgr->FillNtupleFColumn(1, 3, track->GetKineticEnergy());
    mgr->AddNtupleRow(1);

    if(track->GetDynamicParticle()->GetCharge() != 0)
      track->SetTrackStatus(fStopAndKill);

  }
  
  if(ekin < 10.0*CLHEP::MeV && track->GetVolume()->GetName() != "rp_active_voxels")
    track->SetTrackStatus(fStopAndKill);
}
