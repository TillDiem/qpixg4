// -----------------------------------------------------------------------------
//  G4_QPIX | TrackingSD.cpp
//
//  TODO: Class description
//   * Author: Justo Martin-Albo
//   * Creation date: 13 Feb 2019
// -----------------------------------------------------------------------------

#include "TrackingSD.h"
#include "TrackingHit.h"

// Q-Pix includes
#include "MCTruthManager.h"
#include "MCParticle.h"

// GEANT4 includes
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

// C++ includes
#include <vector>


TrackingSD::TrackingSD(const G4String& sd_name, const G4String& hc_name):
  G4VSensitiveDetector(sd_name),
  Event_Cutoff_(0.0)
  // hc_(nullptr)
{
  collectionName.insert(hc_name);

  msg_ = new G4GenericMessenger(this, "/Supernova/", "Control commands of the supernova generator.");
  msg_->DeclareProperty("Event_Cutoff", Event_Cutoff_,  "window to simulate the times").SetUnit("ns");

  // Messenger to set the possibility of tracking the optical photon hits in the main lAr APA
  // Otherwise only their endpoints will be stored in the TrackingSD_Photon class on the extra layors around the APA
  msg_Photons_ = new G4GenericMessenger(this, "/Photons/", "Control commands for photon storage and generation");
  msg_Photons_->DeclareProperty("StorePhotons", StorePhotons_, "Store photon hits in the APA - for debugging purposes");

}


TrackingSD::~TrackingSD()
{
  delete msg_;
}


G4bool TrackingSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Do not store optical photons in the hit variables unless the user wants to - mostly debugging
  if( !StorePhotons_ && aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton" )
  {
	  return false;
  }

  G4double edep = aStep->GetTotalEnergyDeposit();
  if (Event_Cutoff_ != 0.0)
  {
    G4StepPoint* preStepPoint = aStep->GetPostStepPoint();
    G4double hitTime = preStepPoint->GetGlobalTime();
    if (hitTime>Event_Cutoff_) return false;
  }

  //---------------------------------------------------------------------------
  // begin add hit to MCParticle
  //---------------------------------------------------------------------------

  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

  // get MC particle
  MCParticle * particle = mc_truth_manager->GetMCParticle(aStep->GetTrack()->GetTrackID());


  // add hit to MC particle
  particle->AddTrajectoryHit(aStep);

  //---------------------------------------------------------------------------
  // end add hit to MCParticle
  //---------------------------------------------------------------------------

  return true;
}


// void TrackingSD::EndOfEvent(G4HCofThisEvent*)
// {
//   G4int nofHits = hc_->entries();
//   G4cout
//     << G4endl
//     << "-------->Hits Collection: in this event there are " << nofHits
//     << " hits in the tracker." << G4endl;
// }
