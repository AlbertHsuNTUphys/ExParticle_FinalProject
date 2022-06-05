#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;

namespace FP
{
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step*) override;

  private:
    EventAction* fEventAction = nullptr;
};

}

#endif
