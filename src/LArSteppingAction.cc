#include "LArSteppingAction.hh"
#include "LArEventAction.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"

LArSteppingAction::LArSteppingAction(LArEventAction* evt)
  : G4UserSteppingAction(),
    event_action(evt)
{ }

LArSteppingAction::~LArSteppingAction()
{ }

void LArSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = (G4Track*)(step->GetTrack());
  if(track->GetCurrentStepNumber() == 1)
    event_action->GetParticleList().insert(std::make_pair(track->GetTrackID(), track->GetDynamicParticle()->GetPDGcode()));
  
  G4double ekin = track->GetKineticEnergy();
  if(ekin < 1.0*CLHEP::MeV) track->SetTrackStatus(fStopAndKill);
}
