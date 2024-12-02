#include<iostream>    // <--- This line is not needed  
#include<vector>
#include<unordered_map>

template<typename T>
struct  Point
{
    T x, y, z;
    Point(): x(-1), y(-1), z(-1) {}
    Point(T x, T y, T z) : x(x), y(y), z(z) {}
    //Destrcutor
    ~Point() {}
};


template<typename T>
class Octree {
    private:
        Point<t>* point; //POint to represetn the octrree
        Octree<T>* nodes[8]; // Array of 8 children
        Point<T>* topleftfront; //Top left front point
        Point<T>* bottomrightback; //Bottom right back point
    public:
        //Constructor
        Octree<T>(){};
        //Constructor with parameters
        Octree(T x, T y, T z);

        Octree(T x1, T y1, T z1, T x2, T y2, T z2);

        void insert(T x, T y, T z);

        bool isInside(T x, T y, T z);

        Point<T> find(T x, T y, T z);

        void remove(T x, T y, T z);

        bool isLeaf() const;

        void getPoints(std::vector<Point<T>>& points) const;

        bool isEmpty() const;

        int countPoints() const;

        Point<T> closestPoint(T x, T y, T z) const;

        void colapse();

        void combine(Octree<T>& octree);

        void subdivide(T x, T y, T z);

        void clear();
        
        int getOctant(T x, T y, T z);

        bool hasChildren();

        ~Octree();
        
};

/*
#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBottom 4
#define TopRightBottom 5
#define BottomRightBack 6
#define BottomLeftBack 7*/