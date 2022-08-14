#ifndef LArVoxelizedDetectorConstruction_h
#define LArVoxelizedDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class LArVoxelizedDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  LArVoxelizedDetectorConstruction();
  virtual ~LArVoxelizedDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  G4LogicalVolume* GetActiveVolume() const { return active_volume; };
protected:
    G4LogicalVolume*  active_volume;
};
#endif
