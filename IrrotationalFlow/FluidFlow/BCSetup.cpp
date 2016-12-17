#include "BCSetup.h"
#include <math.h>
#include <iostream>

void FlowOverBlock(FlowField* flow, int width, int height) {
	BC_SETUP uniformIO;

	uniformIO.bc = FlowField::BoundaryCondition::HORIZONTAL_FLOW;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = height / 2;
	//entire left side
	int x0 = 0; int y0 = 0; int bcw = 1; int bch = height;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
	//top of right side
	x0 = width - 1; y0 = 0; bch = height - height / 2;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
	//Top Side
	x0 = 0; y0 = 0; bcw = width; bch = 1;
	uniformIO.phi_min = height / 2;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
	//Bottom Side
	y0 = 0; bcw = width; bch = 1;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = 0;
	y0 = height - 1;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
	//block
	x0 = width - width / 2;
	y0 = (int)(height - height / 2 - 0.5);
	bcw = width / 2;
	bch = (height / 2 + 1);
	uniformIO.bc = FlowField::BoundaryCondition::OBSTACLE;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
}


void AddUniformBC(FlowField* flow, int width, int height) {
	BC_SETUP uniformIO;

	uniformIO.bc = FlowField::BoundaryCondition::HORIZONTAL_FLOW;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = height / 2;
	//left side
	int x0 = 0; int y0 = 0; int bcw = 1; int bch = height;
	flow->addBC(x0, y0, bcw, bch, uniformIO);

	//right side
	x0 = width - 1;
	flow->addBC(x0, y0, bcw, bch, uniformIO);

	//Top Side
	x0 = 1; y0 = 0; bcw = width - 2; bch = 1;
	uniformIO.phi_min = height / 2;
	flow->addBC(x0, y0, bcw, bch, uniformIO);

	//Bottom Side
	y0 = height - 1; bcw = width - 2; bch = 1;
	uniformIO.phi_min = 0;
	uniformIO.phi_max = 0;
	flow->addBC(x0, y0, bcw, bch, uniformIO);
}

void AngledPlate(FlowField* flow, int width, int height) {

	double plate_phi = 13.5;


	AddUniformBC(flow, width, height);

	BC_SETUP point;
	//flat plate!
	point.bc = FlowField::BoundaryCondition::POINT;
	point.phi_min = plate_phi;
	point.phi_max = plate_phi;

	int dx = 20; //x component of plate

	for (int xi = 0; xi < dx; xi++) {
		int x = (width / 2) - (dx / 2) + xi;
		int y = (height / 2) - (dx / 2) + xi;
		flow->addBC(x, y, 1, 1, point);

	}
}




double nacaThickness(double t, double c, double x) {
	double y = 5*t*c*(
		(0.2969*sqrt(x/c)) + 
		(-0.1260*(x/c)) + 
		(-0.3516*(x/c)*(x/c)) + 
		(0.2843*(x/c)*(x/c)*(x/c)) + 
		(-0.1015*(x/c)*(x/c)*(x/c)*(x/c)) ); //wikipedia
	return y;
}


void NACA0012(FlowField* flow, int width, int height) {
	AddUniformBC(flow, width, height);


	double nacaPhi = height/4.0;
	int chord = 300; //airfoil length
	int x0 = (width / 2) - (chord / 2); //start on the left side
	int y0 = height / 2; //place airfoil in the center
	double t = 0.12; //NACA 0012

	BC_SETUP point;
	point.bc = FlowField::BoundaryCondition::POINT;
	point.phi_min = nacaPhi;
	point.phi_max = nacaPhi;
	for (int x = 0; x < chord; x++) {
		int dy = (int)(nacaThickness(t, chord, x) + 0.5);
		for (int y = 0; y <= dy; y++) {
			flow->addBC(x + x0, y0 + y, 1, 1, point);
			flow->addBC(x + x0, y0 - y, 1, 1, point);
		}
	}




}





double vortexStreamPhi(double x, double y, double K, double U) {
	return (U*y) + (K*log(sqrt((x*x) + (y*y))));
}
void IrrotationalVortex(FlowField* flow, int width, int height) {
	float K = 17; float U = 1;
	float dx = 1; float dy = 1;

	BC_SETUP vortex;

	//center points
	vortex.bc = FlowField::BoundaryCondition::POINT;
	//horizontal center around the origin
	int x0 = 0;  int y0 = height / 2 - 1; int bcw = 1; int bch = 1; //single point
	for (int i = width / 2 - 1; i < width / 2 + 2; i++) { //iterating X along the centerline
		double phi = vortexStreamPhi((-width / 2 + i) * dx, 0, K, U);
		vortex.phi_min = phi;
		vortex.phi_max = phi;
		x0 = i;
		flow->addBC(x0, y0, bcw, bch, vortex);
	}

	//vertical axis centerline
	x0 = width / 2; bcw = 1; bch = 1;
	for (int i = 0; i < height - 1; i++) { //iterating y
		double phi = vortexStreamPhi(0, (height / 2 - i - 1) * dy, K, U);
		vortex.phi_min = phi;
		vortex.phi_max = phi;
		y0 = i; //single point
		flow->addBC(x0, y0, bcw, bch, vortex);
	}

	//top side - not quite uniform
	vortex.bc = FlowField::BoundaryCondition::POINT; 
	for (int i = 0; i < width; i++) { //iterating X
		double phi_top = vortexStreamPhi((-width / 2 + i) * dx, (height / 2 - 1) * dy, K, U);
		vortex.phi_min = phi_top;
		vortex.phi_max = phi_top;
		x0 = i; y0 = 0; bcw = 1; bch = 1; //single point

		flow->addBC(x0, y0, bcw, bch, vortex);
	}

	//bottom side - not quite uniform
	for (int i = 0; i < width; i++) { //iterating X
		double phi_bot = vortexStreamPhi((-width / 2 + i) * dx, (-height / 2) * dy, K, U);
		vortex.phi_min = phi_bot;
		vortex.phi_max = phi_bot;
		x0 = i; y0 = height - 1; bcw = 1; bch = 1; //single point

		flow->addBC(x0, y0, bcw, bch, vortex);
	}

	//left side - not quite uniform
	for (int i = 0; i < height; i++) { //iterating X
		double phi_left = vortexStreamPhi((-width / 2) * dx, (height / 2 - i - 1) * dy, K, U);
		vortex.phi_min = phi_left;
		vortex.phi_max = phi_left;
		x0 = 0; y0 = i; bcw = 1; bch = 1; //single point

		flow->addBC(x0, y0, bcw, bch, vortex);
	}

	//right side - not quite uniform
	for (int i = 0; i < height; i++) { //iterating X
		double phi_right = vortexStreamPhi((width / 2 - 1) * dx, (height / 2 - i - 1) * dy, K, U);
		vortex.phi_min = phi_right;
		vortex.phi_max = phi_right;
		x0 = width - 1; y0 = i; bcw = 1; bch = 1; //single point

		flow->addBC(x0, y0, bcw, bch, vortex);
	}

}
