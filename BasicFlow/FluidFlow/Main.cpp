#include "iostream"
#include "fstream"
#include "FlowField.h"
#include "BCSetup.h"
#include <time.h>

//Boundary Condition function forward declarations
void FlowOverBlock(FlowField* flow, int width, int height);
void IrrotationalVortex(FlowField* flow, int width, int height);

int main(int argc, int* argv) {
	//set initial variables for flowfield and output information
	time_t start = time(0);
	int width = 100; int height = 100;

	std::cout << "beginning with a " << width << "x" << height << " grid\n";

	FlowField flow(width, height);

	//apply BC function
	//FlowOverBlock(&flow, width, height);
	IrrotationalVortex(&flow, width, height);

	std::cout << "BC Setup at " << difftime(time(0), start) << " seconds\n";

	//solve the flowfield and print time info
	double threshold = 0.001;

	Node* ff = flow.solve(threshold);

	std::cout << "Solved at " << difftime(time(0), start) << " seconds\n";

	//remove existing file and write results to output.txt
	std::remove("output.txt");
	std::ofstream myfile;
	myfile.open("output.txt");

	for (int i = 0; i < width*height; i++) {
		if (i%width == 0 && i != 0) {
			myfile << std::endl;
		}
		myfile << ("%.4f ", ff[i].value) << ",";
	}
	//clean up our resources!
	myfile.close();


	//end of program
	std::cout << "\nEnter a char to exit.\n";
	int a;
	std::cin >> a;


}