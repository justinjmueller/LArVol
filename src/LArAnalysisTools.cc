#include <vector>
#include <map>
#include <string>

#include "LArAnalysisTools.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#define PDG {-11, 11, -13, 13, 22, -211, 211, 130, 310, -321, 321, 111, 2112, 2212, 3222, 3112, 3212, 1000010020, 1000010030, 1000020030, 1000030040}

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
    std::map<int32_t, size_t> pmap;
    for(size_t i(0); i < pdgs.size(); ++i)
      pmap[pdgs[i]] = i;
    for(auto p : particles)
    {
      if(pmap.find(p.pdg) != pmap.end())
      {
	mgr->FillH1(7*pmap[p.pdg]+3, p.energy);
	mgr->FillH1(7*pmap[p.pdg]+4, p.x);
	mgr->FillH1(7*pmap[p.pdg]+5, p.y);
	mgr->FillH1(7*pmap[p.pdg]+6, p.z);
      }
    }
  }
}
