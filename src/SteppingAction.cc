#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

namespace FP
{
  SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
  {}

  SteppingAction::~SteppingAction()
  {}

  void SteppingAction::UserSteppingAction(const G4Step* step)
  {
    G4double edepStep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edepStep);
  }


}
