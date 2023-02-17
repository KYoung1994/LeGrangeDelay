/*
  ==============================================================================

    DelayLine.h
    Created: 16 Feb 2023 8:58:39pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LagrangeInterpolation.h"

class DelayLine
{
public:
    DelayLine()
    {
    }
    
    ~DelayLine()
    {
    }
    
    void prepareDelayLine(double sampleRate, float maxDelayTime, int numChannels) // Function to prepare everything before any processing is done.
    {
        maxDelayInSamples = sampleRate * maxDelayTime / 1000.0; // Calclulate our max delay in samples.
        buffer.setSize(numChannels, maxDelayInSamples + 3);     // Resize the buffer so that it has enough samples for our maximum.
        buffer.clear();                                         // Ensure that buffer is empty.
        
        position.resize(numChannels);                           // Sizing our vectors for the number of channels.
        mixValues.resize(numChannels);
        feedbackValues.resize(numChannels);
        delayValues.resize(numChannels);
        for (int i = 0; i < position.size(); i++) {
            position[i] = 0;                                    // Resetting our positions.
            
            mixValues[i].reset(sampleRate, 0.1);                // Setting the times for our smoothed values.
            feedbackValues[i].reset(sampleRate, 0.1);
            
            delayValues[i].reset(sampleRate, 0.1);
        }
        
        this->sampleRate = sampleRate;
    }
    
    float processSample(float sample, int channel, float delayTime, float feedback, float mix) // Parent function which calls our child functions. Used to neaten up other classes.
    {
        mixValues[channel].setTargetValue(mix);             // Set the parameter target values for smoothing.
        feedbackValues[channel].setTargetValue(feedback);
        delayValues[channel].setTargetValue(feedback);
        
        mix = mixValues[channel].getNextValue();                    // Getting the current smoothed values for parameters.
        feedback = feedbackValues[channel].getNextValue();
        
        delayTime = delayValues[channel].getNextValue();
        
        writeSample(sample, channel);                               // Call child functions.
        float output = readSample(channel, delayTime);
        addToSample(output * feedback, channel);
        return output * mix + sample * (1 - mix);
    }

//protected:
    void writeSample(float sample, int channel) // Funtion to store our current sample.
    {
        auto bufferPointer = buffer.getWritePointer(channel);   // Grab a write pointer.
        bufferPointer[position[channel]] = sample;              // Store the sample.
        
        position[channel]++;                                    // Itterate our positon.
        if (position[channel] >= buffer.getNumSamples()) {      // Ensure that our position doesn't go out of bounds of our buffer.
            position[channel] -= buffer.getNumSamples();
        }
    }
    
    void writeSampleFirst(float sample, int channel) // Funtion to store our current sample.
    {
        position[channel]++;                                    // Itterate our positon.
        if (position[channel] >= buffer.getNumSamples()) {      // Ensure that our position doesn't go out of bounds of our buffer.
            position[channel] -= buffer.getNumSamples();
        }
        
        auto bufferPointer = buffer.getWritePointer(channel);   // Grab a write pointer.
        bufferPointer[position[channel]] = sample;              // Store the sample.
    }
    
    float readSample(int channel, float delayTime) // Function to read a past sample.
    {
        int thisDelay = position[channel] - sampleRate * delayTime / 1000.0;    // Calculate delay position.
        
        if (thisDelay < 0)                                                      // Ensure that our position doesn't go out of bounds of our buffer.
        {
            thisDelay += buffer.getNumSamples();
        }
        
        auto bufferPointer = buffer.getReadPointer(channel);                    // Grab a read pointer.
        return bufferPointer[thisDelay];                                        // Read the sample.
    }
    
    
    void addToSample(float sample, int channel) // Adds a value to our current sample - useful for feedback etc.
    {
        auto bufferPointer = buffer.getWritePointer(channel);   // Grab a write pointer.
        bufferPointer[position[channel]] += sample;             // Add to stored sample.
    }
    
    juce::AudioBuffer<float> buffer;
    std::vector<int> position;
    int maxDelayInSamples;
    double sampleRate;
    std::vector<juce::SmoothedValue<float>> mixValues, feedbackValues, delayValues;
};
