#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

namespace FP
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override;
    const G4ParticleGun* GetParticleGun() const{ return fParticleGun;}
  private:
    G4ParticleGun* fParticleGun = nullptr;

};

}

#endif
