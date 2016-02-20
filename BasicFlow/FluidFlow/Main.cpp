#include "iostream"
#include "fstream"
#include "FlowField.h"
#include <time.h>

int main(int argc, int* argv) {

	time_t start = time(0);

	int width = 500; int height = 500;

	std::cout << "beginning with a " << width << "x" << height << " grid\n";

	FlowField flow(width, height);

	BC_SETUP uniformIO;

	uniformIO.bc = FlowField::BoundaryCondition::HORIZONTAL_FLOW;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = 10;
	//entire left side
	int x0 = 0; int y0 = 0; int bcw = 1; int bch = height;
	flow.addBC(x0, y0, bcw, bch, uniformIO);
	//top of right side
	x0 = width-1; y0 = 0; bch = height - height/2;
	flow.addBC(x0, y0, bcw, bch, uniformIO);
	//Top Side
	x0 = 0; y0 = 0; bcw = width; bch = 1;
	uniformIO.phi_min = 10;
	flow.addBC(x0, y0, bcw, bch, uniformIO);
	//Bottom Side
	y0 = 0; bcw = width; bch = 1;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = 0;
	y0 = height - 1;
	flow.addBC(x0, y0, bcw, bch, uniformIO);
	//block
	x0 = width - width/2;
	y0 = (height - height / 2 - 0.5);
	bcw = width / 2;
	bch = (height / 2 + 1);
	uniformIO.bc = FlowField::BoundaryCondition::OBSTACLE;
	flow.addBC(x0, y0, bcw, bch, uniformIO);


	double* ff = flow.solve(10.0/width/height);

	std::remove("output.txt");
	std::ofstream myfile;
	myfile.open("output.txt");

	for (int i = 0; i < width*height; i++) {

		if (i%width == 0 && i != 0) {
			myfile << std::endl;
		}

		myfile << ("%.4f ", ff[i]) << ",";

	}


	myfile.close();

	std::cout << "Completed in " << difftime(time(0), start) << " seconds.";

	std::cout << "\nEnter a char to exit.\n";


	int a;
	std::cin >> a;
}