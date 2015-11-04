/********************************************************************
	Created:	2014/02/03
	Filename:	geWorld.h
	Author:		Samuel Prince

	Purpose:	Esta clase contiene los datos de Mundo para el juego
				Aquí se almacenan todos los datos de una escena
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase geWorld                                      												*/
/************************************************************************************************************************/
class geWorld
{
	/************************************************************************************************************************/
	/* Definición de variables miembro                                      												*/
	/************************************************************************************************************************/
private:
	//Variables de Render
	SDL_Renderer *m_pRenderer;						//Renderer asociado al mundo
	
	//Variables de Mapa
	geTiledMap *m_pTiledMap;						//Mapa asociado al mundo

	//Variables de Pathfinding
	std::vector<geMapGridWalker*> m_walkersList;	//Lista de Walkers capaces de hacer pathfinding (cada uno tiene un algoritmo diferente)
	geMapGridWalker *m_activeWalker;				//Puntero al Walker activo (que algoritmo estamos usando para pathfinding)
	int8 m_activeWalkerIndex;						//Indice en la lista perteneciente al algoritmo utilizado en este momento
	
	uint16 m_AppResolutionX;						//Resolución de la aplicación en X
	uint16 m_AppResolutionY;						//Resolución de la aplicación en Y

	/************************************************************************************************************************/
	/* Definición de Constructores y Destructores de la clase               												*/
	/************************************************************************************************************************/
public:
	geWorld(void);		//Constructor Standard
	~geWorld(void);		//Destructor

	/************************************************************************************************************************/
	/* Definición de funciones miembro                                      												*/
	/************************************************************************************************************************/
public:
	bool Init(SDL_Renderer* pRenderer);		//Inicialización de la clase
	void Destroy();							//Destrucción de objetos de la clase
	void Update(float deltaTime);			//Actualización
	void Render();							//Impresión de objetos

	/************************************************************************************************************************/
	/* Accesores                                                            												*/
	/************************************************************************************************************************/
	geTiledMap* getTiledMap(){ return m_pTiledMap; }

	/************************************************************************************************************************/
	/* Funciones de control de resolución de la aplicación																	*/
	/************************************************************************************************************************/
	void UpdateResolutionData();			//Esta función avisa de un cambio de resolución a los objetos de mundo dependientes de ella (e.j. UI)
	void setResolution(const uint16 w, const uint16 h) { m_AppResolutionX = w; m_AppResolutionY = h; UpdateResolutionData(); }
	uint16 getResolutionWidth() { return m_AppResolutionX; }
	uint16 getResolutionHeight() { return m_AppResolutionY; }

	/************************************************************************************************************************/
	/* Funciones para pathfinding                                           												*/
	/************************************************************************************************************************/
	void setCurrentWalker(const int8 index);		//Establece el Walker (algoritmo de pathfinding) a utilizar
};

