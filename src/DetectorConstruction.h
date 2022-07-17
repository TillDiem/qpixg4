// -----------------------------------------------------------------------------
//  G4_QPIX | DetectorConstruction.h
//
//  Class for the definition of the detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"

class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

private:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  G4GenericMessenger* det_mesg_;
  G4bool rayleigh_;
  G4double width_=2300;
  G4double height_=6000;
  G4double length_=3000;


};

#endif
