#include "Macierz.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <Windows.h>
#include <random>
#include <queue>

using namespace std;

Macierz::Macierz(int wierzcholki_)
{
	wierzcholki = wierzcholki_;
	a = new int *[wierzcholki];
	GlobalMinRoute = new int[wierzcholki + 1];
	GlobalMin = MAXINT;

	for (int i = 0; i < wierzcholki; i++)
	{
		a[i] = new int[wierzcholki];
	}

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
			a[i][j] = -1;
	}



	//route = new int[wierzcholki + 1];

	//GenerateNewRoute();
}


Macierz::~Macierz()
{
	for (int i = 0; i < wierzcholki; i++)
		delete[] a[i];

	delete[] a;
}

void Macierz::losowaMacierz()
{
	srand(time(NULL));


	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			if (i == j) a[i][j] = -1;

			else a[i][j] = ((rand() % 100) + 1);
		}
	}
}


void Macierz::wpiszMacierz()
{

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			cin >> a[i][j];
		}
	}
}


void Macierz::wyswietlMacierz()
{
	cout << endl << endl << "Graf w postaci macierzy: " << endl << endl;

	for (int i = 0; i < wierzcholki; i++)
	{
		for (int j = 0; j < wierzcholki; j++)
		{
			cout.precision(4);
			cout << setw(4) << a[i][j];
		}

		cout << endl << endl;
	}

	cout << endl << endl;
}


int* Macierz::Swap(int *series, int i, int j)
{
	int temp;
	temp = series[i];
	series[i] = series[j];
	series[j] = temp;

	return series;
}

int* Macierz::Insert(int *series, int i, int j)
{
	if (i > j)
	{
		for (int a = i; a > j; a--)
			Swap(series, a, a - 1);
	}
	else if (i < j)
	{
		for (int a = i; a < j - 1; a++)
			Swap(series, a, a + 1);
	}
	return series;
}

int* Macierz::Invert(int *series, int i, int j)
{
	int substring_length, b, c;
	if (i < j)
	{
		substring_length = j - (i - 1);
		b = i;
		c = j;

		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	if (i > j)
	{
		substring_length = i - (j - 1);
		b = j;
		c = i;

		for (int a = 0; a < floor(substring_length / 2); a++)
		{
			Swap(series, b, c);
			b++;
			c--;
		}
	}
	return series;
}

int Macierz::RouteLength(int *route_)
{
	int length = 0;

	for (int i = 0; i < wierzcholki; i++)
		length += a[route_[i]][route_[i + 1]];

	return length;
}

void Macierz::GenerateNewRoute()
{
	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between

	int random;
	bool *generated;
	generated = new bool[wierzcholki];
	generated[0] = true;
	route[0] = 0;
	route[wierzcholki] = 0;
	for (int i = 1; i < wierzcholki; i++)
		generated[i] = false;

	for (int i = 1; i < wierzcholki; i++)
	{
		random = dist(gen);

		while (generated[random])
			random = dist(gen);

		route[i] = random;
		generated[random] = true;
	}
}

//----------------------------------------- Metody algorytmu genetycznego ------------------------------------------------------------//

int *Macierz::InsertMutation(int *series, float mutationRate)
{

	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between
	uniform_real<double> rnd(0.0, 1.0); // distribute results between

	double random = rnd(gen);

	if (random < (double)mutationRate)
	{
		int i, j;
		i = dist(gen);
		j = dist(gen);

		while (j == i)
			j = dist(gen);


		if (i > j)
		{
			for (int a = i; a > j; a--)
				Swap(series, a, a - 1);
		}
		else if (i < j)
		{
			for (int a = i; a < j - 1; a++)
				Swap(series, a, a + 1);
		}
	}
	return series;
}

int *Macierz::InvertMutation(int *series, float mutationRate)
{
	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between
	uniform_real<double> rnd(0.0, 1.0); // distribute results between

	double random = rnd(gen);

	if (random < (double)mutationRate)
	{
		int i, j;
		i = dist(gen);
		j = dist(gen);

		while (j == i)
			j = dist(gen);

		int substring_length, b, c;

		if (i < j)
		{
			substring_length = j - (i - 1);
			b = i;
			c = j;

			for (int a = 0; a < floor(substring_length / 2); a++)
			{
				Swap(series, b, c);
				b++;
				c--;
			}
		}
		if (i > j)
		{
			substring_length = i - (j - 1);
			b = j;
			c = i;

			for (int a = 0; a < floor(substring_length / 2); a++)
			{
				Swap(series, b, c);
				b++;
				c--;
			}
		}
	}
	return series;
}

void Macierz::CreateFirstPopulation()
{
	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between

	int *basicRoute, *shuffledRoute, randA, randB;
	basicRoute = new int[wierzcholki + 1];
	shuffledRoute = new int[wierzcholki + 1];


	for (int i = 0; i < wierzcholki + 1; i++)
	{
		if (i == wierzcholki)
			basicRoute[i] = 0;
		else
			basicRoute[i] = i;
	}

	for (int i = 0; i < PopSize; i++)
	{

		for (int a = 0; a < wierzcholki + 1; a++)
			shuffledRoute[a] = basicRoute[a];

		for (int j = 0; j < 100; j++)
		{
			randA = dist(gen);
			randB = dist(gen);

			while (randA == randB)
				randB = dist(gen);

			shuffledRoute = Swap(shuffledRoute, randA, randB);
		}

		for (int a = 0; a < wierzcholki + 1; a++)
			population[i][a] = shuffledRoute[a];
	}

	CreateGenotype(population);

	//cout << endl << "Najlepszy osobnik pierwszej populacji: " << endl;
	//for (int i = 0; i < wierzcholki+1; i++)
	//	cout << "->" << GlobalMinRoute[i];
	cout << endl << "Dlugosc sciezki najlepszego osobnika pierwszej generacji: " << GlobalMin << endl << endl;


}

void Macierz::CreateGenotype(int **Population)
{

	for (int i = 0; i < PopSize; i++)
	{
		int d = RouteLength(Population[i]);

		if (d < GlobalMin)
		{
			GlobalMin = d;
			for (int j = 0; j < wierzcholki + 1; j++)
			{
				GlobalMinRoute[j] = Population[i][j];
			}
		}

		fitness[i] = 1 / (double)d;
	}

	// Normalizing fitness values 
	double sum = 0.0;

	for (int i = 0; i < PopSize; i++)
		sum += fitness[i];

	for (int i = 0; i < PopSize; i++)
		fitness[i] /= sum;
}

void Macierz::nextGeneration(float mutationRate, int mutationType, float crossoverRate)
{
	int indexA, indexB, newGenLength1 = 0, newGenLength2 = 0;

	for (int i = 0; i < PopSize; i += 2)
	{
		if (i < PopSize - 1)
		{
			indexA = pickParent();
			indexB = pickParent();

			while (CompareParents(population[indexA], population[indexB]) == true)
				indexB = pickParent();


				PMXCrossover(indexA, indexB, crossoverRate);

				for (int j = 0; j < wierzcholki + 1; j++)
					newPopulation[i][j] = child[j];

				PMXCrossover(indexB, indexA, crossoverRate);

				for (int j = 0; j < wierzcholki + 1; j++)
					newPopulation[i + 1][j] = child[j];


			switch (mutationType)
			{
			case 1:
				newPopulation[i] = InsertMutation(newPopulation[i], mutationRate);
				newPopulation[i + 1] = InsertMutation(newPopulation[i + 1], mutationRate);
				break;
			case 2:
				newPopulation[i] = InvertMutation(newPopulation[i], mutationRate);
				newPopulation[i + 1] = InvertMutation(newPopulation[i + 1], mutationRate);
				break;
			default:
				newPopulation[i] = InsertMutation(newPopulation[i], mutationRate);
				newPopulation[i + 1] = InsertMutation(newPopulation[i + 1], mutationRate);
				break;
			}

			newGenLength1 = RouteLength(newPopulation[i]);
			newGenLength2 = RouteLength(newPopulation[i + 1]);

			if (newGenLength1 < GlobalMin)
			{
				GlobalMin = newGenLength1;
				for (int a = 0; a < wierzcholki + 1; a++)
					GlobalMinRoute[a] = newPopulation[i][a];
			}

			if (newGenLength2 < GlobalMin)
			{
				GlobalMin = newGenLength2;
				for (int a = 0; a < wierzcholki + 1; a++)
					GlobalMinRoute[a] = newPopulation[i + 1][a];
			}
		}

		else
		{
			indexA = pickParent();
			indexB = pickParent();

			while (CompareParents(population[indexA], population[indexB]) == true)
				indexB = pickParent();


				PMXCrossover(indexA, indexB, crossoverRate);

				for (int j = 0; j < wierzcholki + 1; j++)
					newPopulation[i][j] = child[j];


			switch (mutationType)
			{
			case 1:
				newPopulation[i] = InsertMutation(newPopulation[i], mutationRate);
				break;
			case 2:
				newPopulation[i] = InvertMutation(newPopulation[i], mutationRate);
				break;
			default:
				newPopulation[i] = InsertMutation(newPopulation[i], mutationRate);
				break;
			}

			newGenLength1 = RouteLength(newPopulation[i]);

			if (newGenLength1 < GlobalMin)
			{
				GlobalMin = newGenLength1;
				for (int a = 0; a < wierzcholki + 1; a++)
					GlobalMinRoute[a] = newPopulation[i][a];
			}

		}

	}

	for (int i = 0; i < PopSize; i++)
	{
		for (int j = 0; j < wierzcholki + 1; j++)
			population[i][j] = newPopulation[i][j];
	}

	CreateGenotype(population);
}

int Macierz::pickParent()
{
	// Selekcja turniejowa

	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(0, PopSize - 1); // distribute results between

	int random, k = 2, index = dist(gen);

	for (int i = 0; i < k - 1; i++)
	{
		random = dist(gen);
		if (fitness[random] > fitness[index])
			index = random;
	}
	return index;
}

bool Macierz::CompareParents(int *ParentA, int *ParentB)
{
	bool comparison = true;
	for (int i = 0; i < wierzcholki + 1; i++)
	{
		if (ParentA[i] != ParentB[i])
		{
			comparison = false;
			break;
		}
	}
	return comparison;
}

void Macierz::PMXCrossover(int indexA, int indexB, float crossoverRate)
{

	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between
	uniform_real<double> rnd(0.0, 1.0); // distribute results between

	double random = rnd(gen);

	if (random < (double)crossoverRate)
	{

		int random1 = dist(gen), random2 = dist(gen);


		for (int i = 0; i < wierzcholki + 1; i++)
		{
			child[i] = -1;
			ElementPair[i] = true;
			ElementPositionB[population[indexB][i]] = i;
		}


		if (random1 > random2)
		{
			int temp;
			temp = random1;
			random1 = random2;
			random2 = temp;
		}


		for (int i = random1; i <= random2; i++)
		{
			child[i] = population[indexA][i];
		}
		for (int i = random1; i <= random2; i++)
		{
			for (int j = random1; j <= random2; j++)
			{
				if (child[i] == population[indexB][j])
				{
					ElementPair[j] = false;
				}
			}
		}
		for (int i = random1; i <= random2; i++)
		{
			if (ElementPair[i])
			{
				if (ElementPositionB[child[i]] < random1 || ElementPositionB[child[i]] > random2)
					child[ElementPositionB[child[i]]] = population[indexB][i];

				else
				{
					int position = ElementPositionB[child[i]];

					while (child[position] != -1)
						position = ElementPositionB[child[position]];

					child[position] = population[indexB][i];
				}
			}
		}

		for (int i = 0; i < wierzcholki + 1; i++)
		{
			if (child[i] == -1)
				child[i] = population[indexB][i];
		}
	}
	else if (random >= (double)crossoverRate)
	{
		for (int i = 0; i < wierzcholki + 1; i++)
			child[i] = population[indexA][i];
	}
}

void Macierz::OXCrossover(int indexA, int indexB, float crossoverRate)
{
	random_device rd; // non-deterministic generator
	mt19937 gen(rd()); // random engine seeded with rd()
	uniform_int_distribution<> dist(1, wierzcholki - 1); // distribute results between
	uniform_real<double> rnd(0.0, 1.0); // distribute results between

	double random = rnd(gen);

	if (random < (double)crossoverRate)
	{

		int random1 = dist(gen), random2 = dist(gen);
		bool copied = false;

		for (int i = 0; i < wierzcholki + 1; i++)
		{
			child[i] = -1;
			ElementPair[i] = false;
			ElementPositionB[population[indexB][i]] = i;
		}

		child[0] = 0;
		child[wierzcholki] = 0;

		if (random1 > random2)
		{
			int temp;
			temp = random1;
			random1 = random2;
			random2 = temp;
		}

		for (int i = random1; i <= random2; i++)
		{
			child[i] = population[indexA][i];
			ElementPair[child[i]] = true;
		}


		for (int i = random2 + 1; i < wierzcholki; i++)
		{
			if (!ElementPair[population[indexB][i]])
			{
				child[i] = population[indexB][i];
				ElementPair[child[i]] = true;
			}
			else
			{
				int position = i;

				while (ElementPair[population[indexB][position]])
				{
					position++;
					position = position % wierzcholki;
					if (position == 0)
						position++;
				}

				child[i] = population[indexB][position];
				ElementPair[child[i]] = true;
			}
		}
		for (int i = 1; i < random1; i++)
		{
			if (!ElementPair[population[indexB][i]])
			{
				child[i] = population[indexB][i];
				ElementPair[child[i]] = true;
			}
			else
			{
				int position = i;

				while (ElementPair[population[indexB][position]])
				{
					position++;
					position = position % wierzcholki;
					if (position == 0)
						position++;
				}
				child[i] = population[indexB][position];
				ElementPair[child[i]] = true;
			}
		}
	}

	else if (random >= (double)crossoverRate)
	{
		for (int i = 0; i < wierzcholki + 1; i++)
			child[i] = population[indexA][i];
	}
}

void Macierz::GeneticAlgoritmEngine(int PopSize_, double stop_cryterium, float  mutationRate, int mutationType, float crossoverRate)
{
	time_t timer1;
	time(&timer1);
	time_t  timer2;
	double second = 0, czas = 10.0;

	PopSize = PopSize_;

	population = new int *[PopSize];
	newPopulation = new int *[PopSize];
	fitness = new double[PopSize];
	child = new int[wierzcholki + 1];
	ElementPositionB = new int[wierzcholki + 1];
	ElementPair = new bool[wierzcholki + 1];
	GlobalMin = MAXINT;

	for (int i = 0; i < wierzcholki + 1; i++)
		GlobalMinRoute[i] = 0;


	for (int i = 0; i < PopSize; i++)
	{
		population[i] = new int[wierzcholki + 1];
		newPopulation[i] = new int[wierzcholki + 1];
	}
	CreateFirstPopulation();

	while (second <= stop_cryterium)
	{
		nextGeneration(mutationRate, mutationType, crossoverRate);

		time(&timer2);
		second = difftime(timer2, timer1);

		if (second >= czas)
		{
			//cout << endl << "Najlepszy osobnik ostatniej populacji: " << endl;
			//for (int i = 0; i < wierzcholki + 1; i++)
			//	cout << "->" << GlobalMinRoute[i];
			cout << endl << "Dlugosc sciezki najlepszego osobnika po " << czas << " sekundach: "
				<< GlobalMin << endl << endl;
			czas += 10.0;
		}
	}

	//cout << endl << "Najlepszy osobnik ostatecznej populacji: " << endl;
	//for (int i = 0; i < wierzcholki + 1; i++)
	//	cout << "->" << GlobalMinRoute[i];
	cout << endl << "Dlugosc sciezki najlepszego osobnika znalezionego po zadanym czasie: "
		<< GlobalMin << endl << endl;
}