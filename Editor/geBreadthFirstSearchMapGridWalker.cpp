/********************************************************************
	Created:	2014/02/17
	Filename:	geBreadthFirstSearchMapGridWalker.cpp
	Author:		Samuel Prince

	Purpose:	Clase geBreadthFirstSearchMapGridWalker
				Para cálculo de Pathfinding usando el algoritmo BFS
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de los archivos de cabecera necesarios para la compilación 												*/
/************************************************************************************************************************/
#include "stdafx.h"
#include "geBreadthFirstSearchMapGridWalker.h"

/************************************************************************************************************************/
/* Implementación de funciones de la clase                              												*/
/************************************************************************************************************************/
geBreadthFirstSearchMapGridWalker::geBreadthFirstSearchMapGridWalker(void)
{//Constructor standard
	//Limpiamos variables miembro
	m_n = NULL;
	m_nodegrid = NULL;
	m_StartX = m_StartY = 0;
	m_EndX = m_EndY = 0;
}

geBreadthFirstSearchMapGridWalker::geBreadthFirstSearchMapGridWalker(geTiledMap *pMap) : geMapGridWalker(pMap) 
{//Constructor con parámetros (overrided)
	//Limpiamos variables miembro
	m_n = NULL;
	m_nodegrid = NULL;
	m_StartX = m_StartY = 0;
	m_EndX = m_EndY = 0;
}

geBreadthFirstSearchMapGridWalker::~geBreadthFirstSearchMapGridWalker(void)
{//Destructor
	Destroy();
}

bool geBreadthFirstSearchMapGridWalker::Init()
{//Inicializa los objetos para su uso
	//Revisamos que no estén alojados ya los nodos
	if( m_nodegrid != NULL )
	{
		Destroy();
	}

	/*
		NOTA: En esta sección estamos creando un nodo por cada tile en el mapa
		Esta sección puede ser optimizada para su uso de memoria de la siguientes maneras:
		1.- Limitando el número de pasos y creando solo el número de objetos que requeriremos para eso
		2.- Manteniendo una matriz estática reducida para la información utilizada
		3.- Reducir el tipo de variable a un uint16 para las coordenadas
		4.- Reducir la matriz de visitados utilizando máscaras de bits
	*/

	//Alojamos nuevos nodos para el uso del algoritmo
	m_nodegrid = GEE_NEW geMapTileNode*[m_pTiledMap->getMapSize()];
	for(int32 i=0; i<m_pTiledMap->getMapSize(); i++)
	{
		m_nodegrid[i] = GEE_NEW geMapTileNode[m_pTiledMap->getMapSize()];
		for(int32 j=0; j<m_pTiledMap->getMapSize(); j++)
		{
			//Establecemos posiciones y estado de no visitado a todos los nodos
			m_nodegrid[i][j].setVisited(false);
			m_nodegrid[i][j].m_x = i;
			m_nodegrid[i][j].m_y = j;
		}
	}

	return true;	//Si llegamos a este punto, todo salió bien
}

void geBreadthFirstSearchMapGridWalker::Destroy()
{
	//Destruimos los nodos de la matriz bidimensional
	if(m_nodegrid != NULL)
	{
		for(int32 i = 0; i < m_pTiledMap->getMapSize(); i++)
		{
			GEE_DELETE_ARRAY(m_nodegrid[i]);
		}
		GEE_DELETE_ARRAY(m_nodegrid);
	}

	//Limpiamos punteros a los nodos
	m_nodegrid = NULL;
	m_n = NULL;
}

void geBreadthFirstSearchMapGridWalker::Render()
{//Función utilizada para renderear información del nodo en pantalla

}

geMapGridWalker::WALKSTATETYPE geBreadthFirstSearchMapGridWalker::Update()
{//Función de actualización del algoritmo (calcula un paso a la vez del algoritmo)
	//Revisamos si hay objetos en la lista abierta
	if(m_open.size() > 0)
	{//Hay objetos, por lo que podemos seguir calculando una ruta
		m_n = (geMapTileNode*)m_open.front();	//Obtenemos el nodo actual para chequeos
		m_open.pop();							//Sacamos este objeto de la lista abierta
		m_n->setVisited(true);					//Marcamos este nodo como visitado

		//Revisamos si el nodo está en la posición del objetivo
		if(m_n->Equals(*m_end))
		{//Este es el objetivo
			return REACHEDGOAL;	//Indicamos que hemos llegado a la ruta pedida
		}

		//Creamos variables temporales para almacenar la dirección de chequeo
		int32 x, y;

		//Agregamos todos los nodos adyacentes a este
		
		//Agregamos el nodo ESTE
		x = m_n->m_x+1;
		y = m_n->m_y;
		if(m_n->m_x < (m_pTiledMap->getMapSize() - 1))
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo SUD-ESTE
		x = m_n->m_x+1;
		y = m_n->m_y+1;
		if(m_n->m_x < (m_pTiledMap->getMapSize() - 1) && m_n->m_y < (m_pTiledMap->getMapSize() - 1))
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo SUR
		x = m_n->m_x;
		y = m_n->m_y+1;
		if(m_n->m_y < (m_pTiledMap->getMapSize() - 1))
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo SUD-OESTE
		x = m_n->m_x-1;
		y = m_n->m_y+1;
		if(m_n->m_y < (m_pTiledMap->getMapSize() - 1) && m_n->m_x > 0)
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo OUSTE
		x = m_n->m_x-1;
		y = m_n->m_y;
		if(m_n->m_x > 0)
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo NOR-OESTE
		x = m_n->m_x-1;
		y = m_n->m_y-1;
		if(m_n->m_x > 0 && m_n->m_y > 0)
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agregamos el nodo NORTE
		x = m_n->m_x;
		y = m_n->m_y-1;
		if(m_n->m_y > 0)
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		//Agragamos el nodo NOR-ESTE
		x = m_n->m_x+1;
		y = m_n->m_y-1;
		if(m_n->m_y > 0 && m_n->m_x < (m_pTiledMap->getMapSize() - 1))
		{//Si no nos hemos salido del rango del mapa
			visitGridNode(x, y);	//Visitamos el nodo
		}

		return STILLLOOKING;	//Indicamos que aún estamos buscando el objetivo
	}

	return UNABLETOREACHGOAL;	//Si llegamos a este punto indicamos que no es posible encontrar una ruta al objetivo
}

void geBreadthFirstSearchMapGridWalker::visitGridNode(int32 x, int32 y)
{//Esta función "visita" un nodo, esto es para saber si debe agregarse a la lista abierta para su chequeo en el futuro
	// if the node is blocked or has been visited, early out
	if( m_pTiledMap->getCost(x, y) == TILENODE_BLOCKED || m_nodegrid[x][y].getVisited() )
	{//Si este nodo está bloqueado o ya fue visitado
		return;	//Regresamos sin hacer nada
	}

	//Marcamos este nodo como visitable agregándolo a la lista abierta
	m_open.push( &m_nodegrid[x][y] );

	//Tambien marcamos que el nodo en chequeo actual es el padre de este nodo
	m_nodegrid[x][y].m_parent = m_n;
}

void geBreadthFirstSearchMapGridWalker::Reset()
{//Reinicializa la clase para utilizarla otra vez
	//Vaciamos el Queue
	while(m_open.size() > 0)
	{
		m_open.pop();
	}

	//Establecemos que no hay un nodo actual en chequeo
	m_n = NULL;

	//Revisamos que los nodos ya hayan sido creado (Solo en modo Debug)
	GEE_ASSERT( m_nodegrid );

	//Para este punto los nodos ya están creados, solo limpiamos la bandera de visitado a false en todos
	for(int32 i=0; i<m_pTiledMap->getMapSize(); i++)
	{
		for(int j=0; j<m_pTiledMap->getMapSize(); j++)
		{
			m_nodegrid[i][j].setVisited(false);
		}
	}

	//Obtenemos el punto de inicio, lo marcamos como visitado y lo establecemos como el nodo inicial
	int x, y;
	getStartPosition(x, y);
	m_start = &m_nodegrid[x][y];
	m_start->setVisited(true);

	//Obtenemos el punto final, obtenemos el nodo y lo marcamos como el nodo final
	getEndPosition(x, y);
	m_end = &m_nodegrid[x][y];

	//Agregamos el nodo inicial a la lista abierta
	m_open.push(m_start);
}

