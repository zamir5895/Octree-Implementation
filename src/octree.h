#include<iostream>    // <--- This line is not needed  
#include<vector>


template<typename T>
struct  Point
{
    T x, y, z;
    Point(): x(0), y(0), z(0) {}
    Point(T x, T y, T z) : x(x), y(y), z(z) {}
    //Destrcutor
    ~Point() {}
};


template<typename T>
class Octree {
    private:
        Point<t>* point; //POint to represetn the octrree
        Octree<T>* children[8]; //Array of 8 children
        Point<T>* topleftfront; //Top left front point
        Point<T>* bottomrightback; //Bottom right back point
    public:
        //Constructor
        Octree(){};
        //Constructor with parameters
        Octree(T x, T y, T z);

        Octree(T x1, T y1, T z1, T x2, T y2, T z2);


        void insert(T x, T y, T z);


        bool isInside(T x, T y, T z);

        T find(T x, T y, T z);

        void remove(T x, T y, T z);
        
}
