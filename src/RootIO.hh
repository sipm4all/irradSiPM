#ifndef _RootIO_h_
#define _RootIO_h_

#include <string>
#include <map>

class G4Step;
class TFile;
class TH1F;
class TH2F;
class TGraph;

class RootIO
{
  
public:

  static RootIO *Instance() {
    if (!mInstance) mInstance = new RootIO();
    return mInstance;
  };

  void setInteractiveMode(bool val) { mInteractive = val; };
  void Open(std::string filename);
  void Fill(G4Step *aStep);
  void Close();

  void setSensorInstanceID(int val) { mSensorInstanceID = val; };
  
private:
  
  RootIO();

  static RootIO *mInstance;
  bool mInteractive = false;
  TFile *mFile = nullptr;
  TGraph *mNIEL_neutron, *mNIEL_proton, *mNIEL_pion, *mNIEL_electron;
  std::map<int, TH2F*> mHistoXY, mHistoEkinX, mHistoEkinX_log;
  TH2F *mNIEL = nullptr, *mNIELX = nullptr;
  
  int mSensorInstanceID;
  
};

#endif /** _RootIO_h_ **/
