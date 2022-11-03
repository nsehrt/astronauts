#define OLC_PGE_APPLICATION
#include "astronauts.hpp"


int main()
{
	const olc::vi2d dims{800,640};
	if (AstronautApp astronautApp{dims}; 
		astronautApp.Construct(dims.x, dims.y, 1, 1))
		astronautApp.Start();

	return 0;
}