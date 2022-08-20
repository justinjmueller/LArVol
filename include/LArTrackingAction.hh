#ifndef LArTrackingAction_h
#define LArTrackingAction_h

#include "G4UserTrackingAction.hh"
#include "LArEventAction.hh"
#include "globals.hh"

//class LArEvent;

class LArTrackingAction : public G4UserTrackingAction
{
public:
  LArTrackingAction(LArEventAction* evt);
  void PreUserTrackingAction(const G4Track*) override;
private:
  LArEventAction* event_action;
};
#endif
