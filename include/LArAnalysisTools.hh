#ifndef LArAnalysisTools_h
#define LArAnalysisTools_h

#include <vector>
#include <string>

namespace analysis
{
  struct particle
  {
    int32_t pdg;
    float energy;
    float x, y, z;
    particle(int32_t p, float e, float xi, float yi, float zi)
      : pdg(p),
	energy(e),
	x(xi), y(yi), z(zi) {}
  };

  void create_histograms();
  void fill_exiting_histograms(std::vector<particle>& particles);
  void fill_active_histograms(std::vector<particle>& particles);
}
#endif
