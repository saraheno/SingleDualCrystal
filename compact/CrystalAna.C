#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "DD4hep/Printout.h"
#include "DD4hep/Objects.h"
#include "DD4hep/Factories.h"
#include "DDG4/Geant4Particle.h"
#include "DDG4/Geant4Data.h"
#include "../src/DualCrystalCalorimeterHit.h"

#include <vector>
#include <algorithm>


// information about hit channel IT numbers
const int nchan = 4;
const int ichan[nchan] = {64,73,74,75};  // channel 74 is the crystal, 73 and 75 the two kill media
std::string namechan[nchan] = {"air","PD1","crystal","PD2"};


void crystalana(int num_evtsmax, const char* inputfilename) {


  typedef std::vector<dd4hep::sim::Geant4Particle*> GenParts;
  typedef std::vector<CalVision::DualCrystalCalorimeterHit*> CalHits;
  typedef dd4hep::sim::Geant4HitData::MonteCarloContrib Contribution;
  typedef std::vector<dd4hep::sim::Geant4HitData::MonteCarloContrib> Contributions;



  // read in libraries that define the classes
  Long_t result;
  char text[1024];
  const char* dd4hep = gSystem->Getenv("DD4hepINSTALL");
  snprintf(text,sizeof(text)," -I%s/include -D__DD4HEP_DDEVE_EXCLUSIVE__ -Wno-shadow -g -O0",dd4hep);
  gSystem->AddIncludePath(text);
  TString fname = "libDDG4IO";
  const char* io_lib = gSystem->FindDynamicLibrary(fname,kTRUE);
  result = gSystem->Load("libDDG4IO");
  result = gSystem->Load("libDDEvePlugins");
  result = gSystem->Load("libDDEvePlugins");
  result = gSystem->Load("libSingleDualCrystal");
  result = gSystem->Load("libDDG4Plugins");


  // define histograms

  //gen particles
  TH1F *hgenPsize = new TH1F("hgenPsize","number of generator particles",600,0.,40000);
  TH1F *hgenPdgID = new TH1F("hgenpdgID","pdgID of generator particles",600,-200,200);


  // calorimeter infor
  TH1F *hchan = new TH1F("hchan","channel ID number",1028,0.,1028);
  TH1F *hcEcalE = new TH1F("hcEcalE","sum crystal ecal energy",100,0.,100.);
  TH1F *hcEcalncer = new TH1F("hcEcalncer","total number of cerenkov",100,0.,10000);
  TH1F *hcEcalncer0 = new TH1F("hcEcalncer0","total number of cerenkov chan 0",100,0.,10000);
  TH1F *hcEcalncer1 = new TH1F("hcEcalncer1","total number of cerenkov chan 1",100,0.,10000);
  TH1F *hcEcalncer2 = new TH1F("hcEcalncer2","total number of cerenkov chan 2",100,0.,10000);
  TH1F *hcEcalncer3 = new TH1F("hcEcalncer3","total number of cerenkov chan 3",100,0.,10000);

  TH1F *hcEcalE0 = new TH1F("hcEcalE0","energy chan 0",100,0.,10000);
  TH1F *hcEcalE1 = new TH1F("hcEcalE1","energy chan 1",100,0.,10000);
  TH1F *hcEcalE2 = new TH1F("hcEcalE2","energy chan 2",100,0.,10000);
  TH1F *hcEcalE3 = new TH1F("hcEcalE3","energy chan 3",100,0.,10000);



  TH1F *hcerwave0 = new TH1F("hcerwave0","cerenkov wavelength",1000,0.,1000.);
  TH1F *hscintwave0 = new TH1F("hscintwave0","scintillator wavelength",1000,0.,1000.);
  TH1F *hcertime0 = new TH1F("hcertime0","cerenkov time",1000,0.,1000.);
  TH1F *hscinttime0 = new TH1F("hscinttime0","scintillator time",1000,0.,1000.);
  TH2F *hcerxy0 = new TH2F("hcerxy0","x y hit position",40,0.,40.,40,0.,40.);
  TH2F *hscintxy0 = new TH2F("hscintxy0","x y hit position",40,0.,40.,40,0.,40.);

  TH1F *hcerwave1 = new TH1F("hcerwave1","cerenkov wavelength",1000,0.,1000.);
  TH1F *hscintwave1 = new TH1F("hscintwave1","scintillator wavelength",1000,0.,1000.);
  TH1F *hcertime1 = new TH1F("hcertime1","cerenkov time",1000,0.,1000.);
  TH1F *hscinttime1 = new TH1F("hscinttime1","scintillator time",1000,0.,1000.);
  TH2F *hcerxy1 = new TH2F("hcerxy1","x y hit position",40,0.,40.,40,0.,40.);
  TH2F *hscintxy1 = new TH2F("hscintxy1","x y hit position",40,0.,40.,40,0.,40.);

  TH1F *hcerwave2 = new TH1F("hcerwave2","cerenkov wavelength",1000,0.,1000.);
  TH1F *hscintwave2 = new TH1F("hscintwave2","scintillator wavelength",1000,0.,1000.);
  TH1F *hcertime2 = new TH1F("hcertime2","cerenkov time",1000,0.,1000.);
  TH1F *hscinttime2 = new TH1F("hscinttime2","scintillator time",1000,0.,1000.);
  TH2F *hcerxy2 = new TH2F("hcerxy2","x y hit position",40,0.,40.,40,0.,40.);
  TH2F *hscintxy2 = new TH2F("hscintxy2","x y hit position",40,0.,40.,40,0.,40.);

  TH1F *hcerwave3 = new TH1F("hcerwave3","cerenkov wavelength",1000,0.,1000.);
  TH1F *hscintwave3 = new TH1F("hscintwave3","scintillator wavelength",1000,0.,1000.);
  TH1F *hcertime3 = new TH1F("hcertime3","cerenkov time",1000,0.,1000.);
  TH1F *hscinttime3 = new TH1F("hscinttime3","scintillator time",1000,0.,1000.);
  TH2F *hcerxy3 = new TH2F("hcerxy3","x y hit position",40,0.,40.,40,0.,40.);
  TH2F *hscintxy3 = new TH2F("hscintxy3","x y hit position",40,0.,40.,40,0.,40.);


  // open data and output file for histograms

  //  const char* inputfilename="/data/users/eno/dd4hep/DD4hep/DDDetectors/compact/testSid.root";
  const char* outputfilename="hist.root";

  // get Tree
  //  TFile *f = new TFile(inputfilename);
  //f->Print();
  GenParts* pgenparts = new GenParts();
  CalHits* pcalhits = new CalHits();
  int num_evt,nbyte;

  TFile* f = TFile::Open(inputfilename);
  TTree* t = (TTree*)f->Get("EVENT;1");
  t->Print();



  
  // loop over events
  TBranch* b_mc = t->GetBranch("MCParticles");
  TBranch* b_ecal = t->GetBranch("DRCNoSegment");
  int ihaha = b_mc->GetEntries();
  num_evt= std::min(ihaha,num_evtsmax);
  std::cout<<" doing "<<b_mc->GetName()<<std::endl;
  std::cout<<"num_evt gen loop is "<<num_evt<<std::endl;
  
  
  if(num_evt>0) {


    // find branches
    GenParts* gens = new GenParts();
    b_mc->SetAddress(&gens);
    CalHits* ecalhits = new CalHits();
    b_ecal->SetAddress(&ecalhits);


    int SCEPRINT2=10;
    for(int ievt=0;ievt<num_evt; ++ievt) {
      std::cout<<std::endl<<std::endl<<"event number is "<<ievt<<std::endl;


      // gen particles
      nbyte = b_mc->GetEntry(ievt);
      if( nbyte>0) {
	if(ievt<SCEPRINT2) std::cout<<gens->size()<<" Gen particles "<<std::endl;
      }
      float mainee=-1;
      hgenPsize->Fill(gens->size());
      for(size_t i=0;i<gens->size(); ++i) {
        dd4hep::sim::Geant4Particle* agen =gens->at(i);
        hgenPdgID->Fill(agen->pdgID);

	float px=agen->psx;
	float py=agen->psy;
	float pz=agen->psz;
	float mass=agen->mass;
	float ee=sqrt(mass*mass+px*px+py*py+pz*pz);

	if(i==0&&ievt<SCEPRINT2) std::cout<<" first particle pid energy are "<<agen->pdgID<<" "<<ee<<std::endl;

	if(ee>mainee) {
	  mainee=ee;
	  if(ievt<SCEPRINT2) std::cout<<"highest e  gen now has pid "<<agen->pdgID<<" energy "<<ee<<std::endl;
	}
	float vsy=agen->vsy;
	if(vsy>20) std::cout<<" pid "<<agen->pdgID<<" "
			      <<"vs ("<<agen->vsx<<","<<agen->vsy<<","<<agen->vsz<<") "
			      <<"ve ("<<agen->vex<<","<<agen->vey<<","<<agen->vez<<") "
			      <<"ps ("<<agen->psx<<","<<agen->psy<<","<<agen->psz<<") "
			      <<"pe ("<<agen->pex<<","<<agen->pey<<","<<agen->pez<<") "
			      <<std::endl;



      }



    // ECAL hits  
    // there are hits in the crystal and also the photodetectors "kill media"
    // in the crystal, photons created in the crystal are counted and their wavelengths stored
    // in the photodetector, photons that enter are counted, wavelength stored, and then they are killed


      int nbyte = b_ecal->GetEntry(ievt);
      if( nbyte>0) {
        if(ievt<SCEPRINT2) std::cout<<ecalhits->size()<<" Ecal Hits "<<std::endl;
      }
      float esum=0.;
      float esumchan[nchan]={0.,0.,0.,0.};
      int ncerchan[nchan]={0,0,0,0};
      int nscintchan[nchan]={0,0,0,0};
      int ncertot=0;
      int nscinttot=0;
      int SCEPRINT=10;
      for(size_t i=0;i<ecalhits->size(); ++i) {
	CalVision::DualCrystalCalorimeterHit* aecalhit =ecalhits->at(i);
	//	std::cout<<"       "<<i<<" energy "<<aecalhit->energyDeposit<<std::endl;

	int jchan=aecalhit->cellID;
	int kchan=-1;
	for( int i=0;i<nchan;i++ ) {
	  if(ichan[i]==jchan) kchan=i;
	}



	esum+=aecalhit->energyDeposit;
	ncertot+=aecalhit->ncerenkov;
	nscinttot+=aecalhit->nscintillator;
	if(i<SCEPRINT&&ievt<SCEPRINT2) std::cout<<" hit channel is "<<aecalhit->cellID<<" in hex is "<< std::hex<< aecalhit->cellID<<std::dec<<" "<<aecalhit->energyDeposit<<" "<<aecalhit->ncerenkov<<" "<<aecalhit->nscintillator<<std::endl;




	// see ../src/DRCrystal_geo.cpp to see the assignments
	int ihitchan=aecalhit->cellID;
	int idet = (ihitchan & 0xC0)>>6;  // this assignment is made in SCEPCALConstants.xml
	int ilayer = (ihitchan & 0x38)>>3; // this is 1 for crystal and detectors, 0 for air around it
	int islice = (ihitchan & 0x07);  //   this is 1 or 4 for photodetectors, 2 for crystal
	// channels are 64 air
	//             73 75 detectors
	//            74 crystal
	if(i<SCEPRINT&&ievt<SCEPRINT2) std::cout<<" idet,ilayer,islice is ("<<idet<<","<<ilayer<<","<<islice<<")"<<std::endl;



	// print out wavelength spectra
	int ijchan=aecalhit->nfinebin;
	for (int j=0;j<ijchan;j++) {
	//std::cout<<"  ncerwave["<<j<<"]="<<(aecalhit->ncerwave)[j]<<std::endl;
	//std::cout<<"  nscintwave["<<j<<"]="<<(aecalhit->nscintwave)[j]<<std::endl;
	  if(kchan==0) {
	    hcerwave0->Fill(j,(aecalhit->ncerwave)[j]);
	    hscintwave0->Fill(j,(aecalhit->nscintwave)[j]);
	    hcertime0->Fill(j,(aecalhit->ncertime)[j]);
	    hscinttime0->Fill(j,(aecalhit->nscinttime)[j]);
	  }
	  else if(kchan==1) {
	    hcerwave1->Fill(j,(aecalhit->ncerwave)[j]);
	    hscintwave1->Fill(j,(aecalhit->nscintwave)[j]);
	    hcertime1->Fill(j,(aecalhit->ncertime)[j]);
	    hscinttime1->Fill(j,(aecalhit->nscinttime)[j]);
	  }
	  else if(kchan==2) {
	    hcerwave2->Fill(j,(aecalhit->ncerwave)[j]);
	    hscintwave2->Fill(j,(aecalhit->nscintwave)[j]);
	    hcertime2->Fill(j,(aecalhit->ncertime)[j]);
	    hscinttime2->Fill(j,(aecalhit->nscinttime)[j]);
	  }
	  else if(kchan==3) {
	    hcerwave3->Fill(j,(aecalhit->ncerwave)[j]);
	    hscintwave3->Fill(j,(aecalhit->nscintwave)[j]);
	    hcertime3->Fill(j,(aecalhit->ncertime)[j]);
	    hscinttime3->Fill(j,(aecalhit->nscinttime)[j]);
	  }

	}

	for (int j=0;j<aecalhit->ncoarsebin;j++){
	  for (int k=0;k<aecalhit->ncoarsebin;k++){
	    if(kchan==0) {
	      hcerxy0->Fill(j,k,(aecalhit->cerhitpos)[j][k]);	    
	      hscintxy0->Fill(j,k,(aecalhit->scinthitpos)[j][k]);
	    }
	    else if(kchan==1) {
	      hcerxy1->Fill(j,k,(aecalhit->cerhitpos)[j][k]);	    
	      hscintxy1->Fill(j,k,(aecalhit->scinthitpos)[j][k]);
	    }
	    else if(kchan==2) {
	      hcerxy2->Fill(j,k,(aecalhit->cerhitpos)[j][k]);	    
	      hscintxy2->Fill(j,k,(aecalhit->scinthitpos)[j][k]);
	    }
	    else if(kchan==3) {
	      hcerxy3->Fill(j,k,(aecalhit->cerhitpos)[j][k]);	    
	      hscintxy3->Fill(j,k,(aecalhit->scinthitpos)[j][k]);
	    }

	  }
	}


	hchan->Fill(aecalhit->cellID);

      // there is a better way to do this
	if(kchan==-1) {
	  std::cout<<"unknown hit channel is "<< aecalhit->cellID<<std::endl;
	} else {
	  esumchan[kchan]+=aecalhit->energyDeposit;
	  ncerchan[kchan]+=aecalhit->ncerenkov;
	  nscintchan[kchan]+=aecalhit->nscintillator;
	}


        // get MC truth information about individual contributions to this hit
        Contributions zxzz=aecalhit->truth;
        float hacheck=0.;
        for(size_t j=0;j<zxzz.size(); j++) {
          //      std::cout<<"testing truth truth number "<<i<<" with pdgID "<<(zxzz.at(i)).pdgID<<std::endl;
          // other member functions are trackID, deposit, time, length, x,y,z
          hacheck+=(zxzz.at(j)).deposit;
        }
        if(i<SCEPRINT2) {
	  std::cout<<"    difference between truth sum and total deposit is "<<hacheck-aecalhit->energyDeposit<<" where "<<aecalhit->energyDeposit<<" is the hit size."<<std::endl;
          if(aecalhit->energyDeposit>0) std::cout<<"      percent diff is "<<(hacheck-aecalhit->energyDeposit)/aecalhit->energyDeposit<<std::endl;
        }


      }  // end loop over hits

    
      hcEcalE->Fill(esum/1000.);
      hcEcalncer->Fill(ncertot);
      hcEcalncer0->Fill(ncerchan[0]);
      hcEcalncer1->Fill(ncerchan[1]);
      hcEcalncer2->Fill(ncerchan[2]);
      hcEcalncer3->Fill(ncerchan[3]);
      hcEcalE0->Fill(esumchan[0]);
      hcEcalE1->Fill(esumchan[1]);
      hcEcalE2->Fill(esumchan[2]);
      hcEcalE3->Fill(esumchan[3]);


      if(ievt<SCEPRINT2) std::cout<<" total energy deposit "<<esum<<std::endl;
      float check=0.;
      for( int i=0;i<nchan;i++) {
	if(ievt<SCEPRINT2) std::cout<<"esum ["<<namechan[i]<<"]="<<esumchan[i]<<std::endl;
	check+=esumchan[i];
      }
      if(ievt<SCEPRINT2) std::cout<<" check total energy desposit "<<check<<std::endl;

      if(ievt<SCEPRINT2) std::cout<<" total number of cherenkov is "<<ncertot<<std::endl;
      check=0;
      for( int i=0;i<nchan;i++) {
	if(ievt<SCEPRINT2) std::cout<<"ncerenkov ["<<namechan[i]<<"]="<<ncerchan[i]<<std::endl;
	check+=ncerchan[i];
      }
      if(ievt<SCEPRINT2) std::cout<<" check ncerenkov "<<check<<std::endl;


      if(ievt<SCEPRINT2) std::cout<<" total number of scintillator is "<<nscinttot<<std::endl;
      check=0;
      for( int i=0;i<nchan;i++) {
	if(ievt<SCEPRINT2) std::cout<<"nscintillator ["<<namechan[i]<<"]="<<nscintchan[i]<<std::endl;
	check+=nscintchan[i];
      }
      if(ievt<SCEPRINT2) std::cout<<" check nscintillator "<<check<<std::endl;


    }  //end loop over events
  }  // end if no events
    
  


  
 
 
  f->Close();

  TFile * out = new TFile(outputfilename,"RECREATE");
  hgenPsize->Write();
  hgenPdgID->Write();
  hcEcalE->Write();
  hcEcalncer->Write();
  hcEcalncer0->Write();
  hcEcalncer1->Write();
  hcEcalncer2->Write();
  hcEcalncer3->Write();


  hcerwave0->Write();
  hscintwave0->Write();
  hcerwave1->Write();
  hscintwave1->Write();
  hcerwave2->Write();
  hscintwave2->Write();
  hcerwave3->Write();
  hscintwave3->Write();

  hcertime0->Write();
  hscinttime0->Write();
  hcerxy0->Write();
  hscintxy0->Write();

  hcertime1->Write();
  hscinttime1->Write();
  hcerxy1->Write();
  hscintxy1->Write();

  hcertime2->Write();
  hscinttime2->Write();
  hcerxy2->Write();
  hscintxy2->Write();

  hcertime3->Write();
  hscinttime3->Write();
  hcerxy3->Write();
  hscintxy3->Write();



  out->Close();

}


