import time, datetime
from bme280 import BME280
from smbus import SMBus

good_time = True

class METEO:
    def __init__(self):
        self.bus = SMBus(1)
        self.bme = BME280(i2c_dev=self.bus)
        self.iter_num = 5
        self.current_temp = None
        self.current_press = None
        self.current_humid = None

    def get_temp(self):
        temps_list = []
        for i in range(self.iter_num):
            temp = self.bme.get_temperature()
            if i != 0:
                temps_list.append(temp)
            time.sleep(1)
        self.current_temp = convert_float(round(get_avg(temps_list),2))
        
    def get_press(self):
        press_list = []
        for i in range(self.iter_num):
            press = self.bme.get_pressure()
            if i != 0:
                press_list.append(press)
            time.sleep(1)
        self.current_press = convert_float(round(get_avg(press_list),2))

    def get_humid(self):
        humid_list = []
        for i in range(self.iter_num):
            humid = self.bme.get_humidity()
            if i != 0:
                humid_list.append(humid)
            time.sleep(1)
        self.current_humid = convert_float(round(get_avg(humid_list),2))

    def write_data(self):
        file_data = open('/home/raspberry/LetkaGML-Mateo/rpi-firmware/web/graph/datas.txt', "a")
        actual = open("/home/raspberry/LetkaGML-Mateo/rpi-firmware/web/data/actual.txt", "wt")
        time = get_right_time(str(datetime.datetime.now()))
        #print(time)
        file_data.write(time+';'+self.current_temp+';'+self.current_press+';'+self.current_humid+'\n')
        file_data.close()
        actual.write(time+'\n')
        actual.write(self.current_temp+'\n')
        actual.write(self.current_press+'\n')
        actual.write(self.current_humid+'\n')
        actual.close()

def get_avg(list_num):
    return sum(list_num) / len(list_num)
def convert_float(float_num): # we want to change "." separator substitude to "," for excel
    int_str = str(float_num)
    new_int = ""
    for letter in int_str:
        if letter == ".":
            new_int += ","
        else:
            new_int += letter
    return new_int
def get_right_time(time_bad): # the input is the bad setted time on the system, this function it converts to a right current time
    date_code = ''
    for letter in time_bad:
        try:
            integer = int(letter)
            date_code += letter
        except:
            pass
    day = date_code[6:8]
    month = date_code[4:6]
    year = date_code[0:4]
    hour = date_code[8:10]
    minute = date_code[10:12]
    second = date_code[12:14]
    if good_time == 0:
        time_bad = datetime.datetime(int(year),int(month),int(day),int(hour),int(minute),int(second))
        time_shift = datetime.timedelta(hours=0,minutes=0,seconds=0)
        right_time = str(time_bad + time_shift)
    else:
        right_time = day+'.'+month+'.'+year+';'+hour+':'+minute+':'+second
    #print(day,month,year,hour,minute,second)
    return right_time

meteo = METEO()

meteo.get_temp()
meteo.get_press()
meteo.get_humid()

meteo.write_data()


