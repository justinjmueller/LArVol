#ifndef LArVoxelizedDetectorConstruction_h
#define LArVoxelizedDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GenericMessenger.hh"

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
  void SetNLayers(G4String n);
protected:
  G4LogicalVolume*  active_volume;
  G4int n_layers;
  G4GenericMessenger* messenger;
};
#endif
