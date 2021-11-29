#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

double funkcja(double x)
    {
    return pow(x, 2);
    }

    double wzr(double a, double b, double n){

        double xi = 0;
        for(double i = 1; i < n; i++){
            xi += (funkcja(a + ((i / n) * (b - a))));
        }
        return xi;
    }

    double PI(float n) {
    double pi=1.0;
    int i;
    int N;
    for (i=3, N=2*n+1; i<=N; i+=2)
        pi += ((i&2) ? -1.0 : 1.0) / i;
    return 4*pi;

}


int main(){

    int nr;
    int proces;
    double leibniz;
    int wartosc = 400;
    MPI_Status status;
    double a = 1;
    double b = 2;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proces);
    MPI_Comm_rank(MPI_COMM_WORLD, &nr);


    if(nr == 0)
    {
       leibniz = PI(proces);
        printf(" wartosc: %f \n",nr, leibniz);
        MPI_Send(&leibniz,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD);
    }else if(nr+1 == proces) {
        MPI_Recv(&leibniz,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD,&status);
        printf(" wartosc %f \n",nr+1, leibniz);
    }else{
        MPI_Recv(&leibniz,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD,&status);
        printf(" wartosc: %f \n",nr, leibniz);
        MPI_Send(&leibniz,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD);
    }




     double suma = 0;
     double prost = ( b - a) / proces;
        if(nr == proces - 1 ){

        suma += funkcja(a + nr * prost );


        MPI_Send(&suma,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD);

        }
        else if(nr != 0 && nr < proces - 1)  {
            MPI_Recv(&suma,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD,&status);
            suma += funkcja(a + nr * prost );


            MPI_Send(&suma,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD);
        }else{
            MPI_Recv(&suma,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD,&status);
            suma *= prost;
            printf("suma prostokatow: %f\n", suma);


        }






        double xi = 0;

        if(nr == proces - 1 ){

            xi += (funkcja(a + (((double) nr / (double) proces) * (b - a))));
        MPI_Send(&xi,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD);

        }
        else if(nr != 0 && nr < proces - 1)  {
            MPI_Recv(&xi,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD,&status);
            xi += (funkcja(a + (((double) nr / (double) proces) * (b - a))));
            MPI_Send(&xi,1,MPI_DOUBLE,nr-1,wartosc,MPI_COMM_WORLD);
        }else{
            MPI_Recv(&xi,1,MPI_DOUBLE,nr+1,wartosc,MPI_COMM_WORLD,&status);
            suma = ((b-a)/n) * (funkcja(a) / 2 + xi + funkcja(b)/2);
            printf(" suma trapezów:  %f \n", nr, suma);
        }







    MPI_Finalize();
    return 0;
}
