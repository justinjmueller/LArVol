#include "LArVoxelHit.hh"

LArVoxelHit::LArVoxelHit(const G4int i)
  : id(i)
{ }

LArVoxelHit::~LArVoxelHit() {}

void LArVoxelHit::Print()
{
  G4cout << "Hit with ID: " << id << " (PID: "
         << pdg << ") at Voxel("
         << vox_x << ", "
         << vox_y << ", "
         << vox_z << ") with "
         << energy << " MeV deposited."
         << G4endl;
}
