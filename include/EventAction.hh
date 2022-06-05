#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
#include <array>

namespace FP
{


class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep) {fEdep += edep;}

    std::vector<G4double> fSiHitsX;
    std::vector<G4double> fSiHitsY;
    std::vector<G4double> fSiHitsZ;
    std::vector<G4double> fSiHitsEdep;
    std::vector<G4int>    fDetID;
  private:
    RunAction* fRunAction = nullptr;
    G4double   fEdep      = 0.;


};

}

#endif
