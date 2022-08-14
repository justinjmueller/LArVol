#include "LArPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

LArPrimaryGeneratorAction::LArPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  particle_gun(0), 
  cryo(0),
  target(0)
{
  G4int nparticle = 1;
  particle_gun  = new G4ParticleGun(nparticle);

  G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  G4String name;
  G4ParticleDefinition* particle(table->FindParticle(name="e-"));
  particle_gun->SetParticleDefinition(particle);
  particle_gun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particle_gun->SetParticleEnergy(5.*GeV);
}

LArPrimaryGeneratorAction::~LArPrimaryGeneratorAction()
{
  delete particle_gun;
}

void LArPrimaryGeneratorAction::GeneratePrimaries(G4Event* evt)
{
  G4double x(0);
  G4double y(0);
  G4double z(0);

  if(!cryo)
  {
    G4LogicalVolume* vol_cryo(G4LogicalVolumeStore::GetInstance()->GetVolume("lv_cryostat_outer"));
    if(vol_cryo) cryo = dynamic_cast<G4Box*>(vol_cryo->GetSolid());
  }
  if(!target)
  {
    G4LogicalVolume* vol_target(G4LogicalVolumeStore::GetInstance()->GetVolume("lv_target"));
    if(vol_target) target = dynamic_cast<G4Box*>(vol_target->GetSolid());
  }

  if(cryo && target)
  {
    x = 0.0;
    y = 0.0;
    z = -1.0*(cryo->GetZHalfLength() + 10*cm + target->GetZHalfLength()*2.0);
  }  
  else
  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("LArPrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",JustWarning,msg);
  }
  particle_gun->SetParticlePosition(G4ThreeVector(x,y,z));
  particle_gun->GeneratePrimaryVertex(evt);
}
