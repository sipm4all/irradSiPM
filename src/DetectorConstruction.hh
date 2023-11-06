#ifndef _DetectorConstruction_h_
#define _DetectorConstruction_h_

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"

class G4String;
class G4Material;
class G4OpticalSurface;
class G4VPhysicalVolume;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class DetectorConstruction : public G4VUserDetectorConstruction,
			     public G4UImessenger
{
  
public:
  
  DetectorConstruction();
  ~DetectorConstruction() override;
  
  G4VPhysicalVolume *Construct() override;
  void ConstructSDandField() override;
  
protected:

  void SetNewValue(G4UIcommand *command, G4String value);

  static const int nAbsorbers = 100;
  G4UIdirectory *mAbsorberDir[nAbsorbers];
  G4String mAbsorberMaterial[nAbsorbers];
  G4ThreeVector mAbsorberPos[nAbsorbers];
  G4ThreeVector mAbsorberSize[nAbsorbers];
  G4double mCollimatorSize[nAbsorbers];
  G4UIcmdWithAString *mAbsorberMaterialCmd[nAbsorbers];
  G4UIcmdWith3VectorAndUnit *mAbsorberPosCmd[nAbsorbers];
  G4UIcmdWith3VectorAndUnit *mAbsorberSizeCmd[nAbsorbers];
  G4UIcmdWithADoubleAndUnit *mCollimatorSizeCmd[nAbsorbers];

  G4UIdirectory *mSensorDir = nullptr;
  G4ThreeVector mSensorPos;
  G4ThreeVector mSensorSize;
  G4VPhysicalVolume *mSensorPV = nullptr;
  G4UIcmdWith3VectorAndUnit *mSensorPosCmd = nullptr;
  G4UIcmdWith3VectorAndUnit *mSensorSizeCmd = nullptr;
  G4UIcmdWith3VectorAndUnit *mSensorMoveCmd = nullptr;

  G4UIdirectory *mSystemDir = nullptr;
  G4ThreeVector mSystemPos;
  G4ThreeVector mSystemRot;
  G4double mSystemAngle;
  G4UIcmdWith3VectorAndUnit *mSystemPosCmd = nullptr;
  G4UIcmdWith3Vector *mSystemRotCmd = nullptr;
  G4UIcmdWithADoubleAndUnit *mSystemAngleCmd = nullptr;

};

#endif /** _DetectorConstruction_h_ **/
