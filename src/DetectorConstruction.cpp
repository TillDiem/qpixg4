// ----------------------------------------------------111111-------------------------
//  G4_QPIX | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "TrackingSD.h"
#include "TrackingSD_Photon.h"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction()
{
 det_mesg_ = new G4GenericMessenger(this, "/Det/", "Control commands for Detector Properties");
 det_mesg_->DeclareProperty("rayleigh", rayleigh_, "Name of the detector");
 det_mesg_->DeclareProperty("width", width_, "Width (X) of the detector - 2.3m").SetUnit("m");
 det_mesg_->DeclareProperty("height", height_, "Height (Y) of the detector - 6.0m").SetUnit("m");
 det_mesg_->DeclareProperty("length", length_, "Width (Z) of the detector - 3.6m").SetUnit("m");
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  G4double world_size = 15.*m;
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");



  G4Box* world_solid_vol =
    new G4Box("world.solid", world_size/2., world_size/2., world_size/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, "world.logical");
  world_logic_vol->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, "world.physical", 0, false, 0, true);

  // DETECTOR //////////////////////////////////////////////
  // resemble an APA size
  G4double detector_width   = width_;
  G4double detector_height  = height_;
  G4double detector_length  = length_;

  G4Material* detector_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");

  // TODO: Setup messanger to activate/deactivate scintillation processses and the corresponding detectors

  // MATERIAL PROPERTIES
  // Refernce energy    140nm,   133nm   128nm    120nm    116nm
  G4double energy[5] = {8.85*eV, 9.3*eV, 9.68*eV, 10.3*eV, 10.6*eV}; // Energy for Spectrum, Abs, Rindex of lAr 128 nm = 9.68eV
  G4double Scint_lAr[5] = {0.01, 0.1, 1.0, 0.1, 0.01 }; // Weight of the scintillaiton spectrum for lAr (taken from Fig 1 of https://arxiv.org/abs/2002.09346)
  G4double Abs_lAr[5] = {20*m, 20*m, 20*m, 20*m, 20*m}; // Absorption length for lAr using ~3ppm nitrogen:(https://iopscience.iop.org/article/10.1088/1748-0221/8/07/P07011)
  G4double Rindex_lAr[5] = {1.33, 1.35, 1.37, 1.43, 1.5}; // Refractive index for lAr (https://lar.bnl.gov/properties/#scint)
  G4double Rayleigh_lAr[5] = {80.0*cm, 70.0*cm, 50.0*cm, 20*cm, 20.0*cm}; // Rayleigh scattering for lAr
  // SOURCE for Rayleigh https://pure.royalholloway.ac.uk/portal/en/publications/calculation-and-measurement-of-the-rayleigh-scattering-length-of-the-scintillation-wavelength-of-liquid-argon-for-dark-matter-and-neutrino-detectors(be8cb174-d660-476c-82c7-051d30000fa8).html - some extrapolation

//TODO Assumed 20.000 / MeV - has to be adjusted down the road for Voltage dependence etc
  G4double energy_Scint[15] = {1.00000000e-02*MeV,  7.14378571e+01*MeV,  1.42865714e+02*MeV,  2.14293571e+02*MeV,  2.85721429e+02*MeV,  3.57149286e+02*MeV,  4.28577143e+02*MeV,  5.00005000e+02*MeV,  5.71432857e+02*MeV,  6.42860714e+02*MeV,  7.14288571e+02*MeV,  7.85716429e+02*MeV,  8.57144286e+02*MeV,  9.28572143e+02*MeV,  1.00000000e+03*MeV};
  G4double ScintillationParticle[15]= {2.00000000e+02, 1.42875714e+06, 2.85731429e+06, 4.28587143e+06, 5.71442857e+06, 7.14298571e+06, 8.57154286e+06, 1.00001000e+07, 1.14286571e+07, 1.28572143e+07, 1.42857714e+07, 1.57143286e+07, 1.71428857e+07, 1.85714429e+07, 2.00000000e+07};


  // Time and Yields are taken from Anyssa - find nice source later

  G4MaterialPropertiesTable *lAr_MPT = new G4MaterialPropertiesTable();
  // General Light Properties
  lAr_MPT->AddProperty("RINDEX",energy, Rindex_lAr, 5);
  lAr_MPT->AddProperty("ABSLENGTH",energy, Abs_lAr,5);
  if(rayleigh_) lAr_MPT->AddProperty("RAYLEIGH",energy, Rayleigh_lAr,5);
  // Spectrum of Scinitllation light
  lAr_MPT->AddProperty("SCINTILLATIONCOMPONENT1", energy, Scint_lAr, 5);
  lAr_MPT->AddProperty("SCINTILLATIONCOMPONENT2", energy, Scint_lAr, 5);
  lAr_MPT->AddConstProperty("RESOLUTIONSCALE",1.0);
  // Defining the two components for scinitllation
  lAr_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1",7.*ns);
  lAr_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2",1500.*ns);
  // Per Particle Scinitllation Light
  lAr_MPT->AddProperty("ALPHASCINTILLATIONYIELD", energy_Scint, ScintillationParticle, 15);
  lAr_MPT->AddProperty("IONSCINTILLATIONYIELD", energy_Scint, ScintillationParticle, 15);
  lAr_MPT->AddProperty("PROTONSCINTILLATIONYIELD", energy_Scint, ScintillationParticle, 15);
  lAr_MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", energy_Scint, ScintillationParticle, 15);
  //Relative amount of scinitllation light in each component
  lAr_MPT->AddConstProperty("ALPHASCINTILLATIONYIELD1", 0.75);
  lAr_MPT->AddConstProperty("ALPHASCINTILLATIONYIELD2", 0.25);
  lAr_MPT->AddConstProperty("IONSCINTILLATIONYIELD1", 0.75);
  lAr_MPT->AddConstProperty("IONSCINTILLATIONYIELD2", 0.25);
  lAr_MPT->AddConstProperty("PROTONSCINTILLATIONYIELD1", 0.75);
  lAr_MPT->AddConstProperty("PROTONSCINTILLATIONYIELD2", 0.25);
  lAr_MPT->AddConstProperty("ELECTRONSCINTILLATIONYIELD1", 0.33);
  lAr_MPT->AddConstProperty("ELECTRONSCINTILLATIONYIELD2", 0.67);
  // Connecting information to the material
  detector_mat->SetMaterialPropertiesTable(lAr_MPT);

  // TODO: Do I have to set this at all?
  // G4double n_Air[28]={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.6, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  G4double Rindex_Air[5] = {1.0, 1.0, 1.0, 1.0, 1.0}; // Refractive index for Air ...
  G4MaterialPropertiesTable *Air_MPT = new G4MaterialPropertiesTable();
  Air_MPT->AddProperty("RINDEX",energy,Rindex_Air,5);
  world_mat->SetMaterialPropertiesTable(Air_MPT);





  G4Box* detector_solid_vol =
    new G4Box("detector.solid", detector_width/2., detector_height/2., detector_length/2.);

  G4LogicalVolume* detector_logic_vol =
    new G4LogicalVolume(detector_solid_vol, detector_mat, "detector.logical");

  G4ThreeVector offset(detector_width/2., detector_height/2., detector_length/2.);

  new G4PVPlacement(0, offset,
                    detector_logic_vol, "detector.physical", world_logic_vol, false, 0, true);

  //////////////////////////////////////////////////////////
  // TODO : Check if all photons hit the detector
  //// SENSITIVE DETECTOR FOR PHOTON DETECTION //////////////
  // A sensitive area around the detector, enabeling the collection of the photons
  // Create 6 physical volumes for the 6 faces of the detector
  //////////////////////////////////////////////////////////

  /* G4Material* photon_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe"); */
  /* G4double Rindex_photon[5] = {1.33, 1.35, 1.37, 1.43, 1.5}; // Refractive index for lAr (https://lar.bnl.gov/properties/#scint) */
  /* G4MaterialPropertiesTable *photon_MPT = new G4MaterialPropertiesTable(); */
  /* photon_MPT->AddProperty("RINDEX",energy,Rindex_photon,5); */
  /* photon_mat->SetMaterialPropertiesTable(photon_MPT); */




  G4Box* photon_zy_vol = new G4Box("photon_zy_plane.solid",detector_length/2,detector_height/2,0.01*m);
  G4Box* photon_zx_vol = new G4Box("photon_zx_plane.solid",detector_width/2,detector_length/2,0.01*m);
  G4Box* photon_xy_vol = new G4Box("photon_xy_plane.solid",detector_height/2,detector_width/2,0.01*m);
  G4LogicalVolume* photon_zy_log = new G4LogicalVolume(photon_zy_vol,world_mat,"photon_zy_plane.logical");
  G4LogicalVolume* photon_zx_log = new G4LogicalVolume(photon_zx_vol,world_mat,"photon_zx_plane.logical");
  G4LogicalVolume* photon_xy_log = new G4LogicalVolume(photon_xy_vol,world_mat,"photon_xy_plane.logical");


  G4ThreeVector offset_mzy(detector_width+0.01*m,detector_height/2,detector_length/2);
  G4ThreeVector offset_pzy(-0.01*m,detector_height/2,detector_length/2);
  G4RotationMatrix* rotationMatrix_zy = new G4RotationMatrix();
  rotationMatrix_zy->rotateY(90.*deg);
  new G4PVPlacement(rotationMatrix_zy,offset_mzy,photon_zy_log,"photon_mzy_plane.physical",world_logic_vol,false,1,true); // Side planes
  new G4PVPlacement(rotationMatrix_zy,offset_pzy,photon_zy_log,"photon_pzy_plane.physical",world_logic_vol,false,2,true);

  G4ThreeVector offset_mxy(detector_width/2,detector_height/2,detector_length+0.01*m);
  G4ThreeVector offset_pxy(detector_width/2,detector_height/2,-0.01*m);
  G4RotationMatrix* rotationMatrix_xy = new G4RotationMatrix();
  rotationMatrix_xy->rotateZ(90.*deg);
  new G4PVPlacement(rotationMatrix_xy,offset_mxy,photon_xy_log,"photon_mxy_plane.physical",world_logic_vol,false,3,true); // Side planes
  new G4PVPlacement(rotationMatrix_xy,offset_pxy,photon_xy_log,"photon_pxy_plane.physical",world_logic_vol,false,4,true);


  G4ThreeVector offset_mzx(detector_width/2,-0.01*m,detector_length/2);
  G4ThreeVector offset_pzx(detector_width/2,detector_height+0.01*m,detector_length/2);
  G4RotationMatrix* rotationMatrix_zx = new G4RotationMatrix();
  rotationMatrix_zx->rotateX(90.*deg);
  new G4PVPlacement(rotationMatrix_zx,offset_mzx,photon_zx_log,"photon_mzx_plane.physical",world_logic_vol,false,5,true); // Side planes
  new G4PVPlacement(rotationMatrix_zx,offset_pzx,photon_zx_log,"photon_pzx_plane.physical",world_logic_vol,false,6,true);

  return world_phys_vol;
}

void DetectorConstruction::ConstructSDandField()
{
  // SENSITIVE DETECTOR ////////////////////////////////////

  TrackingSD* tracking_sd = new TrackingSD("/G4QPIX/TRACKING", "TrackingHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd);

  G4LogicalVolume* detector_logic_vol =
    G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");

  SetSensitiveDetector(detector_logic_vol, tracking_sd);


  TrackingSD_Photon* tracking_sd_Photon = new TrackingSD_Photon("/G4QPIX/PHOTONTRACKING", "TrackingPhotonHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd_Photon);

  G4LogicalVolume* detector_logic_zx_vol_Photon = G4LogicalVolumeStore::GetInstance()->GetVolume("photon_zx_plane.logical");
  G4LogicalVolume* detector_logic_xy_vol_Photon = G4LogicalVolumeStore::GetInstance()->GetVolume("photon_xy_plane.logical");
  G4LogicalVolume* detector_logic_zy_vol_Photon = G4LogicalVolumeStore::GetInstance()->GetVolume("photon_zy_plane.logical");

  SetSensitiveDetector(detector_logic_zx_vol_Photon, tracking_sd_Photon);
  SetSensitiveDetector(detector_logic_xy_vol_Photon, tracking_sd_Photon);
  SetSensitiveDetector(detector_logic_zy_vol_Photon, tracking_sd_Photon);




  //////////////////////////////////////////////////////////
}
