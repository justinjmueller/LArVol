#ifndef LArActionInitialization_h
#define LArActionInitialization_h

#include "G4VUserActionInitialization.hh"

class LArActionInitialization : public G4VUserActionInitialization
{
public:
  LArActionInitialization();
  virtual ~LArActionInitialization();
  virtual void BuildForMaster() const;
  virtual void Build() const;
};
#endif

    
