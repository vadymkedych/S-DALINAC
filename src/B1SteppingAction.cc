//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fAirVolume(0),
  fFoilVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume || !fAirVolume || !fFoilVolume) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume  = detectorConstruction->GetScoringVolume();
    fAirVolume      = detectorConstruction->GetAirVolume();
    fFoilVolume     = detectorConstruction->GetFoilVolume();
  }

  // get volume of the current step
//   G4LogicalVolume* preStepVolume 
//     = step->GetPreStepPoint()->GetTouchableHandle()
//       ->GetVolume()->GetLogicalVolume();
//       
//       
    G4Track* track = step->GetTrack();
    if(track->GetTrackID() > 1)
       track->SetTrackStatus(fStopAndKill);
          
    G4LogicalVolume* preStepVolume 
        = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
    G4LogicalVolume* postStepVolume 
        = step->GetPostStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
//     G4cout << "Pre volume name: " <<  preStepVolume->GetName() << G4endl;
//     G4cout << "Post volume name: " <<  postStepVolume->GetName() << G4endl;
//     
//     G4cout << "Pre position: " << step->GetPreStepPoint()->GetPosition().getX() / cm << " " << step->GetPreStepPoint()->GetPosition().getY() / cm << " " << step->GetPreStepPoint()->GetPosition().getZ() / cm << G4endl;
//     G4cout << "Post position: " << step->GetPostStepPoint()->GetPosition().getX() / cm << " " << step->GetPostStepPoint()->GetPosition().getY() / cm << " " << step->GetPostStepPoint()->GetPosition().getZ() / cm << G4endl;
//     G4cout << "Energy: " << G4BestUnit(track->GetKineticEnergy(), "Energy") << G4endl;
    
    if(preStepVolume->GetName() == "Detector" && postStepVolume->GetName() == "World"){    
        track->SetTrackStatus(fStopAndKill);
    }
    
    if(!track->GetNextVolume() ||  step->GetPostStepPoint()->GetPosition().getZ() / cm > 42)
        track->SetTrackStatus(fStopAndKill);
    
    if(preStepVolume->GetName() == "Metal" && postStepVolume->GetName() == "World")
        fEventAction->SetAfterFoilPosition(step->GetPostStepPoint()->GetPosition());
    
    if(preStepVolume->GetName() == "World" && postStepVolume->GetName() == "Detector")
        fEventAction->SetDetectorHitPosition(step->GetPostStepPoint()->GetPosition());
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

