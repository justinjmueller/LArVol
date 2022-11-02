#ifndef LArAnalysisTools_h
#define LArAnalysisTools_h

#include <vector>
#include <string>
#include "G4String.hh"

namespace analysis
{
  struct particle
  {
    uint32_t track_id;
    uint64_t event_id;    
    int32_t pdg;
    uint32_t parent_id;
    uint32_t parent_pdg;
    G4String creator_process;
    float vertex_energy;
    float current_energy;
    float destruction_energy;
    float x, y, z;
    float px, py, pz;
    std::vector<G4int> vox_x, vox_y, vox_z;
    std::vector<float> energy;
    uint16_t intv_class;
    float length;
    particle();
    particle(int32_t p, uint32_t tid, uint32_t pid, float e, float xi, float yi, float zi, float l);
  };

  void create_histograms();
  void fill_exiting_histograms(std::vector<particle>& particles);
  void fill_active_histograms(std::vector<particle>& particles);
  void find_intv(std::vector<particle>& particles);
  void create_voxel_tuple(particle& p);
  void create_target_tuple();
  void create_intv_tuple();
  void fill_intv_tuple(std::vector<particle>& particles);
}
#endif
