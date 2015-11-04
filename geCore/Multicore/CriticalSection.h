/********************************************************************
	Created:	2014/02/02
	Filename:	CriticalSection.h
	Author:		Samuel Prince

	Purpose:	Define clases de auda para programación multithread
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase CriticalSection (este tipo de objetos no debe ser copiable)									*/
/************************************************************************************************************************/
class CriticalSection : public GEE_noncopyable
{
	/************************************************************************************************************************/
	/* Declaración e implementación de las funciones de la clase            												*/
	/************************************************************************************************************************/
public:
	CriticalSection()
	{//Constructor
		InitializeCriticalSection( &m_cs );
	}
 
	~CriticalSection()
	{//Destructor
		DeleteCriticalSection( &m_cs );
	}
 
 
	void Lock()
	{//Bloquea la sección crítica
		EnterCriticalSection( &m_cs );
	}

	void Unlock()
	{//Libera la sección crítica
		LeaveCriticalSection( &m_cs );
	}

	/************************************************************************************************************************/
	/* Declaración de las variables de la clase                             												*/
	/************************************************************************************************************************/
protected:
	mutable CRITICAL_SECTION m_cs;
};

/************************************************************************************************************************/
/*	Descripcion
		Clase de ayuda que permite Locking / Unlocking automático de un Recirso, protegido por una sección crítica
		- Hace Lock cuando este objeto es construido
		- Hace Unlock cuando el objeto es destruido (sale del scope)

	Uso
		Cuando necesites acceso protegido aun recurso, haz lo siguiente:
		1. Crea una sección crítica asociada con el recurso
		2. Envuelve el código que accede al recurso en llaves {}
		3. Construye un objeto ScopedCriticalSection

	Ejemplo:
		//Asumimos que m_CriticalSection es una variable privada, y la usamos para proteger 'this' de ser accedido
		//mientras necesitamos acceso seguro a un recurso
		
		//Código que no requiere bloqueo de recursos
		{
			ScopedCriticalSection I_am_locked( m_cs);
			//Código que necesita de bloqueo de threads
		}
		//Código que no requiere bloqueo de recursos
*/
/****************************************************************************************************************************************************************/
class ScopedCriticalSection : public GEE_noncopyable
{
	/************************************************************************************************************************/
	/* Declaración e implementación de funciones de la clase                												*/
	/************************************************************************************************************************/
public:
	ScopedCriticalSection( CriticalSection & csResource) : m_csResource( csResource )
	{//Constructor
		m_csResource.Lock();	//Bloquea la sección crítica
	}

	~ScopedCriticalSection()
	{//Destructor
		m_csResource.Unlock();	//Libera la sección crítica
	}
 
	/************************************************************************************************************************/
	/* Declaración de variables de la clase                                 												*/
	/************************************************************************************************************************/
private:
	CriticalSection & m_csResource;	//La sección crpitica para este recurso
};

/************************************************************************************************************************/
/* concurrent_queue fue sacada de 																						*/
/* http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html		*/
/* y fue escrita por Anthony Williams																					*/
/************************************************************************************************************************/
template<typename Data>
class concurrent_queue
{
	/************************************************************************************************************************/
	/* Declaración de variables de la clase                                 												*/
	/************************************************************************************************************************/
private:
	std::queue<Data> the_queue;		//El objeto queue
	CriticalSection m_cs;			//Sección crítica dedicada a este objeto
	HANDLE m_dataPushed;			//Manejador a un evento disparado por la acción de agregar datos al queue

	/************************************************************************************************************************/
	/* Declaración e implementación de funciones de la clase                												*/
	/************************************************************************************************************************/
public:
	concurrent_queue()
	{//Constructor
		//Crea un nuevo objeto evento
		m_dataPushed = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	/************************************************************************************************************************/
	/* Esta función agrega datos al queue de forma segura para multithread  												*/
	/************************************************************************************************************************/
	void push(Data const& data)
	{
		{
			ScopedCriticalSection locker(m_cs);
			the_queue.push(data);
		}
		PulseEvent(m_dataPushed);
	}

	/************************************************************************************************************************/
	/* Vacia el queue de forma segura para multithread                      												*/
	/************************************************************************************************************************/
	bool empty() const
	{
		ScopedCriticalSection locker(m_cs);
		return the_queue.empty();
	}

	/************************************************************************************************************************/
	/* Intenta hacer un pop al queue, si la lista está vacía regresará false, si tiene un objeto lo copiará en popped_value	*/
	/************************************************************************************************************************/
	bool try_pop(Data& popped_value)
	{
		ScopedCriticalSection locker(m_cs);
		if(the_queue.empty())
		{
			return false;
		}

		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}

	/************************************************************************************************************************/
	/* Esta función hace un pop, esperando de ser necesario hasta que se agregue un objeto al queue							*/
	/************************************************************************************************************************/
	void wait_and_pop(Data& popped_value)
	{
		ScopedCriticalSection locker(m_cs);
		while(the_queue.empty())
		{
			WaitForSingleObject(m_dataPushed);
		}

		popped_value = the_queue.front();
		the_queue.pop();
	}
};