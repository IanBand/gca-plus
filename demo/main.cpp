#include "../src/GCAdapter.h"

int main( int argc, char *argv[] ){

    if(!gca::Setup()){
        std::cout << "Failed to load gca" << std::endl;
	}
	
	ControllerStatus* data;
	
	
	int i;
	for(i = 0; i < 200; i++){
	    data = gca::Process();
	
	    std::cout << data[0] << std::endl;
	}
	
	
	gca::Stop();
	
	
    return 0;
}
