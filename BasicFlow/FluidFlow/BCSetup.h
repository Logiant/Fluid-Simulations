#pragma once
#include "FlowField.h"


void FlowOverBlock(FlowField* flow, int width, int height);


void IrrotationalVortex(FlowField* flow, int width, int height);


void AngledPlate(FlowField* flow, int width, int height);


void NACA0012(FlowField* flow, int width, int height);
