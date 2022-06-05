#ifndef FPTrackerHit_h
#define FPTrackerTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

namespace FP
{

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    TrackerHit(const TrackerHit&) = default;
    ~TrackerHit() override;

    TrackerHit& operator=(const TrackerHit&) = default;
    G4bool operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw() override;
    void Print() override;

    void SetTrackID (G4int track)  { fTrackID = track;};
    void SetDetectorNb(G4int det)  { fDetectorNb = det;};
    void SetEdep(G4double de)      { fEdep = de;};
    void SetPos(G4ThreeVector xyz) { fPos  = xyz;};
    void SetDetPos(G4ThreeVector xyz) { fDetPos = xyz;};

    G4int GetTrackID() const       { return fTrackID;};
    G4int GetDetectorNb() const    { return fDetectorNb;};
    G4double GetEdep()  const      { return fEdep; };
    G4ThreeVector GetPos() const   { return fPos;  };
    G4ThreeVector GetDetPos() const{ return fDetPos; };

  private:
    G4int    fTrackID = -1;
    G4int    fDetectorNb = -1;
    G4double fEdep = 0.;
    G4ThreeVector fPos;
    G4ThreeVector fDetPos;

  bool _isValidHit;

};

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;
extern  G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator)
	  TrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void *) TrackerHitAllocator->MallocSingle();
}
inline void TrackerHit::operator delete(void *hit)
{
  TrackerHitAllocator->FreeSingle((TrackerHit*) hit);
}

}

#endif
