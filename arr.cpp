#include <iostream>
#include <ctime>

using namespace std;

// function to generate and retrun random numbers.
int * getRandom() {

   static int  r[n];

   // set the seed
   srand( 111 );
   
   for (int i = 0; i < 10; ++i) {
      r[i] = rand();
      cout << r[i] << endl;
   }

   return r;
}

// main function to call above defined function.
int main () {

}