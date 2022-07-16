#include "../src/GCAdapter.h"

int main( int argc, char *argv[] ){

    ControllerStatus adapter_buffer[4];

    if(!gca::Setup()){
        std::cout << "Failed to load gca" << std::endl;
        return -1;
    }
    
	int i;
	for(i = 0; i < 200; i++){
	    gca::Process(adapter_buffer);
	    std::cout << adapter_buffer[0] << std::endl;
	}
	gca::Stop();
	
    return 0;
}
