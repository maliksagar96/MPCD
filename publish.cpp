#include <iostream>
#include "mpcd.h"

using namespace std;

void printArray(double arr[], int N){
      for(int i =0;i<N;i++){
         cout<<arr[i]<<endl;
      }
}

void printArray(int arr[], int N){
      for(int i =0;i<N;i++){
         cout<<arr[i]<<endl;
      }
}

void printGrid(int cellNum[][3], int N) {
   for(int i = 0;i<N;i++) {
      cout<<"a = "<<cellNum[i][0]<<" b = "<<cellNum[i][1]<<" c = "<<cellNum[i][2]<<endl;
   }
}

/*
   To generate a file for Ovito.
*/

/******************************************************

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

******************************************************/