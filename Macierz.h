#pragma once
class Macierz
{
public:
	int wierzcholki;
	int sasiedztwo;
	Macierz(int wierzcholki_);
	~Macierz();
	void wyswietlMacierz();
	void losowaMacierz();
	int **a;
	void wpiszMacierz();
	int *route;
	int *Swap(int *series, int i, int j);
	int RouteLength(int *route);
	void GenerateNewRoute();
	int *Insert(int *series, int i, int j);
	int *Invert(int *series, int i, int j);

	//Metody algorytmu genetycznego
	int GlobalMin, PopSize;
	int *GlobalMinRoute, *child, *ElementPositionB;
	bool *ElementPair;
	double *fitness;
	int **population, **newPopulation;
	void CreateFirstPopulation();
	int *InsertMutation(int *series, float mutationRate);
	int *InvertMutation(int *series, float mutationRate);
	void CreateGenotype(int **Population);
	void nextGeneration(float mutationRate, int mutationType, float crossoverRate);
	int pickParent();
	void PMXCrossover(int indexA, int indexB, float crossoverRate);
	void OXCrossover(int indexA, int indexB, float crossoverRate);
	bool CompareParents(int *ParentA, int * ParentB);

	void GeneticAlgoritmEngine(int PopSize_, double stop_cryterium, float mutationRate, int mutationType, float crossoverRate);

};

