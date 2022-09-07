if __name__ == '__main__':
    import time
    import psutil
    import serial
    import math

    serial = serial.Serial(port='/dev/ttyUSB0', baudrate=9600) #/dev/ttyUSB0


    def translate(value, leftMin, leftMax, rightMin, rightMax):
        # Figure out how 'wide' each range is
        leftSpan = leftMax - leftMin
        rightSpan = rightMax - rightMin

        # Convert the left range into a 0-1 range (float)
        valueScaled = float(value - leftMin) / float(leftSpan)

        # Convert the 0-1 range into a value in the right range.
        return rightMin + (valueScaled * rightSpan)


    lastTime = 0
    delay = 5000
    threshold = 19

    while True:
        if ((time.time() * 1000) - lastTime) > delay:
            temps = psutil.sensors_temperatures()
            totalTemp = 0
            values = 0
            for name, entries in temps.items():
                for entry in entries:
                    if entry.current > threshold:
                        totalTemp += entry.current
                        values += 1
            average = totalTemp / values
            fanSpeed = 100

            if 0 < average <= 25:
                fanSpeed = translate(average, 0, 25, 0, 5)
            elif 25 < average <= 35:
                fanSpeed = translate(average, 24, 35, 4, 10)
            elif 35 < average <= 45:
                fanSpeed = translate(average, 34, 45, 10, 15)
            elif 45 < average <= 55:
                fanSpeed = translate(average, 44, 55, 15, 25)
            elif 55 < average <= 65:
                fanSpeed = translate(average, 54, 65, 25, 35)
            elif 65 < average <= 75:
                fanSpeed = translate(average, 64, 75, 65, 80)
            elif 75 < average <= 85:
                fanSpeed = translate(average, 74, 85, 99, 100)

            serial.write(str(math.trunc(fanSpeed)).encode())

            lastTime = time.time() * 1000

    serial.close()