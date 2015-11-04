/********************************************************************
	Created:	2014/02/03
	Filename:	geTiledMap.h
	Author:		Samuel Prince

	Purpose:	Esta clase contiene todo lo referente al mapa de tiles
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Definición de constantes                                             												*/
/************************************************************************************************************************/
//La siguiente línea se utiliza para marcar que estamos haciendo cálculos y renders isométricos
#define MAP_IS_ISOMETRIC

//Tamaño del tile en pixeles
#ifdef MAP_IS_ISOMETRIC
	#define TILESIZE_X 64
	#define TILESIZE_Y 32
	#define TILEHALFSIZE_X 32
	#define TILEHALFSIZE_Y 16

	//Para optimizar las operaciones con tamaños potencia de 2, definimos constantes para hacer Bit Shifts
	#define BITSFT_TILESIZE_X 6
	#define BITSFT_TILESIZE_Y 5
	#define BITSFT_TILEHALFSIZE_X 5
	#define BITSFT_TILEHALFSIZE_Y 4
#else
	//Definición de tiles en un mapa cuadrado
	#define TILESIZE_X 64
	#define TILESIZE_Y 64
	#define TILEHALFSIZE_X 32
	#define TILEHALFSIZE_Y 32

	//Para optimizar las operaciones con tamaños potencia de 2, definimos constantes para hacer Bit Shifts
	#define BITSFT_TILESIZE_X 6
	#define BITSFT_TILESIZE_Y 6
	#define BITSFT_TILEHALFSIZE_X 5
	#define BITSFT_TILEHALFSIZE_Y 5
#endif

//Enumerador para los tipos de terreno
enum eTerrainTypes
{
	TT_WATER = 0,
	TT_WALKABLE,
	TT_MARSH,
	TT_OBSTACLE,
	TT_NUM_OBJECTS
};

/************************************************************************************************************************/
/* Declaración de la clase geTiledMap                                      												*/
/************************************************************************************************************************/
class geTiledMap
{
	/************************************************************************************************************************/
	/* Definición de una clase para el manejo de tiles                         												*/
	/************************************************************************************************************************/
	class MapTile
	{
	public:
		/************************************************************************************************************************/
		/* Declaración de constructores y destructor		                       												*/
		/************************************************************************************************************************/
		MapTile(void);									//Constructor standard
		MapTile(const int8 idType, const int8 cost);		//Constructor con parámetro de costo
		MapTile(const MapTile& copy);					//Constructor copia
		
		/************************************************************************************************************************/
		/* Declaración de operadores                                            												*/
		/************************************************************************************************************************/
		MapTile& operator=(const MapTile& rhs);

		/************************************************************************************************************************/
		/* Declaración de funciones de acceso                                   												*/
		/************************************************************************************************************************/
		FORCEINLINE uint8 getType() const { return m_idType; }
		void setType(const int8 idType) { m_idType = idType; }

		FORCEINLINE int8 getCost() const { return m_cost; }
		void setCost(const int8 cost) { m_cost = cost; }

		/************************************************************************************************************************/
		/* Declaración de variables miembro de la clase                         												*/
		/************************************************************************************************************************/
	private:
		uint8 m_idType;		//Id del tipo de tile que se est
		int8 m_cost;		//Costo de este tile (De momento estamos usando 8 bits pero bien puede ser cualquier tipo de entero)
	};

	/************************************************************************************************************************/
	/* Definición de Constructores y Destructores de la clase               												*/
	/************************************************************************************************************************/
public:
	geTiledMap(void);											//Constructor standard
	geTiledMap(SDL_Renderer* pRenderer, const int32 mapSize);	//Constructor con parámetros
	~geTiledMap(void);											//Destructor

	/************************************************************************************************************************/
	/* Definición de funciones miembro                                      												*/
	/************************************************************************************************************************/
public:
	//Inicialización y destrucción
	bool Init(SDL_Renderer* pRenderer, const int32 mapSize);
	void Destroy();

	//Actualización de lógica y render
	void Update(float deltaTime);
	void Render();

	int32 getMapSize() const { return m_mapSize; }
	
	//Funciones de carga y salvado del mapa
	bool LoadFromImageFile(SDL_Renderer* pRenderer, geString fileName);
	bool SaveToImageFile(SDL_Renderer* pRenderer, geString fileName);

	int8 getCost(const int32 x, const int32 y) const;
	void setCost(const int32 x, const int32 y, const int8 cost);

	int8 getType(const int32 x, const int y) const;
	void setType(const int32 x, const int32 y, const uint8 idtype);

	void setStart(const uint16 x, const uint16 y) { m_startX = x; m_startY = y; PreCalc(); }
	void getStart(uint16 &x, uint16 &y) const { x = m_startX; y = m_startY; }

	void setEnd(const uint16 x, const uint16 y) { m_endX = x; m_endY = y; PreCalc(); }
	void getEnd(uint16 &x, uint16 &y) const { x = m_endX; y = m_endY; }

	void PreCalc()
	{//Esta función recalcula los datos que requerimos sean precalculados y que se utilizan muy frecuentemente en varias de las funciones
		m_PreCalc_MidResolutionX = (uint16)DivX2(m_endX-m_startX);
		m_PreCalc_MidResolutionY = (uint16)DivX2(m_endY-m_startY);
#ifdef MAP_IS_ISOMETRIC	//Estos cálculos aplican solo en el caso de un mapa isométrico
		m_PreCalc_MaxCameraCoordX = DivX2(m_mapSize*TILESIZE_X);
#else	//Estos cálculos aplican solo en el caso de un mapa cuadrado
		m_PreCalc_MaxCameraCoordX = m_mapSize*TILESIZE_X;
#endif
		m_PreCalc_MaxCameraCoordY = m_mapSize*TILESIZE_Y;
	}

	void moveCamera(const float dx, const float dy);	//Desplaza la posición de la cámara (defazamientos flotantes para poder manejar ligeros defazamientos y manejar una velocidad independiente al framerate)

	void setCameraStartPosition(const int32 x, const int32 y);
	void getCameraStartPosition(int32 &x, int32 &y) const { x = m_iCameraX; y = m_iCameraY; }

	void getScreenToMapCoords(const int32 scrX, const int32 scrY, int32 &mapX, int32 &mapY);	//Convierte coordenadas de pantalla a coordenadas de mapa
	void getMapToScreenCoords(const int32 mapX, const int32 mapY, int32 &scrX, int32 &scrY);	//Convierte coordenadas de mapa a coordenadas de pantalla
	/************************************************************************************************************************/
	/* Definición de variables miembro                                      												*/
	/************************************************************************************************************************/
private:
	//Datos del mapa
	MapTile **m_mapGrid;				//Información del mapa
	int32 m_mapSize;					//Tamaño del mapa

	//Variables de control de cámara
	int32 m_iCameraX, m_iCameraY;		//Punto inicial de la cámara (en pixeles, no en tiles)
	float m_fCameraX, m_fCameraY;		//Punto inicial de la cámara (en pixeles, no en tiles)

	//Datos de información de área de impresión
	uint16 m_startX, m_startY;			//Punto de inicio en pantalla (esto establece el rectángulo donde se debe imprimir el mapa)
	uint16 m_endX, m_endY;				//Punto final en pantalla (esto establece el rectángulo donde se debe imprimir el mapa)

	//Datos precalculados (útiles para evitar calcular varias cosas al vuelo cada vez)
	uint16 m_PreCalc_MidResolutionX;		//La mitad de la resolución del área de impresión precalculada
	uint16 m_PreCalc_MidResolutionY;		//La mitad de la resolución del área de impresión precalculada
	int32 m_PreCalc_MaxCameraCoordX;		//Indica cual es la máxima coordenada a la que puede llegar la cámara
	int32 m_PreCalc_MaxCameraCoordY;		//Indica cual es la máxima coordenada a la que puede llegar la cámara
	int32 m_PreCalc_ScreenDefaceX;			//Defazamiento de pantalla en coordenadas de pixeles calculada para el eje X (Estas se actualizan cada vez que se mueve la cámara o cambia)
	int32 m_PreCalc_ScreenDefaceY;			//Defazamiento de pantalla en coordenadas de pixeles calculada para el eje Y

	//Variables de configuración al vuelo
	bool m_bShowGrid;					//Bandera que indica si se debe o no mostrar el grid del mapa

	//Variables para rendereo
	SDL_Renderer *m_pRenderer;			//Renderer asociado con este objeto
	geTexture *m_mapTextures;			//Texturas de mapas (de momento una textura de 512x512 para cada tipo, que se utilizará en pedazos de 32x32)
};
