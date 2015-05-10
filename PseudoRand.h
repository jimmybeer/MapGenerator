/**
 * Implementation of the Park Miller (1988) "minimal standard" linear 
 * congruential pseudo-random number generator.
 * §
 * For a full explanation visit: http://www.firstpr.com.au/dsp/rand31/
 * 
 * The generator uses a modulus constant (m) of 2^31 - 1 which is a
 * Mersenne Prime number and a full-period-multiplier of 16807.
 * Output is a 31 bit unsigned integer. The range of values output is
 * 1 to 2,147,483,646 (2^31-1) and the seed must be in this range too.
 * 
 * Conversion of Michael Baczynski, www.polygonal.de as3 implementation
 */
#pragma once
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()
#include <iostream>

namespace PseudoRand
{
    extern int seed;
    
    static int setSeed(int newSeed = 0)
    {
        if(newSeed != 0) 
        {
	        seed = newSeed;
        }
	    else 
        {
            srand( time( NULL ));
		    seed = rand();
        }
        return seed;
    }
    
    /**
     * generator:
     * new-value = (old-value * 16807) mod (2^31 - 1)
     */
    static int gen()
    {
        //integer version 1, for max int 2^46 - 1 or larger.
        return seed = (seed * 16807) % 2147483647;
        
        /**
         * integer version 2, for max int 2^31 - 1 (slowest)
         */
        //var test:int = 16807 * (seed % 127773 >> 0) - 2836 * (seed / 127773 >> 0);
        //return seed = (test > 0 ? test : test + 2147483647);
        
        /**
         * david g. carta's optimisation is 15% slower than integer version 1
         */
        //var hi:uint = 16807 * (seed >> 16);
        //var lo:uint = 16807 * (seed & 0xFFFF) + ((hi & 0x7FFF) << 16) + (hi >> 15);
        //return seed = (lo > 0x7FFFFFFF ? lo - 0x7FFFFFFF : lo);
    }
	
	/**
           * provides the next pseudorandom number as an unsigned integer (31 bits)
	  */
    static int nextInt() 
    {
	    return int(gen());
	}
	
	/**
	 * provides the next pseudorandom number as a float between nearly 0 and nearly 1.0.
	 */
	static float nextFloat() 
    {
        return (float(gen()) / 2147483647);

	}
	
	/**
 	 * provides the next pseudorandom number as an unsigned integer (31 bits) betweeen
	 * a given range.
	 */
	static int nextIntRange(int min, int max) 
    {
        return int((min + ((max - min) * nextFloat())));
	}
	
	/**
	 * provides the next pseudorandom number as a float between a given range.
	 */
	static float nextFloatRange(float min, float max) 
    {
	    return (min + ((max - min) * nextFloat()));
	}
}