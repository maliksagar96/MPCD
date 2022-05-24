#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>
#include "mpcd.h"
#include <math.h>

//Number of particles
#define N 2

//Number of cells along x, y, z
#define xc 40     
#define yc 20
#define zc 20

// Length of the box along x, y, z
#define Lx 40
#define Ly 20
#define Lz 20

//Length of a single cell along x, y, z 
#define lx Lx/xc
#define ly Ly/yc
#define lz Lz/zc

//Total number of cells
#define cells xc*yc*zc   

using namespace std;
using namespace chrono;

double x[N], y[N], z[N];
double vx[N], vy[N], vz[N];

//Cell number for each particle in which it is present
int cellNum[N];

//Number of particles in the particluar cell
int Nc[cells];

//Avg velocity of 
double avgVx[cells], avgVy[cells], avgVz[cells];

void init() {

   unsigned seed = system_clock::now().time_since_epoch().count();
   default_random_engine generator(seed);
   uniform_real_distribution<double> xdistribution(0,Lx);
   uniform_real_distribution<double> ydistribution(0,Ly);
   uniform_real_distribution<double> zdistribution(0,Lz);

   uniform_real_distribution<double> velocityDistribution(0,1);

   for (int i = 0; i < N; ++i) {
      x[i] = xdistribution(generator);
      y[i] = ydistribution(generator);
      z[i] = zdistribution(generator);
      
      vx[i] = velocityDistribution(generator);
      vy[i] = velocityDistribution(generator);
      vz[i] = velocityDistribution(generator);
   }

   for(int i =0;i<cells;i++){
      avgVx[i] = 0;
      avgVy[i] = 0;
      avgVz[i] = 0;
      Nc[i] = 0;
   }
}

void assignGrids(){
   //Three integers a, b and c to tell in which cell the particle is.
   int a,b,c;
   int lengthx = lx;
   int lengthy = ly;
   int lengthz = lz;

   for(int i = 0;i<N;i++) {
      a = int(x[i]/lengthx);
      b = int(y[i]/lengthy);
      c = int(z[i]/lengthz);
      cellNum[i] = a + xc*b + xc*yc*c;
   }
}

/*Periodic boundary condition*/
void ppp() {
   for(int i =0;i<N;i++) {
      if(x[i]>Lx){
         x[i] = x[i] - Lx;
      }     

      if(x[i]<=0){
         x[i] = x[i] + Lx;
      }

      if(y[i]>Ly){
         y[i] = y[i] - Ly;
      }     

      if(y[i]<=0){
         y[i] = y[i] + Lz;
      }

      if(z[i]>Lz){
         z[i] = y[i] - Lz;
      }     

      if(z[i]<=0){
         z[i] = z[i] + Lz;
      }
   }
}

void updatePosition() {

   for(int i = 0;i<N;i++) {
      x[i] = x[i] + vx[i];  
      y[i] = y[i] + vy[i];  
      z[i] = z[i] + vz[i];  
   }

   ppp();
}

void updateNc(){
   for(int i=0;i<cells;i++) {
      Nc[i] = 0;
   }

   for(int i = 0;i<N;i++) {
      Nc[cellNum[i]]++;
   }
}

void updateAvgVelocity() {

   for(int i = 0;i<cells;i++) {
      avgVx[i] = 0;
      avgVy[i] = 0;
      avgVz[i] = 0;
   }

   for(int i =0;i<N;i++) {
      avgVx[cellNum[i]] += vx[i];
      avgVy[cellNum[i]] += vy[i];
      avgVz[cellNum[i]] += vz[i];
   }

   for(int i = 0;i<cells;i++){
      if(Nc[i] != 0){
         avgVx[i] = avgVx[i]/Nc[i];
         avgVy[i] = avgVy[i]/Nc[i];
         avgVz[i] = avgVz[i]/Nc[i];     
      }
      
   }
}

void rotate() {
   unsigned seed = system_clock::now().time_since_epoch().count();
   default_random_engine generator(seed);   
   uniform_real_distribution<double> R1(0,1);
   double r1, r2;
   double rx, ry, rz, phi, rho;
   double theta, vmag, rmag;
   double vpllx, vplly, vpllz, costheta, sintheta;
   double vperpx, vperpy, vperpz;
   double rxsign, rysign, rzsign;

   r1 = R1(generator);
   r2 = R1(generator);

   phi = 2*M_PI*r1;
   rho = 2*r2-1;

   rx = (sqrt(1-rho*rho))*(cos(phi));
   ry = (sqrt(1-rho*rho))*(sin(phi));
   rz = rho;

   rx = 1/(sqrt(2));
   ry = -1/(sqrt(2));
   rz = 0;

   rmag = sqrt(rx*rx+ry*ry+rz*rz);
   vmag = sqrt(vx[0]*vx[0]+vy[0]*vy[0]+vz[0]*vz[0]);

   theta = acos((vx[0]*rx+vy[0]*ry+vz[0]*rz)/(vmag*rmag));
   costheta = cos(theta);
   sintheta = sin(theta);

   vpllx = vmag*costheta*rx/rmag;
   vplly = vmag*costheta*ry/rmag;
   vpllz = vmag*costheta*rz/rmag;

   cout<<"Theta = "<<theta<<endl;
   cout<<"x = "<<vpllx<<endl;
   cout<<"y = "<<vplly<<endl;
   cout<<"z = "<<vpllz<<endl;



}

int main() {

   init();
   
   x[0] = 0.9;
   y[0] = 0.9;
   z[0] = 0.9;
   vx[0] = -1;
   vy[0] = 0;
   vz[0] = 0;
   rotate();
   exit(0);
   // x[1] = .8;
   // y[1] = .8;
   // z[1] = .8;
   // vx[1] = 1.3;
   // vy[1] = 0;
   // vz[1] = 0;
   

   fstream fluid, x1;   
   fluid.open("fluid.ovito", ios::out);
   
   assignGrids();
   updateNc();
   updateAvgVelocity();
   printArray(avgVx, 10);

   cout<<"------------------------------------------------------"<<endl;
   
   updatePosition();
   assignGrids();
   updateNc();
   updateAvgVelocity();
   printArray(avgVx, 10);

   exit(0);

   for(int t = 0; t<50;t++) {
      
      updateNc();
      updatePosition();
      assignGrids();
      updateAvgVelocity();
      printArray(avgVx, 10);
      cout<<"------------------------------------------------------"<<endl;
      fluid<<"ITEM: TIMESTEP \n"<<t<<endl;
      fluid<<"ITEM: NUMBER OF ATOMS\n"<<N<<endl;
      fluid<<"ITEM: BOX BOUNDS pp pp pp"<<endl; 
      fluid<<0<<"\t"<<Lx<<endl;
      fluid<<0<<"\t"<<Ly<<endl;
      fluid<<0<<"\t"<<Lz<<endl;
      fluid<<"ITEM: ATOMS id x y z vx vy vz"<<endl;
      
      for(int i=0;i<N;i++) {
         fluid<<setprecision(6)<<i<<" "<<x[i]<<" "<<y[i]<<" "<<z[i]<<" "<<vx[i]<<" "<<vy[i]<<" "<<vz[i]<<" "<<endl; 
      }
      exit(0);
   }

   fluid.close();
   return 0;
}
