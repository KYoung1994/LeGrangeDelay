/*
  ==============================================================================

    LagrangeInterpolation.h
    Created: 16 Feb 2023 8:55:38pm
    Author:  Kimberly Young

  ==============================================================================
*/

#pragma once
class LagrangeInterpolation
{
public:
    static double interpolate(int x1, double y1, int x2, double y2, int x3, double y3, double xPos)
    {
        double firstResult =    (((xPos - x2) * (xPos - x3)) / ((x1 - x2) * (x1 - x3))) * y1;
        double secondResult =   (((xPos - x1) * (xPos - x3)) / ((x2 - x1) * (x2 - x3))) * y2;
        double thirdResult =    (((xPos - x1) * (xPos - x2)) / ((x3 - x1) * (x3 - x2))) * y3;
        double result = firstResult + secondResult + thirdResult;
        
        return result;
    }
    
    static double interpolate(int x1, double y1, int x2, double y2, int x3, double y3, int x4, double y4, double xPos)
    {
        double firstResult =    (((xPos - x2) * ( xPos - x3 ) * (xPos - x4)) / ((x1 - x2) * (x1 - x3) * (x1 - x4))) * y1;
        double secondResult =   (((xPos - x1) * ( xPos - x3 ) * (xPos - x4)) / ((x2 - x1) * (x2 - x3) * (x2 - x4))) * y2;
        double thirdResult =    (((xPos - x1) * ( xPos - x2 ) * (xPos - x4)) / ((x3 - x1) * (x3 - x2) * (x3 - x4))) * y3;
        double fourthResult =   (((xPos - x1) * ( xPos - x2 ) * (xPos - x3)) / ((x4 - x1) * (x4 - x2) * (x4 - x3))) * y4;
        double result = firstResult + secondResult + thirdResult + fourthResult;
        
        return result;
    }
    
        static double interpolate(int x1, double y1, int x2, double y2, double xPos)
        {
            double firstResult =    (xPos - x2) / (x1 - x2) * y1;
            double secondResult =   (xPos - x1) / (x2 - x1) * y2;
            double result = firstResult + secondResult;
    
            return result;
        }
};
