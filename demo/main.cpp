#include "mbed.h"
#include "mbed_rpc.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

void Go_StraightCalib(Arguments *in, Reply *out);
RPCFunction GOStraightCalib(&Go_StraightCalib, "goStraightCalib");

int main() {
    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table0[] = {-9.646, -9.784, -9.025, -8.445, -5.082, 0.000, 5.777, 10.364, 9.885, 9.895, 9.965};
    double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table1[] = {-8.530, -8.132, -8.690, -8.929, -4.824, 0.000, 4.829, 8.132, 8.371, 9.849, 9.769};

    // first and fourth argument : length of table                               
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

    char buf[256], outbuf[256];
    FILE *devin = fdopen(&xbee, "r");
    FILE *devout = fdopen(&xbee, "w");
    while (1) {
        memset(buf, 0, 256);
        for( int i = 0; ; i++ ) {
            char recv = fgetc(devin);
            if(recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
    RPC::call(buf, outbuf);
    }
}

void Go_StraightCalib(Arguments *in, Reply *out) {
    double arg0 = in->getArg<double>();
    car.goStraightCalib(arg0);
    //out->putData(arg0);
}