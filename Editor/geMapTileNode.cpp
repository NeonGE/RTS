/********************************************************************
	Created:	2014/02/11
	Filename:	geMapTileNode.cpp
	Author:		Samuel Prince

	Purpose:	Implementación de las clases de nodos de tile
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de archivos de cabeceras necesarios para la compilación    												*/
/************************************************************************************************************************/
#include "stdafx.h"
#include "geMapTileNode.h"

/************************************************************************************************************************/
/* Implementación de la clase geMapTileNode                                          									*/
/************************************************************************************************************************/
geMapTileNode::geMapTileNode(const geMapTileNode &copy)
{//Constructor de copia
	//Copiamos todos los datos del objeto de referencia a nuestras variables miembro
	m_x = copy.m_x;
	m_y = copy.m_y;
	m_cost = copy.m_cost;
	m_parent = copy.m_parent;
	m_visited = copy.m_visited;
}

geMapTileNode & geMapTileNode::operator=(const geMapTileNode &rhs)
{//Operador igual
	//Copiamos todos los datos del objeto de referencia a nuestras variables miembro
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_parent = rhs.m_parent;
	m_visited = rhs.m_visited;
	m_cost = rhs.m_cost;

	return *this;	//Regresamos la referencia a de este mismo objeto
}

bool geMapTileNode::operator==(const geMapTileNode &rhs)
{//Operador de comparación
	return this->Equals(rhs);	//Comparamos los datos necesarios y regresamos el resultado
}

bool geMapTileNode::operator<(const geMapTileNode&rhs)
{//Operador menor que
	return m_cost < rhs.m_cost;	//Regresamos el resultado de la comparación de costos
}

bool geMapTileNode::operator>(const geMapTileNode &rhs)
{//Operador mayor que
	return m_cost > rhs.m_cost;	//Regresamos el resultado de la comparación de costos
}

void geMapTileNode::setCost(const int32 cost)
{//Establece el valor del costo de este nodo
	m_cost = cost;
}

int geMapTileNode::getCost() const
{//Regresa el valor de costo de este nodo
	return m_cost;
}

/************************************************************************************************************************/
/* Implementación de la clase geMapTilePriorityQueue para el control de prioridades de búsqueda							*/
/************************************************************************************************************************/
geMapTilePriorityQueue::geMapTilePriorityQueue()
{//Constructor standard
	//Inicializamos los miembros de la clase
	m_size = 0;

	//Creamos un nodo para la cabeza y uno para la cola con costos menor y mayor a lo permitido respectivamente
	m_head = GEE_NEW QueueNode( GEE_NEW geMapTileNode(-1, -1, NULL, true, -1) );
	m_tail = GEE_NEW QueueNode( GEE_NEW geMapTileNode(-1, -1, NULL, true, 10000));

	//Establecemos las dobles ligaduras en los objetos
	m_head->m_next = m_tail;
	m_head->m_back = NULL;
	m_tail->m_back = m_head;
	m_tail->m_next = NULL;
}

void geMapTilePriorityQueue::enqueue(geMapTileNode *node)
{//Agrega un nodo a la lista (esta función hace la ordenación por costos)
	QueueNode *insertnode = GEE_NEW QueueNode(node);

	//Buscamos el lugar en la lista donde debe ir este nodo
	QueueNode *c = m_head;
	while(node->getCost() > c->m_node->getCost())
	{
		c = c->m_next;
	}

	//Lo insertamos en la posición anterior de la lista
	insertnode->m_back = c->m_back;
	insertnode->m_next = c;

	//Reescribimos los apuntadores de los objetos en la lista
	c->m_back = insertnode;
	insertnode->m_back->m_next = insertnode;

	//Indicamos que la lista ha crecido
	m_size++;
}

geMapTileNode* geMapTilePriorityQueue::dequeue()
{//Remueve un nodo de la lista y regresa la referencia al tile que almacenaba
	//Siempre eliminamos desde el frente
	geMapTileNode *item = NULL;
	QueueNode *removednode;

	//Si la lista tiene objetos
	if(m_size > 0)
	{
		//Primero redireccionamos la referencia del objeto próximo al que vamos a quitar para indicarlo como la nueva cabeza de la lista
		m_head->m_next->m_next->m_back = m_head;

		//Obtenemos el puntero al nodo que eliminaremos
		removednode = m_head->m_next;

		//Establecemos el siguiente objeto en la lista y lo ligamos a la cabeza (recuerden que esto es una lista doblemente ligada ;)
		m_head->m_next = m_head->m_next->m_next;

		//Obtenemos la información del objeto
		item = removednode->m_node;
		
		//Borramos el nodo de la lista
		GEE_DELETE removednode;
	}

	//Reducimos el tamaño de la lista
	m_size--;

	return item;	//Regresamos el puntero al objeto
}

void geMapTilePriorityQueue::remove(geMapTileNode* node)
{//Elimina cualquier nodo que coincida con el enviado en el parámetro
	//Creamos variables temporales
	QueueNode *c = m_head;
	
	//Buscamos en la lista hasta encontrar una coincidencia
	while( !node->Equals( *(c->m_node) ) )
	{//Mientras seguimos buscando
		c = c->m_next;	//Obtenemos el puntero al siguiente objeto para revisarlo iterativamente
	}

	//NOTA: si el objeto que estamos buscando no está en la lista esto es un error, sin embargo en la forma en la que lo estamos usando debe ser imposible
	//De todos modos no hacemos el chequeo para manejar una optimización 

	//Redireccionamos los punteros de los objetos anterior y próximo para que ahora se apunten entre ellos
	c->m_next->m_back = c->m_back;
	c->m_back->m_next = c->m_next;

	//Reducimos el tamaño de la lista
	m_size--;

	GEE_DELETE c;	//Eliminamos el nodo de memoria
}

bool geMapTilePriorityQueue::contains(geMapTileNode* node) const
{//Revisa si la lista ya contiene un nodo con la información del parámetro
	QueueNode *c = m_head;

	//Revisamos la lista hasta que lleguemos al final de ser necesario
	while(c != m_tail)
	{
		if(c->m_node->Equals(*node))
		{//Si la información del nodo indicado es igual a este
			return true;	//Indicamos que los datos si están en la lista
		}

		//Aun no lo encontramos, nos movemos al siguiente objeto
		c = c->m_next;
	}
	
	return false;	//Si llegamos a este punto, significa que el objeto no está en la lista
}

void geMapTilePriorityQueue::makeEmpty()
{//Vacía la lista
	while(!isEmpty())
	{
		dequeue();
	}
}

/************************************************************************************************************************/
/* Implementación de la clase geAStarMapTileNode                                          								*/
/************************************************************************************************************************/
geAStarMapTileNode::geAStarMapTileNode(const geAStarMapTileNode &copy)
{//Constructor por copia
	//Establecemos los valores locales copiando los de la referencia enviada
	m_x = copy.m_x;
	m_y = copy.m_y;
	m_parent = copy.m_parent;
	m_visited = copy.m_visited;
	m_cost = copy.m_cost;
	m_g = copy.m_g;
	m_h = copy.m_h;
	m_f = copy.m_f;
}

geAStarMapTileNode & geAStarMapTileNode::operator=(const geAStarMapTileNode &rhs)
{//Operador de igualación
	//Establecemos los valores locales copiando los de la referencia enviada
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_parent = rhs.m_parent;
	m_visited = rhs.m_visited;
	m_cost = rhs.m_cost;
	m_g = rhs.m_g;
	m_h = rhs.m_h;
	m_f = rhs.m_f;

	return *this;	//Regresamos la referencia a este mismo objeto
}

void geAStarMapTileNode::setCost(const int32 cost)
{//Establece el costo de este nodo
	m_f = cost;	//Lo establecemos en la variable de costo final y no de el costo general de la clase base
}

int geAStarMapTileNode::getCost() const
{//Regresamos el costo final de este nodo
	return m_f;
}
