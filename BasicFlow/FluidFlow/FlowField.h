#pragma once
struct BC_SETUP;

class FlowField
{
public:

	enum BoundaryCondition {
		HORIZONTAL_FLOW, OBSTACLE
	};


	FlowField(int width, int height);
	~FlowField();

	void addBC(int x, int y, int width, int height, BC_SETUP bc);

	double* solve(double thresh);

	double findMax(double* arr, int numVals);

	int i2o(int x, int y);


private:
	int width, height;
	double phiMax, phiMin;

	double* field;
	bool* fieldFlag;

};

struct BC_SETUP {
	FlowField::BoundaryCondition bc;
	double phi_min;
	double phi_max;
};

