#include<iostream>
#include<vector>
#include "octree.h"
#include<unordered_map>








template<typename T>
Octree<T>::Octree(){
    point = new Point<T>();
}

template<typename T>
Octree<T>::Octree(T x, T y, T z){
    point = new Point<T>(x, y, z);
}

template<typename T>
Octree<T>::Octree(T x1, T y1, T z1, T x2, T y2, T z2){
    if( x2 < x1 ||  y2 < y1 || z2 < z1){
        throw std::invalid_argument("Invalid arguments");
    }
    point = nullptr;
    topleftfront = new Point<T>(x1, y1, z1);
    bottomrightback = new Point<T>(x2, y2, z2);
    for(int i = 0; i < 8; i++){
        nodes[i] = new Octree<T>();
    }
    
}

template<typename T>
void Octree<T>::insert(T x, T y, T z) {
    if (isInside(x, y, z)) {
        throw std::invalid_argument("Point is already inside the octree");
    }

    if (x < topleftfront->x || x > bottomrightback->x || y < topleftfront->y || y > bottomrightback->y || z < topleftfront->z || z > bottomrightback->z) {
        throw std::invalid_argument("Point is outside the octree");
    }

    if (!hasChildren()) {
        if (point == nullptr) {
            point = new Point<T>(x, y, z);
            return;
        }
        T xmid = (topleftfront->x + bottomrightback->x) / 2;
        T ymid = (topleftfront->y + bottomrightback->y) / 2;
        T zmid = (topleftfront->z + bottomrightback->z) / 2;
        subdivide(xmid, ymid, zmid);
    }

    int index = getOctant(x, y, z);

    if (nodes[index]->point == nullptr) {
        nodes[index]->point = new Point<T>(x, y, z);
    } else if (nodes[index]->point->x == -1) {
        delete nodes[index];
        nodes[index] = new Octree<T>(x, y, z);
        return;
    } else {
        // Si ya hay un punto en ese octante, lo reinsertamos
        Point<T>* existingPoint = nodes[index]->point;
        nodes[index]->point = nullptr; // Vaciamos el punto anterior
        nodes[index]->insert(existingPoint->x, existingPoint->y, existingPoint->z); // Reinserción del punto anterior
        nodes[index]->insert(x, y, z); // Insertamos el nuevo punto
    }
}

template<typename T>
bool Octree<T>::hasChildren() {
    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            return true;
        }
    }
    return false;
}

template<typename T>
int Octree<T>::getOctant(T x, T y, T z) {
    T xmid = (topleftfront->x + bottomrightback->x) / 2;
    T ymid = (topleftfront->y + bottomrightback->y) / 2;
    T zmid = (topleftfront->z + bottomrightback->z) / 2;

    if (x <= xmid) {
        if (y <= ymid) {
            if (z <= zmid) return 0; // Octante 0
            else return 4;           // Octante 4
        } else {
            if (z <= zmid) return 3; // Octante 3
            else return 7;           // Octante 7
        }
    } else {
        if (y <= ymid) {
            if (z <= zmid) return 1; // Octante 1
            else return 5;           // Octante 5
        } else {
            if (z <= zmid) return 2; // Octante 2
            else return 6;           // Octante 6
        }
    }
}

template<typename T>
void Octree<T>::subdivide(T xmid, T ymid, T zmid) {
    for (int i = 0; i < 8; ++i) {
        if (nodes[i] == nullptr) {
            switch (i) {
                case 0: nodes[i] = new Octree<T>(topleftfront->x, topleftfront->y, topleftfront->z, xmid, ymid, zmid); break;
                case 1: nodes[i] = new Octree<T>(xmid, topleftfront->y, topleftfront->z, bottomrightback->x, ymid, zmid); break;
                case 2: nodes[i] = new Octree<T>(xmid, ymid, topleftfront->z, bottomrightback->x, bottomrightback->y, zmid); break;
                case 3: nodes[i] = new Octree<T>(topleftfront->x, ymid, topleftfront->z, xmid, bottomrightback->y, zmid); break;
                case 4: nodes[i] = new Octree<T>(topleftfront->x, topleftfront->y, zmid, xmid, ymid, bottomrightback->z); break;
                case 5: nodes[i] = new Octree<T>(xmid, topleftfront->y, zmid, bottomrightback->x, ymid, bottomrightback->z); break;
                case 6: nodes[i] = new Octree<T>(xmid, ymid, zmid, bottomrightback->x, bottomrightback->y, bottomrightback->z); break;
                case 7: nodes[i] = new Octree<T>(topleftfront->x, ymid, zmid, xmid, bottomrightback->y, bottomrightback->z); break;
            }
        }
    }
}


