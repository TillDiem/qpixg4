// -----------------------------------------------------------------------------
//  G4_QPIX | TrackingSD_Photon.h
//
//  Class for tracking the photons leaving the lAr APA
//   * Author: Till Dieminger
//   * Creation date: 15. July 2022
// -----------------------------------------------------------------------------

// prevent double import
#ifndef TRACKING_SD_PHOTON_H
#define TRACKING_SD_PHOTON_H

// G4 includes
#include <G4VSensitiveDetector.hh>

// Q-PIX includes
#include "TrackingHit.h"
#include "AnalysisManager.h"

// C++ inlcudes
#include <vector>

// TrackingSD_Photon as child/derivative of the SenstiveDetector
class TrackingSD_Photon: public G4VSensitiveDetector
{
public:
  // Constructor and Destructor
  TrackingSD_Photon(const G4String&, const G4String&);
  virtual ~TrackingSD_Photon();

  // Function processing the hits in the sensitive area of the detector
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};
#endif
