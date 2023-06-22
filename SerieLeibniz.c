#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double leibniz_pi_sequential(int num_cant) {
    double pi = 0.0, start, end, cpu_time_used;
    int sign = 1, denom = 1;
	start = omp_get_wtime();

    for (int i = 0; i < num_cant; i++) {
        pi += (double)sign / denom;
        sign = -sign;
        denom += 2;
    }

    pi = pi * 4.0;
    end = omp_get_wtime();
    cpu_time_used = end - start;
    printf("Aproximacion de Pi (secuencial) : %.15f\n", pi);
    printf("Tiempo de ejecucion (secuencial): %f segundos\n\n", cpu_time_used);
}

void leibniz_pi_parallel(int num_cant, int num_threads) {
    double pi = 0.0, cpu_time_used, start, end;
    int denom = 1;
    omp_set_num_threads(num_threads);
	start = omp_get_wtime();
    #pragma omp parallel
    {
    	double partial_sum = 0.0;
        int num_threads = omp_get_num_threads(), sign, thread_id = omp_get_thread_num();
        for (int i = thread_id; i < num_cant; i += num_threads) {
            sign = i % 2 == 0 ? 1 : -1;
            partial_sum += (double)sign / (2.0*i+1.0);
        }
        #pragma omp critical
        pi += partial_sum;
    }
    pi = pi * 4.0;
    end = omp_get_wtime();
    cpu_time_used = end - start;
    printf("Aproximacion de Pi (paralela)   : %.15f\n", pi);
    printf("Tiempo de ejecucion (paralela)  : %f segundos\n", cpu_time_used);
}

int main() {
	int num_cant = 0;
	int num_threads =  0;
	char continuar = 'n';
	
	do{
		system("CLS");
		printf("Ingrese la cantidad de datos que desea:");
		scanf("%d", &num_cant);
		printf("Ingrese la cantidad de Hilos que desea ocupar (Max 100.000): ");
        scanf("%d", &num_threads);
        system("CLS");
        
		leibniz_pi_parallel(num_cant,num_threads);
		printf("--------------------------------------------------------------\n");
	    leibniz_pi_sequential(num_cant);
	    
	    printf("Continuar... (y/n): ");
	    
        scanf("%s", &continuar);
		
	}while(continuar == 'y'||continuar == 'Y');
    return 0;
}

