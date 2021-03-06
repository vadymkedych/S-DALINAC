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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 150*cm;
  G4double world_sizeZ  = world_sizeXY;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
    // Envelope parameters
  //
  G4Material* matal_mat = nist->FindOrBuildMaterial("G4_Ti");
  G4Box* metal =    
    new G4Box("Metal",                       //its name
       0.5*cm, 0.5*cm, 17.5*um);     //its size
      
  G4LogicalVolume* logicMet =                         
    new G4LogicalVolume(metal,            //its solid
                        matal_mat,             //its material
                        "Metal");         //its name
               
  G4VPhysicalVolume* metalVolume = 
    new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0, 20*um),         //at (0,0,0)
                    logicMet,                //its logical volume
                    "Metal",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                    
  
  // "Detector"
  G4double detectorHalfSizeXY = 0.5*cm;
  G4double detectorHalfSizeZ = 350*um; 
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Box* detector =    
    new G4Box("Detector",                       //its name
       detectorHalfSizeXY, detectorHalfSizeXY, detectorHalfSizeZ);     //its size
      
  G4LogicalVolume* logicDetector =                         
    new G4LogicalVolume(detector,          //its solid
                        det_mat,           //its material
                        "Detector");            //its name
                                   
  G4VPhysicalVolume* detectorVolume = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0, 0, 40*cm),       //at (0,0,0)
                      logicDetector,            //its logical volume
                      "Detector",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                
  fScoringVolume    = logicDetector;
  fAirVolume        = logicWorld;
  fFoilVolume       = logicMet;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
