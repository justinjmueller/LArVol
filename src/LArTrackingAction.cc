#include "LArTrackingAction.hh"

#include "G4PhysicalConstants.hh"
#include "LArEventAction.hh"
#include "G4RunManager.hh"

LArTrackingAction::LArTrackingAction(LArEventAction* evt)
  : G4UserTrackingAction(),
    event_action(evt)
{}

void LArTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  event_action->GetParticleList().insert(std::make_pair(track->GetTrackID(),
							track->GetDynamicParticle()->GetPDGcode()));
}
