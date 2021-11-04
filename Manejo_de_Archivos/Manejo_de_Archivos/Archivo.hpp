#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <functional>
#include <vector>
#include <regex>

#include "Lista.hpp"
#include "Validadores.hpp"

using namespace std;
namespace fs = std::filesystem;

class Archivo {
	fs::path ruta;

public:
	Archivo(fs::path ruta) : ruta(ruta) {}
	Archivo(fs::path ruta, fs::path carpeta_padre) {
		this->ruta = carpeta_padre / ruta;
	}

	fs::path get_ruta() {
		return ruta;
	}

	void leer(function<void(string&)> fun) {
		std::fstream archivo(ruta, ios::in);
		if (archivo.is_open()) {
			string str;
			while (getline(archivo, str)) {
				fun(str);
			}
		}
		archivo.close();
	}

	bool escribir(string& contenido) {
		std::fstream archivo(ruta, ios::out | ios::app);
		if (!archivo.is_open()) return false;
		archivo << contenido;
		archivo.close();
		return true;
	}

	void editar() {
		if (!fs::exists(ruta)) {
			std::fstream archivo(ruta, ios::out);
			archivo.close();
		}
		fs::path ruta_completa = ruta;
		if (ruta_completa.is_relative()) {
			ruta_completa = fs::current_path() / ruta_completa;
		}
		string cmd = string("notepad.exe ") + ruta_completa.string();
		std::system(cmd.c_str());
	}

	static bool validar_nombre_archivo(string nombre) {
		return std::regex_match(nombre, Validador::Archivo);
	}

	static Archivo* crear(fs::path carpeta_padre) {
		string nombre_archivo;
		do {
			cout << "\nNombre del archivo: ";
			cin >> nombre_archivo;
		} while (!validar_nombre_archivo(nombre_archivo));

		short cantidad_campos = 0;
		do {
			cout << "Cantidad de campos: ";
			cin >> cantidad_campos; cout << endl;
		} while (cantidad_campos <= 0);

		vector<string> nombres_campos;
		for (short i = 0; i < cantidad_campos; ++i) {
			string campo;
			cout << "Campo #" << i + 1 << ": ";
			cin >> campo;
			nombres_campos.push_back(campo);
		}

		short cantidad_elementos = 0;
		do {
			cout << "\nCantidad de elementos a insertar: ";
			cin >> cantidad_elementos;
		} while (cantidad_campos < 0);

		// TO DO: Lista puede ser una Cola
		Lista<vector<string>> elementos;
		for (short elem = 0; elem < cantidad_elementos; ++elem) {
			cout << "Elemento #" << elem + 1 << ": " << endl;
			vector<string> entrada;
			for (short campo = 0; campo < cantidad_campos; ++campo) {
				string str;
				cout << nombres_campos[campo] << ": ";
				cin >> str;
				entrada.push_back(str);
			}
			elementos.push_back(entrada);
		}

		fs::path ruta_archivo = carpeta_padre / nombre_archivo;
		char separador = ' ';
		if (ruta_archivo.extension() == ".csv") {
			separador = ',';
		}
		else if (ruta_archivo.extension() == ".tsv") {
			separador = '\t';
		}

		string contenido = "";
		for (short i = 0; i < cantidad_campos; ++i) {
			contenido += nombres_campos[i];
			contenido += separador;
		}
		contenido.pop_back();
		contenido += "\n";

		elementos.for_each([&contenido, separador, cantidad_campos](vector<string>& datos) {
			for (short i = 0; i < cantidad_campos; ++i) {
				contenido += datos[i];
				contenido += separador;
			}
			contenido.pop_back();
			contenido += "\n";
			});

		Archivo* archivo = new Archivo(ruta_archivo);
		archivo->escribir(contenido);
		return archivo;
	}
};
