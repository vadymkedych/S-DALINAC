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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "Analysis.hh" 

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event* event)
{    
//       G4int eventNb = event->GetEventID();
/* 
    G4cout << "\n---> Begin of event: " << eventNb << G4endl;*/

  
    fEventParticles.clear();
    fEventParticlesTime.clear();
    fEventProcess.clear();
  
    fXAfterFoil     = 100*cm;
    fYAfterFoil     = 100*cm;
    fXDetectorHit   = 100*cm;
    fYDetectorHit   = 100*cm;
    
    fScattered      = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
//   fRunAction->SetDetectedParticles(fEventParticles);
//   fRunAction->SetParticlesTime(fEventParticlesTime);
//   fRunAction->SetDetectedProcesses(fEventProcess);
  
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->FillH2(0, fXAfterFoil / mm, fYAfterFoil / mm);
//     if(!fScattered)
    analysisManager->FillH2(1, fXDetectorHit / mm, fYDetectorHit / mm);
}

// void B1EventAction::AddParticle(G4String name){
//         fEventParticles[name] += 1;
// }
// 
// void B1EventAction::AddParticleTime(G4String name, G4double time){
//     fEventParticlesTime[name].push_back(time);
// }
// 
// void B1EventAction::AddProcess(G4String name){
//         fEventProcess[name] += 1;
// }

void B1EventAction::SetAfterFoilPosition(G4ThreeVector position){
    fXAfterFoil = position.getX();
    fYAfterFoil = position.getY();
}

void B1EventAction::SetDetectorHitPosition(G4ThreeVector position){
    fXDetectorHit = position.getX();
    fYDetectorHit = position.getY();
}

// void B1EventAction::SetScatteredTrue(){
//     fScattered = true;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
