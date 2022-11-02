#ifndef LArRunAction_h
#define LArRunAction_h 

#include "LArEventAction.hh"
#include "LArAnalysisTools.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class LArRunAction : public G4UserRunAction
{
public:
  LArRunAction(G4String on, bool wvt, bool wtt, bool wh);
  virtual ~LArRunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  bool GetWriteVoxelTuple() { return write_voxel_tuple; };
  bool GetWriteTargetTuple() { return write_target_tuple; };
  bool GetWriteHistograms() { return write_histograms; };
  analysis::particle dparticle;
private:
  G4String output_name;
  bool write_voxel_tuple;
  bool write_target_tuple;
  bool write_histograms;
};
#endif

