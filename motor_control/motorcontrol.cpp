#include<iostream>
#include "serialib.h"
#include<fstream>
#include<string>
using namespace std;


int main(){
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    unsigned char msg[] = { 'F', 'L', '\r' };
    write(serial_port, msg, sizeof(msg));
    serialib serial;
    char errorOpening = serial.openDevice("/dev/ttyUSB0", 9600);
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n","dev/ttyUSB0");
    serial.writeString("VE\0");
    serial.writeString("FL\0");
    usleep(10000);
    cout << "Motor control program" << endl;
    return 0;
}