#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "RootIO.hh"

/*****************************************************************/

DetectorConstruction::DetectorConstruction()
{

  for (int iabs = 0; iabs < nAbsorbers; ++iabs) {

    std::string dirname = "/absorber" + std::to_string(iabs) + "/";
    mAbsorberDir[iabs] = new G4UIdirectory(dirname.c_str());

    mAbsorberMaterialCmd[iabs] = new G4UIcmdWithAString(std::string(dirname + "material").c_str(), this);
    mAbsorberMaterialCmd[iabs]->SetGuidance("Material of the absorber block");
    mAbsorberMaterialCmd[iabs]->SetParameterName("material", false);
    mAbsorberMaterialCmd[iabs]->AvailableForStates(G4State_PreInit);

    mAbsorberPosCmd[iabs] = new G4UIcmdWith3VectorAndUnit(std::string(dirname + "pos").c_str(), this);
    mAbsorberPosCmd[iabs]->SetGuidance("Position of the absorber block");
    mAbsorberPosCmd[iabs]->SetParameterName("x", "y", "z", false);
    mAbsorberPosCmd[iabs]->SetUnitCategory("Length");
    mAbsorberPosCmd[iabs]->AvailableForStates(G4State_PreInit);

    mAbsorberSizeCmd[iabs] = new G4UIcmdWith3VectorAndUnit(std::string(dirname + "size").c_str(), this);
    mAbsorberSizeCmd[iabs]->SetGuidance("Dimensions of the absorber block");
    mAbsorberSizeCmd[iabs]->SetParameterName("dx", "dy", "dz", false);
    mAbsorberSizeCmd[iabs]->SetUnitCategory("Length");
    mAbsorberSizeCmd[iabs]->AvailableForStates(G4State_PreInit);
    
    mCollimatorSizeCmd[iabs] = new G4UIcmdWithADoubleAndUnit(std::string(dirname + "collimator").c_str(), this);
    mCollimatorSizeCmd[iabs]->SetGuidance("Dimension of the collimator slit");
    mCollimatorSizeCmd[iabs]->SetParameterName("dx", false);
    mCollimatorSizeCmd[iabs]->SetUnitCategory("Length");
    mCollimatorSizeCmd[iabs]->AvailableForStates(G4State_PreInit);

  }
    
  mSensorDir = new G4UIdirectory("/sensor/");

  mSensorPosCmd = new G4UIcmdWith3VectorAndUnit("/sensor/pos", this);
  mSensorPosCmd->SetGuidance("Position of the sensor wall");
  mSensorPosCmd->SetParameterName("x", "y", "z", false);
  mSensorPosCmd->SetUnitCategory("Length");
  mSensorPosCmd->AvailableForStates(G4State_PreInit);

  mSensorSizeCmd = new G4UIcmdWith3VectorAndUnit("/sensor/size", this);
  mSensorSizeCmd->SetGuidance("Dimensions of the sensor wall");
  mSensorSizeCmd->SetParameterName("dx", "dy", "dz", false);
  mSensorSizeCmd->SetUnitCategory("Length");
  mSensorSizeCmd->AvailableForStates(G4State_PreInit);

  mSensorMoveCmd = new G4UIcmdWith3VectorAndUnit("/sensor/move", this);
  mSensorMoveCmd->SetGuidance("Displacement of the sensor wall");
  mSensorMoveCmd->SetParameterName("x", "y", "z", false);
  mSensorMoveCmd->SetUnitCategory("Length");
  mSensorMoveCmd->AvailableForStates(G4State_Idle);

  mSystemDir = new G4UIdirectory("/system/");

  mSystemPosCmd = new G4UIcmdWith3VectorAndUnit("/system/pos", this);
  mSystemPosCmd->SetGuidance("Position of the system");
  mSystemPosCmd->SetParameterName("x", "y", "z", false);
  mSystemPosCmd->SetUnitCategory("Length");
  mSystemPosCmd->AvailableForStates(G4State_PreInit);

  mSystemRotCmd = new G4UIcmdWith3Vector("/system/rot", this);
  mSystemRotCmd->SetGuidance("Rotation of the system");
  mSystemRotCmd->SetParameterName("x", "y", "z", false);
  mSystemRotCmd->AvailableForStates(G4State_PreInit);

  mSystemAngleCmd = new  G4UIcmdWithADoubleAndUnit("/system/angle", this);
  mSystemAngleCmd->SetGuidance("Rotation of the system");
  mSystemAngleCmd->SetParameterName("angle", false);
  mSystemAngleCmd->SetUnitCategory("Angle");
  mSystemAngleCmd->AvailableForStates(G4State_PreInit);

}

/*****************************************************************/

DetectorConstruction::~DetectorConstruction()
{
}

/*****************************************************************/

void
DetectorConstruction::SetNewValue(G4UIcommand *command, G4String value)
{

  for (int iabs = 0; iabs < nAbsorbers; ++iabs) {

    if (command == mAbsorberMaterialCmd[iabs]) {
      mAbsorberMaterial[iabs] = value;
    }
    if (command == mAbsorberSizeCmd[iabs]) {
      mAbsorberSize[iabs] = mAbsorberSizeCmd[iabs]->GetNew3VectorValue(value);
    }
    if (command == mAbsorberPosCmd[iabs]) {
      mAbsorberPos[iabs] = mAbsorberPosCmd[iabs]->GetNew3VectorValue(value);
    }
    if (command == mCollimatorSizeCmd[iabs]) {
      mCollimatorSize[iabs] = mCollimatorSizeCmd[iabs]->GetNewDoubleValue(value);
    }
    
  }
  
  if (command == mSensorPosCmd) {
    mSensorPos = mSensorPosCmd->GetNew3VectorValue(value);
  }
  if (command == mSensorSizeCmd) {
    mSensorSize = mSensorSizeCmd->GetNew3VectorValue(value);
  }
  if (command == mSensorMoveCmd) {
    auto move = mSensorMoveCmd->GetNew3VectorValue(value);
    auto tran = mSensorPV->GetTranslation();
    tran += move;
    mSensorPV->SetTranslation(tran);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }

  if (command == mSystemPosCmd) {
    mSystemPos = mSystemPosCmd->GetNew3VectorValue(value);
  }
  if (command == mSystemRotCmd) {
    mSystemRot = mSystemRotCmd->GetNew3VectorValue(value);
  }
  if (command == mSystemAngleCmd) {
    mSystemAngle = mSystemAngleCmd->GetNewDoubleValue(value);
  }
  
}

/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::Construct() {

  // this must be defined via messengers
  G4String mWorldMaterial = "G4_AIR";
  
  /** materials **/
  G4NistManager *nist = G4NistManager::Instance();
  auto air = nist->FindOrBuildMaterial(mWorldMaterial);
  auto silicon = nist->FindOrBuildMaterial("G4_Si");
  auto aluminum = nist->FindOrBuildMaterial("G4_Al");
  auto lead = nist->FindOrBuildMaterial("G4_Pb");

  /** borated polyethylene **/
  const std::vector<G4String> bpe_elements = {"H", "C", "B", "O"};
  const std::vector<G4double> bpe_weight = {11.6, 61.2, 5.0, 22.2};
  G4double bpe_density = 0.95 * g/cm3;
  auto bpe = nist->ConstructNewMaterial("BPE", bpe_elements, bpe_weight, bpe_density);
  
  /** IBA Solid Plate Phantom (RW3) **/
  auto polystyrene = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  auto titanium_dioxide = nist->FindOrBuildMaterial("G4_TITANIUM_DIOXIDE");
  auto rw3_density = 1.045 * g/cm3; // 0.98 * polystyrene->GetDensity() + 0.02 * titanium_dioxide->GetDensity();
  auto rw3 = new G4Material("RP_RW3", rw3_density, 2);
  rw3->AddMaterial(polystyrene, 0.98);
  rw3->AddMaterial(titanium_dioxide, 0.02);
  //  nist->RegisterMaterial(rw3);
  auto materialTable = G4Material::GetMaterialTable();
  materialTable->push_back(rw3);
  
  //  auto hydrogen = nist->FindOrBuildElement("G4_H");
  //  auto carbon = nist->FindOrBuildElement("G4_C");
  //  auto boron = nist->FindOrBuildElement("G4_B");
  //  auto polyethylene = nist->FindOrBuildElement("G4_POLYETHYLENE");
  //  auto oxygen = nist->FindOrBuildElement("G4_O");
  //  auto bpe_m = new G4Material("RP_BPE", 0.95 * mg/cm3, 2);
  //  bpe_m->AddMaterial(
  //  mat->AddElement(polyethylene, 0.95);
  //  mat->AddElement(F, 0.5);
  
  /** world **/
  auto world_s = new G4Box("world_s", 2. * m, 2. * m, 2. * m);
  auto world_lv = new G4LogicalVolume(world_s, air, "world_lv");
  auto world_pv = new G4PVPlacement(0,               // no rotation
				    G4ThreeVector(),  // at (0,0,0)
				    world_lv,         // its logical volume
				    "world_pv",       // its name
				    0,                // its mother  volume
				    false,            // no boolean operations
				    0,                // copy number
				    false);           // checking overlaps    


  /** enlarge the beam **/

  G4double firstfoil_thick = 0.78 * mm;
  G4double firstfoil_pos = -149. * cm;
  
  auto firstfoil_s = new G4Box("firstfoil_s", 10. * cm, 10. * cm, 0.5 * firstfoil_thick);
  auto firstfoil_lv = new G4LogicalVolume(firstfoil_s, lead, "firstfoil_lv");
  auto firstfoil_pv = new G4PVPlacement(nullptr,
					G4ThreeVector(0., 0., firstfoil_pos), 
					firstfoil_lv,
					"firstfoil_pv",
					world_lv,
					false,
					0,
					false);

  G4double innerring_radius = 11. * mm;
  G4double outerring_radius = 10. * cm;
  G4double innerring_thick = 1.5 * mm;
  G4double outerring_thick = 5. * mm;
  G4double innerring_pos = -99.075 * cm;
  G4double outerring_pos = -99.250 * cm;
    
  auto innerring_s = new G4Tubs("innerring_s", 0., innerring_radius, 0.5 * innerring_thick, 0., 2. * M_PI);
  auto innerring_lv = new G4LogicalVolume(innerring_s, lead, "innerring_lv");
  auto innerring_pv = new G4PVPlacement(nullptr,
					G4ThreeVector(0., 0., innerring_pos), 
					innerring_lv,
					"innerring_pv",
					world_lv,
					false,
					0,
					false);
  
  auto outerring_s = new G4Tubs("outerring_s", innerring_radius, outerring_radius, 0.5 * outerring_thick, 0., 2. * M_PI);
  auto outerring_lv = new G4LogicalVolume(outerring_s, aluminum, "outerring_lv");
  auto outerring_pv = new G4PVPlacement(nullptr,
					G4ThreeVector(0., 0., outerring_pos), 
					outerring_lv,
					"outerring_pv",
					world_lv,
					false,
					0,
					false);
  
  G4double supportfoil_thick = 1. * mm;
  G4double supportfoil_pos = -98.95 * cm;
  
  auto supportfoil_s = new G4Box("supportfoil_s", 10. * cm, 10. * cm, 0.5 * supportfoil_thick);
  auto supportfoil_lv = new G4LogicalVolume(supportfoil_s, aluminum, "supportfoil_lv");
  auto supportfoil_pv = new G4PVPlacement(nullptr,
					G4ThreeVector(0., 0., supportfoil_pos), 
					supportfoil_lv,
					"supportfoil_pv",
					world_lv,
					false,
					0,
					false);

  /** system **/
  auto system_s = new G4Box("system_s", 0.9 * m, 0.9 * m, 0.9 * m);
  auto system_lv = new G4LogicalVolume(system_s, air, "system_lv");
  auto system_pv = new G4PVPlacement(new G4RotationMatrix(G4ThreeVector(mSystemRot.x(), mSystemRot.y(), mSystemRot.z()), mSystemAngle),               // no rotation
				     G4ThreeVector(mSystemPos.x(), mSystemPos.y(), mSystemPos.z()),  // at (0,0,0)
				     system_lv,         // its logical volume
				     "system_pv",       // its name
				     world_lv,                // its mother  volume
				     false,            // no boolean operations
				     0,                // copy number
				     false);           // checking overlaps    
  
  /** absorbers **/

  for (int iabs = 0; iabs < nAbsorbers; ++iabs) {

    if (mAbsorberSize[iabs].x() == 0. || mAbsorberSize[iabs].y() == 0. || mAbsorberSize[iabs].z() == 0.) continue;
    
    auto absorber_m = nist->FindOrBuildMaterial(mAbsorberMaterial[iabs]);

    auto absorber_wall_s = new G4Box("absorber" + std::to_string(iabs) + "_wall_s",
				     0.5 * mAbsorberSize[iabs].x(), 0.5 * mAbsorberSize[iabs].y(), 0.5 * mAbsorberSize[iabs].z());

    G4VSolid *absorber_s = nullptr;
    if (mCollimatorSize[iabs] > 0.) {
      auto absorber_hole_s = new G4Box("absorber" + std::to_string(iabs) + "_hole_s",
				       0.5 * mCollimatorSize[iabs], 0.51 * mAbsorberSize[iabs].y(), 0.51 * mAbsorberSize[iabs].z());
      absorber_s = new G4SubtractionSolid("absorber" + std::to_string(iabs) + "_s", absorber_wall_s, absorber_hole_s);
    } else {
      absorber_s = absorber_wall_s;
    }
    
    auto absorber_lv = new G4LogicalVolume(absorber_s, absorber_m, "absorber" + std::to_string(iabs) + "_lv");

    auto absorber_pv = new G4PVPlacement(nullptr,
					 G4ThreeVector(mAbsorberPos[iabs].x(), mAbsorberPos[iabs].y(), mAbsorberPos[iabs].z()),
					 absorber_lv,
					 "absober" + std::to_string(iabs) + "_pv",
					 system_lv,
					 false,
					 iabs,
					 false);
  }

  /** sensor **/
  auto sensor_s = new G4Box("sensor_s", 0.5 * mSensorSize.x(), 0.5 * mSensorSize.y(), 0.5 * mSensorSize.z());
  auto sensor_lv = new G4LogicalVolume(sensor_s, silicon, "sensor_lv");
  auto sensor_pv = new G4PVPlacement(nullptr,
				     G4ThreeVector(mSensorPos.x(), mSensorPos.y(), mSensorPos.z()),
				     sensor_lv,
				     "sensor_pv",
				     system_lv,
				     false,
				     0,
				     false);
  
  mSensorPV = sensor_pv;
  RootIO::Instance()->setSensorInstanceID(sensor_pv->GetInstanceID());
  
  return world_pv;

}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandField()
{
  auto sensor_sd = new SensitiveDetector("sensor_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensor_sd);
  SetSensitiveDetector("sensor_lv", sensor_sd);
}

/*****************************************************************/

