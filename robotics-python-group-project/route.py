#import RPi.GPIO as GPIO
import time

#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(12, GPIO.OUT) #GPIO 18
#GPIO.setup(13,GPIO.OUT) #GPIO 27

from initialise import *
import place_rec_bits as place

def getStartPos(index):
    return WAYPOINTS[index]

#Returns order of waypoints we ought to traverse
def routepath(start):
#Start is index returned from sonar localisation

    #Get identified coordinates
    startPos = WAYPOINTS[start]

    #Find index of start position in WAYPOINTS_HELP
    index = 0
    while(startPos != WAYPOINTS_HELP[index]):
        index += 1

    #Build up set of waypoints to traverse
    waypoints = []
    waypoints.append(startPos) #Start position
    for i in range(len(WAYPOINTS_HELP)):
        index += 1
        if(index > len(WAYPOINTS_HELP)-1):
            index = 0
        waypoints.append(WAYPOINTS_HELP[index])

    #print('Starting from pos '+str(start)+':'+str(waypoints))
    return waypoints

#Flashing LED at a waypoint
def flash_waypoint():
    return

#############
#   MAIN    #
#############

if __name__ == "__main__":
    
    BrickPi.SensorType[light] = TYPE_SENSOR_COLOR_FULL   #Set the type of sensor at PORT_1
    BrickPiSetupSensors()
    time.sleep(1)
    BrickPi.SensorType[light] = 0
    BrickPiSetupSensors()
    pass 

