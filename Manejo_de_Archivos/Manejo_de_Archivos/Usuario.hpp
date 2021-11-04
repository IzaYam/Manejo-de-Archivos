#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <regex>

#include "Lista.hpp"
#include "Archivo.hpp"
#include "Validadores.hpp"

using namespace std;
namespace fs = std::filesystem;

enum class RolUsuario {
	Lector, Comentador, Editor
};

class Usuario {
	string usuario;
	fs::path ruta_carpeta;
	RolUsuario rol;
	Lista<Archivo*> archivos;

public:
	Usuario(string usuario, RolUsuario rol = RolUsuario::Lector)
		: usuario(usuario), rol(rol) {
		ruta_carpeta = crear_ruta_carpeta(usuario);
		leer_carpeta();
	}

	void leer_carpeta() {
		if (rol != RolUsuario::Editor) return;
		archivos.clear();
		for (const auto& file : fs::directory_iterator(ruta_carpeta)) {
			Archivo* archivo = new Archivo(file.path());
			archivos.push_back(archivo);
		}
	}

	void add_archivo(Archivo* archivo) {
		if (rol != RolUsuario::Editor) return;
		archivos.push_back(archivo);
	}

	bool editar_archivo(string nombre_archivo) {
		if (rol != RolUsuario::Editor) return false;
		auto contenedor = archivos.find([nombre_archivo](Archivo* archivo) {
			return archivo->get_ruta().filename().string() == nombre_archivo;
			});
		if (contenedor == nullptr) {
			cout << "No se encontro el archivo." << endl;
			return false;
		}
		auto archivo = contenedor->elem;
		archivo->editar();
		return true;
	}

	void mostrar_archivos() {
		if (rol != RolUsuario::Editor) return;
		if (archivos.size() == 0) {
			cout << endl << "No cuentas con ningun archivo en tu carpeta." << endl;
			return;
		}

		cout << endl << "Listado de archivos de " << usuario << ":" << endl;
		archivos.for_each([](Archivo* archivo) {
			string nombre = archivo->get_ruta().filename().string();
			cout << nombre << endl;
			});
		cout << endl;
	}

	string get_nombre() {
		return usuario;
	}

	fs::path get_carpeta() {
		return ruta_carpeta;
	}

	RolUsuario get_rol() {
		return rol;
	}

	string get_rol_string() {
		return rol_to_string(rol);
	}

	static string rol_to_string(RolUsuario rol) {
		switch (rol) {
		case RolUsuario::Lector:
			return "Lector";
			break;
		case RolUsuario::Comentador:
			return "Comentador";
			break;
		case RolUsuario::Editor:
			return "Editor";
			break;
		default:
			exit(1);
		}
	}

	static bool validar_usuario(string usuario) {
		return std::regex_match(usuario, Validador::Usuario);
	}

	static fs::path crear_ruta_carpeta(string usuario) {
		return fs::current_path() / "data" / usuario;
	}
};
