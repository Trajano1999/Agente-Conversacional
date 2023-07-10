#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

/*
	No se si hacer privados los métodos actualizarObjetos y calcularCoste
	No olvidar los ---
	No se si tener en cuenta las casillas de Recarga en Costo Uniforme
*/

// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar
Action ComportamientoJugador::think(Sensores sensores){
	
	actual.fila        = sensores.posF;
	actual.columna     = sensores.posC;
	actual.orientacion = sensores.sentido;

	destino.fila       = sensores.destinoF;
	destino.columna    = sensores.destinoC;

	Action accion = actIDLE;

	//Calculamos el camino hasta el destino si no tenemos aun un plan
	if(!hayPlan)
		hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
	
	//---if(actual.fila == destino.fila and actual.columna == destino.columna)
	//	hayPlan = false;

	if(hayPlan and plan.size() > 0){			// Hay un plan no vacio
		accion = plan.front();					// tomamos la siguiente accion del hayPlan
		plan.erase(plan.begin());				// eliminamos la accion de la lista de acciones
	}else{
		// Aqui solo entra cuando no es posible encontrar un comportamiento o esta mal implementado el metodo de busqueda
		cerr << "\nSe ha producido algún error" << endl;
		hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
	}

	return accion;
}

// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			return pathFinding_Profundidad(origen,destino,plan);
			break;
		case 2: cout << "Busqueda en Anchura\n";
			return busquedaAnchura(origen,destino,plan);
			break;
		case 3: cout << "Busqueda Costo Uniforme\n";
			return costoUniforme(origen,destino,plan);
			break;
		case 4: cout << "Busqueda para el reto\n";
			return busquedaNivel2(origen,destino,plan);
			break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}

// Actualiza el mapa para ir mostrando lo que vamos descubriendo
void ComportamientoJugador::actualizarMapa(Sensores sensores){
	
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                				Funciones		   	 		                  |
// |______________________________________________________________________________|

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M')
		return true;
	else
	  	return false;
}

// Anula una matriz, esto es, la iguala a 0
void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                		  Busqueda en Profundidad	     	                  |
// |______________________________________________________________________________|

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};

// Implementación de la búsqueda en profundidad
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan){
	
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados		
	stack<nodo> pila;					  // Lista de Abiertos		

  	nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  	while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "\nNo encontrado plan\n";
	}

	return false;
}

int ComportamientoJugador::interact(Action accion, int valor){
	return false;
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                			Busqueda en Anchura	   	 		                  |
// |______________________________________________________________________________|

// Implementación de la búsqueda en anchura
bool ComportamientoJugador::busquedaAnchura(const estado &origen, const estado &destino, list<Action> &plan){
	cout << "\nCalculando plan...\n";
	plan.clear();
	
	set<estado,ComparaEstados> generados;		// Lista de Cerrados		
	queue <nodo> cola;							// Lista de Abiertos		
 	
	nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

	while (!cola.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna)){
		
		cola.pop();
		generados.insert(current.st);
		
		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			cola.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la cola
		if (!cola.empty()){
			current = cola.front();
		}
	}
	
	cout << "Busqueda terminada!\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan...\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		VisualizaPlan(origen, plan);	// Ver el plan en el mapa
		return true;
	}else
		cout << "\nNo se ha encontrado plan :(\n";

	return false;
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                		 Busqueda de Costo Uniforme   		                  |
// |______________________________________________________________________________|

// Implementación del criterio de menor coste
struct CriterioDeOrden{
	bool operator ()(const nodo &a, const nodo &b) const{
		if (a.coste < b.coste)
			return true;
		else{
			if(a.coste == b.coste)
				ComparaEstados()(a.st,b.st);
			else
				return false;
		}
	}
};

// Implementación de la búsqueda de costo uniforme
bool ComportamientoJugador::costoUniforme(const estado &origen, const estado &destino, list<Action> &plan){
	cout << "\nCalculando plan...\n";
	plan.clear();
	
	set<estado, ComparaEstados> cerrados;			// Lista de Cerrados		// Estados por los que he pasado---
	set<nodo, CriterioDeOrden> 	abiertos;			// Lista de Abiertos		// Nodos que me quedan por examinar---

	nodo current;
	current.st = origen;
	current.secuencia.empty();
	current.coste = 0;

	abiertos.insert(current);

	while (!abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna)){
		
		cerrados.insert(current.st);
		abiertos.erase(current);
		
		actualizarObjetos(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;				// Modifico la orientación
		hijoTurnR.coste += calcularCoste(current.st);							// Incremento el coste del nodo por haber cambiado de estado
		//--- if(hijoTurnR.coste < 0) hijoTurnR.coste = 0;					    // El coste no puede ser negativo
		if (cerrados.find(hijoTurnR.st) == cerrados.end()){						// Si no he pasado por hijoTurnR aún
			hijoTurnR.secuencia.push_back(actTURN_R);		
			abiertos.insert(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		hijoTurnL.coste += calcularCoste(current.st);
		//--- if(hijoTurnL.coste < 0) hijoTurnL.coste = 0;
		if (cerrados.find(hijoTurnL.st) == cerrados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			abiertos.insert(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		hijoForward.coste += calcularCoste(current.st);
		if (!HayObstaculoDelante(hijoForward.st)){
			//--- if(hijoForward.coste < 0) hijoForward.coste = 0;
			if (cerrados.find(hijoForward.st) == cerrados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				abiertos.insert(hijoForward);
			}
		}

		// Tomo el siguiente valor de la cola
		if (!abiertos.empty()){
			current = *(abiertos.begin());										// current toma el valor del primer elemento del set
		}
	}

	cout << "Busqueda terminada!\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan...\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		VisualizaPlan(origen, plan);		// ver el plan en el mapa
		return true;
	}
	else {
		cout << "\nNo se ha encontrado plan :(\n";
	}

	return false;	
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                 		 Busqueda del Nivel 2		      		              |
// |______________________________________________________________________________|

bool ComportamientoJugador::busquedaNivel2(const estado& origen, const estado& destino, list<Action> &plan){
	costoUniforme(origen,destino,plan);		//---
	return true;
}

//  ______________________________________________________________________________ 
// |                                                                              |
// |                  Otros Métodos de ComportamientoJugador 		              |
// |______________________________________________________________________________|

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  	// calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}

// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

// Activa los atributos zapatillas y bikini si pasamos por una casilla de este tipo
void ComportamientoJugador::actualizarObjetos(const estado& st){
	int x = st.fila;
	int y = st.columna;

	if(mapaResultado[x][y] == 'D')
		zapatillas = true;
	if(mapaResultado[x][y] == 'K')
		bikini = true;
}

// Indica el coste que tiene cambiar del estado st a otro
int ComportamientoJugador::calcularCoste(const estado& st){
	int x = st.fila,
	    y = st.columna,
		res = 0;

	if(mapaResultado[x][y] == 'T')
		res = 2;
	else
		res = 1;
	
	if(mapaResultado[x][y] == 'A'){
		if(bikini)
			res = 10;
		else
			res = 100;
	}
	
	if(mapaResultado[x][y] == 'B'){
		if(zapatillas)
			res = 5;
		else
			res = 50;
	}
	
	return res;
}
