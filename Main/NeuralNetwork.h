#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <cassert>
#include <math.h>
#include <chrono>

#include "GClasses/Common/G_Common.h"

#define ANN_Testing

using DecimalNum = double;
using WholeNum = i32;
using UnsignedNum = u32;
using ErrorState = u8;
using ReturnState = bool;


class RNGGenetartor
{
    public:
        static std::random_device Device;
        static std::mt19937::result_type Seed;
        static std::mt19937 Generator;

    public:
        static DecimalNum GetRandomNormallyDistributedNumber(const DecimalNum mean = 0, const DecimalNum standardDeviation = 1)
        {
            std::normal_distribution<> dist(mean, standardDeviation);
            return dist(Generator);
        }
};

std::random_device RNGGenetartor::Device;
std::mt19937::result_type RNGGenetartor::Seed = Device() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );;
std::mt19937 RNGGenetartor::Generator = std::mt19937(Seed);


//******************************************************************************************
class Neuron;

class NeuronLayer;

class Neuron
{
    public:
        DecimalNum OutputValue;
        DecimalNum NetValue;
        DecimalNum ErrorTerm;
        UnsignedNum WeightCount;
        UnsignedNum IndexOfThis;
        std::vector<DecimalNum> Weights;
        std::vector<DecimalNum> DeltaWeights;

    public:
        Neuron() {}
        Neuron(const UnsignedNum prevLayerNeuronCount, const UnsignedNum indexOfThis)
        {
            std::cout << "New neuron created" << std::endl;
            InitialiseCoreValues(indexOfThis);
            InitialiseWeights(prevLayerNeuronCount);
        }


        void ComputeOutput(const std::vector<Neuron> PASS_BY_REFERENCE prevLayer)
        {
            //Updates the NetValue value
            UpdateNetValue(prevLayer);

            //Sets the output value
            OutputValue = SigmoidFunction(NetValue);
        }
        void CalculateErrorTermHiddenLayer(const std::vector<Neuron> PASS_BY_REFERENCE nextLayer)
        {
            ErrorTerm = GetCumulativeErrorTerm(nextLayer) * SigmoidFunctionDerivative();
        }
        void CalculateErrorTermOutputLayer(const DecimalNum targetValue)
        {
            ErrorTerm = (targetValue - OutputValue) * SigmoidFunctionDerivative();
        }
        void UpdateWeights(const std::vector<Neuron> PASS_BY_REFERENCE prevLayer, const DecimalNum eta, const DecimalNum alpha)
        {
            //Cycles through the weights and updates each of them
            for(UnsignedNum i = WeightCount; i-- > 0;)
            {
                //Updates the weights
                UpdateWeight(Weights[i], DeltaWeights[i], prevLayer[i].OutputValue, eta, alpha);
            }

            //Updates the bias
            UpdateWeight(Weights.back(), DeltaWeights.back(), 1, eta, alpha);
        }
        DecimalNum SigmoidFunctionDerivative() const
        {
            return OutputValue * (1 - OutputValue);
        }

    public:
        DecimalNum GetSquaredError(const DecimalNum targetValue) const
        {
            DecimalNum temp = (targetValue - OutputValue);
            return temp * temp;
        }
        DecimalNum GetAverageDeltaWeight() const
        {
            DecimalNum runningSum = 0;
            for(UnsignedNum i = 0; i < DeltaWeights.size(); i++)
                runningSum += DeltaWeights[i];

            return runningSum / DeltaWeights.size();
        }

    public:
        static DecimalNum SigmoidFunction(const DecimalNum input)
        {
            return 1 / (1 + exp(-input));
        }
        static DecimalNum SigmoidFunctionDerivative(const DecimalNum input)
        {
            DecimalNum temp = SigmoidFunction(input);
            return temp * (1 - temp);
        }

    private:
        inline void InitialiseWeights(const UnsignedNum prevLayerNeuronCount)
        {
            //Sets the weight count
            WeightCount = prevLayerNeuronCount;

            //Allocates the space for the weights
            //reserves an extra spot for the bias weight
            Weights.reserve(prevLayerNeuronCount + 1);

            //Sets the delta weights to 0
            DeltaWeights.resize(prevLayerNeuronCount + 1, 0);

            //Sets the values to random values capped between 0 and 1 by the sigmoid function
            for(UnsignedNum i = prevLayerNeuronCount + 1; i-- > 0;)
            {
                Weights.push_back(SigmoidFunction(RNGGenetartor::GetRandomNormallyDistributedNumber()));
            }
        }
        inline void InitialiseCoreValues( const UnsignedNum indexOfThis)
        {
            OutputValue = 0;
            NetValue = 0;
            ErrorTerm = 0;
            IndexOfThis = indexOfThis;
        }

        inline void UpdateWeight(DecimalNum PASS_BY_REFERENCE weight, DecimalNum PASS_BY_REFERENCE deltaWeight, const DecimalNum currInput, const DecimalNum eta, const DecimalNum alpha)
        {
            //Saves last delta weight for momentum calculation
            DecimalNum lastDeltaWeight = deltaWeight;

            //Calculates new delta weight
            deltaWeight = eta * ErrorTerm * currInput

                    + //Adding of momentum
                    alpha * lastDeltaWeight;


            //Updates the weight
            weight += deltaWeight;
        }
        inline void UpdateNetValue(const std::vector<Neuron> PASS_BY_REFERENCE prevLayer)
        {
            //resets the values
            NetValue = 0.0;

            //Cycles through all neurons from the previous layer
            for(UnsignedNum i = 0; i < prevLayer.size(); i++)
            {
                //adds the input from each neuron multiplied by its appropriate weight
                NetValue += prevLayer.at(i).OutputValue * Weights.at(i);
            }

            //Adds the bias weight //(* 1) - is not needed for obvious reasons
            NetValue += Weights.back(); // * 1
        }
        inline DecimalNum GetCumulativeErrorTerm(const std::vector<Neuron> PASS_BY_REFERENCE nextLayer)
        {
            DecimalNum sum = 0;

            //Sums the error term of neurons of the next layer multiplied by the weights connecting it to this
            for(UnsignedNum i = 0; i < nextLayer.size(); i++)
            {
                sum += nextLayer[i].ErrorTerm * nextLayer[i].Weights[IndexOfThis];
            }

            return sum;
        }
};

class NeuronLayer
{
    public:
        std::vector<Neuron> Neurons;

    public:
        NeuronLayer() {}
        NeuronLayer(const UnsignedNum thisLayerNeuronCount, const UnsignedNum prevLayerNeuronCount)
        {
            //Creates enough space for all the neurons
            Neurons.reserve(thisLayerNeuronCount);

            //Pushes on all of the neurons
            std::cout << "=========== New layer created ==========" << std::endl;
            for(UnsignedNum i = 0; i < thisLayerNeuronCount; i++)
            {
                Neuron neuron(prevLayerNeuronCount, i);
                Neurons.push_back(neuron);
            }
        }

    public:
        void PropagateForward(const NeuronLayer PASS_BY_REFERENCE lastLayer)
        {
            //Calls the ComputeOutput on each of the neurons
            for(UnsignedNum i = 0; i < Neurons.size(); i ++)
            {
                Neurons[i].ComputeOutput(lastLayer.Neurons);
            }
        }
        void PropagateBackward(const NeuronLayer PASS_BY_REFERENCE nextLayer)
        {
            //Calls the CalculateErrorTermHiddenLayer on each of the neurons
            for(UnsignedNum i = 0; i < Neurons.size(); i ++)
            {
                Neurons[i].CalculateErrorTermHiddenLayer(nextLayer.Neurons);
            }
        }

        void CalculateOutputLayerErrorTerm(const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            //Calls the CalculateErrorTermOutputLayer on each of the neurons
            for(UnsignedNum i = 0; i < Neurons.size(); i++)
            {
                Neurons[i].CalculateErrorTermOutputLayer(targetValues[i]);
            }
        }
        void UpdateWeights(const NeuronLayer PASS_BY_REFERENCE prevLayer, const DecimalNum eta, const DecimalNum alpha)
        {
            for(UnsignedNum i = 0; i < Neurons.size(); i++)
            {
                Neurons[i].UpdateWeights(prevLayer.Neurons, eta, alpha);
            }
        }

        //Is used for setting the intputs into the network
        void SetOutputs(const std::vector<DecimalNum> PASS_BY_REFERENCE inputs)
        {
            //Sets the inputs
            for(size_t i = Neurons.size(); i-- > 0;)
                Neurons[i].OutputValue = inputs[i];
        }
        //Is used for retrieving of the outputs of the network
        void GetOutputs(std::vector<DecimalNum> PASS_BY_REFERENCE outputs)
        {
            //Sets the outputs
            for(size_t i = Neurons.size(); i-- > 0;)
                outputs[i] = Neurons[i].OutputValue;
        }

    public:
        DecimalNum GetErrorRMS(const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            DecimalNum sum = 0;
            for(UnsignedNum i = 0; i < Neurons.size(); i++)
            {
                sum += Neurons[i].GetSquaredError(targetValues[i]);
            }

            return sqrt(sum / Neurons.size());
        }
        DecimalNum GetAverageDeltaWeight()
        {
            DecimalNum sum = 0;
            for(UnsignedNum i = 0; i < Neurons.size(); i++)
            {
                sum += Neurons[i].GetAverageDeltaWeight();
            }

            return sum / Neurons.size();
        }

    public:
        void PrintLayer()
        {
            for(UnsignedNum i = 0; i < Neurons.size(); i++)
                std::cout << "n:" << Neurons[i].OutputValue << " ";

            std::cout << std::endl;
        }
};

class NeuralNetStatisticContainer
{
    public:
        DecimalNum ErrorRMS = 0;
        DecimalNum AverageDeltaWeight = 0;
        std::chrono::duration<DecimalNum> Time;

    public:
        NeuralNetStatisticContainer() = default;
        NeuralNetStatisticContainer(const DecimalNum errorRMS, const DecimalNum averageDeltaWeight, const std::chrono::duration<DecimalNum> time)
            : ErrorRMS(errorRMS),
            AverageDeltaWeight(averageDeltaWeight),
            Time(time)
        {}

};

class NeuralNetStatistics
{
    public:
        std::vector<NeuralNetStatisticContainer> Statistics;
        UnsignedNum AllocatedSize;
        std::chrono::time_point<std::chrono::high_resolution_clock> TempTimePoint;
        std::chrono::duration<double> TempDuration;


    public:
        NeuralNetStatistics() {}
        NeuralNetStatistics(const UnsignedNum statisticsCount) {this->Reinitialise(statisticsCount);}

    public:
        inline void StartMeasureTime()
        {
            TempTimePoint = std::chrono::high_resolution_clock::now();
        }
        inline std::chrono::duration<double> StopMeasureTime()
        {
            TempDuration = TempTimePoint -std::chrono::high_resolution_clock::now();
            return TempDuration;
        }
        void Reinitialise(const UnsignedNum statisticsCount)
        {
            Statistics.clear();

            Statistics.reserve(statisticsCount);
            AllocatedSize = statisticsCount;
        }
        inline void AddDataPoint(const NeuralNetStatisticContainer container)
        {
            Statistics.push_back(container);
        }
        inline void PopulateContainerTime(NeuralNetStatisticContainer PASS_BY_REFERENCE cont) const
        {
            cont.Time = TempDuration;
        }
};

class NeuralNet
{
    public:
        std::vector<NeuronLayer> Layers;
        NeuralNetStatistics Stats;
        DecimalNum Eta;
        DecimalNum Alpha;


    public:
        NeuralNet(const std::vector<UnsignedNum> PASS_BY_REFERENCE neuronCounts)
        {
            this->BuildNet(neuronCounts);
            SetEtaAlpha(0.15, 0.3);
        }

    public:
        void BuildNet(const std::vector<UnsignedNum> PASS_BY_REFERENCE neuronCounts)
        {
            //Clears the current NetValue
            Layers.clear();

            //Creates the layer vector
            Layers.reserve(neuronCounts.size());

            //Goes through each layer and creates it according to the given size and previous size
            //Is split since the fisrt layer is a special case

            //Passes the first neuron count and 0 (for previous layer count)
            {
                NeuronLayer firstLayer(neuronCounts.at(0), 0);
                Layers.push_back(firstLayer);
            }

            //Goes through all other layers till the end
            for(UnsignedNum i = 1; i < neuronCounts.size(); i ++)
            {
                NeuronLayer layer(neuronCounts.at(i), neuronCounts.at(i - 1));
                Layers.push_back(layer);
            }
        }
        void LoadNet();
        void SaveNet()
        {

        }

    public:
        void SetInput(const std::vector<DecimalNum> PASS_BY_REFERENCE inputs)
        {
            //checks the inputs
            {
                if(inputs.size() < Layers.front().Neurons.size())
                {
                    std::cout << "Error - too few inputs;  returning" << std::endl;
                    return;
                }
                //Checks the input range
                for(size_t i = inputs.size(); i-- > 0;)
                    if(inputs[i] < 0 || inputs[i] > 1)
                    {
                        std::cout << "Error - input out of range; returning" << std::endl;
                        return;
                    }
            }

            //Input the values into the network
            {
                Layers.front().SetOutputs(inputs);
            }
        }
        void GetOutput(std::vector<DecimalNum> PASS_BY_REFERENCE outputs)
        {
            //checks the output
            {
                if(outputs.size() < Layers.front().Neurons.size())
                    outputs.resize(Layers.front().Neurons.size());
            }

            //Input the values into the network
            {
                Layers.back().GetOutputs(outputs);
            }
        }
        void PropagateForward()
        {
            //Cycle through all of the layers and call PropagateForward on each of them
            {
                for(UnsignedNum i = 1; i < Layers.size(); i++)
                {
                    Layers[i].PropagateForward(Layers[i - 1]);
                }
            }
        }
        void PropagateBackward(const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            //Calculates the last layer error term
            Layers.back().CalculateOutputLayerErrorTerm(targetValues);

            //Starts with the second last layer and iterates backwards calling PropagateBackward on each layer
            for(size_t i = Layers.size() - 1; i-- > 0;)
            {
                Layers[i].PropagateBackward(Layers[i+1]);
            }
        }

        void UpdateWeights(const DecimalNum eta, const DecimalNum alpha)
        {
            for(size_t i = 1; i < Layers.size(); i++)
            {
                Layers[i].UpdateWeights(Layers[i - 1], eta, alpha);
            }
        }

        void TrainSingle(const std::vector<DecimalNum> PASS_BY_REFERENCE inputs,
                         const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            //Sets the inputs
            SetInput(inputs);

            //Propagates forwards
            PropagateForward();

            //Propagates backwards
            PropagateBackward(targetValues);

            //Updates the weights
            UpdateWeights(Eta, Alpha);
        }
        void TrainSingle(const std::vector<DecimalNum> PASS_BY_REFERENCE inputs,
                         const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues,
                         std::vector<DecimalNum> PASS_BY_REFERENCE outputs,
                         NeuralNetStatistics PASS_BY_REFERENCE stats)
        {
            //Starts the timer
            stats.StartMeasureTime();

            TrainSingle(inputs, targetValues);

            //Ends the timer
            stats.StopMeasureTime();
            FillStatistics(stats, targetValues);

            //Retrieves the output
            GetOutput(outputs);
        }

    public:
        void FillStatistics(NeuralNetStatistics PASS_BY_REFERENCE stats, const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            NeuralNetStatisticContainer cont;
            CalculateAverageDeltaWeight(cont);
            CalculateErrorRMS(cont, targetValues);
            stats.AddDataPoint(cont);
        }

        inline void SetEtaAlpha(const DecimalNum eta, const DecimalNum alpha)
        {
            Eta = eta;
            Alpha = alpha;
        }

    private:
        void CalculateAverageDeltaWeight(NeuralNetStatisticContainer PASS_BY_REFERENCE cont)
        {
            DecimalNum sum = 0;

            for(UnsignedNum i = 1; i < Layers.size(); i++)
            {
                sum += Layers[i].GetAverageDeltaWeight();
            }

            cont.AverageDeltaWeight = sum / (Layers.size()-1);
        }
        void CalculateErrorRMS(NeuralNetStatisticContainer PASS_BY_REFERENCE cont, const std::vector<DecimalNum> PASS_BY_REFERENCE targetValues)
        {
            cont.ErrorRMS = Layers.back().GetErrorRMS(targetValues);
        }

    public:
        void PrintLayers()
        {
            //Prints all layers
            std::cout << "Printing layers" << std::endl;
            for(UnsignedNum i = 0; i < Layers.size(); i++)
                Layers[i].PrintLayer();
            std::cout << std::endl;
        }

};

void RunNeuralNetwork()
{
    std::vector<UnsignedNum> networkSize = {2, 2, 1};
    NeuralNet ann(networkSize);

    ann.PrintLayers();

    std::vector<DecimalNum> inputs1 = {0, 0};
    std::vector<DecimalNum> inputs2 = {1, 0};
    std::vector<DecimalNum> inputs3 = {0, 1};
    std::vector<DecimalNum> inputs4 = {1, 1};

    std::vector<DecimalNum> targetValues1 = {0};
    std::vector<DecimalNum> targetValues2 = {1};
    std::vector<DecimalNum> targetValues3 = {1};
    std::vector<DecimalNum> targetValues4 = {0};

    std::vector<DecimalNum> outputs = {0};

    for(int i = 0; i < 32000; i++)
    {
        ann.TrainSingle(inputs1, targetValues1, outputs, ann.Stats);
        //ann.PrintLayers();

        ann.TrainSingle(inputs2, targetValues2, outputs, ann.Stats);
        //ann.PrintLayers();

        ann.TrainSingle(inputs3, targetValues3, outputs, ann.Stats);
        //ann.PrintLayers();

        ann.TrainSingle(inputs4, targetValues4, outputs, ann.Stats);
        //ann.PrintLayers();
    }
    ann.TrainSingle(inputs1, targetValues1, outputs, ann.Stats);
    ann.PrintLayers();

    ann.TrainSingle(inputs2, targetValues2, outputs, ann.Stats);
    ann.PrintLayers();

    ann.TrainSingle(inputs3, targetValues3, outputs, ann.Stats);
    ann.PrintLayers();

    ann.TrainSingle(inputs4, targetValues4, outputs, ann.Stats);
    ann.PrintLayers();
}

#endif // NEURALNETWORK_H
