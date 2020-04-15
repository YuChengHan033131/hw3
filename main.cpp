#include "accelerometer.h"
#include "function.h"
Serial pc(USBTX, USBRX);
InterruptIn sw(SW2);
DigitalOut led (LED_GREEN);
Timer t;
Thread t1,t2,t3;
EventQueue entrance,layer1,layer1_a;
void printAccelerometer();
void blink();
void swRised(){
    int idA=layer1.call_every(100,printAccelerometer);
    int idB=layer1_a.call_every(1000,blink);
    t.reset();
    while(t.read()<=10){}
    layer1.cancel(idA);
    layer1_a.cancel(idB);
    pc.printf("END\r\n");
}
void printAccelerometer(){
    float x,y,z;
    accelerometer(x,y,z);
    pc.printf("%f\r\n%f\r\n%f\r\n%f\r\n",x,y,z,t.read());
}
void blink(){
    led=0;
    wait(0.5);
    led=1;
}
int main(){
    led=1;
    t.start();
    //pc.printf("\r\n");
    t1.start(callback(&entrance,&EventQueue::dispatch_forever));
    t2.start(callback(&layer1,&EventQueue::dispatch_forever));
    t3.start(callback(&layer1_a,&EventQueue::dispatch_forever));
    sw.rise(entrance.event(swRised));
    while(1){}
}