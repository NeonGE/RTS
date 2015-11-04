/********************************************************************
	Created:	2014/02/13
	Filename:	geMapGridWalker.h
	Author:		Samuel Prince

	Purpose:	Declaración de la clase base geMapGridWalker
				Esta clase es utilizada como una clase base para la
				implementación de diferentes algoritmos de
				pathfinding calculados utilizando el un mapa
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Inclusión de cabeceras requeridas                                    												*/
/************************************************************************************************************************/
#include "geTiledMap.h"
#include "geMapTileNode.h"

/************************************************************************************************************************/
/* Declaración de la clase geMapGridWalker                              												*/
/************************************************************************************************************************/
class geMapGridWalker
{
	/************************************************************************************************************************/
	/* Tipo definido enumerador para reconocer los estados del iterador de pathfinding										*/
	/************************************************************************************************************************/
public:
	typedef enum WALKSTATE
	{
		STILLLOOKING,
		REACHEDGOAL,
		UNABLETOREACHGOAL
	}WALKSTATETYPE;

	/************************************************************************************************************************/
	/* Constructores y destructor virtual (es virtual para poder derivar clases a partir de esta)							*/
	/************************************************************************************************************************/
public:
	geMapGridWalker(void);
	geMapGridWalker(geTiledMap *pMap) { m_pTiledMap = pMap; }
	virtual ~geMapGridWalker(void);

	/************************************************************************************************************************/
	/* Funciones de ayuda de la clase                                      													*/
	/************************************************************************************************************************/
public:

	virtual bool Init(){return false;}									//Función de inicialización de la clase (por default en este aspecto regresamos false porque no estamos haciendo nada)
	virtual void Destroy()=0;											//Destruye los objetos de la clase
	virtual WALKSTATETYPE Update()=0;									//Actualiza el estado del pathfinding (puede hacerse por pasos o recursivamente)
	virtual void Render()=0;											//Renderea la información necesaria para su uso en pathfinding
	virtual void Reset()=0;												//Reinicializa las variables de esta clase para su uso en un nuevo cálculo

	virtual bool weightedGraphSupported(){ return false; }				//Indica si este Walker soporta la asignación y el uso de pesos a los nodos del graph
	virtual bool heuristicsSupported(){ return false; }					//Indica si este Walker soporta heuristicas

	void setTiledMap(geTiledMap *pMap){ m_pTiledMap = pMap; }			//Establecemos un puntero al mapa que será utilizado para todos los cálculos
	geTiledMap *getMapGrid() { return m_pTiledMap; }					//Regresa el puntero al mapa que se está utilizando para todos los cálculos

	void setStartPosition(const int32 x, const int32 y) {m_StartX = x; m_StartY = y;}	//Esta función se utiliza para establecer el punto inicial de búsqueda
	void setEndPosition(const int32 x, const int32 y) {m_EndX = x; m_EndY = y;}			//Esta función se utiliza para establecer el punto final de la búsqueda

	void getStartPosition(int32 & x, int32 & y){x=m_StartX; y=m_StartY;}				//Función de ayuda para obtener el punto inicial de búsqueda
	void getEndPosition(int32 & x, int32 & y){x=m_EndX; y=m_EndY;}						//Función de ayuda para obtener el punto final de búsqueda

protected:
	virtual void visitGridNode(int32 x, int32 y) = 0;					//Marca un nodo de mapa como visitado (esto lo procesa según el algoritmo utilizado)

	/************************************************************************************************************************/
	/* Declaración de variables miembro de la clase                                 										*/
	/************************************************************************************************************************/
protected:
	geTiledMap *m_pTiledMap;									//Puntero al mapa que estamos usando para calcular
	int32 m_StartX, m_StartY;									//Variables que contendrán la posición de inicio de búsqueda
	int32 m_EndX, m_EndY;										//Variables que contendrán la posición de destino a buscar
};
