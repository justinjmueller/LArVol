#ifndef LArDetectorConstruction_h
#define LArDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class LArDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  LArDetectorConstruction();
  virtual ~LArDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  G4LogicalVolume* GetActiveVolume() const { return active_volume; };

  protected:
    G4LogicalVolume*  active_volume;
};
#endif
