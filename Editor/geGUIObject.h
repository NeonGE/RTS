/********************************************************************
	Created:	2014/02/18
	Filename:	geGUIObject.h
	Author:		Samuel Prince

	Purpose:	Declaración de la clase geGUIObject como clase base
				de objetos de interface (botones, fondos, marcos, etc)
				Declaración de objetos mínimos y funciones compartidas
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase base geGUIObject																				*/
/************************************************************************************************************************/
class geGUIObject
{
	/************************************************************************************************************************/
	/* Constructor y destructor virtual                                     												*/
	/************************************************************************************************************************/
public:
	geGUIObject(void);
	virtual ~geGUIObject(void);
};
