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
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"

#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
    fdetectedParticles.clear();
    fParticlesTime.clear();
    fDetectedProcesses.clear();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile("outpoot.root");
    
    analysisManager->CreateH2("AfterFoil", "XY distribution after foil", 2000,-100*mm, 100*mm, 2000,-100*mm, 100*mm);
    analysisManager->CreateH2("Detector", "XY distribution on detector", 2000,-100*mm, 100*mm, 2000,-100*mm, 100*mm);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

//   Run conditions
//    note: There is no primary generator action object for "master"
//          run manager for multi-threaded mode.
//   G4cout<< G4endl << "================RESULTS================" << G4endl;
//   G4cout <<"Number of unique Particles: " << fdetectedParticles.size() <<G4endl;
//   
//   for(auto& [key, value] : fdetectedParticles){
//         G4cout << key << " : " << value << G4endl;
//     }
//     
//   G4cout <<"Detected processes: " << fDetectedProcesses.size() <<G4endl;  
//   for(auto& [key, value] : fDetectedProcesses){
//         G4cout << key << " : " << value << G4endl;
//     }
//   G4cout <<"Global Time for gamma: " << fParticlesTime["gamma"].size() <<G4endl;
//     for(auto item : fParticlesTime["gamma"]){
//         G4cout << item << G4endl;
//     }
//   G4cout<< G4endl << "=======================================" << G4endl;
//   
//   Write to files
//   G4String path = "/outputFiles/";
//   G4String fileName = "";
//   
//   for(auto& [key, timeVector] : fParticlesTime){
//     fileName = key + ".txt";
//     std::ofstream file;         
//     file.open(fileName);
//     if(file.is_open()){
//         for(auto i  : timeVector) {
//           file << i << std::endl;
//         }
//         
//         file.close();
//     } else std::cout << "Can't open file" << std::endl;
//   }
  
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1RunAction::SetDetectedParticles(std::map <G4String, G4int> detectedParticles) {
    for(auto& [key, value] : detectedParticles){
        fdetectedParticles[key] += value;
    }
}

void B1RunAction::SetParticlesTime(std::map <G4String, std::vector<G4double>> particlesTime) {
    for(auto& [key, timeVector] : particlesTime){
        fParticlesTime[key].insert(fParticlesTime[key].end(), timeVector.begin(), timeVector.end());
    }
}

void B1RunAction::SetDetectedProcesses(std::map <G4String, G4int> detectedProcesses) {
    for(auto& [key, value] : detectedProcesses){
            fDetectedProcesses[key] += value;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

