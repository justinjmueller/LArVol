#include "LArEventAction.hh"
#include "LArRunAction.hh"
#include "LArVoxelHit.hh"
#include "LArVoxTrack.cc"
#include "LArAnalysisTools.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

LArEventAction::LArEventAction(LArRunAction* raction)
: G4UserEventAction(),
  run_action(raction),
  hcid(-1),
  particle_list()
{ } 

LArEventAction::~LArEventAction()
{ }

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
  std::map<G4int, VoxTrack> tracks;
  std::vector<analysis::particle> active_particles;
  for(size_t n(0); n < hit_collection->entries(); ++n)
  {
    auto h = (*hit_collection)[n];
    auto id = h->GetID();
    if(tracks.find(h->GetID()) == tracks.end())
    {
      tracks.insert(std::make_pair(id, VoxTrack()));
      tracks[id].track_id = id;
      tracks[id].event_id = h->GetEvent();
      tracks[id].pdg = h->GetPDG();
      tracks[id].parent_id = h->GetParentID();
      tracks[id].parent_pdg = particle_list.at(h->GetParentID());
      tracks[id].creator_process = h->GetCreatorProcess();
      tracks[id].vertex_energy = h->GetVertexEnergy();
      tracks[id].vertex_x = h->GetVertexX();
      tracks[id].vertex_y = h->GetVertexY();
      tracks[id].vertex_z = h->GetVertexZ();
      tracks[id].vertex_px = h->GetMomX();
      tracks[id].vertex_py = h->GetMomY();
      tracks[id].vertex_pz = h->GetMomZ();
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
    run_action->vox_track = m.second;
    mgr->FillNtupleIColumn(0, 0, m.second.track_id);
    mgr->FillNtupleIColumn(0, 1, m.second.event_id);
    mgr->FillNtupleIColumn(0, 2, m.second.pdg);
    mgr->FillNtupleIColumn(0, 3, m.second.parent_id);
    mgr->FillNtupleIColumn(0, 4, m.second.parent_pdg);
    mgr->FillNtupleSColumn(0, 5, m.second.creator_process);
    mgr->FillNtupleFColumn(0, 6, m.second.vertex_energy);
    mgr->FillNtupleFColumn(0, 7, m.second.current_energy);
    mgr->FillNtupleFColumn(0, 8, m.second.destruction_energy);
    mgr->FillNtupleFColumn(0, 9, m.second.vertex_x);
    mgr->FillNtupleFColumn(0, 10, m.second.vertex_y);
    mgr->FillNtupleFColumn(0, 11, m.second.vertex_z);
    mgr->FillNtupleFColumn(0, 12, m.second.vertex_px);
    mgr->FillNtupleFColumn(0, 13, m.second.vertex_py);
    mgr->FillNtupleFColumn(0, 14, m.second.vertex_pz);
    mgr->AddNtupleRow(0);

    active_particles.push_back(analysis::particle(m.second.pdg,
						  m.second.current_energy,
						  m.second.vertex_x,
						  m.second.vertex_y,
						  m.second.vertex_z));
  }
  particle_list.clear();

  analysis::fill_exiting_histograms(exiting_particles);
  analysis::fill_active_histograms(active_particles);
  exiting_particles.clear();
  active_particles.clear();
}
