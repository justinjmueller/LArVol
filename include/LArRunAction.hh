#ifndef LArRunAction_h
#define LArRunAction_h 

#include "LArEventAction.hh"
#include "LArAnalysisTools.hh"

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"

class G4Run;

class LArRunAction : public G4UserRunAction
{
public:
  LArRunAction();
  virtual ~LArRunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  void SetOutputName(G4String n);
  void SetWriteVoxelTuple(G4String n);
  void SetWriteTargetTuple(G4String n);
  void SetWriteHistograms(G4String n);
  bool GetWriteVoxelTuple() { return write_voxel_tuple; };
  bool GetWriteTargetTuple() { return write_target_tuple; };
  bool GetWriteHistograms() { return write_histograms; };
  analysis::particle dparticle;
private:
  G4String output_name;
  bool write_voxel_tuple;
  bool write_target_tuple;
  bool write_histograms;
  G4GenericMessenger* messenger;
};

#endif

