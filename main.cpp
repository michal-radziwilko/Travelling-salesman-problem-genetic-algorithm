#include "Macierz.h"
#include <iostream>
#include <string>
#include <fstream>
#include "tinyxml2.h"
#include <random>

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main()
{
	int PopSize = 131, mutationType = 2;
	float mutationRate = 0.1, crossoverRate = 0.8;
	double stop_cryterium = 60;
	int choice;

	double edge_cost;
	int vertex_number, wierzcholki, vertex_i;

	std::string fname = "gil262";
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLNode * pRoot;
	XMLNode * graph;
	XMLElement * vertex;
	XMLElement * edge;

	Macierz *macierz;

	while (true)
	{

		std::cout << std::endl << "          1. Wczytaj dane z pliku (.xml)";
		std::cout << std::endl << "          2. Wprowadz kryterium stopu";
		std::cout << std::endl << "          3. Wprowadz wielkosc populacji";
		std::cout << std::endl << "          4. Wprowadz wspolczynnik mutacji";
		std::cout << std::endl << "          5. Wprowadz wspolczynnik krzyzowania";
		std::cout << std::endl << "          6. Wybierz metode mutacji";
		std::cout << std::endl << "          7. Uruchom algorytm genetyczny";
		std::cout << std::endl;
		std::cout << std::endl << "          ";

		std::cin >> choice;

		switch (choice)
		{
		case 1:
			std::cout << std::endl << "          Podaj nazwe pliku do wczytania grafu: ";
			std::cin >> fname;
			fname = fname + ".xml";

			eResult = xmlDoc.LoadFile(fname.c_str());
			XMLCheckResult(eResult);


			pRoot = xmlDoc.FirstChildElement("travellingSalesmanProblemInstance");
			if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

			graph = pRoot->FirstChildElement("graph");
			if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

			vertex = graph->FirstChildElement("vertex");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;

			edge = vertex->FirstChildElement("edge");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;


			wierzcholki = 0;

			while (vertex != nullptr)
			{
				vertex = vertex->NextSiblingElement("vertex");

				wierzcholki++;
			}

			macierz = new Macierz(wierzcholki);

			vertex = graph->FirstChildElement("vertex");
			if (vertex == nullptr) return XML_ERROR_PARSING_ELEMENT;

			vertex_i = 0;

			while (vertex != nullptr)
			{
				while (edge != nullptr)
				{

					eResult = edge->QueryIntText(&vertex_number);
					XMLCheckResult(eResult);

					eResult = edge->QueryDoubleAttribute("cost", &edge_cost);
					XMLCheckResult(eResult);

					edge = edge->NextSiblingElement("edge");

					macierz->a[vertex_i][vertex_number] = (int)edge_cost;

				}
				vertex = vertex->NextSiblingElement("vertex");

				if (vertex != nullptr)
					edge = vertex->FirstChildElement("edge");

				vertex_i++;
			}
			break;

		case 2:
			std::cout << std::endl << "     Podaj maksymalny czas(w sekundach) szukania rozwiazania: ";
			std::cin >> stop_cryterium;
			break;

		case 3:
			std::cout << std::endl << "     Podaj wielkosc populacji: ";
			std::cin >> PopSize;
			break;
		case 4:
			std::cout << std::endl << "     Podaj wspolczynnik mutacji (jako wartosc z przedzialu [0,1]): ";
			std::cin >> mutationRate;
			break;
		case 5:
			std::cout << std::endl << "     Podaj wspolczynnik krzyzowania (jako wartosc z przedzialu [0,1]): ";
			std::cin >> crossoverRate;
			break;
		case 6:
			std::cout << std::endl << "     Wybierz metode mutacji (1-Insertion, 2-Invertion): ";
			std::cin >> mutationType;
			break;
		case 7:
			macierz->GeneticAlgoritmEngine(PopSize, stop_cryterium, mutationRate, mutationType, crossoverRate);
			break;
		}
	}
}
