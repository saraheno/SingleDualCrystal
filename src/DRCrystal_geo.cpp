//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
//
// Specialized generic detector constructor
// 
//==========================================================================
#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"



using namespace std;
using namespace dd4hep;
using namespace dd4hep::detail;

static Ref_t create_detector(Detector& description, xml_h e, SensitiveDetector sens)  {

  std::cout<<"Creating DRCrystal"<<std::endl;

  static double tol = 0.0;
  // material to underly it all
  Material      air       = description.air();



  xml_det_t     x_det     = e;


  // for volume tags in detector
  int           det_id    = x_det.id();
  std::cout<<" det_id is "<<det_id<<std::endl;
  string        det_name  = x_det.nameStr();
  std::cout<<" det_name is .. "<<det_name<<std::endl;








  // look in DDCore/include/Parsers/detail/Dimension.h for arguments
  xml_comp_t    x_dim     = x_det.dimensions();


  double hwidth   = x_dim.width()/2.;
  double hzmax = x_dim.z_length()/2.;
  std::cout<<"half width zmax are "<<hwidth<<" "<<hzmax<<std::endl;


  OpticalSurfaceManager surfMgr = description.surfaceManager();

  OpticalSurface cryS  = surfMgr.opticalSurface("/world/"+det_name+"#dielectricSurface");




  // these refer to different fields in the xml file for this detector
  xml_comp_t fX_struct( x_det.child( _Unicode(structure) ) );
  xml_comp_t fX_sipm_l( fX_struct.child( _Unicode(slice) ) );
  xml_comp_t fX_sipm_r( fX_struct.child( _Unicode(disk) ) );
  xml_comp_t fX_crystal( fX_struct.child( _Unicode(shape) ) );




  DetElement    sdet      (det_name,det_id);
  Volume        motherVol = description.pickMotherVolume(sdet);


  Box hedra  (10.*hwidth,10.*hwidth, 10.*hzmax);
  Volume        envelope  (det_name,hedra,air);
  PlacedVolume  env_phv   = motherVol.placeVolume(envelope,RotationZYX(0,0,0));

  env_phv.addPhysVolID("system",det_id);
  sdet.setPlacement(env_phv);  // associate the placed volume to the detector element
  sens.setType("calorimeter");
  envelope.setSensitiveDetector(sens); //To make the world sensitive to get steps visible in spew

  Position a_pos(0.,0.,0.);

  // make crystal
  std::cout<<"making crystal"<<std::endl;
  dd4hep::Box c_box(fX_crystal.depth(),fX_crystal.depth(),fX_crystal.thickness());
  dd4hep::Volume s_vol_crystal("crystal",c_box,description.material(fX_crystal.materialStr()));
  s_vol_crystal.setAttributes(description,fX_crystal.regionStr(),fX_crystal.limitsStr(),fX_crystal.visStr());
  if ( fX_crystal.isSensitive() ) {
    s_vol_crystal.setSensitiveDetector(sens);
  }
  DetElement crystal_det("crystal",det_id);
  a_pos={0.,0.,0.};
  PlacedVolume  crystal_phv   = envelope.placeVolume(s_vol_crystal,a_pos);
  crystal_phv.addPhysVolID("slice",1);
  crystal_det.setPlacement(crystal_phv);
  crystal_phv.addPhysVolID("system",det_id);


  // make sipm left 
  std::cout<<"making sipm left"<<std::endl;
  dd4hep::Box sl_box(fX_sipm_l.depth(),fX_sipm_l.depth(),fX_sipm_l.thickness());
  dd4hep::Volume s_vol_sipml("Sipml",sl_box,description.material(fX_sipm_l.materialStr()));
  s_vol_sipml.setAttributes(description,fX_sipm_l.regionStr(),fX_sipm_l.limitsStr(),fX_sipm_l.visStr());
  if ( fX_sipm_l.isSensitive() ) {
    s_vol_sipml.setSensitiveDetector(sens);
  }
  DetElement sipml_det("sipml",det_id);
  a_pos={0.,0.,-(fX_crystal.thickness()+fX_sipm_l.thickness())};
  PlacedVolume  sipml_phv   = envelope.placeVolume(s_vol_sipml,a_pos);
  sipml_phv.addPhysVolID("slice",2);
  sipml_det.setPlacement(sipml_phv);
  sipml_phv.addPhysVolID("system",det_id);


  // make sipm left 
  std::cout<<"making sipm right"<<std::endl;
  dd4hep::Box sr_box(fX_sipm_l.depth(),fX_sipm_l.depth(),fX_sipm_r.thickness());
  dd4hep::Volume s_vol_sipmr("Sipmr",sr_box,description.material(fX_sipm_r.materialStr()));
  s_vol_sipmr.setAttributes(description,fX_sipm_r.regionStr(),fX_sipm_r.limitsStr(),fX_sipm_r.visStr());
  if ( fX_sipm_r.isSensitive() ) {
    s_vol_sipmr.setSensitiveDetector(sens);
  }
  DetElement sipmr_det("sipmr",det_id);
  a_pos={0.,0.,(fX_crystal.thickness()+fX_sipm_r.thickness())};
  PlacedVolume  sipmr_phv   = envelope.placeVolume(s_vol_sipmr,a_pos);
  sipmr_phv.addPhysVolID("slice",3);
  sipmr_det.setPlacement(sipmr_phv);
  sipmr_phv.addPhysVolID("system",det_id);







  std::cout<<"exiting DRCrystal creator"<<std::endl;

  return sdet;
}

DECLARE_DETELEMENT(DD4hep_DRCrystal,create_detector)

DECLARE_DEPRECATED_DETELEMENT(DRCrystal,create_detector)
