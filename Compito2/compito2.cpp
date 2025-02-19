
#include <cmath>
#include <fstream>
#include <vector>
#include<functional>
#include<iostream>

#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TLegend.h>


#include "OdeSolver.h"
#include "Vector.h"
#include "MatPoint.h"

using namespace std;

Vector fInternal(unsigned int i,unsigned int j, double t, vector<MatPoint> p){
  //STEP 2 definizione forza interna
  Vector F_int(p[i].Mass()*p[j].GravField(p[i].R()));
  return F_int;
}

Vector fExternal(unsigned int i, double t, vector<MatPoint> p){
  //STEP 2 definizione forza esterna
  Vector F_ext(0,0,0);
  return F_ext;
}


int main(){ 
  
  TApplication app("app",0,NULL);
  gStyle->SetOptStat(0);

  string method_name;
  cout<<"What method do you want to use?"<<endl;
  cout<<"(Enter 'ELR' for Eulero,'RK2' for Runge-Kutta2 and 'VV' for Verltet velocity)"<<endl;
  cin>>method_name;

  //Lettura dei dati dal file
  vector<MatPoint> V;
  Particle p;
  Vector r;
  Vector v;
  double mass;
  double x,y,z,vx,vy,vz;
  ifstream f("fileInput");

  OdeSolver ode;
  ode.SetMethod(method_name);

  while (f >> mass >> vx >> x >> vy >> y >> vz >> z){
    MatPoint MP(mass,0,Vector(x,y,z),Vector(vx,vy,vz));
    ode.SetMatPoint(MP);
  }

  // STEP 1 creazione dell'oggetto della classe OdeSolver
  //   - creazione oggetto OdeSolver vuoto, assegnazione con il metodo Punto
  //   - creazione di vector<MatPoint> e poi creazione di OdelSover

  // Creazione classe OdeSolver (per la soluzione dell'equ. diff.)
  ode.fInternal = fInternal;
  ode.fExternal = fExternal;
  ode.Step(0.5);

  //Creazione dei grafici delle traiettorie e del grafico del momento
  vector<TGraph> gr(ode.N());
  TGraph gr3;

  TCanvas c("c","",10,10,500,500);
  c.Divide(2,1,0.01,0.01,0);

  c.cd(2);
  //Preparazione grafico delle coordinate dei pianeti
  double size=5; // numero unita' astronimiche
  gPad->DrawFrame(-size,-size,size,size);
  auto legend = new TLegend(0.1,0.7,0.2,0.9);
  legend->AddEntry(&gr[0],"Sun","l");
  legend->AddEntry(&gr[1],"Mercury","l");
  legend->AddEntry(&gr[2],"Venus","l");
  legend->AddEntry(&gr[3],"Earth","l");
  legend->AddEntry(&gr[4],"Moon","l");
  legend->AddEntry(&gr[5],"Mars","l");
  legend->AddEntry(&gr[6],"Jupiter","l");
  legend->AddEntry(&gr[7],"Saturn","l");
  legend->AddEntry(&gr[8],"Uranus","l");
  legend->AddEntry(&gr[9],"Neptune","l");
  legend->AddEntry(&gr[10],"Pluto","l");
  legend->Draw();
  int color[11]={kOrange+1,kViolet+1,kGreen+2,kAzure+1,kRed+2,kRed-7,kCyan-8,kBlue-7,kBlue+1,kBlue+2,kGray};

  double Ltot0=ode.GetMomentum();  //Calcolo il momento totale prima del ciclo
  cout<<Ltot0<<endl;
  c.cd(1);
  //gPad->DrawFrame(0,1.79e-8,380,1.81e-8);
  gPad->DrawFrame(0,-1e-12,380,1e-12);
  gr3.SetPoint(0,0,0);
  gr3.SetMarkerColor(kRed);
  gr3.SetMarkerSize(10);
  gr3.SetTitle("Momento angolare totale vs tempo;t[s];Ltot[Kg*m^2/s]");
  gr3.Draw("P");

  c.cd(2);
  for (unsigned int i=0;i<ode.N();i++){
    gr[i].SetPoint(0,ode.GetMatPoint(i).R().X(),ode.GetMatPoint(i).R().Y());
    gr[i].SetMarkerColor(color[i]); gr[i].SetMarkerStyle(20); gr[i].SetLineColor(color[i]);
    if (i==0) gr[i].SetMarkerSize(1);
    else gr[i].SetMarkerSize(0.2);
    gr[i].Draw("P");
  }
  gPad->Modified(); gPad->Update();

  //Run del metodo numerico + grafico in tempo reale delle coordinate e del mom. angolare totale
  double a,b,w;
  double S=365;
  while (ode.T()<S){
    ode.Solve();
    for (unsigned int i=0;i<ode.N();i++){
      //STEP 4 riempimento del grafico gr[i] con le coordinate aggiornate dei pianeti
      gr[i].SetPoint(gr[i].GetN(),ode.GetMatPoint(i).R().X(),ode.GetMatPoint(i).R().Y());

      //if (ode.GetMomentum()<100*Ltot0 && ode.GetMomentum()>0){  //Faccio il grafico del momento angolare solo se i valori del momento che ottengo durante il ciclo sono al
      gr3.SetPoint(gr[i].GetN(),ode.T(),ode.GetMomentum()-Ltot0);     //massimo 2 ordini di grandezza più grandi del momento calcolato prima del ciclo e solo se i valori che ottengo
      //}                                                         //sono positivi in quanto sto calcolando il modulo di un vettore.
    }
   if(ode.T()==ode.Step()){
    a=ode.GetMomentum();
    }
    if(ode.T()==S-ode.Step()){
    b=ode.GetMomentum();
    }
   c.cd(1);
   gPad->Modified(); gPad->Update();
   c.cd(2);
   gPad->Modified(); gPad->Update();
  }

 app.Run(true);

  return 0;
  
}
