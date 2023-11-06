#include "RootIO.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"

RootIO *RootIO::mInstance = nullptr;

RootIO::RootIO()
{
    mNIEL_neutron = new TGraph("niel.neutron.dat");
    mNIEL_neutron->Sort();
    mNIEL_proton = new TGraph("niel.proton.dat");
    mNIEL_proton->Sort();
    mNIEL_pion = new TGraph("niel.pion.dat");
    mNIEL_pion->Sort();
    mNIEL_electron = new TGraph("niel.electron.dat");
    mNIEL_electron->Sort();
}

void
RootIO::Open(std::string filename)
{
  if (mInteractive) return;

  mFile = TFile::Open(filename.c_str(), "RECREATE");

  mNIEL = new TH2F("hNIEL", ";x (cm);y (cm)", 1000, -2.5, 2.5, 1000, -2.5, 2.5);
  mNIELX = new TH2F("hNIELX", ";x (cm)", 1000, -2.5, 2.5, 1000, 0., 10.);

}

void
RootIO::Fill(G4Step *aStep)
{
  if (mInteractive) return;
  if (!aStep->IsFirstStepInVolume()) return;

  auto preStep = aStep->GetPreStepPoint();    
  auto track = aStep->GetTrack();
  auto particle = track->GetParticleDefinition();
  auto volume = track->GetOriginTouchable()->GetVolume();

  if (volume->GetInstanceID() == mSensorInstanceID) return; // track created in sensor volume

  auto touchable = preStep->GetTouchable();
  auto translation = touchable->GetTranslation();
  auto position = preStep->GetPosition();

  auto x = position.x() - translation.x();
  auto y = position.y() - translation.y();
  auto Ekin = preStep->GetKineticEnergy();
  auto pdg = particle->GetPDGEncoding();
  
  if (mHistoXY.count(pdg) == 0) {
    mHistoXY[pdg] = new TH2F(Form("hHistoXY_%d", pdg), ";x (cm);y (cm)", 3000, -30., 30., 3000, -30., 30.);
    mHistoEkinX[pdg] = new TH2F(Form("hHistoEkinX_%d", pdg), ";x (cm);E_{kin} (MeV)", 1000, -5., 5., 2000., 0., 200.);
    mHistoEkinX_log[pdg] = new TH2F(Form("hHistoEkinX_log_%d", pdg), ";x (cm);log_{10}(E_{kin}/eV)", 1000, -5., 5., 900, 0., 9.);
  }
  mHistoXY[pdg]->Fill(x / cm, y / cm);
  mHistoEkinX[pdg]->Fill(x / cm, Ekin / MeV);
  mHistoEkinX_log[pdg]->Fill(x / cm, std::log10(Ekin / eV));

  auto niel = 0.;
  if (abs(pdg) == 2112) niel = mNIEL_neutron->Eval(Ekin / MeV);
  if (abs(pdg) == 2212) niel = mNIEL_proton->Eval(Ekin / MeV);
  if (abs(pdg) == 211) niel = mNIEL_pion->Eval(Ekin / MeV);
  if (abs(pdg) == 11) niel = mNIEL_electron->Eval(Ekin / MeV);

  if (niel > 0.) {
    mNIEL->Fill(x / cm, y / cm, niel);
    mNIELX->Fill(x / cm, niel);
  }
}

void
RootIO::Close()
{
  if (mInteractive) return;

  mFile->cd();

  for (auto &h : mHistoXY) {
    h.second->Write();
    delete h.second;
  }
  mHistoXY.clear();
  
  for (auto &h : mHistoEkinX) {
    h.second->Write();
    delete h.second;
  }
  mHistoEkinX.clear();

  for (auto &h : mHistoEkinX_log) {
    h.second->Write();
    delete h.second;
  }
  mHistoEkinX_log.clear();

  mNIEL->Write();
  mNIELX->Write();

  mFile->Close();
}

