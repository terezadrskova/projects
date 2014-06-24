################################
#       INITIALISATION
################################

from BrickPi import * #import BrickPi.py file to use BrickPi operations
BrickPiSetup() # setup the serial port for communication

#Universal constants
PI = 3.1415926535

#Robot-specific movement constants

#carpet
settings = {}
settings['SPEED'] = 180 #150
settings['TURNSPEED'] = 60 #70
settings['FORWARD_CONSTANT'] = 38#41
settings['LEFT_ROTATE_CONSTANT'] = 5.5#6.6
settings['RIGHT_ROTATE_CONSTANT'] = 5.5#6.6
settings['SONAR_SPEED'] = 70 #Best not to keep varying this?
settings['SONAR_ROTATE_CONSTANT'] = 2 * 5 #Encoder to degree ratio * gear ratio
settings['LEFT_BOOST'] = 1.0
settings['RIGHT_BOOST'] = 1.03
settings['WHEEL_SEPARATION'] = 16 #cm
settings['ROTATE_CONSTANT'] = 5.5 #Alex's 5.7
settings['MAX_DISTANCE'] = 60 #999
settings['NO_BINS'] = 120
settings['N_SAMPLING'] = 3

#Ports for motors and sensors
settings['motorLeft'] = PORT_A
settings['motorRight'] = PORT_D
#settings['leftBumper'] = PORT_1
#settings['rightBumper'] = PORT_3
settings['sonar'] = PORT_2
settings['motorSonar'] = PORT_B
settings['light'] = PORT_3

NUM_OF_PARTICLES = 300
motorLeft = settings['motorLeft']
motorRight = settings['motorRight']
motorSonar = settings['motorSonar']
#leftBumper = settings['leftBumper']
#rightBumper = settings['rightBumper']
sonar = settings['sonar']
SPEED = settings['SPEED']
light = settings['light']

#Enable motors
BrickPi.MotorEnable[motorLeft] = 1 
BrickPi.MotorEnable[motorRight] = 1
BrickPi.MotorEnable[motorSonar] = 1
#BrickPi.SensorType[leftBumper] = TYPE_SENSOR_TOUCH #Set the type of sensor at PORT_1
#BrickPi.SensorType[rightBumper] = TYPE_SENSOR_TOUCH #Set the type of sensor at PORT_1
BrickPi.SensorType[sonar] = TYPE_SENSOR_ULTRASONIC_CONT
BrickPi.SensorType[light] = 0 #TYPE_SENSOR_COLOR_FULL

#BrickPi.SensorType[PORT_3] = TYPE_SENSOR_COLOR_FULL   #Set the type of sensor at PORT_1


BrickPiSetupSensors() #Send the properties of sensors to BrickPi

#WAYPOINTS

#Set of waypoints to visit, excluding helper waypoints
WAYPOINTS = [(84,30),(180,30),(126,54),(126,168),(30,54)] #Set of waypoints to visit

#Set of waypoints to visit including helper waypoints
WAYPOINTS_HELP = [(84,30),(180,30),(126,54),(126,120),(126,168),(126,54),(30,54)] #2nd (126,54) is a helper waypoint

