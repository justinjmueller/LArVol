#ifndef LArSteppingAction_h
#define LArSteppingAction_h

#include "LArEventAction.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;
class LArSteppingAction : public G4UserSteppingAction
{
public:
  LArSteppingAction(LArEventAction* eact, bool wtt);
  virtual ~LArSteppingAction();
  virtual void UserSteppingAction(const G4Step*);
private:
  LArEventAction* event_action;
  bool write_target_tuple;
};
#endif
