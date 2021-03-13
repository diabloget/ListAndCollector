#include <iostream>

using namespace std;

/*!
* @struct c_node
* @brief c_node almacena las direcciones.
* @details Se usa este nodo para almacenar las direcciones de variables eliminadas y así reutilizarlas.
* @public
*/
struct c_node
{
    void* data;
    c_node *next;
};

/*!
* @class c_linked_list
* @brief c_linked_list lista de los nodos de direcciones.
* @details Esta lista almacena direcciones de variables eliminadas, tal que el collector pueda accesarlas y reciclarlas.
* @public
*/
class c_linked_list
{
private:
    c_node *head,*tail;
public:
    c_linked_list()
    {
        head = NULL;
        tail = NULL;
    }

    /*!
    * @brief add_node() agrega un nodo con un pointer a la lista.
    * @param n pointer por almacenar en la lista.
    * @return head cabeza de la lista
    */
    void add_node(void* n)
    {
        c_node *tmp = new c_node;
        tmp->data = n;
        tmp->next = NULL;

        if(head == NULL)
        {
            head = tmp;
            tail = tmp;
        }
        else
        {
            tail->next = tmp;
            tail = tail->next;
        }
    }

    /*!
    * @brief gethead() retorna la cabeza de la lista.
    * @return head cabeza de la lista
    */
    c_node* gethead(){
        return head;
    }

    /*!
    * @brief update() actualiza la cabeza de la lista.
    */
    void update(){
        head = head->next;
    }
};

/*!
* @class collector
* @brief Collector reclicla el espacio de variables eliminadas.
* @details Cada vez que un nodo se crea, requiere de collector para verificar si hay algún espacio disponible para reciclar.
* @public
*/
class collector{
private:
    c_linked_list clist;
public:
    collector(){
        c_linked_list tmp;
        this->clist = tmp;
    }
    /*!
    * @brief getSpace() retorna un puntero reciclado.
    * @return tmp si hay un puntero a reciclar.
    * @return NULL si no hay espacio a reciclar.
    */
    void* getSpace(){
        if(clist.gethead()!= NULL){
            void* tmp = clist.gethead()->data;
            clist.update();
            return tmp;
        }
        else{
            return NULL;
        }
    }

    /*!
    * @brief saveSpace() guarda el puntero del nodo eliminado en la lista.
    * @param space pointer del nodo a reciclar.
    */
    void saveSpace(void* space){
        clist.add_node(space);
    }


};

collector list_collector;

/*!
* @class node
* @brief node almacena números y referencias al nodo siguiente.
* @details Se almacenan números enteros y la referencia al siguiente nodo, sobrecarga los métodos de new y delete para hacer uso de la clase Collector.
* @public
*/
class node{
    int data;
    node *next;
public:
    node(){
        cout<< "Constructor de Nodo llamado\n" ;
    }
    node(int data, node* next){
        this->data = data;
        this->next = next;
    }
    void * operator new(size_t size)
    {
        if(list_collector.getSpace() != NULL){
            return list_collector.getSpace();
        } else{
            void * space = ::new node();
            return space;
        }
    }
    void operator delete(void * space)
    {
        list_collector.saveSpace(space);
    }
    /*!
    * @brief getNext() retorna el nodo siguiente.
    * @param nodo a asignar como siguiente.
    * @return next es el nodo siguiente.
    */
    void setNext(node* next){
        this->next = next;
    }
    /*!
    * @brief getNext() retorna el nodo siguiente.
    * @return next es el nodo siguiente.
    */
    node* getNext(){
        return next;
    }
    /*!
    * @brief getData() retorna la información del nodo.
    * @return data Numero del nodo.
    */
    int getData(){
        return data;
    }
};

/*!
* @class linked_list
* @brief linked_list es una lista enlazada simple de números.
* @details Lista enlazada que guarda enteros haciendo uso de la clase node. Cada nodo añadido toma la posición de la cabeza y cada eliminado hace uso de Collector.
* @public
*/
class linked_list{
    node* head;
    int len;
public:
    linked_list(){
        this->head = NULL;
        this->len =0;
    }
    /*!
    * @brief add() añade un nuevo nodo a la lista.
    * @param data El numero que alamacenará el nodo.
    */
    void add(int data){
        len = len+1;

        node *tmp = new node(data, NULL);
        if(head == NULL){
            head = tmp;
        } else{
            tmp->setNext(head);
            this->head = tmp;
        }
    }
    /*!
    * @brief dell() método para eliminar un nodo.
    * @param data El numero del nodo a eliminar.
    */
    void dell(int data){
        len = len-1;
        if(head->getData()== data){
            node*tmp = head->getNext();
            this->head = head->getNext();
            tmp->operator delete(tmp);
        }else{
            dell_aux(data, head);
        }
    }
    /*!
    * @brief dell_aux() método recursivo auxiliar para buscar el nodo a eliminar.
    * @param data El numero del nodo a eliminar, current Nodo a revisar en la recursión.
    */
    void dell_aux(int data, node* current){
        if(current->getNext()->getData()== data){
            node*tmp=current->getNext()->getNext();
            current->getNext()->operator delete(current->getNext());

            current->setNext(tmp);
        }else{
            dell_aux(data, current->getNext());
        }
    }

    /*!
    * @brief print() imprime los elementos en la lista.
    */
    void print(){
        node* current = head;
        while(current!=NULL){
            cout<< "Nodo = " <<current->getData() <<"\n";
            current = current->getNext();
        };
    }

};



int main()
{
    linked_list a;
    a.add(5);
    a.add(3);
    a.add(6);
    a.add(15);
    a.print();
    cout<< "Ahora borrando el 6\n";
    a.dell(6);
    a.print();
    cout<< "Ahora verificamos al agregar otro elemento (6)\n";
    a.add(6);
    a.print();

    return 0;
}
