#include <stdint.h>
#include <video.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <syscalls.h>
#include <interrupts.h>
#include <speaker.h>
#include <process.h>
// #include "./Scheduler/scheduler.h"

int borrar();
int borrar3();
int borrar2 = 300;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

char * shellArgs[] = {"shell", NULL};


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{

	load_idt(); //Setup idt before terminal runs
	save_original_regs();
	restore_stack();
	// draw_string(100,100,"EJEMPLIN", 10, GREEN, BLACK);
	// while(1);
	initScheduler(1);
	int pid = createProcess("borrar", 0, 4*4096, 4*4096, shellArgs, &borrar); // CAMBIOS
	int shPid = createProcess("shell", 0, 4*4096, 4*4096, shellArgs, &borrar3); // CAMBIOS
	includeTerminal(shPid);
	
	// ((EntryPoint)sampleCodeModuleAddress)(); //Calling sampleCodeModule's main address
	while(1){
		draw_string(100,100,"1", 2, GREEN, BLACK);
		draw_string(100,100,"0", 2, GREEN, BLACK);
	}
	return 0;
}

int borrar(){
    
    while(1){
	draw_string(300,100,"EJEMPLIN", 0x0F, GREEN, BLACK);	
	}
    return 0;
}

int borrar3(){

	while(1){
		draw_string(500,100,"HOLA", 0x0F, GREEN, BLACK);
	}
    return 0;
}