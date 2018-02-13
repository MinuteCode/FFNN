#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <time.h>
#include <chrono>

enum NEURON_TYPE {
	INPUT,
	HIDDEN,
	OUTPUT
};

class Neuron
{
public:
	double outputScale = 1;
	std::vector<double> inputValues;
	double outputValue = 0;
	std::vector<double> weights;

	Neuron(NEURON_TYPE type, int weightsNumber = 1, int inputValuesNumber = 1);
	Neuron();
	~Neuron();

	void computeOutput() {
		double sum = 0;
		outputValue = 0;
		for (int i = 0; i < weights.size(); i++) {
			sum += weights[i] * inputValues[i];
		}
		outputValue += activationFunction(sum);
	}

	void initializeWeights() {
		for (int i = 0; i < weights.size(); i++) {
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::uniform_real_distribution<double> distr(-0.5, 0.5);
			std::mt19937_64 rng(seed);
			weights[i] = distr(rng);
			std::cout << "Neuron weight " << i << " : " << weights[i] << std::endl;
		}
		std::cout << std::endl;
	}

private:
	double activationFunction(double value) {
		return (1 / (1 + std::exp(-value)));
	}

};

