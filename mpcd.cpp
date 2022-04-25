#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>

#define N 1
#define grid 5                   // Grid lines in one direction like along x direction.
#define L 20                     // Length of the big box.
#define l L/grid                 // Length of the grid box.
#define grids grid*grid*grid     // Total number of grids.

using namespace std;
using namespace chrono;

double x[N], y[N], z[N];
double vx[N], vy[N], vz[N];

//Particle grid number
int pgridNum[N];

//Grid boundaries
//first elements give x boundaries, 2nd two y boundaries, and 3rd z boundaries
double gridBounds[grids][6];

int gridNum[N][3];

void init(){

   unsigned seed = system_clock::now().time_since_epoch().count();
   default_random_engine generator(seed);
   uniform_real_distribution<double> distribution(0,L);
   uniform_real_distribution<double> velocityDistribution(0,1);

   for (int i = 0; i < N; ++i) {
      x[i] = distribution(generator);
      y[i] = distribution(generator);
      z[i] = distribution(generator);
      
      vx[i] = velocityDistribution(generator);
      vy[i] = velocityDistribution(generator);
      vz[i] = velocityDistribution(generator);
   }

}

void assignGrids(){
   //Three integers a, b and c to tell in which grid the particle is.
   int a,b,c;
   int length = l;
   for(int i = 0;i<N;i++) {
      a = int(x[i]/length);
      b = int(y[i]/length);
      c = int(z[i]/length);
      gridNum[i][0] = a;
      gridNum[i][1] = b;
      gridNum[i][2] = c;
   }
}

/*Periodic boundary condition*/
void ppp() {
   for(int i =0;i<N;i++) {
      if(x[i]>L){
         x[i] = x[i] - L;
      }     

      if(x[i]<=0){
         x[i] = x[i] + L;
      }

      if(y[i]>L){
         y[i] = y[i] - L;
      }     

      if(y[i]<=0){
         y[i] = y[i] + L;
      }

      if(z[i]>L){
         z[i] = y[i] - L;
      }     

      if(z[i]<=0){
         z[i] = z[i] + L;
      }
   }
}

void updatePosition() {
   double x_old[N], y_old[N], z_old[N];

   for(int i =0;i<N;i++) {
      x_old[i] = x[i];
      y_old[i] = y[i];
      z_old[i] = z[i];
   } 

   for(int i = 0;i<N;i++) {
      y[i] = y[i] + 1.3;
      // x[i] = x_old[i] + vx[i]; 
      // y[i] = y_old[i] + vy[i]; 
      // z[i] = z_old[i] + vz[i]; 
      // x_old[i] = x[i];
      // y_old[i] = y[i];
      // z_old[i] = z[i];
   }

   ppp();
}

void printGrid() {
   for(int i = 0;i<N;i++) {
      cout<<"a = "<<gridNum[i][0]<<" b = "<<gridNum[i][1]<<" c = "<<gridNum[i][2]<<endl;
   }
}

void printArray(double arr[]){
      for(int i =0;i<N;i++){
         cout<<arr[i]<<endl;
      }
}

int main() {

   init();
   x[0] = 1;
   y[0] = 1;
   z[0] = 1;

   fstream fluid, x1;   
   fluid.open("fluid.ovito", ios::out);

   for(int t = 0; t<10;t++) {
      
      updatePosition();
      assignGrids();
      cout<<y[0]<<endl;
      printGrid();
      cout<<"------------------------------------------------------"<<endl;
      fluid<<"ITEM: TIMESTEP \n"<<t<<endl;
      fluid<<"ITEM: NUMBER OF ATOMS\n"<<N<<endl;
      fluid<<"ITEM: BOX BOUNDS pp pp pp"<<endl; 
      fluid<<0<<"\t"<<L<<endl;
      fluid<<0<<"\t"<<L<<endl;
      fluid<<0<<"\t"<<L<<endl;
      fluid<<"ITEM: ATOMS id x y z vx vy vz"<<endl;
      
      for(int i=0;i<N;i++) {
         fluid<<setprecision(6)<<i<<" "<<x[i]<<" "<<y[i]<<" "<<z[i]<<" "<<vx[i]<<" "<<vy[i]<<" "<<vz[i]<<" "<<endl; 
      }
   }

   fluid.close();
   return 0;
}
