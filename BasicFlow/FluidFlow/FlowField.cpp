#include "FlowField.h"

#include "iostream"


FlowField::FlowField(int width_, int height_) {
	width = width_; height = height_;
	field = new double[width*height];
	fieldFlag = new bool[width*height];

	for (int i = 0; i < width*height; i++) {
		field[i] = -1;
		fieldFlag[i] = false;
	}

}


FlowField::~FlowField() {
	delete[] field;
	delete[] fieldFlag;
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
				field[i2o(xi, yi)] = phiNew;
				fieldFlag[i2o(xi, yi)] = true;
				break;
			case BoundaryCondition::OBSTACLE:
				field[i2o(xi, yi)] = phiNew;
				fieldFlag[i2o(xi, yi)] = true;
				break;
			}
		}
	}
}


double* FlowField::solve(double thresh) {
	double* error = new double[width*height];
	double* old = new double[width*height];
	memcpy(error, field, width*height);
	memcpy(old, field, width*height);

	for (int i = 0; i < width*height; i++) {
		error[i] = 0;
	}

	double err = 100.0;

	while (err > thresh) {

		for (int i = 1; i < width - 1; i++) {
			for (int j = 1; j < height - 1; j++) {
				//check if this value is a BC (uneditable)
				bool check = fieldFlag[i2o(i, j)];

				if (!check) {
					field[i2o(i, j)] = (field[i2o(i + 1, j)] + field[i2o(i - 1, j)] + field[i2o(i, j + 1)] + field[i2o(i, j - 1)]) / 4;
				}

				error[i2o(i, j)] = field[i2o(i, j)] - old[i2o(i, j)];
				old[i2o(i, j)] = field[i2o(i, j)];
			}
		} //End current iteration


		err = findMax(error, width*height);
	}

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