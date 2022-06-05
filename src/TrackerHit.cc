#include "TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

namespace FP
{

G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator = nullptr;

TrackerHit::TrackerHit()
{}

TrackerHit::~TrackerHit() {}

G4bool TrackerHit::operator==(const TrackerHit& right) const
{
  return ( this == &right ) ? true : false;
}

void TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
void TrackerHit::Print()
{
  G4cout
     << "  trackID: " << fTrackID << " detetorNb: " << fDetectorNb
     << "  Edep: "
     << std::setw(3) << fEdep/CLHEP::keV //G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(3) << fPos/CLHEP::mm //G4BestUnit( fPos,"Length")
     << " DetPosition: "
     << std::setw(3) << fDetPos/CLHEP::mm //G4BestUnit( fDetPos,"Length")
     << G4endl;

}

}
