#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"

/*****************************************************************/

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  mParticleSource = new G4GeneralParticleSource();
}

/*****************************************************************/

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete mParticleSource;
}

/*****************************************************************/

void
PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  mParticleSource->GeneratePrimaryVertex(event);
}
