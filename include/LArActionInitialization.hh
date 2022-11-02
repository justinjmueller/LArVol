#ifndef LArActionInitialization_h
#define LArActionInitialization_h

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"
#include "G4GenericMessenger.hh"

class LArActionInitialization : public G4VUserActionInitialization
{
public:
  LArActionInitialization();
  virtual ~LArActionInitialization();
  void SetOutputName(G4String n);
  void SetWriteVoxelTuple(G4String n);
  void SetWriteTargetTuple(G4String n);
  void SetWriteHistograms(G4String n);
  virtual void BuildForMaster() const;
  virtual void Build() const;
private:
  G4String output_name;
  bool write_voxel_tuple;
  bool write_target_tuple;
  bool write_histograms;
  G4GenericMessenger* messenger;
};
#endif

    
