#ifndef LArSteppingAction_h
#define LArSteppingAction_h

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;
class LArSteppingAction : public G4UserSteppingAction
{
public:
  LArSteppingAction();
  virtual ~LArSteppingAction();
  virtual void UserSteppingAction(const G4Step*);
};
#endif
