#ifndef LARVOXTRACK_h
#define LARVOXTRACK_h

#include <vector>

struct VoxTrack
{
  G4int track_id;
  G4int event_id;
  G4int pdg;
  G4int parent_id;
  G4String creator_process;
  float vertex_energy;
  float vertex_x;
  float vertex_y;
  float vertex_z;
  float vertex_px;
  float vertex_py;
  float vertex_pz;
  std::vector<float> x;
  std::vector<float> y;
  std::vector<float> z;
  std::vector<G4int> vox_x;
  std::vector<G4int> vox_y;
  std::vector<G4int> vox_z;
  std::vector<float> energy;
};
#endif
