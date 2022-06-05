#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "TrackerHit.hh"

using std::array;
using std::vector;

namespace{

G4VHitsCollection* GetHC(const G4Event* event, G4int collId){
  auto hce = event->GetHCofThisEvent();
  if(!hce){
    G4ExceptionDescription msg;
    msg << "Oops. No hits collection of this event found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
		 "Code001", JustWarning, msg);
    return nullptr;
  }
  auto hc = hce->GetHC(collId);
  if( ! hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
	       "Code001", JustWarning, msg);
  }
  return hc;
}

}

namespace FP
{

  EventAction::EventAction()
  {
    G4RunManager::GetRunManager()->SetPrintProgress(1);
  }

  EventAction::~EventAction()
  {}

  void EventAction::BeginOfEventAction(const G4Event*)
  {
    fEdep = 0.;

    fSiHitsX.clear();
    fSiHitsY.clear();
    fSiHitsZ.clear();
    fSiHitsEdep.clear();
    fDetID.clear();
  }

  void EventAction::EndOfEventAction(const G4Event* event)
  {
    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    auto primary = event->GetPrimaryVertex(0)->GetPrimary(0);

    G4cout << G4endl
	   << ">>> End of event action : Event " << event->GetEventID() << "Simulation truth : " 
	   << primary->GetG4code()->GetParticleName()
	   << " " << primary->GetMomentum() << G4endl;

    G4cout << " fEdep " << fEdep/CLHEP::keV << G4endl;
    G4int eventID = event->GetEventID();

    auto analysisManager = G4AnalysisManager::Instance();

    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    auto nhit = hc->GetSize();
    
    analysisManager->FillH1(0, fEdep/CLHEP::MeV);
    analysisManager->FillNtupleIColumn(0, eventID);
    analysisManager->FillNtupleIColumn(1, fEdep/CLHEP::keV);
    analysisManager->FillNtupleIColumn(2, nhit);
    analysisManager->FillNtupleDColumn(3, primary->GetMomentum().x());
    analysisManager->FillNtupleDColumn(4, primary->GetMomentum().y());
    analysisManager->FillNtupleDColumn(5, primary->GetMomentum().z());

    G4cout << " fill the hit vector " << G4endl;

    for(unsigned long i = 0; i < hc->GetSize(); i++){
      auto hit = static_cast<TrackerHit*>(hc->GetHit(i));
      G4cout << " hit " << i << " detz " << hit->GetDetPos() << G4endl;
      fSiHitsX.push_back(hit->GetDetPos().x()/CLHEP::mm);
      fSiHitsY.push_back(hit->GetDetPos().y()/CLHEP::mm);
      fSiHitsZ.push_back(hit->GetDetPos().z()/CLHEP::mm);
      fSiHitsEdep.push_back(hit->GetEdep()/CLHEP::keV);
      fDetID.push_back(hit->GetDetectorNb());
    }

    analysisManager->AddNtupleRow();
  
  
  }
}
