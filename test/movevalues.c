#include <stdio.h>

void moveValues(float *data,int size,float newf) {
    for (int i = 1;i<size;i++) {
        data[i-1]=data[i];
    }
    data[size-1] = newf;
}

int main() {
    float data[11] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0};
    moveValues(data,11,1.1);
    for (int i = 0 ; i < 11 ; i++) {
        printf("%.4f ",data[i]);
    }
    return 0;
}