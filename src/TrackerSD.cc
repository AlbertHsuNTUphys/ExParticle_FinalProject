#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

namespace FP
{

TrackerSD::TrackerSD(const G4String& name,
		     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

TrackerSD::~TrackerSD()
{}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection
    = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection);
}

G4bool TrackerSD::ProcessHits(G4Step* aStep,
		              G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if(edep==0.) return false;
  TrackerHit* newHit = new TrackerHit();

  newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
  newHit->SetDetectorNb(aStep->GetPreStepPoint()->GetTouchableHandle()
			->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  G4ThreeVector origin(0.,0.,0.);
  G4ThreeVector globalOrigin = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);
  newHit->SetDetPos(globalOrigin);
  fHitsCollection->insert(newHit);
  return true;
 

}
void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nofHits = fHitsCollection->entries();
  G4cout << G4endl
         << "-------->Hits Collection: in this event they are " << nofHits
         << " hits in the tracker detectors: " << G4endl;
  for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();


}

}

