
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
  cout<<"Inserire nome metodo"<<endl;
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

//std::cout<<V[1].R()<<" "<<V[1].V()<<std::endl;
//std::cout<<V[1].Mass()<<std::endl;

  // STEP 1 creazione dell'oggetto della classe OdeSolver
  //   - creazione oggetto OdeSolver vuoto, assegnazione con il metodo Punto
  //   - creazione di vector<MatPoint> e poi creazione di OdelSover

  // Creazione classe OdeSolver (per la soluzione dell'equ. diff.)
  ode.fInternal = fInternal;
  ode.fExternal = fExternal;
  ode.Step(0.5);

  
  //Creazione dei grafici (uno per pianeta)
  vector<TGraph> gr(ode.N());
  TCanvas c("c","",10,10,500,500);
  
  //Preparazione grafico delle coordinate dei pianeti
  double size=5; // 5 unita' astronimiche
  gPad->DrawFrame(-size,-size,size,size);
  int color[10]={kOrange+1,kViolet+1,kGreen+2,kAzure+1,kRed+2,kRed-7,kCyan-8,kBlue-7,kBlue+1,kBlue+2};
  for (unsigned int i=0;i<ode.N();i++){
    gr[i].SetPoint(0,ode.GetMatPoint(i).R().X(),ode.GetMatPoint(i).R().Y());
    gr[i].SetMarkerColor(color[i]); gr[i].SetMarkerStyle(20); 
    if (i==0) gr[i].SetMarkerSize(1);
    else gr[i].SetMarkerSize(0.2);
    gr[i].Draw("P");
  }
  gPad->Modified(); gPad->Update();
  app.Run(true);

  //Run del metodo numerico + grafico in tempo reale delle coordinate e del mom. angolare totale
  vector<double> L(ode.N());
  vector<double> w(ode.N());
  vector<double> b(ode.N());
  vector<double> a(ode.N());

  double S=300;
  while (ode.T()<S){
    ode.Solve();
    for (unsigned int i=0;i<ode.N();i++){
      //STEP 4 riempimento del grafico gr[i] con le coordinate aggiornate dei pianeti
      gr[i].SetPoint(gr[i].GetN(),ode.GetMatPoint(i).R().X(),ode.GetMatPoint(i).R().Y());
      L[i]=ode.GetMomentum(i);
    }
    if(ode.T()==0.5){
    w=L;
    }
    if(ode.T()==S-0.5){
    b=L;
    }
  gPad->Modified(); gPad->Update();
  }

    for (unsigned int i=0;i<ode.N();i++){
    a[i]=abs(b[i]/w[i]-1)*100;
    cout<<a[i]<<endl;
    }

  app.Run(true);

  return 0;
  
}
