#include "stdafx.h"
#include "Neuron.h"

Neuron::Neuron(NEURON_TYPE type, int weightsNumber, int inputValuesNumber) {
	switch (type) {
	case INPUT:
		inputValuesNumber = 1;
		break;

	case HIDDEN:
		break;

	case OUTPUT:
		break;
	}

	for (int i = 0; i < inputValuesNumber; i++) {
		inputValues.push_back(0);
	}

	for (int i = 0; i < weightsNumber; i++) {
		weights.push_back(1);
	}
}

Neuron::Neuron()
{
}


Neuron::~Neuron()
{
}
