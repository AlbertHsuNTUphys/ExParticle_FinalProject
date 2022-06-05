#ifndef FPDetectorConstruction_h
#define FPDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace FP
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const{ return fScoringVolume;}

    void ConstructSDandField() override;
  protected:
    G4LogicalVolume* fScoringVolume = nullptr;
};

}

#endif
