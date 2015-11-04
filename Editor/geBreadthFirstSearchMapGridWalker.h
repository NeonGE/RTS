/********************************************************************
	Created:	2014/02/14
	Filename:	geBreadthFirstSearchMapGridWalker.h
	Author:		Samuel Prince

	Purpose:	Declaración de la clase utilizada para hacer
				pathfinding a partir del algorimo Breadth First
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Incluimos cabeceras de los archivos necesarios                       												*/
/************************************************************************************************************************/
#include "geMapGridWalker.h"
#include "geMapTileNode.h"
#include <queue>

class geBreadthFirstSearchMapGridWalker : public geMapGridWalker
{
	/************************************************************************************************************************/
	/* Declaración de constructores y destructor virtual                    												*/
	/************************************************************************************************************************/
public:
	geBreadthFirstSearchMapGridWalker(void);				//Constructor standard
	geBreadthFirstSearchMapGridWalker(geTiledMap *pMap);	//Constructor con parámetro del mapa que utilizaremos para calcular
	virtual ~geBreadthFirstSearchMapGridWalker(void);		//Destructor virtual

	/************************************************************************************************************************/
	/* Funciones de ayuda de la clase                                      													*/
	/************************************************************************************************************************/
public:
	virtual bool Init();										//Función de inicialización de la clase (crea los objetos para su uso posterior)
	virtual void Destroy();										//Destruye los objetos de la clase (despues de llamarlo, las llamadas a otras funciones son inválidas)
	virtual WALKSTATETYPE Update();								//Actualiza el estado del pathfinding (puede hacerse por pasos o recursivamente)
	virtual void Render();										//Renderea la información necesaria para su uso en pathfinding
	virtual void Reset();										//Reinicializa las variables de esta clase para su uso en un nuevo cálculo

	virtual bool weightedGraphSupported(){ return false; }		//Indica si este Walker soporta la asignación y el uso de pesos a los nodos del graph

protected:
	virtual void visitGridNode(int32 x, int32 y);				//Marca un nodo de mapa como visitado (esto lo procesa según el algoritmo utilizado)

	/************************************************************************************************************************/
	/* Declaración de variables miembro exclusivas de esta clase            												*/
	/************************************************************************************************************************/
private:
	std::queue<geMapTileNode*> m_open;			//Nuestra lista abierta utilizando un queue ordinario
	geMapTileNode *m_start, *m_n, *m_end;		//Punteros a los nodos de inicio, uso y final
	geMapTileNode **m_nodegrid;					//Matriz para almacenamiento de los nodos del mapa
};
