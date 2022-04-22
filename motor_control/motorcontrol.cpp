#include<iostream>
#include<fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include<fstream>
#include<string>
#include <stdio.h>
#include <string.h>
using namespace std;

void initialize_port(int serial_port);


int main(int argc, char** argv){
    int el = open("/dev/ttyUSB0", O_RDWR);
    int az = open("/dev/ttyUSB1", O_RDWR);
    initialize_port(az);
    initialize_port(el);

    unsigned char fl_msg[] = { "FL\r" };
    //unsigned char forw_msg[] = { 'D', 'I', '4', '0','0', '0', '\r' };
    unsigned char forw_msg[] = { "DI4000\r" };
    unsigned char back_msg[] = { 'D', 'I','-','8', '0','0', '\r' };
    unsigned char read_encoder[] = { "EP\r" };
    unsigned char sp_msg[] = { "SP0\r" };

    int input_az = atoi(argv[1]) * 60;
    int input_el = atoi(argv[2]) * 60;
    char* fp = "FP";
    char* r = "\r";
    char az_result[100];
    strcpy(az_result,fp);
    strcat(az_result,to_string(input_az).c_str());
    strcat(az_result,r);
    char el_result[100];
    strcpy(el_result,fp);
    strcat(el_result,to_string(input_el).c_str());
    strcat(el_result,r);
    //unsigned char fp_msg_az[] = { "FP"+(unsigned char)input_az+"\r" };
    unsigned char fp_msg_el[] = { "FP22000\r" };
    // write(el, forw_msg, sizeof(forw_msg));
    // write(el, fl_msg, sizeof(fl_msg));
    // write(az, forw_msg, sizeof(forw_msg));
    // write(az, fl_msg, sizeof(fl_msg));
    write(el, sp_msg, sizeof(sp_msg));
    write(az, sp_msg, sizeof(sp_msg));
    unsigned int microsecond = 1000000;
    usleep(3 * microsecond); //sleeps for 3 second
    write(el, el_result, sizeof(el_result));
    write(az, az_result, sizeof(az_result));
    int count = 0;
    while(count<100) {
        write(el, read_encoder, sizeof(read_encoder));
        char read_buf [256];
        memset(&read_buf, '\0', sizeof(read_buf));
        write(el, read_encoder, sizeof(read_encoder));
        int num_bytes = read(el, &read_buf, sizeof(read_buf));
        write(el, read_encoder, sizeof(read_encoder));
        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
          printf("Error reading: %s", strerror(errno));
          return 1;
        }
        
        // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
        // print it to the screen like this!)
        printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
        cout << endl;
        count = count + 1;
    }
    cout << "Motor control program finished" << endl;
    return 0;
}

void initialize_port(int serial_port) {
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
    
    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;
    
    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    
    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    unsigned char mt_msg[] = { "MT1\r" };
    write(serial_port,mt_msg,sizeof(mt_msg));
}