#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "TrackerSD.hh"
#include "G4SDManager.hh"
#include "G4PVReplica.hh"

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"


namespace FP
{

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = false;

  G4double world_size = 200*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld = 
    new G4Box("World",
	      0.5*world_size,
	      0.5*world_size,
	      0.5*world_size);

  G4LogicalVolume* logicWorld = 
    new G4LogicalVolume(solidWorld,
		        world_mat,
			"World");
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,
		      G4ThreeVector(),
		      logicWorld,
		      "World",
		      0,
		      false,
		      0,
		      checkOverlaps);

  G4Material* targetMaterial = nist->FindOrBuildMaterial("G4_Si");
  int index = 0;
  for(int layer = 0; layer < 10; layer++){
      G4double ratio = pow(20.,(G4double)layer/9.);
//      G4int    ndivision = (G4int)(80*sqrt(ratio));
      G4int    ndivision = (G4int)(5000*3.14*2/100);
      G4double radius = 5.*ratio*cm;
      G4double length = radius*M_PI/ndivision*2.;
      G4double zstart = -5.*length;
      G4Box* targetSolid
              = new G4Box("target-Solid",
                          300*micrometer,
                          0.5*length,
                          0.5*length);
      G4LogicalVolume* targetLogical
              = new G4LogicalVolume(targetSolid,
                                    targetMaterial,
                                    "logicTarget");
      for(int copyNo=0; copyNo < ndivision*10; copyNo++){
        G4int    angle_index = copyNo%ndivision;
        G4int    z_index     = copyNo/ndivision;
        G4double angle       = 2*(M_PI*angle_index)/ndivision;
        G4RotationMatrix *zRot = new G4RotationMatrix;
        zRot->rotateZ(-1.*angle);
        G4ThreeVector origin(radius*cos(angle),radius*sin(angle),zstart+length*z_index);
        G4VPhysicalVolume* targetPhys
            = new G4PVPlacement(zRot,
                                origin,
                                targetLogical,"Disc",
                                logicWorld,false,index);
        index++;
      }
    }
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  G4String trackerChamberSDname = "/TrackerChamberSD";
  TrackerSD* aTrackerSD = new TrackerSD(trackerChamberSDname, "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

  SetSensitiveDetector("logicTarget", aTrackerSD, true);


  G4double BzValue = 2.0 *tesla;

  G4MagneticField* magField =
        new G4UniformMagField(G4ThreeVector(0.,0.,BzValue));
  auto transportationManager = G4TransportationManager::GetTransportationManager();

  G4FieldManager* fieldMgr = transportationManager-> GetFieldManager();

  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);

}


}
