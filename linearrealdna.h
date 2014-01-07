#ifndef LINEARDOUBLEDNA_H
#define LINEARDOUBLEDNA_H

#include <math.h>
#include "baselineardna.h"

namespace Genetic
{
	class LinearRealDna : public BaseLinearDna<double>
	{
	public:
		void mutate(double parameter);
		void generate(double dnaSize);
		double getDistance(BaseLinearDna <double>* other);
	};

}

void Genetic::LinearRealDna::mutate(double parameter)
{
    double dlt = 0.0;
    for(double i = 1.0; i < parameter; ++i)
    {
        if(static_cast<double>(rand()%10000) / 10000.0 <= 1.0 / parameter)
        {
            dlt += pow(2.0, -i);
        }
    }

    if(rand() & 1)
    {
        dlt = -dlt;
    }

    dlt *= 0.5;
    int a = rand() % dna.size();

    dna[a] += dlt;
}

void Genetic::LinearRealDna::generate(double dnaSize)
{
    dna.resize(dnaSize);
    for(int i = 0; i < dna.size(); ++i)
    {
        dna[i] = ((double)(rand() % 1000000)) / 1000.0;
    }
}

double Genetic::LinearRealDna::getDistance(Genetic::BaseLinearDna <double>* other)
{
}

#endif // LINEARDOUBLEDNA_H
