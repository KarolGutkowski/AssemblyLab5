
#include <stdio.h>
#include <string.h>

void mul_m44_v4_C(float A[4][4], float v[][4], float r[][4], int n);
void mul_m44_m44_C1(float A[4][4], float B[4][4], float R[4][4]);
void mul_m44_m44_C2(float A[4][4], float B[4][4], float R[4][4]);
void vectorPrint(float a[][4], float b[][4], int n);
void matrixPrint(float a[4][4], float b[4][4]);

extern "C" void mul_m44_v4(float A[4][4], float v[][4], float r[][4], int n);
extern "C" void mul_m44_m44(float A[4][4], float B[4][4], float R[4][4]);

const char *TransformationNames[] = {
    "Identycznosc",
    "Przesuniecie o wektor [-3,5,2]",
    "Obrot wzgledem osi OZ o kat 90",
    "Symetria wzgledem plaszczyzny YOZ",
    "Symetria wzgledem osi OX",
    "Symetria srodkowa",
    };

float Points[][4] = {
    {  1.0,  1.0,  1.0,  1.0 },
    {  0.0,  0.0,  0.0,  1.0 },
    {  1.5, -2.0,  0.5,  1.0 },
    { -3.0,  5.0, -7.0,  1.0 } };

float Transformations[][4][4] = {

    { //  0 - Identycznosc
    {  1.0,  0.0,  0.0,  0.0 },
    {  0.0,  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0,  0.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    { //  1 - Przesuniecie o wektor [5,-7,3]
    {  1.0,  0.0,  0.0,  5.0 },
    {  0.0,  1.0,  0.0, -7.0 },
    {  0.0,  0.0,  1.0,  3.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    { //  2 - Obrot wzgledem osi OZ o kat 90
    {  0.0, -1.0,  0.0,  0.0 },
    {  1.0,  0.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0,  0.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    { //  3 - Symetria wzgledem plaszczyzny YOZ
    { -1.0,  0.0,  0.0,  0.0 },
    {  0.0,  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0,  0.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    { //  4 - Symetria wzgledem osi OX
    {  1.0,  0.0,  0.0,  0.0 },
    {  0.0, -1.0,  0.0,  0.0 },
    {  0.0,  0.0, -1.0,  0.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    { //  5 - Symetria srodkowa
    { -1.0,  0.0,  0.0,  0.0 },
    {  0.0, -1.0,  0.0,  0.0 },
    {  0.0,  0.0, -1.0,  0.0 },
    {  0.0,  0.0,  0.0,  1.0 } },

    };

int main()
    {
    const int NT = sizeof(Transformations)/sizeof(float[4][4]);
    const int NP = sizeof(Points)/sizeof(float[4]);
    float TC[4][4];
    float TA[4][4];
    float pointsC[NP][4];
    float pointsA[NP][4];

    printf("\n*** Zadanie 1 - mnozenie macierz razy wektor ***\n");

    for ( int i=0 ; i<NT ; ++i )
        {
        printf("\n%d:  %s",i, TransformationNames[i]);
        memcpy(pointsC,Points,sizeof(Points));
        mul_m44_v4_C(Transformations[i],pointsC,pointsC,NP);
        memcpy(pointsA,Points,sizeof(Points));
        mul_m44_v4(Transformations[i],pointsA,pointsA,NP);
        vectorPrint(pointsA,pointsC,NP);
        }

    printf("\n*** Zadanie 2 - mnozenie macierzy ***\n");

    mul_m44_m44_C1(Transformations[3],Transformations[4],TC);
    mul_m44_m44(Transformations[3],Transformations[4],TA);
    matrixPrint(TA,TC);

    mul_m44_m44_C1(Transformations[1],Transformations[5],TC);
    mul_m44_m44(Transformations[1],Transformations[5],TA);
    matrixPrint(TA,TC);

    mul_m44_m44_C1(Transformations[2],Transformations[2],TC);
    mul_m44_m44_C1(TC,Transformations[2],TC);
    mul_m44_m44_C1(Transformations[2],TC,TC);
    mul_m44_m44(Transformations[2],Transformations[2],TA);
    mul_m44_m44(TA,Transformations[2],TA);
    mul_m44_m44(Transformations[2],TA,TA);
    matrixPrint(TA,TC);

    return 0;
    }


// mnożenie macierzy 4x4 przez n wektorów 4x1
void mul_m44_v4_C(float A[4][4], float vn[][4], float rn[][4], int n)
    {
    float t[4], *v, *r;
    for ( int k=0 ; k<n ; ++k )
        {
        v = vn[k];
        t[0] = A[0][0]*v[0] + A[0][1]*v[1] + A[0][2]*v[2] + A[0][3]*v[3];
        t[1] = A[1][0]*v[0] + A[1][1]*v[1] + A[1][2]*v[2] + A[1][3]*v[3];
        t[2] = A[2][0]*v[0] + A[2][1]*v[1] + A[2][2]*v[2] + A[2][3]*v[3];
        t[3] = A[3][0]*v[0] + A[3][1]*v[1] + A[3][2]*v[2] + A[3][3]*v[3];
        r = rn[k];
        r[0] = t[0];
        r[1] = t[1];
        r[2] = t[2];
        r[3] = t[3];
        }
    }

// mnożenie macierzy 4x4  -  A*B wersja pierwsza
void mul_m44_m44_C1(float A[4][4], float B[4][4], float R[4][4])
    {
    float T[4][4] = { { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 } };
    for ( int i=0 ; i<4 ; ++i )
        for( int j=0 ; j<4 ; ++j )
            {
            T[i][0] += A[i][j] * B[j][0];
            T[i][1] += A[i][j] * B[j][1];
            T[i][2] += A[i][j] * B[j][2];
            T[i][3] += A[i][j] * B[j][3];
            }
    memcpy(R,T,sizeof(T));
    }

// mnożenie macierzy 4x4  -  A*B wersja druga
void mul_m44_m44_C2(float A[4][4], float B[4][4], float R[4][4])
    {
    float T[4][4];
    for ( int i=0 ; i<4 ; ++i )
        {
        T[i][0] = A[i][0]*B[0][0] + A[i][1]*B[1][0] + A[i][2]*B[2][0] + A[i][3]*B[3][0];
        T[i][1] = A[i][0]*B[0][1] + A[i][1]*B[1][1] + A[i][2]*B[2][1] + A[i][3]*B[3][1];
        T[i][2] = A[i][0]*B[0][2] + A[i][1]*B[1][2] + A[i][2]*B[2][2] + A[i][3]*B[3][2];
        T[i][3] = A[i][0]*B[0][3] + A[i][1]*B[1][3] + A[i][2]*B[2][3] + A[i][3]*B[3][3];
        }
    memcpy(R,T,sizeof(T));
   }

void vectorPrint(float a[][4], float b[][4], int n)
    {
    bool eq;
    printf("\n");
    for ( int i=0 ; i<n ; ++i )
        {
        eq = !memcmp(a[i],b[i],sizeof(float[4]));
        printf("[ %7.4f %7.4f %7.4f %7.4f ] %2.2s [ %7.4f %7.4f %7.4f %7.4f ]\n",
               a[i][0], a[i][1], a[i][2], a[i][3], (eq?"==":"!="), b[i][0], b[i][1], b[i][2], b[i][3] );
        }
    }

void matrixPrint(float a[4][4], float b[4][4])
    {
    bool eq;
    printf("\n");
    for ( int i=0 ; i<4 ; ++i )
        {
        eq = !memcmp(a[i],b[i],sizeof(float[4]));
        printf("[ %7.4f %7.4f %7.4f %7.4f ] %2.2s [ %7.4f %7.4f %7.4f %7.4f ]\n",
               a[i][0], a[i][1], a[i][2], a[i][3], (eq?"==":"!="), b[i][0], b[i][1], b[i][2], b[i][3] );
        }
    }
