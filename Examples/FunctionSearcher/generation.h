
#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "individual.h"
#include "geneticsettings.h"

template <class T>
bool individualComparator(T* i1, T* i2)
{
	return (i1 -> getScore() < i2 -> getScore());
}

using std::endl;

namespace Genetic {


/**
 * @class Generation
 * This class implements a single generation.
 */

	template <typename Individual>
	class Generation
	{
	public:

		Generation(int _individualsNum, Genetic::GeneticSettings* _geneticSettings);
		~Generation();

		void genNext();
		void init(unsigned int seed, double dnaGenerationParameter);

	protected:

		std::vector <Individual*> individuals;
		int individualsNum;

		GeneticSettings* geneticSettings;

	public:

		/**
		 * Set the array of individuals
		 * @param new_var the new array of individuals
		 */
		void setIndividuals(const std::vector <Individual*>& value);

		/**
		 * Get the array of individuals
		 * @return the array of individuals
		 */
		std::vector <Individual*>& getIndividuals();

		/**
		 * Get the array of individuals sorted by score
		 * @return the array of individuals sorted by score
		 */
		std::vector <Individual*>& getIndividualsSorted();

		/**
		 * Get the best individual in generation
		 * @return the best individual in generation
		 */
		Individual* getIndividualBest();

		/**
		 * Set the value of individualsNum
		 * @param new_var the new value of individualsNum
		 */
		void setIndividualsNum(int value);

		/**
		 * Get the value of individualsNum
		 * @return the value of individualsNum
		 */
		int getIndividualsNum();

		/**
		 * Set the value of individualsNum
		 * @param new_var the new value of individualsNum
		 */
		void setGeneticSettings(GeneticSettings* value);

		/**
		 * Get the value of individualsNum
		 * @return the value of individualsNum
		 */
		GeneticSettings* getGeneticSettings();
	};
}; // end of package namespace

template <typename Individual>
Genetic::Generation <Individual>::Generation(int _individualsNum, Genetic::GeneticSettings* _geneticSettings)
{
	individualsNum = _individualsNum;
	geneticSettings = _geneticSettings;
	assert(individualsNum % 2 == 0);
}

template <typename Individual>
Genetic::Generation <Individual>::~Generation()
{
	for(int i = 0; i < individuals.size(); ++i)
	{
		delete individuals[i];
	}
	individuals.clear();
}

template <typename Individual>
void Genetic::Generation <Individual>::genNext()
{
	std::vector <Individual*> nextIndividuals(individualsNum);
	for(int i = 0; i < individualsNum; ++i)
	{
		nextIndividuals[i] = new Individual();
	}

    // Recombination
	switch(geneticSettings -> getParentsSelectionType())
	{
        case PANMIXIA:
        {
            int individualsNum = individuals.size();
            for(int i = 0; i < individualsNum / 2; ++i)
            {
                int firstParent = rand() % individualsNum;
                int secondParent = rand() % (individualsNum - 1);
                if(secondParent >= firstParent)
                {
                    ++secondParent;
                }
                Individual::recombine(individuals[firstParent], individuals[secondParent],
                                      nextIndividuals[i * 2], nextIndividuals[i * 2 + 1], geneticSettings);
            }
            break;
        }
        case INBREEDING_FENOTYPE:
        {
            int firstParent, secondParent;
            double minDistance;
            double currentDistance;
            for(int i = 0; i < individualsNum / 2; ++i)
            {
                firstParent = rand() % individualsNum;
                minDistance = (firstParent == 0) ? (individuals[0] -> getDna() -> getDistance(individuals[1] -> getDna()))
                    : individuals[0] -> getDna() -> getDistance(individuals[firstParent] -> getDna());
                secondParent = (firstParent == 0) ? 1 : 0;
                for(int j = 0; j < individualsNum; ++j)
                {
                    if((j != firstParent)
                       && (minDistance > (currentDistance = individuals[firstParent]
                                          -> getDna() -> getDistance(individuals[j] -> getDna()))))
                    {
                        minDistance = currentDistance;
                        secondParent = j;
                    }
                }

                Individual::recombine(individuals[firstParent], individuals[secondParent],
                                      nextIndividuals[i * 2], nextIndividuals[i * 2 + 1], geneticSettings);
            }
            break;
        }
        case INBREEDING_GENOTYPE:
        {
            int firstParent, secondParent;
            double minDistance;
            double currentDistance;
            for(int i = 0; i < individualsNum / 2; ++i)
            {
                firstParent = rand() % individualsNum;
                minDistance = (firstParent == 0) ? fabs(individuals[0] -> getScore() - individuals[1] -> getScore())
                    : fabs(individuals[0] -> getScore() - individuals[firstParent] -> getScore());
                secondParent = (firstParent == 0) ? 1 : 0;
                for(int j = 0; j < individualsNum; ++j)
                {
                    if((j != firstParent)
                       && (minDistance > (currentDistance =
                                          fabs(individuals[firstParent] -> getScore() - individuals[j] -> getScore()))))
                    {
                        minDistance = currentDistance;
                        secondParent = j;
                    }
                }

                Individual::recombine(individuals[firstParent], individuals[secondParent],
                                      nextIndividuals[i * 2], nextIndividuals[i * 2 + 1], geneticSettings);
            }
            break;
        }
        case OUTBREEDING_FENOTYPE:
        {
            int firstParent, secondParent;
            double maxDistance;
            double currentDistance;
            for(int i = 0; i < individualsNum / 2; ++i)
            {
                firstParent = rand() % individualsNum;
                maxDistance = (firstParent == 0) ? (individuals[0] -> getDna() -> getDistance(individuals[1] -> getDna()))
                    : individuals[0] -> getDna() -> getDistance(individuals[firstParent] -> getDna());
                secondParent = (firstParent == 0) ? 1 : 0;
                for(int j = 0; j < individualsNum; ++j)
                {
                    if((j != firstParent)
                       && (maxDistance < (currentDistance = individuals[firstParent]
                                          -> getDna() -> getDistance(individuals[j] -> getDna()))))
                    {
                        maxDistance = currentDistance;
                        secondParent = j;
                    }
                }

                Individual::recombine(individuals[firstParent], individuals[secondParent],
                                      nextIndividuals[i * 2], nextIndividuals[i * 2 + 1], geneticSettings);
            }
            break;
        }
        case OUTBREEDING_GENOTYPE:
        {
            int firstParent, secondParent;
            double maxDistance;
            double currentDistance;
            for(int i = 0; i < individualsNum / 2; ++i)
            {
                firstParent = rand() % individualsNum;
                maxDistance = (firstParent == 0) ? fabs(individuals[0] -> getScore() - individuals[1] -> getScore())
                    : fabs(individuals[0] -> getScore() - individuals[firstParent] -> getScore());
                secondParent = (firstParent == 0) ? 1 : 0;
                for(int j = 0; j < individualsNum; ++j)
                {
                    if((j != firstParent)
                       && (maxDistance < (currentDistance =
                                          fabs(individuals[firstParent] -> getScore() - individuals[j] -> getScore()))))
                    {
                        maxDistance = currentDistance;
                        secondParent = j;
                    }
                }

                Individual::recombine(individuals[firstParent], individuals[secondParent],
                                      nextIndividuals[i * 2], nextIndividuals[i * 2 + 1], geneticSettings);
            }
            break;
        }
	}

	// Testing
    // For old `parents`
	for(int i = 0; i < individualsNum; ++i)
	{
		individuals[i] -> test();
		nextIndividuals.push_back(individuals[i]);
	}
    // For new `children`
	for(int i = 0; i < individualsNum; ++i)
	{
		nextIndividuals[i] -> mutate(geneticSettings);
		nextIndividuals[i] -> test();
	}

	individuals.clear();
	sort(nextIndividuals.begin(), nextIndividuals.end(), individualComparator<Individual>);

	// Selection individuals for new generation.
	switch(geneticSettings -> getNewGenerationSelectionType())
	{
	case TRUNCATION_SELECTION:
	{
		int possibleIndividualsNum = individualsNum *
                                     (geneticSettings -> getGenerationSelectionParameter());
		std::vector <bool> used(nextIndividuals.size(), false);
		assert(possibleIndividualsNum != 0);
		for(int i = 0; i < individualsNum; ++i)
		{
			int currentId = rand() % possibleIndividualsNum;
			used[currentId] = true;
			Individual* tmp = new Individual();
			*tmp = *nextIndividuals[currentId];
			individuals.push_back(tmp);
			delete nextIndividuals[i];
		}
		break;
	}
	case ELITE_SELECTION:
	{
		for(int i = 0; i < individualsNum; ++i)
		{
			individuals.push_back(nextIndividuals[i]);
			delete nextIndividuals[individualsNum + i];
		}
		break;
	}
	default:
		break;
	}

	#ifdef ENABLE_DNALOG
	dnalog << "Best: ";
	nextIndividuals[0] -> getDna() -> print();
	dnalog << " with score " << nextIndividuals[0] -> getScore() << std::endl;
	#endif

	nextIndividuals.clear();
}

template <typename Individual>
void Genetic::Generation <Individual>::init(unsigned int seed, double dnaGenerationParameter)
{
    srand(seed);

    for(int i = 0; i < individuals.size(); ++i)
	{
		delete individuals[i];
	}
	individuals.clear();

	Individual* tmpIndividual;

	for(int i = 0; i < individualsNum; ++i)
	{
		tmpIndividual = new Individual();
		tmpIndividual -> generate(dnaGenerationParameter);
		tmpIndividual -> test();
		individuals.push_back(tmpIndividual);
	}
}

template <typename Individual>
void Genetic::Generation <Individual>::setIndividuals(const std::vector <Individual*>& value)
{
	individuals = value;
}

template <typename Individual>
std::vector <Individual*>& Genetic::Generation <Individual>::getIndividuals()
{
	return individuals;
}

template <typename Individual>
std::vector <Individual*>& Genetic::Generation <Individual>::getIndividualsSorted()
{
    std::vector <Individual*> ans = individuals;
	sort(ans.begin(), ans.end(), individualComparator<Individual>);
	return ans;
}

template <typename Individual>
Individual* Genetic::Generation <Individual>::getIndividualBest()
{
    assert(individuals.size() > 0);
    Individual* ans = individuals[0];
    for(int i = 1; i < individuals.size(); ++i)
        if(ans -> getScore() > individuals[i] -> getScore())
            ans = individuals[i];
    return ans;
}

template <typename Individual>
void Genetic::Generation <Individual>::setGeneticSettings(Genetic::GeneticSettings* value)
{
	geneticSettings = value;
}

template <typename Individual>
Genetic::GeneticSettings* Genetic::Generation <Individual>::getGeneticSettings()
{
	return geneticSettings;
}

#endif // GENERATION_H
