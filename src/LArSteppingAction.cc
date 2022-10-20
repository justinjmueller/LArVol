#include "LArSteppingAction.hh"
#include "LArEventAction.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"

LArSteppingAction::LArSteppingAction(LArEventAction* eact)
  : G4UserSteppingAction(),
    event_action(eact)
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
    
    G4AnalysisManager* mgr = G4AnalysisManager::Instance();
    G4int evt(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    G4int parent_pdg(track->GetParentID() == 0 ? 0 : event_action->GetParticleList().at(track->GetParentID()));
    G4String creator(track->GetParentID() == 0 ? "Primary" : track->GetCreatorProcess()->GetProcessName());
    mgr->FillNtupleIColumn(1, 0, track->GetTrackID());
    mgr->FillNtupleIColumn(1, 1, evt);
    mgr->FillNtupleIColumn(1, 2, track->GetDynamicParticle()->GetPDGcode());
    //mgr->FillNtupleIColumn(1, 3, track->GetParentID());
    //mgr->FillNtupleIColumn(1, 4, parent_pdg);
    //mgr->FillNtupleSColumn(1, 5, creator);
    //mgr->FillNtupleFColumn(1, 6, track->GetVertexKineticEnergy());
    mgr->FillNtupleFColumn(1, 3, track->GetKineticEnergy());
    //mgr->FillNtupleFColumn(1, 8, track->GetVertexPosition().x());
    //mgr->FillNtupleFColumn(1, 9, track->GetVertexPosition().y());
    //mgr->FillNtupleFColumn(1, 10, track->GetVertexPosition().z());
    //mgr->FillNtupleFColumn(1, 11, track->GetVertexMomentumDirection().x());
    //mgr->FillNtupleFColumn(1, 12, track->GetVertexMomentumDirection().y());
    //mgr->FillNtupleFColumn(1, 13, track->GetVertexMomentumDirection().z());
    mgr->AddNtupleRow(1);
    
    if(track->GetDynamicParticle()->GetCharge() != 0)
      track->SetTrackStatus(fStopAndKill);

  }
  
  if(ekin < 10.0*CLHEP::MeV && track->GetVolume()->GetName() != "rp_active_voxels")
    track->SetTrackStatus(fStopAndKill);
}
