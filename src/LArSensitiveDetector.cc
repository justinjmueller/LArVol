#include "LArSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

LArSensitiveDetector::LArSensitiveDetector(G4String name)
  : G4VSensitiveDetector(name)
{
  G4cout << "Creating Sensitive Detector with name: " << name << G4endl;
  collectionName.insert("voxel_hit_collection");
}

LArSensitiveDetector::~LArSensitiveDetector() {}

void LArSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  hit_collection = new LArVoxelHitCollection(GetName(), collectionName[0]);
  static G4int hcid = -1;
  if (hcid < 0) hcid = GetCollectionID(0);
  hce->AddHitsCollection(hcid, hit_collection);
}

G4bool LArSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory*)
{
  G4StepPoint* pre = step->GetPreStepPoint();
  G4TouchableHistory* touchable = (G4TouchableHistory*)(pre->GetTouchable());
  G4Track* track = step->GetTrack();
  G4double energy(step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit());
  if(track->GetKineticEnergy() > 1.0 && touchable->GetVolume()->GetName() == "rp_active_voxels")
  {
    LArVoxelHit* hit = new LArVoxelHit(step->GetTrack()->GetTrackID());
    hit->SetEvent(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    hit->SetPDG(track->GetDynamicParticle()->GetPDGcode());
    hit->SetParentID(track->GetParentID());
    hit->SetCreatorProcess(track->GetCreatorProcess()->GetProcessName());
    hit->SetVertexEnergy(track->GetVertexKineticEnergy());
    const auto& vertex = track->GetVertexPosition();
    hit->SetVertex(vertex.getX(), vertex.getY(), vertex.getZ());
    const auto& momentum = track->GetMomentumDirection();
    hit->SetMomentum(momentum.getX(), momentum.getY(), momentum.getZ());
    hit->SetVox(touchable->GetReplicaNumber(2),
		touchable->GetReplicaNumber(1),
		touchable->GetReplicaNumber(0));
    hit->SetEnergy(energy);
    hit->SetCurrentEnergy(pre->GetKineticEnergy());
    hit_collection->insert(hit);
  }
  return true;
}

void LArSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{ }
