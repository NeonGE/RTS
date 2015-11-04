/********************************************************************
	Created:	2014/02/11
	Filename:	geMapTileNode.h
	Author:		Samuel Prince

	Purpose:	Este archivo define las clases de nodos de referencia
				al mapa para pathfinding. Estos nodos se crean al 
				hacer el pathing con diferentes algoritmos y se
				almacenan en una lista para su análisis posterior
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de constantes para uso en esta clase                    													*/
/************************************************************************************************************************/
#define TILENODE_BLOCKED 3000	//Define un valor muy alto que indica que un nodo está bloqueado y no debe pasarse

/************************************************************************************************************************/
/* Definición de la clase geMapTilNode, esta es la clase base de todos los nodos de traverse para pathfinfing			*/
/************************************************************************************************************************/
class geMapTileNode
{
	/************************************************************************************************************************/
	/* Constructores y destructor virtual (virtual porque derivaremos clases de esta)                                      	*/
	/************************************************************************************************************************/
public:
	geMapTileNode(void)
	{//Constructor standard
		//Inicializamos las variables miembro de la clase
		m_cost = m_x = m_y = 0;
		m_parent = NULL;
		m_visited = false;
	}
	geMapTileNode(const int32 x, const int32 y, geMapTileNode *parent, const bool visited, const int32 cost)
	{//Constructor con parámetros establecidos
		//Inicializa los datos de la clase con los datos de los parámetros
		m_x = x; m_y = y; m_parent = parent;
		m_visited = visited;
		m_cost = cost;
	}
	geMapTileNode(const geMapTileNode &copy);	//Contructor de copia, genera un nuevo objeto utilizando los valores del objeto de referencia

	virtual ~geMapTileNode(void)
	{//Destructor virtual
		m_parent = NULL;	//Limpiamos la referencia al parent al destruirlo
	}

	/************************************************************************************************************************/
	/* Declaración de operadores virtuales para la clase                           											*/
	/************************************************************************************************************************/
public:
	virtual geMapTileNode &operator=(const geMapTileNode &rhs);		//Operador de referencia
	virtual bool operator==(const geMapTileNode &rhs);				//Operador de comparación igual		
	virtual bool operator<(const geMapTileNode &rhs);				//Operador de comparación menor a
	virtual bool operator>(const geMapTileNode &rhs);				//Operador de comparación mayor a

	/************************************************************************************************************************/
	/* Declaración de funciones de acceso                                   												*/
	/************************************************************************************************************************/
	void setParent(geMapTileNode* parent)
	{//Establece el parent de este nodo
		m_parent = parent;
	}
	void setVisited(const bool visited)
	{//Establece el estado de visitado de este nodo
		m_visited = visited;
	}
	bool getVisited() const
	{//Regresa el valor de visitado de este nodo
		return m_visited;
	}

	virtual void setCost(const int32 cost);	//Función virtual para establecer el costo de este nodo
	virtual int32 getCost() const;			//Función virtual de acceso al costo de este nodo

	/************************************************************************************************************************/
	/* Funciones de ayuda                                                   												*/
	/************************************************************************************************************************/
	bool Equals(const geMapTileNode &rhs) const
	{//Compara las posiciones entre este y otro nodo e indica si son iguales
		return ((m_x == rhs.m_x) && (m_y == rhs.m_y));
	}

	/************************************************************************************************************************/
	/* Declaración de variables miembro de la clase                         												*/
	/************************************************************************************************************************/
public:
	int32 m_x;						//Indica la posición en el eje X del nodo
	int32 m_y;						//Indica la posición en el eje Y del nodo
	int32 m_cost;					//Indica el costo para moverse a este nodo
	bool m_visited;					//Bandera que indica si este nodo fue o no visitado
	geMapTileNode *m_parent;
};

/************************************************************************************************************************/
/* Declaración de la clase geAStarMapTileNode, derivada de geMapTileNode (extiende el nodo para su uso en A*)			*/
/************************************************************************************************************************/
class geAStarMapTileNode : public geMapTileNode
{
	/************************************************************************************************************************/
	/* Declaración de contructores e implementación del destructor 															*/
	/************************************************************************************************************************/
public:
	geAStarMapTileNode() : geMapTileNode()
	{//El constructor standard, llama al constructor de la clase padre e inicializa sus propias variables
		m_g = m_f = m_h = 0;
	}
	geAStarMapTileNode(const geAStarMapTileNode &copy);

	//Destructor virtual
	virtual ~geAStarMapTileNode()
	{//Este destructor hace un override al destructor de la clase padre, por lo que tenemos que reimplementar
		m_parent = NULL;	//Limpiamos la referencia al parent
	}

	/************************************************************************************************************************/
	/* Definición de operadores                                             												*/
	/************************************************************************************************************************/
	virtual geAStarMapTileNode &operator=(const geAStarMapTileNode &rhs);

	/************************************************************************************************************************/
	/* Funciones de acceso a datos                                          												*/
	/************************************************************************************************************************/
	virtual void setCost(const int32 cost);		//Override de la función para establecer costo de un nodo utilizado para AStar
	virtual int32 getCost() const;				//Override de la función para obtener costo de un nodo utilizado para AStar

	/************************************************************************************************************************/
	/* Declaración de variables miembro de la clase                         												*/
	/************************************************************************************************************************/
public:
	int32 m_f;	//Fitness
	int32 m_g;	//Goal
	int32 m_h;	//Heuristic
};

/************************************************************************************************************************/
/* Declaración de la clase geMapTilePriorityQueue para el manejo de listas de prioridad usadas en pathfinding			*/
/************************************************************************************************************************/
class geMapTilePriorityQueue
{
	/************************************************************************************************************************/
	/* Declaración de la subclase QueueNode (estructura básica de lista doblemente ligada)									*/
	/************************************************************************************************************************/
private:
	class QueueNode
	{
		/************************************************************************************************************************/
		/* Constructores y destructores de la subclase																			*/
		/************************************************************************************************************************/
	public:
		QueueNode()
		{//Constructor standard
			//Inicializa los miembros de la clase a NULL
			m_node = NULL;
			m_next = m_back = NULL;
		}
		QueueNode(geMapTileNode *node)
		{//Constructor con parámetros
			//Copia la referencia al nodo enviado
			m_node = node;
			m_next = m_back = NULL;
		}

		/************************************************************************************************************************/
		/* Declaración de miembros de la subclase																				*/
		/************************************************************************************************************************/
	public:
		geMapTileNode *m_node;		//Puntero al nodo
		QueueNode *m_next;			//Puntero al siguiente objeto
		QueueNode *m_back;			//Puntero al objeto anterior
	};

	/************************************************************************************************************************/
	/* Constructor y destructor de la clase geMapTilePriorityQueue															*/
	/************************************************************************************************************************/
public:
	geMapTilePriorityQueue();
	~geMapTilePriorityQueue()
	{//Destructor
		makeEmpty();	//Primero vaciamos la lista
		
		//Destruimos las referencias al primer y último objeto de la lista
		SAFE_DELETE(m_head->m_node);
		SAFE_DELETE(m_tail->m_node);
		SAFE_DELETE(m_head);
		SAFE_DELETE(m_tail);
	}

	/************************************************************************************************************************/
	/* Funciones utiles para el control y acceso al queue                                                               	*/
	/************************************************************************************************************************/
public:
	void makeEmpty();							//Vacía la lista

	void enqueue(geMapTileNode *node);			//Agrega un nodo a la lista (esta función hace la ordenación por costos)
	geMapTileNode* dequeue();					//Remueve un nodo de la lista y regresa la referencia al tile que almacenaba

	bool isEmpty(){return m_size == 0;}			//Indica si la lista está vacía o no

	void remove(geMapTileNode *node);			//Elimina cualquier nodo que coincida con el enviado en el parámetro
	bool contains(geMapTileNode *node) const;	//Revisa si la lista ya contiene un nodo con la información del parámetro

private:
	uint32 m_size;								//Tamaño actual de la lista (número de objetos)
	QueueNode *m_head;							//Objeto creado como primer objeto en la lista
	QueueNode *m_tail;							//Objeto creado como último objeto en la lista
};