#include "LArVoxelizedDetectorConstruction.hh"
#include "LArSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"


LArVoxelizedDetectorConstruction::LArVoxelizedDetectorConstruction()
: G4VUserDetectorConstruction(),
  active_volume(0),
  target_thickness(30.0)
{
  messenger = new G4GenericMessenger(this, "/larvol/", "Base for LArVol parameters.");
  messenger->DeclareMethod("thickness", &LArVoxelizedDetectorConstruction::SetTarget);
}

void LArVoxelizedDetectorConstruction::SetTarget(G4String n)
{
  target_thickness = std::stof(n);
}


LArVoxelizedDetectorConstruction::~LArVoxelizedDetectorConstruction()
{ }


G4VPhysicalVolume* LArVoxelizedDetectorConstruction::Construct()
{
  std::cout << "Target thickness: " << target_thickness << "!" << std::endl;
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
    = new G4Box("bx_world",
		0.5*cav_x,
		0.5*cav_y,
		0.5*cav_z);
  G4LogicalVolume* logical_world
    = new G4LogicalVolume(world,
			  ambient_mat,
			  "lv_world");
  G4VPhysicalVolume* physical_world
    = new G4PVPlacement(0,
			G4ThreeVector(),
			logical_world,
			"sv_world",
			0,
			false,
			0,
			false);
  // Cryostat.
  G4double cryo_x(1672.64*cm), cryo_y(1562.64*cm), cryo_z(6362.64*cm);
  G4Box* cryo_outer
    = new G4Box("bx_cryostat_outer",
		0.5*cryo_x,
		0.5*cryo_y,
		0.5*cryo_z);
  G4LogicalVolume* logical_cryo_outer
    = new G4LogicalVolume(cryo_outer,
			  stainless_steel,
			  "lv_cryostat_outer");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logical_cryo_outer,
		    "sv_cryostat_outer",
		    logical_world,
		    false,
		    0,
		    false);

  G4double cryo_foam_x(1670.24*cm), cryo_foam_y(1560.24*cm), cryo_foam_z(6360.24*cm);
  G4Box* cryo_foam
    = new G4Box("bx_cryostat_foam",
		0.5*cryo_foam_x,
		0.5*cryo_foam_y,
		0.5*cryo_foam_z);
  G4LogicalVolume* logical_cryo_foam
    = new G4LogicalVolume(cryo_foam,
			  polyurethane,
			  "lv_cryostat_foam");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logical_cryo_foam,
		    "sv_cryostat_foam",
		    logical_cryo_outer,
		    false,
		    0,
		    false);

  G4double cryo_inner_x(1510.24*cm), cryo_inner_y(1400.24*cm), cryo_inner_z(6200.24*cm);
  G4Box* cryo_inner
    = new G4Box("bx_cryostat_inner",
		0.5*cryo_inner_x,
		0.5*cryo_inner_y,
		0.5*cryo_inner_z);
  G4LogicalVolume* logical_cryo_inner
    = new G4LogicalVolume(cryo_inner,
			  stainless_steel,
			  "lv_cryostat_inner");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logical_cryo_inner,
		    "sv_cryostat_inner",
		    logical_cryo_foam,
		    false,
		    0,
		    false);
  
  // Full argon volume.
  G4double vol_x(1510*cm), vol_y(1400*cm), vol_z(6200*cm);
  G4Material* vol_mat(nist->FindOrBuildMaterial("G4_lAr"));
  G4Box* vol
    = new G4Box("bx_volume",
		0.5*vol_x,
		0.5*vol_y,
		0.5*vol_z);
  G4LogicalVolume* logical_vol
    = new G4LogicalVolume(vol,
			  vol_mat,
			  "lv_volume");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logical_vol,
		    "sv_volume",
		    logical_cryo_inner,
		    false,
		    0,
		    false);

  // Active volume.
  G4Box* act
    = new G4Box("bx_active",
		1401*cm / 2.0,
		1200*cm / 2.0,
		5820*cm / 2.0);
  G4LogicalVolume* logical_act
    = new G4LogicalVolume(act,
			  vol_mat,
			  "lv_active");
  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logical_act,
		    "sv_active",
		    logical_vol,
		    false,
		    0,
		    false);
  
  // The logical volume "logical_act" contains the voxelized
  // region. The division into voxels will happen in stages
  // sequentially through division and replication along x,
  // y, and finally z.
  G4Box* voxx
    = new G4Box("bx_active_splitx",
		G4double(0.3*cm / 2.0),
		G4double(1200*cm / 2.0),
		G4double(5820*cm / 2.0));
  G4LogicalVolume* logical_voxx
    = new G4LogicalVolume(voxx, vol_mat, "lv_active_splitx");
  new G4PVReplica("rp_active_splitx",
		  logical_voxx,
		  logical_act,
		  kXAxis,
		  G4int(4670),
		  G4double(0.3*cm));
  
  G4Box* voxy
    = new G4Box("bx_active_splity",
		G4double(0.3*cm / 2.0),
		G4double(0.3*cm / 2.0),
		G4double(5820*cm / 2.0));
  G4LogicalVolume* logical_voxy
    = new G4LogicalVolume(voxy, vol_mat, "lv_active_splity");
  new G4PVReplica("rp_active_splity",
		  logical_voxy,
		  logical_voxx,
		  kYAxis,
		  G4int(4000),
		  G4double(0.3*cm));

  G4Box* voxz
    = new G4Box("bx_active_voxels",
		G4double(0.3*cm / 2.0),
		G4double(0.3*cm / 2.0),
		G4double(0.3*cm / 2.0));
  G4LogicalVolume* logical_voxelized
    = new G4LogicalVolume(voxz, vol_mat, "lv_active_voxels");
  new G4PVReplica("rp_active_voxels",
		  logical_voxelized,
		  logical_voxy,
		  kZAxis,
		  G4int(19400),
		  G4double(0.3*cm));

  

  // Target volume.
  G4double tar_x(30*cm), tar_y(30*cm), tar_z(target_thickness*cm);
  G4Material* target_mat(nist->FindOrBuildMaterial("G4_Pb"));
  G4Box* target
    = new G4Box("bx_target",
		0.5*tar_x,
		0.5*tar_y,
		0.5*tar_z);
  G4LogicalVolume* logical_target
    = new G4LogicalVolume(target,
			  target_mat,
			  "lv_target");
  new G4PVPlacement(0,
		    G4ThreeVector(0.0*m, 0.0*m, -0.5*cryo_z - 0.5*tar_z - 10*cm),
		    logical_target,
		    "sv_target",
		    logical_world,
		    false,
		    0,
		    false);
  
  active_volume = logical_act;

  #ifndef G4MULTITHREADED
  std::cout << "Running in sequential mode." << std::endl;
  LArSensitiveDetector* sd = new LArSensitiveDetector("LArSD");
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();
  sd_manager->AddNewDetector(sd);
  logical_voxelized->SetSensitiveDetector(sd);
  #endif
  
  return physical_world;
}

void LArVoxelizedDetectorConstruction::ConstructSDandField()
{
  #ifdef G4MULTITHREADED
  std::cout << "Running in multithreaded mode." << std::endl;
  LArSensitiveDetector* sd = new LArSensitiveDetector("LArSD");
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();
  sd_manager->AddNewDetector(sd);
  SetSensitiveDetector("lv_active_voxels", sd);
  #endif
}
