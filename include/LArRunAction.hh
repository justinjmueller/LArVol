#ifndef LArRunAction_h
#define LArRunAction_h 

#include "LArEventAction.hh"
#include "LArVoxTrack.cc"

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
  VoxTrack vox_track;
};

#endif

