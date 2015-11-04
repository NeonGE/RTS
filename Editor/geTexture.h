/********************************************************************
	created:	2014/01/22
	created:	22:1:2014   23:25
	filename: 	geTexture.h
	author:		Samuel Prince
	
	purpose:	Declaración de una clase para manejo de texturas
*********************************************************************/
#pragma once

class geTexture
{
public:
	geTexture(void);	//Constructor standard
	~geTexture(void);	//Destructor

	//Funciones de carga y creación
	bool LoadFromFile( SDL_Renderer *pRenderer, geString path );	//Carga una imagen a partir de la ruta especificada

	//Funciones genericas
	void Free();											//Desaloja la textura
	void SetColor( Uint8 red, Uint8 green, Uint8 blue );	//Establece modulación de color
	void SetBlendMode( SDL_BlendMode blending );			//Establece el modo de blend
	void SetAlpha( Uint8 alpha );							//Establece la modulación del canal alpha
	void Render( const int32 x, const int32 y, SDL_Rect* clip=NULL);	//Renderea la textura haciendo una copia a pantalla sin ningún tipo de efecto
	void RenderEx( const int32 x, const int32 y, SDL_Rect* clip=NULL, double angle=0.0, SDL_Point* center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE );	//Renderea la textura aplicando giro o efectos

	//Estas funciones regresan información sobre la textura
	int GetWidth();		//Longitud
	int GetHeight();	//Altura

private:
	SDL_Renderer* m_Renderer;	//Puntero al objeto de renderer utilizado para crear este recurso
	SDL_Texture* m_Texture;		//Esta es la textura en hardware

	//Propiedades de la imagen
	int32 m_Width;	//Longitud
	int32 m_Height;	//Altura
};

