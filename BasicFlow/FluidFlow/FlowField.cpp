#include "FlowField.h"

#include "iostream"


FlowField::FlowField(int width_, int height_) {
	width = width_; height = height_;
	field = new Node[width*height];

	for (int i = 0; i < width*height; i++) {
		field[i].value = -1;
		field[i].boundary = false;
		if (i > width && i < (width*height - 1) - width //not the top or bottom row
			&& i % width != 0 && (i+1) % width != 0) { //not the left or right side
			field[i].leftNeighbor = &field[i - 1];
			field[i].rightNeighbor = &field[i + 1];
			field[i].topNeighbor = &field[i - width];
			field[i].bottomNeighbor = &field[i + width];
		}
	}

}


FlowField::~FlowField() {
	delete[] field;
}

void FlowField::addBC(int x0, int y0, int bc_width, int bc_height, BC_SETUP bs) {
	for (int xi = x0; xi < x0 + bc_width; xi++) {
		for (int yi = y0; yi < y0 + bc_height; yi++) {

			double phiNew = (bs.phi_min + bs.phi_max) / 2;

			switch (bs.bc) {
			case BoundaryCondition::HORIZONTAL_FLOW:
				if (bc_height > 1) {
					phiNew = bs.phi_min + ((bs.phi_max - bs.phi_min) * (((double)((bc_height - yi - 1) - y0)) / (bc_height - 1)));
				}
				field[i2o(xi, yi)].value = phiNew;
				field[i2o(xi, yi)].boundary= true;
				break;
			case BoundaryCondition::OBSTACLE:
			case BoundaryCondition::POINT:
				field[i2o(xi, yi)].value = phiNew;
				field[i2o(xi, yi)].boundary = true;
				break;
			}
		}
	}
}


Node* FlowField::solve(double thresh) {

	double* error = new double[width*height];
	double* old = new double[width*height];
	memcpy(error, field, width*height);
	memcpy(old, field, width*height);

	for (int i = 0; i < width*height; i++) {
		error[i] = 0;
	}

	double err = 100.0;

	int numIter = 0;

	while (err > thresh) {

		for (int i = 1; i < width - 1; i++) {
			for (int j = 1; j < height - 1; j++) {
				//check if this value is a BC (don't update it)
				Node* thisNode = &field[i2o(i, j)];

				if (!(thisNode->boundary)) { //if this isn't a specified boundary condition
					thisNode->value = 0.25*(thisNode->bottomNeighbor->value + thisNode->topNeighbor->value + thisNode->leftNeighbor->value + thisNode->rightNeighbor->value);
				}

				error[i2o(i, j)] = thisNode->value - old[i2o(i, j)];
				old[i2o(i, j)] = thisNode->value;
			}
		} //End current iteration


		err = findMax(error, width*height);

		numIter++;
	}

	std::cout << numIter << " iterations\n";

	delete[] error;

	return field;
}

int FlowField::i2o(int x, int y) {
	return x + y*width;
}

double FlowField::findMax(double* array, int size) {
	double maxVal = 0;
	for (int i = 0; i < size; i++) {
		if (array[i] > maxVal) {
			maxVal = array[i];
		}
	}
	return maxVal;
}