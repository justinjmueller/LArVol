#ifndef LArAnalysisTools_h
#define LArAnalysisTools_h

#include <vector>
#include <string>

namespace analysis
{
  struct particle
  {
    int32_t pdg;
    uint32_t track_id;
    uint32_t parent_id;
    float energy;
    float x, y, z;
    bool intv;
    float length;
    particle(int32_t p, uint32_t tid, uint32_t pid, float e, float xi, float yi, float zi, float l)
      : pdg(p),
	track_id(tid),
	parent_id(pid),
	energy(e),
	x(xi), y(yi), z(zi),
	intv(false),
	length(l) {}
  };

  void create_histograms();
  void fill_exiting_histograms(std::vector<particle>& particles);
  void fill_active_histograms(std::vector<particle>& particles);
  void find_intv(std::vector<particle>& particles);
}
#endif
