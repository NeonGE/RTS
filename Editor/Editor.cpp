/********************************************************************/
/**
 * @file   Editor.cpp
 * @Author Samuel Prince (samuel.prince.quezada@gmail.com)
 * @date   2014/01/23
 * @brief  Entry point for the RTS - Editor Application
 * @bug	   No known bugs.
 */
 /********************************************************************/

/************************************************************************************************************************/
/* Include required headers																								*/
/************************************************************************************************************************/
#include "stdafx.h"
#include <Commdlg.h>
#include <tchar.h>
#include <AntTweakBar.h>
#include <vld.h>

/************************************************************************************************************************/
/* Constants definitions																								*/
/************************************************************************************************************************/
const char* DEFAULT_VSYNC = "1";
const int DEFAULT_FULLSCREEN = 0;
const int DEFAULT_WINDOW_WIDTH = 1920;
const int DEFAULT_WINDOW_HEIGHT = 1080;

/************************************************************************************************************************/
/* Functions declarations																								*/
/************************************************************************************************************************/
bool Init();								//Inicializa la aplicación
void Destroy();								//Destruye los objetos de la aplicación

void Update(float deltaTime);				//Esta función actualiza la lógica de la escena
void Render();								//Esta función es la que lleva a cabo el render de la escena

void UpdateInput();							//Lee y actualiza la información de los inputs para este ciclo
bool CheckKeyState(SDL_Keycode key);		//Revisa el estado de la tecla y regresa true si está presionada false si no

//Funciones de inicialización de objetos de juego
bool InitGameObjects();						//Aquí se deben hacer todas las inicializaciones de objetos de juego
bool InitGUI();								//Inicializamos la GUI de la aplicación
bool InitScriptingSystem();					//Inicializamos objetos de Lua y toLua++

bool InitViewBackSystem();					//Inicializamos el sistema de viewback
void UpdateViewBackData();					//Actualización de información del servidor de viewback
void DestroyViewBack();						//Destruye los objetos de viewback

//Procesamiento de eventos de ventana al estilo de SDL
void SDL_ProcessEvent(const SDL_Event *e);	//Esta función procesa los eventos de la ventana según el standard de SDL

/************************************************************************************************************************/
/* Declaración de variables globales																					*/
/************************************************************************************************************************/
geMemoryDump g_MiniDump(false);		//Clase de control para generar un memory dump en caso de error

float g_FrameDeltaTime;				/*!< Elapsed time for the current frame */
float g_fCurrentGameTime;			/*!< Current game time or time since the game began (float) */
int32 g_iCurrentGameTime;			/*!< Current game time or time since the game began (int)   */

bool g_ActiveApp = false;			//Indica si la aplicación está corriendo en este instante (partes de la ejecución cuando se minimiza o se sale de la ventana)
SDL_Window *g_Window = NULL;		//La ventana a la cual estaremos rendereando
SDL_Renderer *g_Renderer = NULL;	//El objeto de render

uint16 g_ResolutionX = 0;			//Variable que contendrá la resolución horizontal de la aplicación
uint16 g_ResolutionY = 0;			//Variable que contendrá la resolución vertical de la aplicación

int32 g_MouseX = 0;					//Variable que contendrá la posición actual del ratón en cada ciclo (coordenada horizontal)
int32 g_MouseY = 0;					//Variable que contendrá la posición actual del ratón en cada ciclo (coordenada vertical)

const Uint8 *g_keyboardStates=NULL;	//Puntero que contendrá los estados de todas las teclas en el teclado

geWorld g_MyWorld;					//This is MY WORLD!
geGUI *g_pMainGUI = NULL;			//Objeto de control del GUI de la aplicación

geVector2D g_MapMovementSpeed(1024.f,1024.f);	//Indica la velocidad (pixeles por segundo) a la que se desplaza la cámara del mapa (NOTA: Mover esto a opciones del juego)

//Objeto principal de la aplicación para LUA
lua_State* g_LuaState = NULL;

//Objetos para el uso de viewback
vb_group_handle_t g_Mouse_Group;
vb_channel_handle_t g_MouseX_Channel;
vb_channel_handle_t g_MouseY_Channel;

/************************************************************************************************************************/
/* Implementación de la función de inicialización de la aplicación														*/
/************************************************************************************************************************/
void TW_CALL LoadMapFromFile(void * /*clientData*/)
{
	OPENFILENAME ofn;
	TCHAR CurrentDirectory[MAX_PATH];
	bool bMustLoad = false;
	memset(&ofn, 0, sizeof(ofn));

	TCHAR* FileName = GEE_NEW TCHAR[MAX_PATH];

	//Obtenemos el directorio actual, pera reestablecerlo cuando lo requiramos
	GetCurrentDirectory(MAX_PATH, CurrentDirectory);

	//Rellenamos la información de la estructura para establecer la carpeta inicial y los tipos de archivo soportados
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrDefExt = TEXT(".bmp");
	ofn.lpstrFilter = TEXT("Bitmap File\0*.BMP\0All\0*.*\0");
	ofn.lpstrInitialDir = TEXT("Maps\\");
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Abrimos el dialogo para seleccionar un archivo
	if( GetOpenFileName(&ofn) )
	{//El usuario seleccionó un archivo
		if (_tcslen(FileName) > 0)
		{//El nombre del archivo no está vacio
			bMustLoad = true;	//Indicamos que debe cargarse el archivo
		}
	}

	//Restablecemos la ruta inicial de la aplicación (esto es porque el diálogo cambia la carpeta de trabajo)
	//NOTA: Si no hicieramos esto aquí, cualquier próxima carga de archivos sin ruta completa fallará dentro de la aplicación, al igual que las escrituras estarán fuera de lugar
	SetCurrentDirectory(CurrentDirectory);

	//Revisamos si se debe de cargar un archivo de mapa
	if( bMustLoad )
	{//Si debe cargarse, así que mandamos llamar la función del mapa para esto
		g_MyWorld.getTiledMap()->LoadFromImageFile(g_Renderer, FileName);
	}

	//Eliminamos el buffer de nombre de archivo
	SAFE_DELETE_ARRAY(FileName);
}

int TestLUA(lua_State* lState)
{ 
	int argUno = (int)lua_tointeger(lState, 1);
	int argDos = (int)lua_tointeger(lState, 2);

	lua_Integer result = argUno+argDos;

	lua_pushinteger(lState, result);

	geString MsgText(TEXT("HOLA LUA: El resultado es: "));
	MsgText += ToStr((uint32)result);

	MessageBox(NULL, MsgText.c_str(), TEXT("Prueba"), MB_OK);
	
	return 1;
}

bool Init()
{
	bool bExito = true;	//Bandera de inicialización

	//Inicializamos el los sistemas de números Random
	RandInit(0);
	SRandInit( (int32)time(NULL) );

	//Inicializamos el API de sockets de windows
	WSADATA wsaData = {0};
	int32 wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != 0)
	{
		GEE_ERROR(TEXT("Fallo en la inicialización de WSAStartup"));
		return false;
	}

	//Inicializamos el logger
	geLogger::Init( TEXT("Config\\Logging.xml") );
	GEE_LOG( TEXT("Editor"), TEXT("***** Logger Inicializado *****") );

	geString TestRoute = TEXT("Config\\Logging.xml\\Test\\File");
	CheckFolderSlashes( TestRoute, TEXT("/") );
	GEE_LOG( TEXT("Editor"), TestRoute);

	//Inicializamos SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{//Si ocurrió algún error al inicializar el sistema de video
		GEE_ERROR(TEXT("SDL No pudo inicializarse! SDL Error: ") + s2gs(SDL_GetError()) );
		bExito = false;
	}
	else
	{//Se inicializó el video correctamente
		GEE_LOG( TEXT("Editor"), TEXT("***** SDL Inicializado *****") );

		//Activamos la sincronización vertical (TODO: Esto debe ir a una variable de configuración)
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, DEFAULT_VSYNC ) )
		{//Falló la activación del VSync
			GEE_WARNING( TEXT("VSync no fue activado!") );
		}

		//Establecemos el filtro de texturas a lineal (TODO: Esto debe ir a una variable de configuración)
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{//Falló al establecer el filtrado
			GEE_WARNING( TEXT("Filtrado lineal de texturas no activado!") );
		}

		//Creamos la ventana (TODO: Varios de estos parámetros deben irse a variables de configuración)
		g_Window = SDL_CreateWindow( "Editor RTS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, (DEFAULT_FULLSCREEN ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE) /*| SDL_WINDOW_OPENGL*/ );
		if( g_Window == NULL )
		{//La ventana no pudo ser creada
			GEE_ERROR( TEXT("La ventana no pudo ser creada! SDL Error: ") + s2gs(SDL_GetError()) );
			bExito = false;
		}
		else
		{//La ventana ha sido creada
			GEE_LOG( TEXT("Editor"), TEXT("***** Ventana Creada *****") );

			//Creamos el renderer para la ventana
			g_Renderer = SDL_CreateRenderer( g_Window, -1, SDL_RENDERER_ACCELERATED );
			if( g_Renderer == NULL )
			{//El renderer no pudo ser creado
				GEE_ERROR( TEXT("El Renderer no pudo ser creado! SDL Error: ") + s2gs(SDL_GetError()) );
				bExito = false;
			}
			else
			{//Se ha creado el renderer
				//Inicializamos el color del renderer
				SDL_SetRenderDrawColor( g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Inicializamos el cargador de PNG
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{//Falló al inicializar el cargador
					GEE_ERROR( TEXT("SDL_image no puede ser inicializado! SDL_image Error: ") + s2gs(IMG_GetError()) );
					bExito = false;
				}
			}

			//Obtenemos la resolución de la ventana y la almacenamos globalmente
			SDL_DisplayMode sMyDMode;
			if( SDL_GetWindowDisplayMode(g_Window, &sMyDMode) == 0 )
			{
				g_ResolutionX = (uint16)sMyDMode.w;
				g_ResolutionY = (uint16)sMyDMode.h;
			}

			//Inicializamos el sistema de scripting
			if( !InitScriptingSystem() )
			{//TODO: Controlar y marcar errores
				bExito = false;
			}

			//Inicializamos el sistema de GUI
			if( !InitGUI() )
			{//TODO: Controlar y marcar errores
				bExito = false;
			}

			//Inicializamos viewback
			if( !InitViewBackSystem() )
			{
				bExito = false;
			}
		}
	}

	return bExito;
}

/************************************************************************************************************************/
/* Función de desalojo de recursos y cierre de la aplicación															*/
/************************************************************************************************************************/
void Destroy()
{
	//Destruimos el GUI
	SAFE_DELETE(g_pMainGUI);
	TwTerminate();

	//Aquí debemos destruir los objetos de la aplicación antes de destruir los objetos de SDL
	g_MyWorld.Destroy();

	//Destruimos el renderer y la ventana de SDL
	SDL_DestroyRenderer( g_Renderer );
	SDL_DestroyWindow( g_Window );
	g_Window = NULL;
	g_Renderer = NULL;

	//Limpiamos el puntero a los estados del teclado
	g_keyboardStates = NULL;

	//Quitamos los subsistemas de SDL
	IMG_Quit();
	SDL_Quit();

	//Cerramos la máquina de LUA
	if(g_LuaState)
	lua_close(g_LuaState);

	//Destruimos el servidor de viewback
	DestroyViewBack();

	//Limpiamos los objetos del API de sockets de windows
	WSACleanup();

	GEE_LOG( TEXT("Editor"), TEXT("***** Destroy() *****") );

	//Destruimos el logger
	geLogger::Destroy();
}

/************************************************************************************************************************/
/* Inicializamos los objetos de juego                                                           						*/
/************************************************************************************************************************/
bool InitGameObjects()
{
	GEE_LOG( TEXT("Editor"), TEXT("***** InitGameObjects() *****") );

	//Inicializamos el mundo
	g_MyWorld.Init(g_Renderer);
	g_MyWorld.setResolution(g_ResolutionX, g_ResolutionY);

	//Creamos el objeto de GUI
	g_pMainGUI = GEE_NEW geGUI();
	GEE_ASSERT(g_pMainGUI);
	g_pMainGUI->Init(g_Renderer);

	return true;
}

/************************************************************************************************************************/
/* Inicializamos objetos de Lua y toLua++																				*/
/************************************************************************************************************************/
bool InitScriptingSystem()
{
	//Inicializamos LUA
	g_LuaState = luaL_newstate();
	if( g_LuaState == NULL )
	{
		GEE_ERROR(TEXT("Error inicializando Lua!"));
		return false;
	}

	//Esta función invoca al intérprete
	luaL_openlibs(g_LuaState);

	tolua_open(g_LuaState);

	//Registramos los objetos de la aplicación expuestos al sistema de script
	lua_register(g_LuaState, "HelloLuaWorld", &TestLUA);
	
	//BORRAR: Ejecutamos un archivo de prueba
	luaL_dofile(g_LuaState, "unit_test.lua");

	return true;
}

/************************************************************************************************************************/
/* Inicializamos la GUI de la aplicación																				*/
/************************************************************************************************************************/
bool InitGUI()
{
	//Inicializamos el sistema de interfaces
	TwInit(TW_DIRECT3D9, SDL_RenderGetD3D9Device(g_Renderer));	//Lo inicializamos en modo de DirectX9
	TwWindowSize(g_ResolutionX, g_ResolutionY);

	//Creamos una barra Tweak
	TwBar *bar = TwNewBar(TEXT("Variables"));

	//Agregamos las variables que queremos exponer a la barra
	TwAddVarRO(bar, TEXT("Window Width"), TW_TYPE_INT16, &g_ResolutionX, TEXT(" Group='Window' label='Width' help='Current graphics window width.' "));
	TwAddVarRO(bar, TEXT("Window Height"), TW_TYPE_INT16, &g_ResolutionY, TEXT(" Group='Window' label='Height' help='Current graphics window height.' "));
	
	TwAddVarRW(bar, TEXT("Map movement on X axis"), TW_TYPE_FLOAT, &g_MapMovementSpeed.X, TEXT(" Group='Map' label='Map speed X' help='Actual movement speed over the map on X axis.' keyincr='+' step='0.5' precision='2' "));
	TwAddVarRW(bar, TEXT("Map movement on Y axis"), TW_TYPE_FLOAT, &g_MapMovementSpeed.Y, TEXT(" Group='Map' label='Map speed Y' help='Actual movement speed over the map on Y axis.' keydecr='-' step='0.5' precision='2' "));
	
	TwAddButton(bar, TEXT("Load Map from BMP"), LoadMapFromFile, NULL, TEXT(" Group='Map' label='Load Map...' "));
	TwAddButton(bar, TEXT("Save Map to BMP"), LoadMapFromFile, NULL, TEXT(" Group='Map' label='Save Map...' "));

	//Cambiamos el estilo visual de la barra
	TwDefine(TEXT(" Variables color='0 128 255' alpha=128 "));
	TwDefine(TEXT(" Variables text=light "));
	TwDefine(TEXT(" Variables position='5 35' "));
	TwDefine(TEXT(" Variables size='250 350' "));
	TwDefine(TEXT(" Variables valueswidth=90 "));
	TwDefine(TEXT(" Variables refresh=10.5 "));

	return true;
}

/************************************************************************************************************************/
/** \fn bool InitViewBackSystem()
 * @brief Initialize the viewback server
 * @return true  - if all went smoothly
 * @return false - if there was any error
 */
/************************************************************************************************************************/
bool InitViewBackSystem()
{
	WSADATA wsaData = {0};
	int32 wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != 0)
	{
		GEE_ERROR(TEXT("Error initializing WSAStartup"));
		return false;
	}

	//Initialize viewback
	vb_util_initialize();

	//Create a test group
	vb_util_add_group("Mouse Data", &g_Mouse_Group);

	//Create test channels
	vb_util_add_channel("MouseX", VB_DATATYPE_INT, &g_MouseX_Channel);
	vb_util_add_channel("MouseY", VB_DATATYPE_INT, &g_MouseY_Channel);

	//Add channels to the group
	vb_util_add_channel_to_group(g_Mouse_Group, g_MouseX_Channel);
	vb_util_add_channel_to_group(g_Mouse_Group, g_MouseY_Channel);

	//Create the viewback server
	if( !vb_util_server_create("geEngine - RTS Editor") )
	{
		GEE_ERROR(TEXT("Error creating the viewback server"));
		return false;
	}

	return true;
}

/************************************************************************************************************************/
/** \fn void UpdateViewBackData(float deltaTime)
 * @brief Updates the viewback server data
 * @param [in]	deltaTime Elapsed time since the last frame
 * @return		Put return information here
 */
/************************************************************************************************************************/
void UpdateViewBackData()
{
	//This function must be called at least once per frame
	vb_server_update(g_iCurrentGameTime);

	//Send the updated data
	if( !vb_data_send_int(g_MouseX_Channel, g_MouseX) )
	{
		GEE_LOG(TEXT("Editor"), TEXT("Error sending data to the Viewback chanel"));
	}
	if( !vb_data_send_int(g_MouseY_Channel, g_MouseY) )
	{
		GEE_LOG(TEXT("Editor"), TEXT("Error sending data to the Viewback chanel"));
	}
}

/************************************************************************************************************************/
/* Destruye el servidor de viewback																						*/
/************************************************************************************************************************/
void DestroyViewBack()
{
	vb_server_shutdown();
	vb_util_initialize();
}

/************************************************************************************************************************/
/* Esta función lee los datos de input para este ciclo y los almacena para su posterior uso								*/
/************************************************************************************************************************/
void UpdateInput()
{
	//Creamos variables temporales
	SDL_Event myEvent;

	//Obtenemos la posición del ratón para este ciclo
	SDL_GetMouseState( &g_MouseX, &g_MouseY );

	//Obtenemos el estado del teclado
	g_keyboardStates = SDL_GetKeyboardState(NULL);
	GEE_ASSERT(g_keyboardStates);

	//Procesamos eventos genéricos del teclado
	if( CheckKeyState(SDLK_ESCAPE) )
	{//Si se presionó la tecla escape, enviamos un mensaje para salir de la aplicación
		myEvent.type = SDL_QUIT;
		SDL_PushEvent(&myEvent);
	}
}

bool CheckKeyState(SDL_Keycode key)
{
	GEE_ASSERT(g_keyboardStates);
	return g_keyboardStates[SDL_GetScancodeFromKey(key)]?true:false;
}

/************************************************************************************************************************/
/* Esta función actualiza la lógica de la escena																		*/
/************************************************************************************************************************/
void Update(float deltaTime)
{
	//Imprimimos el tiempo elapsado entre cuadros
	static int32 FPS = 0;
	static float elapsedTime = 0;

	elapsedTime += deltaTime;

	if( elapsedTime >= 1.0f )
	{
		//printf("Frames Per Second: %d\n", FPS);
		FPS = 0;
		elapsedTime = 0.0f;
	}

	++FPS;

	geVector2D numerouno(3.0f, 3.0f);
	geVector2D numerodos(1.0f, 1.0f);
	geVector2D numerotres;
	numerotres = numerouno-numerodos;

	//Primero actualizamos los datos de los inputs
	UpdateInput();

	//Revisamos si debemos mover la cámara
	float map_dx = 0.f;
	float map_dy = 0.f;

	if( g_MouseX == 0	|| CheckKeyState(SDLK_a) || CheckKeyState(SDLK_LEFT) )
	{
#ifdef MAP_IS_ISOMETRIC
		map_dx += -1.f;
		map_dy += 1.f;
#else
		map_dx += -1.f;
#endif // MAP_IS_ISOMETRIC
	}
	if( g_MouseX == g_ResolutionX-1	|| CheckKeyState(SDLK_d) || CheckKeyState(SDLK_RIGHT) )
	{
#ifdef MAP_IS_ISOMETRIC
		map_dx += 1.f;
		map_dy += -1.f;
#else
		map_dx += 1.f;
#endif // MAP_IS_ISOMETRIC
	}
	if( g_MouseY == 0	|| CheckKeyState(SDLK_w) || CheckKeyState(SDLK_UP) )
	{
#ifdef MAP_IS_ISOMETRIC
		map_dx += -1.f;
		map_dy += -1.f;
#else
		map_dy += -1.f;
#endif // MAP_IS_ISOMETRIC
	}
	if( g_MouseY == g_ResolutionY-1	|| CheckKeyState(SDLK_s) || CheckKeyState(SDLK_DOWN) )
	{
#ifdef MAP_IS_ISOMETRIC
		map_dx += 1.f;
		map_dy += 1.f;
#else
		map_dy += 1.f;
#endif // MAP_IS_ISOMETRIC
	}

	if( map_dx != 0.f || map_dy != 0.f )
	{
		g_MyWorld.getTiledMap()->moveCamera(map_dx*g_MapMovementSpeed.X*deltaTime, map_dy*g_MapMovementSpeed.Y*deltaTime);
	}

	//Actualizamos la lógica del mundo
	g_MyWorld.Update(deltaTime);

	//Actualizamos el GUI (despues del mundo porque hay valores que dependen de el)
	g_pMainGUI->Update(deltaTime);
}

/************************************************************************************************************************/
/* Esta función se encarga de renderear todos los elementos de la escena												*/
/************************************************************************************************************************/
void Render()
{
	//Limpiamos la pantalla
	SDL_SetRenderDrawColor( g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( g_Renderer );

	//Enviamos a renderear el mundo aquí
	g_MyWorld.Render();

	//Pintamos un grid mostrando donde estamos parados en el mapa con el raton
	SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0xFF, 0x00, 0xFF);

	//Dibujamos el perímetro del tile en el que el ratón está parado actualmente
	int32 posMapaX=0, posMapaY=0;
	int32 posScreenX=0, posScreenY=0;
	g_MyWorld.getTiledMap()->getScreenToMapCoords(g_MouseX, g_MouseY, posMapaX, posMapaY);

	//Reconvertimos las posiciones y dibujamos
	g_MyWorld.getTiledMap()->getMapToScreenCoords(posMapaX, posMapaY, posScreenX, posScreenY);

#ifdef MAP_IS_ISOMETRIC	//En un mapa isométrico, necesitamos pintar el diamante
	SDL_Point diamondPoints[5];
	
	diamondPoints[0].x = posScreenX+TILEHALFSIZE_X;
	diamondPoints[0].y = posScreenY;
	
	diamondPoints[1].x = posScreenX+TILESIZE_X;
	diamondPoints[1].y = posScreenY+TILEHALFSIZE_Y;
	
	diamondPoints[2].x = posScreenX+TILEHALFSIZE_X;
	diamondPoints[2].y = posScreenY+TILESIZE_Y;

	diamondPoints[3].x = posScreenX;
	diamondPoints[3].y = posScreenY+TILEHALFSIZE_Y;

	diamondPoints[4].x = posScreenX+TILEHALFSIZE_X;
	diamondPoints[4].y = posScreenY;

	SDL_RenderDrawLines(g_Renderer, diamondPoints, 5);
#else	//Para el mapa cuadrado solo pintamos un rectángulo en el perímetro del tile
	SDL_Rect tmpRect;
	tmpRect.x = posScreenX;
	tmpRect.y = posScreenY;
	tmpRect.w = TILESIZE_X;
	tmpRect.h = TILESIZE_Y;
	SDL_RenderDrawRect(g_Renderer, &tmpRect);
#endif // MAP_IS_ISOMETRIC

	//Imprimimos el GUI sobre todo lo demás
	g_pMainGUI->Render();
	TwDraw();	//Impresión del manejador de ventanas

	//Actualizamos la pantalla
	SDL_RenderPresent( g_Renderer );
}

/************************************************************************************************************************/
/* Esta función procesa los eventos de la ventana según el standard de SDL												*/
/************************************************************************************************************************/
void SDL_ProcessEvent(const SDL_Event *e)
{
#pragma region SDL_KEYDOWN
	//Determinamos si este es un evento de teclado
	if( e->type == SDL_KEYDOWN )
	{//Este es un evento de teclado
		//TODO: Determinamos si debemos procesar estos eventos ya que las actualizaciones las hacemos con el polling
		//SDL_Log("KEYDOWN %d ", e->key.keysym.scancode );
	}
#pragma endregion
#pragma region SDL_KEYUP
	else if( e->type ==  SDL_KEYUP )
	{

	}
#pragma endregion
#pragma region SDL_MOUSEBUTTONDOWN
	else if( e->type ==  SDL_MOUSEBUTTONDOWN )
	{

	}
#pragma endregion
#pragma region SDL_MOUSEBUTTONUP
	else if( e->type ==  SDL_MOUSEBUTTONUP )
	{

	}
#pragma endregion
#pragma region SDL_WINDOWEVENT
	else if(e->type == SDL_WINDOWEVENT)
	{//Este es un evento de ventana
		switch (e->window.event)
		{
		case SDL_WINDOWEVENT_SHOWN:
			SDL_Log("SDL_WINDOWEVENT_SHOWN %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			SDL_Log("SDL_WINDOWEVENT_HIDDEN %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_Log("SDL_WINDOWEVENT_EXPOSED %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_MOVED:
			SDL_Log("SDL_WINDOWEVENT_MOVED %d to %d,%d",
				e->window.windowID, e->window.data1,
				e->window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_Log("SDL_WINDOWEVENT_RESIZED %d to %dx%d",
				e->window.windowID, e->window.data1,
				e->window.data2);
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			g_MyWorld.setResolution(0, 0);
			TwWindowSize(0, 0);
			SDL_Log("SDL_WINDOWEVENT_MINIMIZED %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_Log("SDL_WINDOWEVENT_MAXIMIZED %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_RESTORED:
			g_ActiveApp = true;
			SDL_Log("SDL_WINDOWEVENT_RESTORED %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_ENTER:
			SDL_Log("SDL_WINDOWEVENT_ENTER %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			SDL_Log("SDL_WINDOWEVENT_LEAVE %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			g_ActiveApp = true;
			SDL_Log("SDL_WINDOWEVENT_FOCUS_GAINED %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			g_ActiveApp = false;
			SDL_Log("SDL_WINDOWEVENT_FOCUS_LOST %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_CLOSE:
			SDL_Log("SDL_WINDOWEVENT_CLOSE %d", e->window.windowID);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			g_ResolutionX = (uint16)e->window.data1;
			g_ResolutionY = (uint16)e->window.data2;
			g_MyWorld.setResolution(g_ResolutionX, g_ResolutionY);
			TwWindowSize(g_ResolutionX, g_ResolutionY);
			SDL_Log("SDL_WINDOWEVENT_SIZE_CHANGED %d to %dx%d", e->window.windowID, e->window.data1, e->window.data2);
			break;
		default:
			SDL_Log("Window %d got unknown event %d",
				e->window.windowID, e->window.event);
			break;
		}
	}
#pragma endregion
}

/************************************************************************************************************************/
/* Implementación de la función main																					*/
/************************************************************************************************************************/
int32 _tmain(int32, _TCHAR**)
{
	//Inicializamos la aplicación y los subsistemas
	if( !Init() )
	{//Falló al inicializar, indicamos el error
		GEE_ERROR( TEXT("Falló al inicializar!") );
	}
	else
	{//Todos los sistemas en orden
		//Cargamos los assets
		if( !InitGameObjects() )
		{//Falló al cargar medios, indicamos el error
			GEE_ERROR( TEXT("Falló al inicializar los objetos del juego") );
		}
		else
		{//Media lista para utilizarse
			//Bandera de control para el loop principal
			bool bQuit = false;

			//Manejador de eventos
			SDL_Event e;

			//Variables para el manejo de tiempo elapsado
			int thisTime = 0;
			int lastTime = 0;
			g_fCurrentGameTime = 0.0;

			//Variables para determinar si un evento ya fue manejado
			int32 bEventHandled = 0;
			g_ActiveApp = true;

			//Loop de la aplicación
			GEE_LOG( TEXT("Editor"), TEXT("***** Inicia el Loop de la Aplicación *****") );
			while( !bQuit )
			{
				thisTime = SDL_GetTicks();
				g_FrameDeltaTime = (float)(thisTime - lastTime) / 1000.0f;
				g_fCurrentGameTime = (float)thisTime;
				g_iCurrentGameTime = thisTime;
				lastTime = thisTime;

				//Manejamos los eventos en el queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					SDL_ProcessEvent(&e);

					bEventHandled = TwEventSDL(&e, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
					if( !bEventHandled )
					{//El evento no fue manejado por AntTweakBar, lo procesamos
						//El usuario pidió salir de la aplicación
						if( e.type == SDL_QUIT )
						{
							bQuit = true;
						}
					}
				}

				if(g_ActiveApp)
				{
					Update(g_FrameDeltaTime);
					Render();
				}

				//Actualizamos la información del servidor de viewback
				UpdateViewBackData();
			}
		}
	}

	//Free resources and close SDL
	Destroy();

	return 0;
}
