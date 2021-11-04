#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <functional>
#include <vector>

#include "Lista.hpp"
#include "AdministradorUsuarios.hpp"
#include "Archivo.hpp"
#include "Menu.hpp"

using namespace std;
namespace fs = std::filesystem;

class Administrador {
	Usuario* usuario_actual;
	Lista<Usuario*> usuarios;

public:
	Administrador() {
		usuario_actual = nullptr;
		// TO DO: Leer usuarios de archivo de configuración a la lista
	}

	void menu_inicio() {
		Menu menu("\nSISTEMA DE MANEJO DE ARCHIVOS\n");
		menu.crear_opcion("Iniciar sesion", [this]() {
			usuario_actual = AdministradorUsuarios::iniciar_sesion();
			menu_usuario();
			});
		menu.crear_opcion("Crear cuenta", [this]() {
			usuario_actual = AdministradorUsuarios::crear_usuario();
			usuarios.push_back(usuario_actual);
			menu_usuario();
			});
		menu.crear_opcion("Salir", []() {
			exit(0);
			});

		while (true) {
			menu.iniciar();
		}
	}

	void menu_usuario() {
		bool salir = false;

		string cabecera = "\nSesion iniciada: " + usuario_actual->get_nombre() + "\n";
		cabecera += ("Rol: " + usuario_actual->get_rol_string());
		Menu menu(cabecera);
		if (usuario_actual->get_rol() == RolUsuario::Editor) {
			menu.crear_opcion("Crear un nuevo archivo", [this]() {
				Archivo* archivo = Archivo::crear(usuario_actual->get_carpeta());
				usuario_actual->add_archivo(archivo);
				});
			menu.crear_opcion("Buscar y abrir un archivo", [this]() {
				string file;
				do {
					cout << "\nArchivo a editar: ";
					cin >> file;
				} while (!Archivo::validar_nombre_archivo(file) || !usuario_actual->editar_archivo(file));
				});
			menu.crear_opcion("Ver todos mis archivos", [this]() {
				usuario_actual->mostrar_archivos();
				});
		}
		else if (usuario_actual->get_rol() == RolUsuario::Comentador) {
			menu.crear_opcion("Comentar un archivo", [this]() {
				string usuario, file;
				fs::path carpeta_usuario;
				do {
					cout << "\nUsuario del archivo: ";
					cin >> usuario;
					carpeta_usuario = Usuario::crear_ruta_carpeta(usuario);
				} while (!Usuario::validar_usuario(usuario) || !fs::exists(carpeta_usuario));
				do {
					cout << "\nArchivo a comentar: ";
					cin >> file;
				} while (!Archivo::validar_nombre_archivo(file) || !fs::exists(carpeta_usuario / file));

				Archivo archivo_objetivo(carpeta_usuario / file);
				cout << "\nContenido del archivo:\n\n";
				archivo_objetivo.leer([](string& linea) { cout << linea << endl; });

				cout << "\nEscribe tu comentario en el block de notas que se abrira.\n";

				Archivo archivo_comentario(fs::current_path() / "data" / "comentario.txt");
				archivo_comentario.editar();

				string comentario_procesado;
				archivo_comentario.leer([&comentario_procesado, this](string& linea) {
					comentario_procesado += "// Comentario de ";
					comentario_procesado += usuario_actual->get_nombre();
					comentario_procesado += (": " + linea + "\n");
					});
				comentario_procesado += "\n";

				archivo_objetivo.escribir(comentario_procesado);

				fs::remove(fs::current_path() / "data" / "comentario.txt");
				});
		}
		//REVISAR Y ARREGLAR
		if (usuario_actual->get_rol() == RolUsuario::Editor) {
			menu.crear_opcion("Filtracion: Igual a ", [this]() { // muestra lo que contiene un archivo a ingresar
				string nombrearch;
				do {
					cout << "\nArchivo que desee abrir: ";
					cin >> nombrearch;
				} while (!Archivo::validar_nombre_archivo(nombrearch));
				auto directory = fs::directory_iterator(usuario_actual->get_carpeta());
				for (const auto& file : directory) {
					if (file.path().filename() == nombrearch) {
						cout << "\nEl archivo: " << file.path().filename() << " contiene: " << ifstream(file.path()).rdbuf();
					}
				}
				});

			menu.crear_opcion("Filtracion: Inicia con ", [this]() { // muestra la primera palabra de cada archivo
				auto directory = fs::directory_iterator(usuario_actual->get_carpeta());
				string cadena;
				for (const auto& file : directory) {
					ifstream archivo(file.path());
					string ruta_archivo = file.path().filename().extension().string();
					char separador = ' ';
					if (ruta_archivo == ".csv") {
						separador = ',';
					}
					else if (ruta_archivo == ".tsv") {
						separador = '\t';
					}
					getline(archivo, cadena, separador);
					cout << "\nEl archivo: " << file.path().filename() << " contiene como primera palabra: \n";
					cout << cadena << "\n";
				}

				});

			menu.crear_opcion("Filtracion: Contenido de todos los archivos", [this]() { //muestra cada archivo y lo que contiene
				auto directory = fs::directory_iterator(usuario_actual->get_carpeta());
				for (const auto& file : directory) {
					cout << "\nEl archivo: " << file.path().filename() << " contiene: " << ifstream(file.path()).rdbuf();
				}
				});

			menu.crear_opcion("Filtracion: Contenido en Archivos omitiendo uno a escoger ", [this]() { // muestra lo que contiene todos los archivos excepto 1 a ingresar
				string nombrearch;
				do {
					cout << "\nArchivo que desee omitir: ";
					cin >> nombrearch;
				} while (!Archivo::validar_nombre_archivo(nombrearch));
				auto directory = fs::directory_iterator(usuario_actual->get_carpeta());
				for (const auto& file : directory) {
					if (!(file.path().filename() == nombrearch)) {
						cout << "\nEl archivo: " << file.path().filename() << " contiene: " << ifstream(file.path()).rdbuf();
					}
				}
				});
		}
		menu.crear_opcion("Cerrar sesion", [this, &salir]() {
			delete usuario_actual;
			usuario_actual = nullptr;
			salir = true;
			});

		while (!salir) {
			menu.iniciar();
		}
	}
};
