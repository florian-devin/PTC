#include <stdio.h>
#include <string.h>
long map(long x, long in_min, long in_max, long out_min, long out_max);
int main(){
    int x = 0;
    int y = map(x,-90,90,1846,3692);
    printf("%d\n",y);
    return 0;
}

//int map(int x, int fromLow, int fromHigh, int toLow, int toHigh){
//    int y = 0;
//    x = x-(fromLow-toLow);
//    y = x * (toHigh-toLow) / (fromHigh-fromLow);
//    return y;
//}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}