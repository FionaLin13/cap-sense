from time import sleep
from typing import List, Tuple
import csv
import serial


class ArduinoSensor:
    START_MESSAGE = 's'
    END_MESSAGE = 'e'

    def __init__(self, com_port, baudrate, timeout=1):
        self.serial = serial.Serial(com_port, baudrate, timeout=timeout)

    def start(self):
        self.serial.write(bytes(self.START_MESSAGE, 'utf-8'))

    def stop(self):
        self.serial.write(bytes(self.END_MESSAGE, 'utf-8'))

    def readline(self):
        """
        Returns a list (possibly empty) of <timestamp, raw_count>.

        Timestamp is in ms.
        """
        bytes = self.serial.readline()
        if bytes == b'':
            return None

        string = bytes.decode('utf-8')
        timestamp_str, raw_count_str = string.split(',')
        timestamp = int(timestamp_str)
        raw_count = int(raw_count_str)

        return timestamp, raw_count


class NucleoMotor:
    START_MESSAGE = 's'
    END_MESSAGE = 'e'

    def __init__(self, com_port, baudrate):
        self.serial = serial.Serial(com_port, baudrate)

    def start(self):
        self.serial.write(bytes(self.START_MESSAGE, 'utf-8'))

    def stop(self):
        self.serial.write(bytes(self.END_MESSAGE, 'utf-8'))

    def motorstatus(self):
        """
        Returns the status of stepper motor. 
        """
        bytes = self.serial.readline()
        if bytes == b'':
            return ('Motor no response')
        else:
            return (bytes.decode('utf-8'))


def main():
    arduino = ArduinoSensor('/dev/cu.usbmodem141201', 115200)
    motor = NucleoMotor('/dev/cu.usbmodem141303', 9600)

    data = []

    try:
        motor.start()
        while True:
            line = arduino.readline()
            motor_status = motor.motorstatus()
            print(line)
            print(motor_status)
            if line is not None:
                data.append(line)
        print('Experiment done.')
    except KeyboardInterrupt:
        print('Experiment stopped by hooman.')
    finally:
        print('Stopping experiment...')
        arduino.stop()
        print('Experiment stopped.')
        print('Dumping data...')
        with open('data.csv', 'w') as datafile:
            writer = csv.writer(datafile)
            writer.writerows(data)
        print('Done! Exiting.')


if __name__ == '__main__':
    main()
