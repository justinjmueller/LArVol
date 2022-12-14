#include "LArEventAction.hh"
#include "LArRunAction.hh"
#include "LArVoxelHit.hh"
#include "LArAnalysisTools.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

LArEventAction::LArEventAction(LArRunAction* raction, bool wvt, bool wh)
: G4UserEventAction(),
  run_action(raction),
  write_voxel_tuple(wvt),
  write_histograms(wh),
  hcid(-1),
  particle_list() { } 

LArEventAction::~LArEventAction() { }

void LArEventAction::BeginOfEventAction(const G4Event*)
{
  G4SDManager* sd = G4SDManager::GetSDMpointer();
  if(hcid < 0)
    hcid = sd->GetCollectionID("voxel_hit_collection");
}

void LArEventAction::EndOfEventAction(const G4Event* evt)
{
  if(hcid < 0)
    return;

  G4HCofThisEvent* hce = evt->GetHCofThisEvent();
  auto hit_collection
    = static_cast<LArVoxelHitCollection*>(hce->GetHC(hcid));

  
  G4AnalysisManager* mgr = G4AnalysisManager::Instance();
  std::map<G4int, analysis::particle> tracks;
  std::vector<analysis::particle> active_particles;
  for(size_t n(0); n < hit_collection->entries(); ++n)
  {
    auto h = (*hit_collection)[n];
    auto id = h->GetID();
    if(tracks.find(h->GetID()) == tracks.end())
    {
      tracks.insert(std::make_pair(id, analysis::particle()));
      tracks[id].track_id = id;
      tracks[id].event_id = h->GetEvent();
      tracks[id].pdg = h->GetPDG();
      tracks[id].parent_id = h->GetParentID();
      tracks[id].parent_pdg = particle_list.at(h->GetParentID());
      tracks[id].creator_process = h->GetCreatorProcess();
      tracks[id].vertex_energy = h->GetVertexEnergy();
      tracks[id].x = h->GetVertexX();
      tracks[id].y = h->GetVertexY();
      tracks[id].z = h->GetVertexZ();
      tracks[id].px = h->GetMomX();
      tracks[id].py = h->GetMomY();
      tracks[id].pz = h->GetMomZ();
      tracks[id].current_energy = h->GetCurrentEnergy();
    }
    if(h->GetEnergy() > 0.1)
    {
      tracks[id].vox_x.push_back(h->GetVoxX());
      tracks[id].vox_y.push_back(h->GetVoxY());
      tracks[id].vox_z.push_back(h->GetVoxZ());
      tracks[id].energy.push_back(h->GetEnergy());
      tracks[id].destruction_energy = h->GetCurrentEnergy();
    }
  }
  for(auto& m : tracks)
  {
    //if(write_voxel_tuple)
    //{
      run_action->dparticle = m.second;
      mgr->FillNtupleIColumn(0, 0, m.second.track_id);
      mgr->FillNtupleIColumn(0, 1, m.second.event_id);
      mgr->FillNtupleIColumn(0, 2, m.second.pdg);
      mgr->FillNtupleIColumn(0, 3, m.second.parent_id);
      mgr->FillNtupleIColumn(0, 4, m.second.parent_pdg);
      mgr->FillNtupleSColumn(0, 5, m.second.creator_process);
      mgr->FillNtupleFColumn(0, 6, m.second.vertex_energy);
      mgr->FillNtupleFColumn(0, 7, m.second.current_energy);
      mgr->FillNtupleFColumn(0, 8, m.second.destruction_energy);
      mgr->FillNtupleFColumn(0, 9, m.second.x);
      mgr->FillNtupleFColumn(0, 10, m.second.y);
      mgr->FillNtupleFColumn(0, 11, m.second.z);
      mgr->FillNtupleFColumn(0, 12, m.second.px);
      mgr->FillNtupleFColumn(0, 13, m.second.py);
      mgr->FillNtupleFColumn(0, 14, m.second.pz);
      mgr->AddNtupleRow(0);
      //}

    auto length = m.second.vox_x.size() > 2 ? (std::sqrt(std::pow(m.second.vox_x.front() - m.second.vox_x.back(), 2) +
							 std::pow(m.second.vox_y.front() - m.second.vox_y.back(), 2) +
							 std::pow(m.second.vox_z.front() - m.second.vox_z.back(), 2))) : 0;
    active_particles.push_back(analysis::particle(m.second.pdg,
						  m.second.track_id,
						  m.second.parent_id,
						  m.second.current_energy,
						  m.second.x,
						  m.second.y,
						  m.second.z,
						  0.3*length));
  }
  particle_list.clear();

  /*if(write_histograms)
    {*/
  analysis::find_intv(active_particles);
  //analysis::fill_exiting_histograms(exiting_particles);
  //analysis::fill_active_histograms(active_particles);
  analysis::fill_intv_tuple(active_particles);
    /*}*/
  exiting_particles.clear();
  active_particles.clear();
}
