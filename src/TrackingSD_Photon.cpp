// -----------------------------------------------------------------------------
//  G4_QPIX | TrackingSD_Photon.cpp
//
//  Class for tracking the photons leaving the lAr APA
//   * Author: Till Dieminger
//   * Creation date: 15. July 2022
// -----------------------------------------------------------------------------

#include "TrackingSD_Photon.h"
#include "TrackingHit.h"

// Q-Pix includes
#include "MCTruthManager.h"
#include "MCParticle.h"

// GEANT4 includes
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

// C++ includes
#include <vector>

// Constructor
TrackingSD_Photon::TrackingSD_Photon(const G4String& sd_name, const G4String& hc_name):
  G4VSensitiveDetector(sd_name)
{}

// Destructor
TrackingSD_Photon::~TrackingSD_Photon()
{}

// TODO: Try to implement on touch of SD?!
G4bool TrackingSD_Photon::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  // Hand over the hit to the AnalysisManager, where it is written in to the photon_tree
  AnalysisManager::Instance()->AddMCParticle_Photon(aStep);


  // Do not propagate the track after the last step - remove track
  G4Track *track = aStep->GetTrack();
  track->SetTrackStatus(fStopAndKill);

  // Default return value
  return true;
}
