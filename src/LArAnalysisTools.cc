#include <vector>
#include <map>
#include <string>

#include "LArAnalysisTools.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#define PDG {-11, 11, -13, 13, 22, -211, 211, 130, 310, -321, 321, 111, 2112, 2212, 3222, 3112, 3212, 1000010020, 1000010030, 1000020030, 1000020040}
#define INTV {-321, -211, 211, 321, 2212, 1000010020}

namespace analysis
{
  
  void create_histograms()
  {
    auto mgr = G4AnalysisManager::Instance();
    std::vector<int32_t> pdgs = PDG;
    for(auto v : pdgs)
    {
      auto pname = std::to_string(v);
      if(v < 0) pname[0] = 'm';
      else pname = "p"+pname;
      // Target-exiting tracks.
      mgr->CreateH1("target_exiting_" + pname + "_energy",
		    "Energy", 5000, 0, 5.0*GeV);
      mgr->CreateH1("target_exiting_" + pname + "_xcoord",
		    "X-Coordinate", 1000, -15*cm, 15*cm);
      mgr->CreateH1("target_exiting_" + pname + "_ycoord",
		    "Y-Coordinate", 1000, -15*cm, 15*cm);
      // Active Volume tracks.
      mgr->CreateH1("active_" + pname + "_energy",
		    "Energy", 5000, 0, 5.0*GeV);
      mgr->CreateH1("active_" + pname + "_xcoord",
		    "X-Coordinate", 1401, -700.5*cm, 700.5*cm);
      mgr->CreateH1("active_" + pname + "_ycoord",
		    "Y-Coordinate", 1200, -600*cm, 600*cm);
      mgr->CreateH1("active_" + pname + "_zcoord",
		    "Z-Coordinate", 5820, -2910*cm, 2910*cm);
    }
    std::vector<int32_t> intv_pdgs = INTV;
    for(auto v: intv_pdgs)
    {
      auto pname = std::to_string(v);
      if(v < 0) pname[0] = 'm';
      else pname = "p"+pname;

      mgr->CreateH1("intv_" + pname + "_energy",
		    "Energy", 5000, 0, 0.5*GeV);
      mgr->CreateH1("intv_" + pname + "_length",
		    "Length", 10000, 0, 1000*cm);
    }
  }

  void fill_exiting_histograms(std::vector<particle>& particles)
  {
    auto mgr = G4AnalysisManager::Instance();
    std::vector<int32_t> pdgs = PDG;
    std::map<int32_t, size_t> pmap;
    for(size_t i(0); i < pdgs.size(); ++i)
      pmap[pdgs[i]] = i;
    for(auto p : particles)
    {
      mgr->FillH1(7*pmap[p.pdg]+0, p.energy);
      mgr->FillH1(7*pmap[p.pdg]+1, p.x);
      mgr->FillH1(7*pmap[p.pdg]+2, p.y);
    }
  }

  void fill_active_histograms(std::vector<particle>& particles)
  {
    auto mgr = G4AnalysisManager::Instance();
    std::vector<int32_t> pdgs = PDG;
    std::vector<int32_t> intv = INTV;
    std::map<int32_t, size_t> pmap, pmap_intv;
    for(size_t i(0); i < pdgs.size(); ++i)
      pmap[pdgs[i]] = i;
    for(size_t i(0); i < intv.size(); ++i)
      pmap_intv[intv[i]] = i;
    for(auto p : particles)
    {
      if(pmap.find(p.pdg) != pmap.end())
      {
	mgr->FillH1(7*pmap[p.pdg]+3, p.energy);
	mgr->FillH1(7*pmap[p.pdg]+4, p.x);
	mgr->FillH1(7*pmap[p.pdg]+5, p.y);
	mgr->FillH1(7*pmap[p.pdg]+6, p.z);
      }
      if(p.intv && pmap_intv.find(p.pdg) != pmap.end())
      {
	mgr->FillH1(7*pdgs.size()+2*pmap_intv[p.pdg]+0, p.energy);
	mgr->FillH1(7*pdgs.size()+2*pmap_intv[p.pdg]+1, p.length);
      }
    }
  }

  void find_intv(std::vector<particle>& particles)
  {
    std::map<uint32_t, std::vector<uint32_t>> hierarchy;
    std::map<uint32_t, size_t> pmap;
    std::vector<int32_t> pdgs = INTV;
    for(size_t i(0); i < particles.size(); ++i)
    {
      if(hierarchy.find(particles[i].parent_id) != hierarchy.end())
	hierarchy[particles[i].parent_id].push_back(particles[i].track_id);
      else
	hierarchy[particles[i].parent_id] = std::vector<uint32_t>(1, particles[i].track_id);
      pmap[particles[i].track_id] = i;
    }
    for(auto &p : hierarchy)
    {
      std::vector<bool> passes;
      // Identify parent neutrons.
      if(pmap.find(p.first) != pmap.end() && particles[pmap[p.first]].pdg == 2112)
      {
        for(auto &d : p.second)
	  passes.push_back(particles[pmap[d]].length > 10.0 && std::find(pdgs.begin(), pdgs.end(), particles[pmap[d]].pdg) != pdgs.end());
	if(std::count(passes.begin(), passes.end(), true) >= 1)
	{
	  for(size_t i(0); i < passes.size(); ++i)
	    if(passes[i]) particles[pmap[p.second[i]]].intv = true;
	}
      }
    }
  }
}
