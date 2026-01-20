#ifndef MISC_H
#define MISC_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>

using namespace std;

//------------------структуры данных--------------------//

struct Vect{
    double x;
    double y;
    double z;
};

struct Part{
    Vect p;
    Vect m;
};

struct triplet {
    size_t max;
    size_t min;
    size_t count;
    double mean;
    double mean2;
    double mean4;
};

Vect distance_pbc(const Vect& a, const Vect& b, const Vect& size);

double hamiltonianDipolarPBC(const Part *a, const Part *b, const Vect& size);


inline double distance(const Vect &a, const Vect &b)
{
    Vect d = {a.x-b.x,a.y-b.y};
    return sqrt(
        d.x * d.x +
        d.y * d.y
        );
}

inline double distance_2(const Vect &a, const Vect &b)
{
    Vect d = {a.x-b.x,a.y-b.y};
    return
        d.x * d.x +
        d.y * d.y;
}

inline double length(const Vect &d){
    return sqrt(
        d.x * d.x +
        d.y * d.y +
        d.z * d.z
        );
}

inline Vect normalise(const Vect &q){
    double l = length(q);
    return {q.x / l, q.y / l};
}

inline double scalar(const Vect &a, const Vect &b)
{
    return (a.x * b.x) + (a.y * b.y);
}


#endif //MISC_H