#pragma once
struct BC_SETUP;
struct Node;



class FlowField
{
public:

	enum BoundaryCondition {
		HORIZONTAL_FLOW, OBSTACLE, POINT
	};


	FlowField(int width, int height);
	~FlowField();

	void addBC(int x, int y, int width, int height, BC_SETUP bc);

	Node* solve(double thresh);

	double findMax(double* arr, int numVals);

	int i2o(int x, int y);


private:
	int width, height;

	Node* field;

};

struct BC_SETUP {
	FlowField::BoundaryCondition bc;
	double phi_min;
	double phi_max;
};

struct Node {
	Node* topNeighbor;
	Node* bottomNeighbor;
	Node* leftNeighbor;
	Node* rightNeighbor;
	bool boundary;
	double value;
};