#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

#include "Usuario.hpp"

using namespace std;
namespace fs = std::filesystem;

class AdministradorUsuarios {
	AdministradorUsuarios() {}

	static fs::path get_ruta_config() {
		return fs::current_path() / "data" / "usuarios.txt";
	}

	static void inicializar_config() {
		fs::path config_archivo = get_ruta_config();
		if (!fs::exists(config_archivo.parent_path())) {
			fs::create_directories(config_archivo.parent_path());
		}
		fstream config(config_archivo, ios::out | ios::app);
		if (!config.is_open()) {
			cout << "No se pudo escribir al archivo de configuracion." << endl;
			exit(1);
		}
		config.close();
	}

public:
	static Usuario* crear_usuario() {
		cout << "\nCreacion de cuenta nueva" << endl;
		string usuario;
		do {
			cout << "Usuario: ";
			cin >> usuario;
		} while (!Usuario::validar_usuario(usuario));
		return crear_usuario(usuario);
	}

	static Usuario* crear_usuario(string usuario) {
		short tipo_rol = 0;
		do {
			cout << "0.Lector \n1. Comentador\n2.Editor\n";
			cout << "Rol: ";
			cin >> tipo_rol;
		} while (tipo_rol < 0 || tipo_rol > 2);

		inicializar_config();
		fstream config(get_ruta_config(), ios::out | ios::app);
		config << usuario << " " << tipo_rol << "\n";
		config.close();

		if (tipo_rol == 2) {
			fs::path carpeta_usuario = Usuario::crear_ruta_carpeta(usuario);
			if (!fs::exists(carpeta_usuario)) {
				fs::create_directories(carpeta_usuario);
			}
		}

		return new Usuario(usuario, (RolUsuario)tipo_rol);
	}

	static short obtener_rol_config(string usuario) {
		fstream config(get_ruta_config(), ios::in);
		string linea_config;
		short tipo_rol = -1;
		while (getline(config, linea_config)) {
			stringstream ss(linea_config);
			string valor;
			getline(ss, valor, ' ');
			if (valor == usuario) {
				getline(ss, valor, ' ');
				tipo_rol = stoi(valor);
				break;
			}
		}
		config.close();
		return tipo_rol;
	}

	static Usuario* iniciar_sesion() {
		cout << "\nInicio de sesion" << endl;
		string usuario;
		do {
			cout << "Usuario: ";
			cin >> usuario;
		} while (!Usuario::validar_usuario(usuario));

		short tipo_rol = obtener_rol_config(usuario);

		if (tipo_rol == -1) {
			cout << "Usuario no registrado." << endl;
			char opcion = 'n';
			do {
				cout << "\nDeseas crear tu cuenta? (S/N): ";
				cin >> opcion;
				opcion = tolower(opcion);
			} while (opcion != 's' && opcion != 'n');
			if (opcion == 'n') {
				exit(0);
			}
			return crear_usuario(usuario);
		}

		if (tipo_rol == 2) {
			fs::path carpeta_usuario = Usuario::crear_ruta_carpeta(usuario);
			if (!fs::exists(carpeta_usuario)) {
				fs::create_directories(carpeta_usuario);
			}
		}

		return new Usuario(usuario, (RolUsuario)tipo_rol);
	}
};
