/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008, 19-AUG-2008
 *
 */
package de.kechel.particlemovementsimulator;

public class TimeStepGenerator
{
		double step_size;
		double current_time;
		double start_time;
		double end_time;

		public TimeStepGenerator(double step_size, double start_time, double end_time)
		{
				this.step_size = step_size;
				this.start_time = start_time;
				this.end_time = end_time;
				current_time = start_time;
		}

		public boolean hasNext()
		{
			if( current_time <= end_time)
					return true;
				return false;
		}

		public boolean setNextStep()
		{
				current_time += step_size;
				if( current_time <= end_time)
					return true;
				return false;
		}

		public double getCurrentTime()
		{
				return current_time;
		}
                
                public long countSteps()
                {
                    return (long) (Math.abs(start_time - end_time)/step_size);
                }
                public int getPercentTime()
                {
                    int i = (int) ((current_time - start_time) / step_size / (double) countSteps() * 100.0);
                    return i;
                }
                public double getStepSize()
                {
                    return this.step_size;
                }
}

