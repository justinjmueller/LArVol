#ifndef LArSensitiveDetector_h
#define LArSensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "LArVoxelHit.hh"

class G4Step;
class G4HCofThisEvent;

class LArSensitiveDetector : public G4VSensitiveDetector
{
public:
  LArSensitiveDetector(G4String name);
  ~LArSensitiveDetector();
  G4bool ProcessHits(G4Step *step, G4TouchableHistory*);
  void Initialize(G4HCofThisEvent* hce);
  void EndOfEvent(G4HCofThisEvent*);
private:
  LArVoxelHitCollection* hit_collection;
};

#endif
