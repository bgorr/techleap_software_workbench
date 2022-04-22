import serial

ser_el = serial.Serial('/dev/ttyUSB0')
ser_az = serial.Serial('/dev/ttyUSB1')
sp_msg = ('SP0\r').encode('UTF-8')
el_setting = ('FP'+str(60*45)+'\r').encode('UTF-8')
az_setting = ('FP'+str(60*45)+'\r').encode('UTF-8')
fl = ('FL\r').encode('UTF-8')

def main():
    print(el_setting)
    ser_el.write(sp_msg)
    ser_az.write(sp_msg)
    ser_el.write(el_setting)
    ser_az.write(az_setting)
    # ser_el.write(fl)
    # ser_az.write(fl)
    
if __name__ == "__main__":
    main()