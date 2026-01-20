#include "misc.h"


Vect distance_pbc(const Vect& a, const Vect& b, const Vect& size){
    Vect dist = {a.x-b.x,a.y-b.y,a.z-b.z};
    if (fabs(dist.x) > size.x/2){
        if (a.x < b.x){
            dist.x += size.x;
        } else {
            dist.x -= size.x;
        }
    }

    if (fabs(dist.y) > size.y/2){
        if (a.y < b.y){
            dist.y += size.y;
        } else {
            dist.y -= size.y;
        }
    }

    if (fabs(dist.z) > size.z/2){
        if (a.z < b.z){
            dist.z += size.z;
        } else {
            dist.z -= size.z;
        }
    }
    
    return dist;
}

double hamiltonianDipolarPBC(const Part *a, const Part *b, const Vect& size)
{
    Vect rij = distance_pbc(b->p,a->p, size);
    double r2, r, r5,E;
    r2 = rij.x * rij.x + rij.y * rij.y + rij.z * rij.z;
    r = sqrt(r2); //трудное место, заменить бы
    r5 = r2 * r2 * r; //радиус в пятой
    
    E = //энергия считается векторным методом, так как она не нужна для каждой оси
            (( (a->m.x * b->m.x + a->m.y * b->m.y + a->m.z * b->m.z) * r2)
                -
                (3 * (b->m.x * rij.x + b->m.y * rij.y + b->m.z * rij.z) * (a->m.x * rij.x + a->m.y * rij.y + a->m.z * rij.z)  )) / r5;
    return E;
}