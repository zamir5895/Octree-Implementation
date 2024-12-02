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
        Point<T>* existingPoint = nodes[index]->point;
        nodes[index]->point = nullptr; 
        nodes[index]->insert(existingPoint->x, existingPoint->y, existingPoint->z); 
        nodes[index]->insert(x, y, z); 
    }
    colapse(); 
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


template<typename T>
bool Octree<T>::isInside(T x, T y, T z) {
    if (x < topLeftFront->x
            || x > bottomRightBack->x
            || y < topLeftFront->y
            || y > bottomRightBack->y
            || z < topLeftFront->z
            || z > bottomRightBack->z)
            return false;

    T xmid = (topleftfront->x + bottomrightback->x) / 2;
    T ymid = (topleftfront->y + bottomrightback->y) / 2;
    T zmid = (topleftfront->z + bottomrightback->z) / 2;


    int index = getOctant(x, y, z);
    

    if(nodes[index]->point == nullptr){
        return nodes[index]->isInside(x, y, z);
    }
    else if(nodes[index]->point->x == -1){
        return false;
    }
    else{
        if(x == nodes[index]->point->x && y==nodes[index]->point->y && z==nodes[index]->point->z){
            return true;
        }
        
    }
    return false;
}

template<typename T>
Point<T> Octree<T>::find(T x, T y, T z) {
    if (!isInside(x, y, z)) {
        throw std::invalid_argument("Point is not inside the octree");
    }

    if (point != nullptr) {
        return *point;
    }

    int index = getOctant(x, y, z);
    return nodes[index]->find(x, y, z);
}


template<typename T>
void Octree<T>::remove(T x, T y, T z) {
    if (!isInside(x, y, z)) {
        throw std::invalid_argument("Point is not inside the octree");
    }

    if (point != nullptr) {
        delete point;
        point = nullptr;
        return;
    }

    int index = getOctant(x, y, z);
    nodes[index]->remove(x, y, z);
    colapse();  

}

template<typename T>
bool Octree<T>::isLeaf() const {
    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            return false;
        }
    }
    return true;
}

template<typename T>
void Octree<T>::getPoints(std::vector<Point<T>>& points) const {
    if (point != nullptr) {
        points.push_back(*point);
    }

    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            nodes[i]->getPoints(points);
        }
    }
}

template<typename T>
bool Octree<T>::isEmpty() const {
    if (point != nullptr) {
        return false;
    }

    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            if (!nodes[i]->isEmpty()) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
int Octree<T>::countPoints() const {
    int count = 0;
    if (point != nullptr) {
        count++;
    }

    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            count += nodes[i]->countPoints();
        }
    }
    return count;
}

template<typename T>
Point<T> Octree<T>::closestPoint(T x, T y, T z) const {
    if (point != nullptr) {
        T dist = distance(x, y, z, point->x, point->y, point->z);
        Point<T> closest = *point;
        
        T minDist = dist;
        
        for (int i = 0; i < 8; ++i) {
            if (nodes[i] != nullptr) {
                Point<T> subNodeClosest = nodes[i]->closestPoint(x, y, z);
                T subNodeDist = distance(x, y, z, subNodeClosest.x, subNodeClosest.y, subNodeClosest.z);
                
                if (subNodeDist < minDist) {
                    closest = subNodeClosest;
                    minDist = subNodeDist;
                }
            }
        }
        
        return closest;
    }

    int index = getOctant(x, y, z);
    return nodes[index]->closestPoint(x, y, z);
}

template<typename T>
T Octree<T>::distance(T x1, T y1, T z1, T x2, T y2, T z2) const {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}
template<typename T>
void Octree<T>::colapse() {
    if (isLeaf()) {
        return;
    }

    for (int i = 0; i < 8; ++i) {
        if (nodes[i] != nullptr) {
            nodes[i]->colapse();
        }
    }

    if (countPoints() == 0) {
        for (int i = 0; i < 8; ++i) {
            delete nodes[i]; 
            nodes[i] = nullptr;  
        }
    }
}


template<typename T>
void Octree<T>::combine(Octree<T>& octree) {
    std::vector<Point<T>> points;
    octree.getPoints(points);

    for (const Point<T>& p : points) {
        insert(p.x, p.y, p.z);
    }
}
template<typename T>
bool Octree<T>::contains(T x, T y, T z) const {
    if (point != nullptr) {
        return (point->x == x && point->y == y && point->z == z);
    }

    int index = getOctant(x, y, z);
    if (nodes[index] != nullptr) {
        return nodes[index]->contains(x, y, z);  
    }

    return false; 
}

template<typename T>
void Octree<T>::combine(Octree<T>& octree) {
    std::vector<Point<T>> points;
    octree.getPoints(points);

    for (const Point<T>& p : points) {
        if (!contains(p.x, p.y, p.z)) {
            insert(p.x, p.y, p.z);  
        }
    }
}
