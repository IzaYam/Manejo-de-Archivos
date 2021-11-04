#pragma once

#include <iostream>

using namespace std;

template <typename T>
struct Nodo {
	T elem;
	Nodo<T>* ant;
	Nodo<T>* sig;
	Nodo(T elemento) : elem(elemento), sig(nullptr), ant(nullptr) {}
};

template <class T>
class Lista {
	struct Contenedor {
		T elem;
		Contenedor(T elemento) : elem(elemento) {}
	};

	Nodo<T>* inicio;
	Nodo<T>* fin;
	size_t cantidad;
public:
	Lista() {
		inicio = fin = nullptr;
		cantidad = 0;
	}

	~Lista() {
		clear();
	}

	size_t size() {
		return cantidad;
	}

	void clear() {
		while (inicio != nullptr) {
			Nodo<T>* aux = inicio;
			inicio = inicio->sig;
			delete aux;
		}
		fin = nullptr;
		cantidad = 0;
	}

	Contenedor* find(function<bool(T&)> comparador) {
		Nodo<T>* aux = inicio;
		while (aux != nullptr) {
			if (comparador(aux->elem)) {
				return new Contenedor(aux->elem);
			}
			aux = aux->sig;
		}
		return nullptr;
	}

	void push_front(T valor) {
		Nodo<T>* nuevo = new Nodo<T>(valor);
		if (cantidad == 0) {
			fin = nuevo;
		}
		else {
			nuevo->sig = inicio;
			inicio->ant = nuevo;
		}
		inicio = nuevo;
		++cantidad;
	}

	void push_back(T valor) {
		Nodo<T>* nuevo = new Nodo<T>(valor);
		if (cantidad == 0) {
			inicio = nuevo;
		}
		else {
			nuevo->ant = fin;
			fin->sig = nuevo;
		}
		fin = nuevo;
		++cantidad;
	}

	void for_each(function<void(T&)> fun) {
		Nodo<T>* aux = inicio;
		while (aux != nullptr) {
			fun(aux->elem);
			aux = aux->sig;
		}
	}

	size_t buscar(T valor) {
		Nodo<T>* aux = inicio;
		size_t pos = 0;
		while (aux != nullptr) {
			if (aux->elem == valor) {
				return pos + 1;
			}
			aux = aux->sig;
			++pos;
		}
	}

	T valor_por_posicion(size_t p) {
		Nodo<T>* aux = inicio;
		size_t pos = 0;
		if (pos > cantidad && pos < 0) {
			while (pos < p) {
				aux = aux->sig;
				++pos;
			}
			return aux->elem;
		}
		exit(1);
	}
};
