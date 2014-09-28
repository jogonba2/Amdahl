/** Amdahl's law -> http://es.wikipedia.org/wiki/Ley_de_Amdahl **/
/** Includes **/
#include <stdio.h>
#include <time.h>
#include <math.h>

/** <<USER INPUT>> Include .c with your program **/
#include "tobeimproved.c"

/** <<USER INPUT>> Define your options and params to test **/
/** <<Specify params>> **/
// Example: #define V1 {1,2,3,4,5}
//          #define V2 {5,4,3,2,1}
//          #define N   5
#define N 35      
          
/** <<USER INPUT>> <<Specify functions>> **/     
// Example: #define ORIGINAL_FUNCTION my_function_1(V1,N); 
//          #define IMPROVED_FUNCTION my_other_function(V1,N,V2);
#define ORIGINAL_FUNCTION original_fibo(N);   // <<insert your original function>> (supposed slower) //
#define IMPROVED_FUNCTION improved_fibo(N);   // <<insert your new function>>      (supposed faster) //
#define ORIGINAL_PROCESS  original_process(); // <<insert your global process>>    (supposed slower) //
#define IMPROVED_PROCESS  improved_process(); // <<insert your new process>>       (supposed faster) //

/** <<USER INPUT>> General test options **/
#define ITERATIONS 10                         // Iterations to do calculating average time //
#define PARALLELISM 0                         // Activate parallelism with omp (it could pollute tests) //

/** Functions **/

// i to switch #defines, app is forced to work only with this defines //
// Supposed that this multiplex has time cost upper limit O(1) //
double check_time_functions(int opFunction){
	clock_t ti,te,tt;
	tt    = 0;
	int i = 0;
	#pragma omp parallel for if(PARALLELISM==1)
	for(i=0;i<ITERATIONS;i++){
		ti = clock();
		/** If te=clock(); tt += te-ti is at the end, we add an overload checking function **/
		if(opFunction==0){ 
			ORIGINAL_FUNCTION;
			#pragma omp critical
			{te = clock(); tt += (te-ti);}
			continue; 
		}
		if(opFunction==1){ 
			IMPROVED_FUNCTION;
			#pragma omp critical
			{te = clock(); tt += (te-ti);}
			continue; 
		}
		if(opFunction==2){ 
			ORIGINAL_PROCESS;
			#pragma omp critical
			{te = clock(); tt += (te-ti);}
			continue; 
		}
		if(opFunction==3){ 
			IMPROVED_PROCESS; 
			#pragma omp critical
			{te = clock(); tt += (te-ti);}
			continue; 
		}
	}
	return tt/ITERATIONS;
}

double get_global_improvement(double original_process_time,double improved_process_time)  { return original_process_time/improved_process_time; }
double get_local_improvement(double original_function_time,double improved_function_time) { return original_function_time/improved_function_time; }
// Fm = (1-SGlobal)/(SGlobal+(SGlobal-SLocal)) , obtained from SGlobal = 1/(1-Fm)+(Fm/SLocal)
double get_time_factor_of_use(double global_improvement,double local_improvement){ return (1-global_improvement)/(-global_improvement+(global_improvement/local_improvement)); }
double get_experimental_use_factor(double original_process_time,double improved_process_time){ return (original_process_time-improved_process_time)/(original_process_time); }

void header(){
	fprintf(stdout,"\n\n------------------------------ INIT ------------------------------\n\n\
Initialising benchmark, this may take a few minutes...\n\n\
------------------------------------------------------------------\n\n");
    if(PARALLELISM==1) fprintf(stdout,"Notice: Parallelism is activated [!]\n\n\
------------------------------------------------------------------\n\n");
}

void footer(){
	fprintf(stdout,"\n\n------------------------------ END ------------------------------\n\n");
}

void usage(){
	fprintf(stdout,"\n\n-----------------------    USAGE    -----------------------\n\n\
Benchmark your code using amdahl's law, this program is managed by using defines, you only have to include your .c application (which contains functions \
definitions), and specify functions and parameters required.\n\n");
	footer();
}
	
int main(int argc,char* argv[]){  
	if(argc>=2){ usage(); exit(0); }
	// Header //
	header();
	// Times in miliseconds (Prints)//
	fprintf(stdout,"\n\n------------------------------ TIMES ------------------------------\n\n");
	double original_function_time = check_time_functions(0)/CLOCKS_PER_SEC*1000;
	printf("Original function time: %0.2f ms\n",original_function_time);
	double improved_function_time = check_time_functions(1)/CLOCKS_PER_SEC*1000;
	printf("Improved function time: %0.2f ms\n",improved_function_time);
	double original_process_time = check_time_functions(2)/CLOCKS_PER_SEC*1000;
	printf("Original process  time: %0.2f ms\n",original_process_time);
	double improved_process_time = check_time_functions(3)/CLOCKS_PER_SEC*1000;
	printf("Improved process  time: %0.2f ms\n\n",improved_process_time);
	fprintf(stdout,"\n\n--------------------------------------------------------------------\n\n");
	
	// Check time cases //
	fprintf(stdout,"\n\n----------------------------- RESULTS ------------------------------\n\n");
	if(original_function_time==0){ fprintf(stdout,"What do you want to improve? p.p"); exit(0); }
	// Obtain and check improvements //
	double global_improvement = get_global_improvement(original_process_time,improved_process_time); 
	if(global_improvement==INFINITY) fprintf(stdout,"1) You have obtained an infinite improvement in global process, congratz :D\n");
	else							 printf("1) The new process is %0.2f times faster than the old \n",global_improvement);
	double local_improvement  = get_local_improvement(original_function_time,improved_function_time);
	if(local_improvement==INFINITY) fprintf(stdout,"2) You have obtained an infinite improvement in local function, congratz :D\n");
	else							printf("2) The new function is %0.2f times faster than the old \n",local_improvement);
	if(global_improvement==INFINITY || local_improvement==INFINITY || global_improvement==local_improvement) fprintf(stdout,"3) Factor time can not be calculated due to an \
infinite improvement or SL=SG :( \n");
	else 							printf("3) Use factor of local function is: %0.2f percent \n",get_time_factor_of_use(global_improvement,local_improvement)*100); 
	fprintf(stdout,"4) Experimental use factor is: %0.2f percent \n",get_experimental_use_factor(original_process_time,improved_process_time)*100);
	footer();
	return 0;
}

