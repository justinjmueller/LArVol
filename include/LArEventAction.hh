#ifndef LArEventAction_h
#define LArEventAction_h

#include "LArAnalysisTools.hh"

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
  void AddExitingParticle(analysis::particle p) { exiting_particles.push_back(p); };
private:
  LArRunAction* run_action;
  G4int hcid;
  std::map<G4int, G4int> particle_list;
  std::vector<analysis::particle> exiting_particles;
};

#endif

    
