#include "../src/GCAdapter.h"

int main( int argc, char *argv[] ){

    ControllerStatus adapter_buffer[4];

    if(!gca::Setup()){
        std::cout << "Failed to load gca" << std::endl;
    }
    
	int i;
	for(i = 0; i < 200; i++){
	
	    // Process() assumes argument points to a buffer with size sizeof(ControllerStatus) * 4. It will write to undefined addresses if not.
	    gca::Process(adapter_buffer);
	    std::cout << adapter_buffer[0] << std::endl;
	}
	gca::Stop();
	
    return 0;
}
