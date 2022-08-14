#include "LArDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"


LArDetectorConstruction::LArDetectorConstruction()
: G4VUserDetectorConstruction(),
  active_volume(0)
{ }


LArDetectorConstruction::~LArDetectorConstruction()
{ }


G4VPhysicalVolume* LArDetectorConstruction::Construct()
{
  // Get NIST material manager.
  G4NistManager* nist = G4NistManager::Instance();

  // Polyurethane
  G4Element* carbon = nist->FindOrBuildElement("C", true);
  G4Element* hydrogen = nist->FindOrBuildElement("H", true);
  G4Element* nitrogen = nist->FindOrBuildElement("N", true);
  G4Element* oxygen = nist->FindOrBuildElement("O", true);

  auto *polyurethane = new G4Material("Polyurethane",
				      90.*mg/cm3,
				      4,
				      kStateSolid);
  polyurethane->AddElement(carbon, 25);
  polyurethane->AddElement(hydrogen, 42);
  polyurethane->AddElement(nitrogen, 2);
  polyurethane->AddElement(oxygen, 6);

  // Stainless steel
  G4Element* chromium = nist->FindOrBuildElement("Cr", true);
  G4Element* iron = nist->FindOrBuildElement("Fe", true);
  G4Element* nickel = nist->FindOrBuildElement("Ni", true);
  auto *stainless_steel = new G4Material("StainlessSteel",
					 7.85*g/cm3,
					 4,
					 kStateSolid);
  stainless_steel->AddElement(carbon, 0.0010);
  stainless_steel->AddElement(chromium, 0.1792);
  stainless_steel->AddElement(iron, 0.7298);
  stainless_steel->AddElement(nickel, 0.0900);

  // World creation.
  G4double cav_x(19.8*m), cav_y(28.0*m), cav_z(0.5*144.5*m);
  G4Material* ambient_mat(nist->FindOrBuildMaterial("G4_AIR"));
  G4Box* world
    = new G4Box("world",
		0.5*cav_x,
		0.5*cav_y,
		0.5*cav_z);
  G4LogicalVolume* logical_world
    = new G4LogicalVolume(world,
			  ambient_mat,
			  "world");
  G4VPhysicalVolume* physical_world
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_world,
			"world",
			0,
			false,
			0,
			false);
  // Cryostat.
  G4double cryo_x(18.9*m), cryo_y(17.8*m), cryo_z(65.8*m);
  
  G4Box* cryo_outer
    = new G4Box("cryostat_outer",
		0.5*cryo_x,
		0.5*cryo_y,
		0.5*cryo_z);
  G4LogicalVolume* logical_cryo_outer
    = new G4LogicalVolume(cryo_outer,
			  stainless_steel,
			  "cryostat_outer");
  G4VPhysicalVolume* physical_cryo_outer
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_cryo_outer,
			"cryostat_outer",
			logical_world,
			false,
			0,
			false);

  G4Box* cryo_foam
    = new G4Box("cryostat_foam",
		0.5*cryo_x - 1.2*cm,
		0.5*cryo_y - 1.2*cm,
		0.5*cryo_z - 1.2*cm);
  G4LogicalVolume* logical_cryo_foam
    = new G4LogicalVolume(cryo_foam,
			  polyurethane,
			  "cryostat_foam");
  G4VPhysicalVolume* physical_cryo_foam
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_cryo_foam,
			"cryostat_foam",
			logical_cryo_outer,
			false,
			0,
			false);

  G4Box* cryo_inner
    = new G4Box("cryostat_inner",
		0.5*cryo_x - 1.2*cm - 80.0*cm,
		0.5*cryo_y - 1.2*cm - 80.0*cm,
		0.5*cryo_z - 1.2*cm - 80.0*cm);
  G4LogicalVolume* logical_cryo_inner
    = new G4LogicalVolume(cryo_inner,
			  stainless_steel,
			  "cryostat_inner");
  G4VPhysicalVolume* physical_cryo_inner
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_cryo_inner,
			"cryostat_inner",
			logical_cryo_foam,
			false,
			0,
			false);
  // Full argon volume.
  //G4double vol_x(15.1*m), vol_y(14.0*m), vol_z(62.0*m);
  G4double vol_x(cryo_x - 1.2*cm - 80.0*cm - 0.12*cm);
  G4double vol_y(cryo_y - 1.2*cm - 80.0*cm - 0.12*cm);
  G4double vol_z(cryo_z - 1.2*cm - 80.0*cm - 0.12*cm);
  G4Material* vol_mat(nist->FindOrBuildMaterial("G4_lAr"));
  G4Box* vol
    = new G4Box("vol",
		0.5*vol_x,
		0.5*vol_y,
		0.5*vol_z);
  G4LogicalVolume* logical_vol
    = new G4LogicalVolume(vol,
			  vol_mat,
			  "vol");
  G4VPhysicalVolume* physical_vol
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_vol,
			"vol",
			logical_cryo_inner,
			false,
			0,
			false);
  // Active volume.
  G4double act_x(14.0*m), act_y(12.0*m), act_z(58.2*m);
  G4Material* act_mat(nist->FindOrBuildMaterial("G4_lAr"));
  G4Box* act
    = new G4Box("act",
		0.5*act_x,
		0.5*act_y,
		0.5*act_z);
  G4LogicalVolume* logical_act
    = new G4LogicalVolume(act,
			  act_mat,
			  "act");
  G4VPhysicalVolume* physical_act
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_act,
			"vol",
			logical_vol,
			false,
			0,
			false);
  // Target volume.
  G4double tar_x(1.0*m), tar_y(1.0*m), tar_z(1.0*cm);
  G4Material* target_mat(nist->FindOrBuildMaterial("G4_Pb"));
  G4Box* target
    = new G4Box("target",
		0.5*tar_x,
		0.5*tar_y,
		0.5*tar_z);
  G4LogicalVolume* logical_target
    = new G4LogicalVolume(target,
			  target_mat,
			  "target");
  G4VPhysicalVolume* physical_target
    = new G4PVPlacement(0,
			G4ThreeVector(0.0*m, 0.0*m, -0.5*cryo_z - 0.5*tar_z - 10.0*cm),
			logical_target,
			"target",
			logical_world,
			false,
			0,
			false);
  
  //active_volume = logical_act;
  active_volume = logical_act;

  return physical_world;
}
