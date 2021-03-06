//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_math.h
// Destination: 8150F020 Carte Master
// Description: Fonctions mathematiques elementaire
//------------------------------------------------------

//for my_atan
#define sq2p1     2.414213
#define sq2m1     0.414213
#define p4       16.153641
#define p3      268.425481
#define p2     1153.029351
#define p1     1780.406316
#define p0      896.785974
#define q4       58.956970
#define q3      536.265374
#define q2     1666.783814
#define q1     2079.334974
#define q0      896.785974
#define PI        3.141593
#define PI_2      1.570796
#define PI_4      0.785398


double my_sqrt(double x){
    if (x == 0.0)
        return 0;
    else{
        double M = 1.0;
        double Xn = x;
        double A = 0;
        double B = 0;
        while (Xn >= 2.0){
            Xn = 0.25 * Xn;
            M = 2.0 * M;
        }
        while (Xn < 0.5){
            Xn = 4.0 * Xn;
            M = 2.0 * M;
        }
        A = Xn;
        B = 1.0 - Xn;
        do{
            A = A * (1.0 + 0.5 * B);
            B = 0.25 * (3.0 + B) * B * B;
        } while (B > 0.000001);
        return (A * M);
    }
}

double my_atan(double x){
    char flag;
    double arg,temp,value,result,argsq;
    if(x > 0){
        flag = 0;  
        arg = x;
    }
    else{
        flag = 1;
        arg = -x;
    }
    if(arg < sq2m1){
         temp = arg;
         argsq = temp*temp;
         value = ( ( ( (p4*argsq + p3)*argsq + p2) *argsq + p1) *argsq + p0);
         value = value /( ( ( ( (argsq + q4)*argsq + q3) *argsq + q2)*argsq + q1) *argsq + q0);
         if(flag == 0)
             result = value*temp;
         else
             result = -1*(value*temp);
    }
    else if(arg > sq2p1){
        temp = 1/arg;
        argsq = temp*temp;
        value = ( ( ( (p4*argsq + p3)*argsq + p2) *argsq + p1) *argsq + p0);
        value = value /( ( ( ( (argsq + q4)*argsq + q3) *argsq + q2) *argsq + q1) *argsq + q0);
        if(flag == 0)
            result = PI_2 - (value*temp);
        else
            result = -1*(PI_2 - (value*temp));
    }
    else{
        temp = (arg-1)/(arg+1);
        argsq = temp*temp;
        value = ( ( ( (p4*argsq + p3)*argsq + p2) *argsq + p1) *argsq + p0);
        value = value /( ( ( ( (argsq + q4)*argsq + q3) *argsq + q2) *argsq + q1) *argsq + q0);
        if(flag == 0)
            result = (PI_4) +  (value*temp);
        else
            result = -1*((PI_4) +  (value*temp));
    }
    return result;
}