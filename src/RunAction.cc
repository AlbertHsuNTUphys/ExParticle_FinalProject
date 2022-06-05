#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "EventAction.hh"

namespace FP
{

RunAction::RunAction(EventAction* eventAction)
  :  fEventAction(eventAction)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel(1);

  analysisManager->SetNtupleMerging(true);
  analysisManager->SetFileName("Output");

  analysisManager->CreateH1("Edep","Energy Deposit (MeV)", 3000, 0., 30);

  if ( fEventAction ) {
    G4int ntupleID = analysisManager->CreateNtuple("TrackHits", "TrackHits");

    analysisManager->CreateNtupleIColumn("evtNo");  // column Id = 0
    analysisManager->CreateNtupleIColumn("Edep_keV");  // column Id = 1

    analysisManager->CreateNtupleIColumn("NHits");  // column Id = 2
    analysisManager->CreateNtupleDColumn("GenPX_MeV");  // column Id = 3
    analysisManager->CreateNtupleDColumn("GenPY_MeV");  // column Id = 4
    analysisManager->CreateNtupleDColumn("GenPZ_MeV");  // column Id = 5


    analysisManager->CreateNtupleDColumn("Hits_DetX_mm", fEventAction->fSiHitsX);  // column Id = 6
    analysisManager->CreateNtupleDColumn("Hits_DetY_mm", fEventAction->fSiHitsY);  // column Id = 7
    analysisManager->CreateNtupleDColumn("Hits_DetZ_mm", fEventAction->fSiHitsZ);  // column Id = 8
    analysisManager->CreateNtupleDColumn("Hits_DetE_keV", fEventAction->fSiHitsEdep);  // column Id = 9
    analysisManager->CreateNtupleIColumn("Hits_DetID", fEventAction->fDetID);  // column Id = 10


    analysisManager->FinishNtuple(ntupleID);
  }
  analysisManager->SetNtupleFileName(0, "Output");

}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Reset();
  analysisManager->OpenFile();

}

void RunAction::EndOfRunAction(const G4Run*run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if(nofEvents == 0) return;

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  G4double edep = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double rms = edep2-edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // save histograms & ntuple
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();


}

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}


}
