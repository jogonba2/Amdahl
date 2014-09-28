#include <stdlib.h>
#include <stdio.h>
#define NUM_ITEMS_VECTOR 35

// Se ve bien lo que escribo?
int original_fibo(int n){
	return n<2 ? n : original_fibo(n-2)+original_fibo(n-1);
}

int improved_fibo(int n){
	int *fib = (int*)malloc(n*sizeof(int));
	fib[0] = 1; fib[1] = 1; int i = 2;
	for(;i<n;fib[i++] = fib[i-1] + fib[i-2]);
	i = fib[n-1];
	free(fib);
	return i;
}

// Gracias a que la secuencia está ordenada podemos hacer búsqueda binaria, luego el proceso global sería calcular fibo + busqueda binaria //
// Como se puede ver, solo hace falta mantener las dos versiones (original y mejorada) de los procesos que queremos, solo se da soporte a un cambio //
char binary_search(int vector[NUM_ITEMS_VECTOR],int n){
	int centro,inf=0,sup=NUM_ITEMS_VECTOR;
	while(inf<=sup){
		centro=(sup+inf)/2;
		if(vector[centro]==n) return 1;
		else if(n < vector[centro]) sup=centro-1;
        else inf=centro+1;
    }
    return -1;
}

void original_process(){
	int i = 0;
	int vector[NUM_ITEMS_VECTOR];
	for(;i<NUM_ITEMS_VECTOR;i++) vector[i] = original_fibo(i);
	binary_search(vector,5702887);
}

void improved_process(){
	int i = 0;
	int vector[NUM_ITEMS_VECTOR];
	for(;i<NUM_ITEMS_VECTOR;i++) vector[i] = improved_fibo(i);
	binary_search(vector,5702887);
}