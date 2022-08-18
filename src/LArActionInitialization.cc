#include "LArActionInitialization.hh"
#include "LArPrimaryGeneratorAction.hh"
#include "LArRunAction.hh"
#include "LArEventAction.hh"
#include "LArSteppingAction.hh"

LArActionInitialization::LArActionInitialization()
 : G4VUserActionInitialization()
{}

LArActionInitialization::~LArActionInitialization()
{}

void LArActionInitialization::BuildForMaster() const
{
  LArRunAction* run_action = new LArRunAction;
  SetUserAction(run_action);
}

void LArActionInitialization::Build() const
{
  SetUserAction(new LArPrimaryGeneratorAction);
  LArRunAction* run_action = new LArRunAction;
  SetUserAction(run_action);
  
  LArEventAction* event_action = new LArEventAction(run_action);
  SetUserAction(event_action);

  SetUserAction(new LArSteppingAction(event_action));
}
