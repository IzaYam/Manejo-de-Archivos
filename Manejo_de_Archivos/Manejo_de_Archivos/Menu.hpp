#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

class Menu {
	struct Opcion {
		string texto;
		function<void()> lambda;

		Opcion(string texto, function<void()> lambda)
			: texto(texto), lambda(lambda) {}
	};

	string titulo;
	vector<Opcion> opciones;

public:
	Menu(string titulo) : titulo(titulo) {}

	void crear_opcion(string texto, function<void()> lambda) {
		opciones.push_back(Opcion(texto, lambda));
	}

	void iniciar() {
		cout << titulo << endl;
		for (size_t i = 0; i < opciones.size(); ++i) {
			cout << i + 1 << ") " << opciones[i].texto << endl;
		}
		short opcion = 0;
		do {
			cout << "Opcion: ";
			cin >> opcion;
		} while (opcion <= 0 || opcion > opciones.size());
		auto& lambda = opciones[opcion - 1].lambda;
		lambda();
	}
};
