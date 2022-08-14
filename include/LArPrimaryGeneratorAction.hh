#ifndef LArPrimaryGeneratorAction_h
#define LArPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;
class LArPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  LArPrimaryGeneratorAction();    
  virtual ~LArPrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event* evt);         
  const G4ParticleGun* GetParticleGun() const
  { return particle_gun; };
private:
  G4ParticleGun*  particle_gun;
  G4Box* cryo;
  G4Box* target;
};

#endif
