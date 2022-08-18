#ifndef LArEventAction_h
#define LArEventAction_h

#include <map>
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
  std::map<G4int, G4int>& GetParticleList() { return particle_list; };
private:
  LArRunAction* run_action;
  G4int hcid;
  std::map<G4int, G4int> particle_list;
};

#endif

    
