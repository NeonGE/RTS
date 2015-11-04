/********************************************************************
	Created:	2014/02/03
	Filename:	geTiledMap.cpp
	Author:		Samuel Prince

	Purpose:	Implementación de las funciones de geTiledMap
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de cabeceras necesarias para la compilación                												*/
/************************************************************************************************************************/
#include "stdafx.h"
#include "geTiledMap.h"

/************************************************************************************************************************/
/* Implementación de funciones de la clase geTiledMap                              										*/
/************************************************************************************************************************/
geTiledMap::geTiledMap(void)
{//Constructor Standard
	//Limpiamos las variables miembro
	m_mapGrid = NULL;
	m_mapSize = 0;
	m_startX = m_startY = 0;
	m_endX = m_endY = 0;
	m_iCameraX = m_iCameraY = 0;
	m_fCameraX = m_fCameraY = 0.f;

	//Aún no creamos ninguna textura para el mapa
	m_pRenderer = NULL;
	m_mapTextures = NULL;
	m_bShowGrid = false;
}

geTiledMap::geTiledMap(SDL_Renderer* pRenderer, const int32 mapSize)
{
	m_mapGrid = NULL;
	m_mapTextures = NULL;

	Init(pRenderer, mapSize);
}

geTiledMap::~geTiledMap(void)
{//Destructor
	Destroy();
}

/************************************************************************************************************************/
/* Esta función aloja la memoria necesaria para el mapa y carga las texturas para el render								*/
/*																														*/
/* Parametros:																											*/
/*     mapSize: Indica el tamaño del mapa (se creará un mapa cuadrado e.j. maxSize*maxSize )							*/
/************************************************************************************************************************/
bool geTiledMap::Init(SDL_Renderer* pRenderer, const int32 mapSize)
{
	//Revisamos que esta función pueda ser llamada
	if( m_mapGrid != NULL )
	{//Este mapa ya fue creado anteriormente, destruimos los datos actuales y reinicializamos
		Destroy();
	}

	//Variables temporales
	geString textureName;	//Temporal útilizada para indicar nombres de texturas

	//Copiamos localmente el puntero del renderer
	GEE_ASSERT(pRenderer);
	m_pRenderer = pRenderer;

	//Creamos el grid del mapa según el tamaño indicado
	//Lo creamos como una matriz accesible en dos dimensiones PERO EVITAMOS CREARLA COMO UNA MATRIZ DE DOS DIMENSIONES
	m_mapGrid = GEE_NEW MapTile*[mapSize];
	GEE_ASSERT(m_mapGrid);

	for( int32 i=0 ; i<mapSize; ++i )
	{
		m_mapGrid[i] = NULL;
		m_mapGrid[i] = GEE_NEW MapTile[mapSize];
		GEE_ASSERT(m_mapGrid[i]);
	}

	//Copiamos localmente el tamaño del mapa para futuras referencias
	m_mapSize = mapSize;

	//Establecemos posiciones seguras para la cámara
	setCameraStartPosition(0, 0);

	//Aquí cargamos las texturas necesarias para renderear el mapa
	m_mapTextures = NULL;

	//Primero alojamos memoria para los objetos
	m_mapTextures = GEE_NEW geTexture[TT_NUM_OBJECTS];
	GEE_ASSERT(m_mapTextures);

#ifdef MAP_IS_ISOMETRIC	//El mapa está en modo isométrico
	//Cargamos las imágenes de sus archivos respectivos
	for( uint8 i=0; i<TT_NUM_OBJECTS; ++i )
	{
		textureName = TEXT("Textures\\Terrain\\iso_terrain_")+ ToStr(0) +TEXT(".png");
		m_mapTextures[i].LoadFromFile(m_pRenderer, textureName);
	}
#else	//Estamos utilizando el sistema de mapa cuadrado
	//Cargamos las imágenes de sus archivos respectivos
	for( uint8 i=0; i<TT_NUM_OBJECTS; ++i )
	{
		textureName = TEXT("Textures\\Terrain\\terrain_")+ ToStr(i) +TEXT(".png");
		m_mapTextures[i].LoadFromFile(m_pRenderer, textureName);
	}
#endif

	//Hacemos los precálculos necesarios según los cambios en los datos
	PreCalc();

	return true;
}

void geTiledMap::Destroy()
{
	//Revisamos si ya se creó un mapa
	if(m_mapGrid != NULL)
	{//Fue creado y necesitamos destruirlo
		for(int32 i = 0; i < m_mapSize; i++)
		{
			SAFE_DELETE_ARRAY( m_mapGrid[i] );
		}

		SAFE_DELETE_ARRAY( m_mapGrid );
	}

	//Destruimos las texturas del mapa
	SAFE_DELETE_ARRAY( m_mapTextures );

	//Limpiamos las otras variables miembro de la clase
	m_mapSize = 0;
	setCameraStartPosition(0, 0);
	PreCalc();	//Rehacemos precálculos para evitar utilizar datos inválidos

	m_pRenderer = NULL;
	m_bShowGrid = false;
}

int8 geTiledMap::getCost (const int32 x, const int32 y) const
{
	GEE_ASSERT( (x>=0) && (x<m_mapSize) && (y>=0) && (y<m_mapSize) );
	return m_mapGrid[x][y].getCost();
}

void geTiledMap::setCost(const int32 x, const int32 y, const int8 cost)
{
	GEE_ASSERT( (x>=0) && (x<m_mapSize) && (y>=0) && (y<m_mapSize) );
	m_mapGrid[x][y].setCost(cost);
}

int8 geTiledMap::getType(const int32 x, const int32 y) const
{
	GEE_ASSERT( (x>=0) && (x<m_mapSize) && (y>=0) && (y<m_mapSize) );
	return m_mapGrid[x][y].getType();
}

void geTiledMap::setType(const int32 x, const int32 y, const uint8 idtype)
{
	GEE_ASSERT( (x>=0) && (x<m_mapSize) && (y>=0) && (y<m_mapSize) );
	m_mapGrid[x][y].setType(idtype);
}

void geTiledMap::moveCamera(const float dx, const float dy)
{//Desplaza la posición de la cámara (defazamientos en coordenadas de pantalla)
	//Almacenamos el movimiento en nuestra variable de cámara flotante, así no se perderán pequeñas variaciones
	m_fCameraX += dx;
	m_fCameraY += dy;

#ifdef MAP_IS_ISOMETRIC	//Sistema de mapa isométrico
	//Hacemos un clamp a las variables flotantes para evitar que salgan del rango permitido (al fin y al cabo tambien representan espacio de pantalla)
	m_fCameraX = Max( 0.f, Min( m_fCameraX, (float)m_PreCalc_MaxCameraCoordX ));
	m_fCameraY = Max( 0.f, Min( m_fCameraY, (float)m_PreCalc_MaxCameraCoordY ));
#else	//Sistema de Mapa cuadrado
	//Hacemos un clamp a las variables flotantes para evitar que salgan del rango permitido (al fin y al cabo tambien representan espacio de pantalla)
	m_fCameraX = Max( 0.f, Min( m_fCameraX, (float)m_PreCalc_MaxCameraCoordX ));
	m_fCameraY = Max( 0.f, Min( m_fCameraY, (float)m_PreCalc_MaxCameraCoordY ));
#endif

	setCameraStartPosition( Trunc(m_fCameraX), Trunc(m_fCameraY) );
}

void geTiledMap::setCameraStartPosition(const int32 x, const int32 y)
{//Establece una nueva posición para la cámara (En coordenadas de pantalla)
	int32 newX = x;
	int32 newY = y;
	
#ifdef MAP_IS_ISOMETRIC
	//Hacemos un clamp a las variables flotantes para evitar que salgan del rango permitido (al fin y al cabo tambien representan espacio de pantalla)
	newX = Max( 0, Min( newX, m_PreCalc_MaxCameraCoordX ));
	newY = Max( 0, Min( newY, m_PreCalc_MaxCameraCoordY ));
#else
	//Hacemos un clamp a las variables flotantes para evitar que salgan del rango permitido (al fin y al cabo tambien representan espacio de pantalla)
	newX = Max( 0, Min( newX, m_PreCalc_MaxCameraCoordX ));
	newY = Max( 0, Min( newY, m_PreCalc_MaxCameraCoordY ));
#endif

	//Establecemos la posición de la cámara
	m_iCameraX = newX;
	m_iCameraY = newY;

	//Almacenamos los precálculos de defazamiento para las funciones de transformacíon de pantalla
	//NOTA: Esto es una optimización ya que estos datos se utilizan muy seguido y no cambian más que cuando se mueve la cámara lo cual solo puede hacerse una vez por ciclo
#ifdef MAP_IS_ISOMETRIC	//Estamos usando el sistema isométrico
	m_PreCalc_ScreenDefaceX = m_startX + m_PreCalc_MidResolutionX - (m_iCameraX-m_iCameraY);
	m_PreCalc_ScreenDefaceY = m_startY + m_PreCalc_MidResolutionY - DivX2(m_iCameraX+m_iCameraY);
#else
	m_PreCalc_ScreenDefaceX = m_startX + m_PreCalc_MidResolutionX - m_iCameraX;
	m_PreCalc_ScreenDefaceY = m_startY + m_PreCalc_MidResolutionY - m_iCameraY;
#endif
}

void geTiledMap::getScreenToMapCoords(const int32 scrX, const int32 scrY, int32 &mapX, int32 &mapY)
{//Esta función convierte coordenadas de pantalla y las regresa como coordenadas de mapa
#ifdef MAP_IS_ISOMETRIC
	//Hacemos el reajuste de la posición de la cámara y la pantalla (tambien quitamos la mitad de la longitud de un tile para ajustar por el redondeo de las coordenadas)
	float fscrX = ((float)(scrX-m_PreCalc_ScreenDefaceX)/TILEHALFSIZE_X)-1;	//El menos uno aquí aplica para hacer un ajuste de punto medio del tile (podría restarse TILEHALFSIZE_X antes de la división y daría lo mismo)
	float fscrY = ((float)(scrY-m_PreCalc_ScreenDefaceY)/TILEHALFSIZE_Y);

	//Hacemos la conversión a coordenadas de mapa
	mapX = DivX2(Trunc( fscrX + fscrY ));
	mapY = DivX2(Trunc( fscrY - fscrX ));
#else	//Es un mapa isométrico
	//Agregamos la posición de la cámara a la posición del punto en pantalla y dividimos entre la longitud del tile
	mapX = (scrX-m_PreCalc_ScreenDefaceX)>>BITSFT_TILESIZE_X;
	mapY = (scrY-m_PreCalc_ScreenDefaceY)>>BITSFT_TILESIZE_Y;
#endif	//MAP_IS_ISOMETRIC

	mapX = Max( 0, Min( mapX, m_mapSize-1 ) );
	mapY = Max( 0, Min( mapY, m_mapSize-1 ) );
}

void geTiledMap::getMapToScreenCoords(const int32 mapX, const int32 mapY, int32 &scrX, int32 &scrY)
{//Esta función convierte coordenadas de mapa a coordenadas de pantalla
	//Revisamos que nadie pase datos inválidos del mapa (a diferencia de las coordenadas de pantalla, las coordenadas de mapa nunca deberían llegar aquí de manera inválida)
	GEE_ASSERT( (mapX>=0) && (mapX<m_mapSize) && (mapY>=0) && (mapY<m_mapSize) );

#ifdef MAP_IS_ISOMETRIC	//Estamos usando el sistema isométrico
	scrX = (mapX - mapY) <<5 ;
	scrY = (mapX + mapY) <<4 ;

	scrX += m_PreCalc_ScreenDefaceX;
	scrY += m_PreCalc_ScreenDefaceY;
#else	//Sistema de mapa cuadrado
	//Convertimos el tipo de coordenadas
	scrX = (mapX<<BITSFT_TILESIZE_X)+m_PreCalc_ScreenDefaceX;
	scrY = (mapY<<BITSFT_TILESIZE_Y)+m_PreCalc_ScreenDefaceY;
#endif	//MAP_IS_ISOMETRIC
}

void geTiledMap::Update(float deltaTime)
{
	//Aquí deben hacerse las actualizaciones de objetos del mapa para cada ciclo
	(void*)&deltaTime;
}

void geTiledMap::Render()
{
	//Creamos variables temporales
	int32 tmpX = 0;
	int32 tmpY = 0;
	int32 tmpTypeTile = 0;
	SDL_Rect clipRect;

	//Obtenemos los puntos de mapa inicial y final que debemos renderear según los puntos en la resolución
	int32 tileIniX = 0, tileIniY = 0;
	int32 tileFinX = 0, tileFinY = 0;

#ifdef MAP_IS_ISOMETRIC	//Sección para mapas isométricos
	//Creamos esta variables ya que necesitamos sacar las cuatro esquinas de la pantalla y desechar algunos datos cada vez
	int32 trashCoord=0;
	getScreenToMapCoords(m_startX, m_startY, tileIniX, trashCoord);
	getScreenToMapCoords(m_endX, m_endY, tileFinX, trashCoord);

	getScreenToMapCoords(m_endX, m_startY, trashCoord, tileIniY);
	getScreenToMapCoords(m_startX, m_endY, trashCoord, tileFinY);
#else	//Sección para mapas cuadrados
	getScreenToMapCoords(m_startX, m_startY, tileIniX, tileIniY);
	getScreenToMapCoords(m_endX, m_endY, tileFinX, tileFinY);
#endif
	//Obtenidas ya las posiciones iniciales y finales, rendereamos las texturas
	for(int32 iterX=tileIniX; iterX<=tileFinX; iterX++)
	{
		for(int32 iterY=tileIniY; iterY<=tileFinY; iterY++)
		{
			getMapToScreenCoords(iterX, iterY, tmpX, tmpY);

			//Revisamos si este tile debe imprimirse haciendo una eliminación temprana
			if( tmpX > m_endX || tmpY > m_endY || (tmpX+TILESIZE_X) < m_startX || (tmpY+TILESIZE_X) < m_startY )
			{
				continue;
			}

			tmpTypeTile = m_mapGrid[iterX][iterY].getType();
			clipRect.x = (iterX<<BITSFT_TILESIZE_X) % m_mapTextures[tmpTypeTile].GetWidth();
			clipRect.y = (iterY<<BITSFT_TILESIZE_Y) % m_mapTextures[tmpTypeTile].GetHeight();
			clipRect.w = TILESIZE_X;
			clipRect.h = TILESIZE_Y;

			m_mapTextures[tmpTypeTile].Render(tmpX, tmpY, &clipRect);
		}
	}

	if( m_bShowGrid )
	{//Si se pidió que imprimieramos el grid
		//Establecemos el color para las líneas
		SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		
		for(int32 iterX=tileIniX; iterX<=tileFinX; iterX++)
		{
#ifdef MAP_IS_ISOMETRIC	//Sección para mapas isométricos
			int32 tmpX2, tmpY2;
			getMapToScreenCoords(iterX, tileIniY, tmpX, tmpY);
			getMapToScreenCoords(iterX, tileFinY, tmpX2, tmpY2);

			SDL_RenderDrawLine(m_pRenderer, tmpX+TILEHALFSIZE_X, tmpY, tmpX2, tmpY2+TILEHALFSIZE_Y);
#else	//En mapas cuadrados
			getMapToScreenCoords(iterX, tileIniY, tmpX, tmpY);
			SDL_RenderDrawLine(m_pRenderer, tmpX, tmpY, tmpX, m_endY);
#endif
		}

		for(int32 iterY=tileIniY; iterY<=tileFinY; iterY++)
		{
#ifdef MAP_IS_ISOMETRIC	//Sección para mapas isométricos
			int32 tmpX2, tmpY2;
			getMapToScreenCoords(tileIniX, iterY, tmpX, tmpY);
			getMapToScreenCoords(tileFinX, iterY, tmpX2, tmpY2);

			SDL_RenderDrawLine(m_pRenderer, tmpX, tmpY+TILEHALFSIZE_Y, tmpX2+TILEHALFSIZE_X, tmpY2);
#else
			getMapToScreenCoords(tileIniX, iterY, tmpX, tmpY);
			SDL_RenderDrawLine(m_pRenderer, tmpX, tmpY, m_endX, tmpY);
#endif
		}
	}
}

/************************************************************************************************************************/
/* Implementación de las funciones de la subclase geTiledMap::MapTile                                        			*/
/************************************************************************************************************************/
geTiledMap::MapTile::MapTile()
{//Constructor Standard
	m_idType = 1;
	m_cost = 1;
}

geTiledMap::MapTile::MapTile(const int8 idType, const int8 cost)
{//Constructor con parámetros
	m_idType = idType;
	m_cost = cost;
}

geTiledMap::MapTile::MapTile(const MapTile& copy)
{//Constructor de copia
	m_idType = copy.m_idType;
	m_cost = copy.m_cost;
}

geTiledMap::MapTile &geTiledMap::MapTile::operator=(const MapTile& rhs)
{
	m_idType = rhs.m_idType;
	m_cost = rhs.m_cost;

	return (*this);
}

/************************************************************************************************************************/
/* Funciones de carga y salvado																							*/
/************************************************************************************************************************/
bool geTiledMap::LoadFromImageFile(SDL_Renderer* pRenderer, geString fileName)
{
	//Revisamos que esta función pueda ser llamada
	if( m_mapGrid != NULL )
	{//Este mapa ya fue creado anteriormente, destruimos los datos actuales y reinicializamos
		//Destroy();
	}

	//Cargamos el archivo de imagen especificado primero
#if PLATFORM_TCHAR_IS_1_BYTE == 1
	SDL_Surface* loadedSurface = IMG_Load( fileName.c_str() );
#else
	SDL_Surface* loadedSurface = IMG_Load( ws2s(fileName).c_str() );
#endif // PLATFORM_TCHAR_IS_1_BYTE == 1
	if( loadedSurface == NULL )
	{//Si falló en cargar la superficie
		GEE_WARNING(TEXT("geTiledMap::LoadFromImageFile: Falló al cargar el archivo ") + fileName);
		return false;
	}
	else
	{//La superficie se cargó con éxito
		//Obtenemos el tamaño de la imagen e inicializamos los objetos de la clase dependiendo de lo requerido
		if(!Init(pRenderer, loadedSurface->w))	//TODO: Cambiar la inicialización para poder generar mapas con proporciones no cuadradas
		{//Ocurrió un error al inicializar la información del mapa
			GEE_ERROR(TEXT("geTiledMap::LoadFromImageFile: Falló al inicializar la información del mapa "));
			return false;
		}
		//Ahora hacemos un barrido por la imagen y establecemos los tipos de terreno según el color del pixel de la imagen
		int32 NumBytesPerPixel = loadedSurface->pitch/loadedSurface->w;
		BYTE* pPixeles = (BYTE*)loadedSurface->pixels;

		for(int32 tmpY=0; tmpY<m_mapSize; tmpY++)
		{
			for(int32 tmpX=0; tmpX<m_mapSize; tmpX++)
			{
				uint8 tipoTerreno = TT_OBSTACLE;	//Tipo Default
				BYTE r = pPixeles[ (tmpY*loadedSurface->pitch)+(tmpX*NumBytesPerPixel)+2 ];
				BYTE g = pPixeles[ (tmpY*loadedSurface->pitch)+(tmpX*NumBytesPerPixel)+1 ];
				BYTE b = pPixeles[ (tmpY*loadedSurface->pitch)+(tmpX*NumBytesPerPixel)+0 ];

				//Revisamos que color fue el encontrado y establecemos el valor en el mapa
				if(r == 0x00 && g == 0x00 && b == 0xFF)
				{//Esto es Agua
					tipoTerreno = TT_WATER;
				}
				else if(r == 0x00 && g == 0xFF && b == 0x00)
				{//Esto es Pasto
					tipoTerreno = TT_WALKABLE;
				}
				else if(r == 0xFF && g == 0xFF && b == 0x00)
				{//Esto es Marsh
					tipoTerreno = TT_MARSH;
				}

				//Establece el tipo final
				setType(tmpX, tmpY, tipoTerreno);
			}
		}
	}

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return true;
}

bool geTiledMap::SaveToImageFile(SDL_Renderer*, geString)
{
	return false;
}
