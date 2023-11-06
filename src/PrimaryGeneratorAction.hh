#ifndef _PrimaryGeneratorAction_h_
#define _PrimaryGeneratorAction_h_

#include "G4VUserPrimaryGeneratorAction.hh"

class G4event;
class G4VPrimaryGenerator;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  
public:
  
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;
  
  void GeneratePrimaries(G4Event *event) override;
  
protected:

  G4VPrimaryGenerator *mParticleSource;
  
};

#endif /** _PrimaryGeneratorAction_h_ **/
