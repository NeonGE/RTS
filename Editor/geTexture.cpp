/********************************************************************
	created:	2014/01/22
	created:	22:1:2014   23:25
	filename: 	geTexture.cpp
	author:		Samuel Prince
	
	purpose:	Implementación de una clase para manejo de texturas
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de archivos necesarios                                    													*/
/************************************************************************************************************************/
#include "stdafx.h"
#include "geTexture.h"

/************************************************************************/
/* Implementación del constructor                                       */
/************************************************************************/
geTexture::geTexture(void)
{
	//Inicializamos las variables de la clase
	m_Renderer = NULL;
	m_Texture = NULL;
	m_Width = 0;
	m_Height = 0;
}

/************************************************************************/
/* Implementación del destructor                                        */
/************************************************************************/
geTexture::~geTexture(void)
{
	//Desalojamos objetos de la clase
	Free();
}

/************************************************************************/
/* Esta función carga una textura a partir de un archivo de imagen      */
/************************************************************************/
bool geTexture::LoadFromFile( SDL_Renderer *pRenderer, geString path )
{
	//Nos deshacemos de cualquier información anterior
	Free();

	GEE_ASSERT(pRenderer);
	m_Renderer = pRenderer;	//Almacenamos localmente una referencia al renderer que se utilizará para crear este objeto

	//Creamos un puntero a la nueva textura que alojaremos en memoria
	SDL_Texture* newTexture = NULL;

	//Cargamos la imagen desde la ruta especificada
#if PLATFORM_TCHAR_IS_1_BYTE == 1
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
#else
	SDL_Surface* loadedSurface = IMG_Load( ws2s(path).c_str() );
#endif // PLATFORM_TCHAR_IS_1_BYTE == 1
	if( loadedSurface == NULL )
	{//Si falló en cargar la superficie
		GEE_WARNING(TEXT("Falló al cargar la imagen: ") + path);
	}
	else
	{//La superficie se cargó con éxito
		//Establecemos el color key de la imagen
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Creamos la textura a partir de los pixeles de la superficie
		newTexture = SDL_CreateTextureFromSurface( m_Renderer, loadedSurface );
		if( newTexture == NULL )
		{//Si falló al crear la textura
			GEE_WARNING(TEXT("Falló al crear la textura: ") + path);
		}
		else
		{
			//Almacenamos las dimensiones de la imagen
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}

/************************************************************************/
/* Desaloja los recursos de la clase                                    */
/************************************************************************/
void geTexture::Free()
{
	//Liberamos la textura si es que existe
	if( m_Texture != NULL )
	{
		SDL_DestroyTexture( m_Texture );
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}

	//Eliminamos nuestra referencia al renderer
	m_Renderer = NULL;
}

/************************************************************************/
/* Modula el rgb de la textura                                          */
/************************************************************************/
void geTexture::SetColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( m_Texture, red, green, blue );
}

/************************************************************************/
/* Establece la función de blending                                     */
/************************************************************************/
void geTexture::SetBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( m_Texture, blending );
}

/************************************************************************/
/* Modula el alpha de la textura                                        */
/************************************************************************/
void geTexture::SetAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( m_Texture, alpha );
}

/************************************************************************/
/* Establece el espacio de rendereo y renderea la imagen a la pantalla  */
/* Esta es la versión que no procesa efectos (es más rápida)			*/
/************************************************************************/
void geTexture::Render(const int32 x, const int32 y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Establece las dimensiones del clip de render
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Dibuja a pantalla
	SDL_RenderCopy( m_Renderer, m_Texture, clip, &renderQuad);
	//SDL_RenderCopyEx( m_Renderer, m_Texture, clip, &renderQuad, angle, center, flip );
}

/************************************************************************/
/* Establece el espacio de rendereo y renderea la imagen a la pantalla  */
/* Esta es la versión que procesa efectos (consume más tiempo)			*/
/************************************************************************/
void geTexture::RenderEx( const int32 x, const int32 y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Establece las dimensiones del clip de render
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Dibuja a pantalla
	SDL_RenderCopyEx( m_Renderer, m_Texture, clip, &renderQuad, angle, center, flip );
}

/************************************************************************/
/* Regresa la longitud de la imagen                                     */
/************************************************************************/
int geTexture::GetWidth()
{
	return m_Width;
}

/************************************************************************/
/* Regresa la altura de la imagen                                       */
/************************************************************************/
int geTexture::GetHeight()
{
	return m_Height;
}
