#ifndef LArSteppingAction_h
#define LArSteppingAction_h

#include "G4UserSteppingAction.hh"
#include "LArEventAction.hh"
#include "globals.hh"

class LArEventAction;
class G4LogicalVolume;

class LArSteppingAction : public G4UserSteppingAction
{
public:
  LArSteppingAction(LArEventAction* evt);
  virtual ~LArSteppingAction();
  virtual void UserSteppingAction(const G4Step*);
private:
  LArEventAction* event_action;
};
#endif
