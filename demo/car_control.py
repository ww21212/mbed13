import time
import serial
import sys,tty,termios
class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

def get():
    inkey = _Getch()
    while(1):
        k=inkey()
        if k!='':
            break
    if k=='\x1b':
        k2 = inkey()
        k3 = inkey()
        if k3=='A':
            time.sleep(2)
            print ("go straight forward then backward")
            s.write("/goStraightCalib/run 8 \n".encode())
            time.sleep(3)
            s.write("/goStraightCalib/run -8 \n".encode())
        if k3=='B':
            time.sleep(2)
            print ("go straight backward then forward")
            s.write("/goStraightCalib/run 8 \n".encode())
            time.sleep(3)
            s.write("/goStraightCalib/run -8 \n".encode())
        if k3=='C':
            print ("right")
            s.write("/turn/run 100 -0.3 \n".encode())
        if k3=='D':
            print ("left")
            s.write("/turn/run 100 0.3 \n".encode())
        time.sleep(3)
        s.write("/stop/run \n".encode())
    elif k=='q':
        print ("quit")
        return 0
    else:
        print ("not an arrow key!")
    return 1

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])
while get():
    i = 0