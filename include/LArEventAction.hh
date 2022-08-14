#ifndef LArEventAction_h
#define LArEventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"

class LArRunAction;
class LArEventAction : public G4UserEventAction
{
public:
  LArEventAction(LArRunAction* run_action);
  virtual ~LArEventAction();
  virtual void BeginOfEventAction(const G4Event* evt);
  virtual void EndOfEventAction(const G4Event* evt);
private:
  LArRunAction* run_action;
  G4int hcid;
};

#endif

    
