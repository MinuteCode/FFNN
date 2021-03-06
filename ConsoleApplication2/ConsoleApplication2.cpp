// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <random>
#include "Neuron.h"
#include "ConsoleApplication2.h"

static double learningRate = 0.5;

int main()
{
	char exit;
	std::cout << "Hello World ! This is a FFNN program" << std::endl;
	srand(time(NULL));
		
	std::vector<Neuron> inputLayer = std::vector<Neuron>();
	std::vector<Neuron> hiddenLayer = std::vector<Neuron>();
	std::vector<Neuron> hiddenLayer2 = std::vector<Neuron>();
	std::vector<Neuron> outputLayer = std::vector<Neuron>();

	for (int i = 0; i < 4; i++) {
		inputLayer.push_back(Neuron(INPUT));
	}

	for (int i = 0; i < 10; i++) {
		hiddenLayer.push_back(Neuron(HIDDEN, inputLayer.size(), inputLayer.size()));
	}

	for (int i = 0; i < 10; i++) {
		hiddenLayer2.push_back(Neuron(HIDDEN, hiddenLayer.size(), hiddenLayer.size()));
	}

	for (int i = 0; i < 1; i++) {
		Neuron n = Neuron(OUTPUT, hiddenLayer2.size(), hiddenLayer2.size());
		int exponent = inputLayer.size() - 1;
		n.outputScale = 10;
		outputLayer.push_back(n);
	}
	
	std::cout << "Hidden layer weights init" << std::endl;
	for (int i = 0; i < hiddenLayer.size(); i++) {
		hiddenLayer[i].initializeWeights();
	}

	for (int i = 0; i < hiddenLayer2.size(); i++) {
		hiddenLayer2[i].initializeWeights();
	}

	std::cout << "Output layer weights init" << std::endl;
	for (int i = 0; i < outputLayer.size(); i++) {
		outputLayer[i].initializeWeights();
	}

	std::cin >> exit;

	int i = 0;
	double totalError = 0;
	double meanError = 1000;
	double expectedResult = 0;
	while(meanError > std::pow(10, -4)) {
		int bitOne = std::rand() % 2;
		int bitTwo = std::rand() % 2;
		int bitThree = std::rand() % 2;
		int bitFour = std::rand() % 2;
		std::vector<int> entry = { bitOne, bitTwo, bitThree, bitFour };
		double expectedResultDecimal = double(entry[0] * std::pow(2,2)) + double(entry[1] * std::pow(2,1)) + double(entry[2] * std::pow(2,0));
		expectedResult = (double(entry[0] * std::pow(2,2)) + double(entry[1] * std::pow(2,1)) + double(entry[2] * std::pow(2,0))) / 10;

		feedNetwork2(entry, inputLayer, hiddenLayer, hiddenLayer2, outputLayer);

		totalError = totalError + 0.5 * std::pow((expectedResult - outputLayer[0].outputValue), 2);
		meanError = double((1 / (double(i) + 1))) * totalError;
		for (int i = 0; i < outputLayer.size(); i++) {
			double dETotaldOut = -(expectedResult - outputLayer[i].outputValue);
			double dOutdNet = outputLayer[i].outputValue * (1 - outputLayer[i].outputValue);
			
			for (int j = 0; j < hiddenLayer2.size(); j++) {
				double dNetdW = hiddenLayer2[j].outputValue;
				double dETotaldw = dETotaldOut * dOutdNet * dNetdW;
				outputLayer[i].weights[j] = outputLayer[i].weights[j] - learningRate * dETotaldw;
			}
		}
		
		
		for (int i = 0; i < outputLayer.size(); i++) {
			double dETotaldOutHj = 0;
			double dEOutidNetOuti = -(expectedResult - outputLayer[i].outputValue) * (outputLayer[i].outputValue * (1 - outputLayer[i].outputValue));
			for (int j = 0; j < hiddenLayer2.size(); j++) {
				double dNetOutidOutHiddenj = outputLayer[i].weights[j];
				double dEOutidOutHj = dEOutidNetOuti * dNetOutidOutHiddenj;
				dETotaldOutHj += dEOutidOutHj;

				
				double dOutHjdNetHj = hiddenLayer2[j].outputValue * (1 - hiddenLayer2[j].outputValue);
				for (int k = 0; k < hiddenLayer.size(); k++) {
					double dNetHjdWk = hiddenLayer[k].outputValue;
					double dETotaldWk = dETotaldOutHj * dOutHjdNetHj * dNetHjdWk;
					hiddenLayer2[j].weights[k] = hiddenLayer2[j].weights[k] - learningRate * dETotaldWk;
				}
			}

			for (int j = 0; j < hiddenLayer.size(); j++) {
				double dNetOutidOutHiddenj = hiddenLayer2[i].weights[j];
				double dEOutidOutHj = dEOutidNetOuti * dNetOutidOutHiddenj;
				dETotaldOutHj += dEOutidOutHj;


				double dOutHjdNetHj = hiddenLayer[j].outputValue * (1 - hiddenLayer[j].outputValue);
				for (int k = 0; k < inputLayer.size(); k++) {
					double dNetHjdWk = inputLayer[k].outputValue;
					double dETotaldWk = dETotaldOutHj * dOutHjdNetHj * dNetHjdWk;
					hiddenLayer[j].weights[k] = hiddenLayer[j].weights[k] - learningRate * dETotaldWk;
				}
			}
		}
		if (i % 100000 == 0) {
			std::system("cls");
			std::cout << "/////////// ITERATION #" << i << "///////////" << std::endl;
			std::cout << "Number tried : " << expectedResultDecimal <<  std::endl;
			std::cout << "NN output : " << outputLayer[0].outputValue * outputLayer[0].outputScale << std::endl;
			std::cout << "Expected result : " << expectedResult << std::endl;
			std::cout << "Mean net error : " << meanError << std::endl;
			std::cout << std::endl;
		}
		i++;
	}

	std::cout << "/////////// ITERATION #" << i << "///////////" << std::endl;
	std::cout << "output values : " << std::endl;
	for (int i = 0; i < outputLayer.size(); i++) {
		std::cout << "\toutput value " << i << " : " << outputLayer[i].outputValue << std::endl;
	}
	std::cout << "Expected result : " << expectedResult << std::endl;
	std::cout << "Mean net error : " << meanError << std::endl;

	while (exit != 'x') {
		std::cout << "Enter bit value (4 bits max) : " << std::endl;
		int biggestBit, littlestBit, mediumBit;
		std::cin >> biggestBit;
		std::cin >> mediumBit;
		std::cin >> littlestBit;

		std::cout << "Number entered : " << double(biggestBit * std::pow(2,2)) + double(mediumBit * std::pow(2,1)) + double(littlestBit * std::pow(2,0)) << std::endl;

		std::vector<int> guess = { biggestBit, mediumBit, littlestBit };

		std::cout << feedNetwork2(guess, inputLayer, hiddenLayer, hiddenLayer2, outputLayer) << std::endl;

		std::cin >> exit;
	}
    return 0;
}

double feedNetwork(std::vector<int> &entry, std::vector<Neuron> &inputLayer, std::vector<Neuron> &hiddenLayer, std::vector<Neuron> &outputLayer)
{
	for (int i = 0; i < entry.size(); i++) {
		inputLayer[i].inputValues[0] = entry[i];
		inputLayer[i].computeOutput();
	}

	for (int i = 0; i < hiddenLayer.size(); i++) {
		for (int j = 0; j < inputLayer.size(); j++) {
			hiddenLayer[i].inputValues[j] = inputLayer[j].outputValue;
		}
		hiddenLayer[i].computeOutput();
	}

	for (int i = 0; i < outputLayer.size(); i++) {
		for (int j = 0; j < hiddenLayer.size(); j++) {
			outputLayer[i].inputValues[j] = hiddenLayer[j].outputValue;
		}
		outputLayer[i].computeOutput();
	}

	return outputLayer[0].outputValue * outputLayer[0].outputScale;
}

double feedNetwork2(std::vector<int> &entry, std::vector<Neuron> &inputLayer, std::vector<Neuron> &hiddenLayer, std::vector<Neuron> &hiddenLayer2, std::vector<Neuron> &outputLayer)
{
	for (int i = 0; i < entry.size(); i++) {
		inputLayer[i].inputValues[0] = entry[i];
		inputLayer[i].computeOutput();
	}

	for (int i = 0; i < hiddenLayer.size(); i++) {
		for (int j = 0; j < inputLayer.size(); j++) {
			hiddenLayer[i].inputValues[j] = inputLayer[j].outputValue;
		}
		hiddenLayer[i].computeOutput();
	}

	for (int i = 0; i < hiddenLayer2.size(); i++) {
		for (int j = 0; j < hiddenLayer.size(); j++) {
			hiddenLayer2[i].inputValues[j] = hiddenLayer[j].outputValue;
		}
		hiddenLayer2[i].computeOutput();
	}

	for (int i = 0; i < outputLayer.size(); i++) {
		for (int j = 0; j < hiddenLayer2.size(); j++) {
			outputLayer[i].inputValues[j] = hiddenLayer2[j].outputValue;
		}
		outputLayer[i].computeOutput();
	}

	return outputLayer[0].outputValue * outputLayer[0].outputScale;
}

