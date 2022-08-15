#ifndef LArVoxelHit_h
#define LArVoxelHit_h

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class LArVoxelHit : public G4VHit
{
public:
  LArVoxelHit(const G4int i);
  ~LArVoxelHit();
  void Print();
  void SetEvent(const G4int e)
  { event = e; };
  void SetPDG(const G4int p)
  { pdg = p; };
  void SetParentID(const G4int p)
  { parent_id = p; };
  void SetCreatorProcess(const G4String s)
  { creator_process = s; };
  void SetVertexEnergy(const G4double e)
  { vertex_energy = e; };
  void SetVertex(const G4double cx, const G4double cy, const G4double cz)
  { vertex_x = cx; vertex_y = cy; vertex_z = cz; };
  void SetMomentum(const G4double px, const G4double py, const G4double pz)
  { vertex_px = px; vertex_py = py; vertex_pz = pz; };
  void SetVox(const G4int cx, const G4int cy, const G4int cz)
  { vox_x = cx; vox_y = cy; vox_z = cz; };
  void SetEnergy(const G4double e)
  { energy = e; };
  G4int GetID() { return id; };
  G4int GetEvent() { return event; };
  G4int GetPDG() { return pdg; };
  G4int GetParentID() { return parent_id; };
  G4String GetCreatorProcess() { return creator_process; };
  G4double GetVertexEnergy() { return vertex_energy; };
  G4double GetVertexX() { return vertex_x; };
  G4double GetVertexY() { return vertex_y; };
  G4double GetVertexZ() { return vertex_z; };
  G4double GetMomX() { return vertex_px; };
  G4double GetMomY() { return vertex_py; };
  G4double GetMomZ() { return vertex_pz; };
  G4int GetVoxX() { return vox_x; };
  G4int GetVoxY() { return vox_y; };
  G4int GetVoxZ() { return vox_z; };
  G4double GetEnergy() { return energy; };

private:
  G4int id;
  G4int event;
  G4int pdg;
  G4int parent_id;
  G4String creator_process;
  G4double vertex_energy;
  G4double vertex_x, vertex_y, vertex_z;
  G4double vertex_px, vertex_py, vertex_pz;
  G4int vox_x, vox_y, vox_z;
  G4double energy;
};

typedef G4THitsCollection<LArVoxelHit> LArVoxelHitCollection;

#endif
