from BrickPi import * #import BrickPi.py file to use BrickPi operations
from initialise import *
import movementLibrary as mv
import particles as pt
import likelihood as ly
import math

import time

def toRadians(theta):
    return (theta*math.pi)/180

def toDegrees(theta):
    return (theta*180)/math.pi

# Does steps 2,3,4 in particle filtering
def  measureNormaliseResample(particles):
    #take sonar reading, z
    N = settings['N_SAMPLING']
    for i in range(N):
        if not BrickPiUpdateValues():
            z = BrickPi.Sensor[sonar]
        #normaliseweight(particles,z)
        particles = ly.normalise_weight(particles, z)
        #resample
        particles = ly.resample(particles)
    return particles

def getThetaTarget(Theta_current, X_current, Y_current, X, Y):
    #Change current Theta to degrees
    Theta_current = toDegrees(Theta_current)

    #x, y displacements to move
    delta_x = X - X_current
    delta_y = Y - Y_current

    alpha_rad = math.atan(abs(float(delta_y)/delta_x))
    alpha = toDegrees(alpha_rad)

    ##########################
    #0-90 quadrant
    if (int(delta_y) >= 0 and int(delta_x) >= 0):
        theta = alpha
    #90-180 quadrant
    elif int(delta_y) >= 0 and int(delta_x) < 0:
        theta = 180 - alpha
    #180-270 quadrant
    elif int(delta_y) < 0 and int(delta_x) < 0:
        theta = 180 + alpha
    #270-360 quadrant if delta_y < 0 and delta_x > 0
    else:
        theta = 360 - alpha
    ##########################

    #to turn:
    theta_toTurn = theta - Theta_current
    
    ##########################
    
    #If turning counterclockwise (left)
    if theta_toTurn >= 0:
        if theta_toTurn > 180:
            theta_toTurn = -(360 - theta_toTurn)
    #Turning clockwise
    else:
        if theta_toTurn < -180:
            theta_toTurn = 360 + theta_toTurn
    
    return theta_toTurn

def navigateToWaypoint(X, Y, particles):

    #Get current position
    X_current, Y_current, Theta_current = pt.getMean(particles)
    #Theta_current in radians    

    print "======================================="
    print "i want to go to", X, Y
    print "i think i'm at", X_current, Y_current, Theta_current

    #do two turns:
    for i in xrange(1):
        theta_toTurn = getThetaTarget(Theta_current, X_current, Y_current, X, Y)
    
        #turn the robot if it isn't a really small angle:
        if int(theta_toTurn) != 0:
            print "I am now going to turn", theta_toTurn
            mv.turn(theta_toTurn, settings)
            particles = pt.turn(particles,toRadians(theta_toTurn))
            particles = measureNormaliseResample(particles)
    
    #distance to move:
    delta_x = X - X_current
    delta_y = Y - Y_current
    D = math.sqrt(delta_x**2 + delta_y**2)

    #update measurements after turn
#   X_current, Y_current, theta = pt.getMean(particles)
#   delta_x = X - X_current
#   delta_y = Y - Y_current
#   D = math.sqrt(delta_x**2 + delta_y**2)

    #move the robot:
    if int(D) != 0:
            print "I am now going to move", D
            mv.move(D, settings)
            particles = pt.move(particles,D)
            particles = measureNormaliseResample(particles)
    
    print "======================================="

    #finally, if the waypoint is one of the ones we need to visit, pause and flash the lights before going on to the next one:
    ledCheckWaypoints((X, Y))
    
    return particles

# This function checks if the (X, Y) tuple argument passed in matches one of the waypoints. If it does, flash the LED lights.
def ledCheckWaypoints(tup):
    try:
        index = WAYPOINTS.index(tup)
        #If no valueError is thrown, flash the LEDS:
        BrickPi.SensorType[light] = TYPE_SENSOR_COLOR_FULL   #Set the type of sensor at PORT_1
        BrickPiSetupSensors()
        time.sleep(1)
        BrickPi.SensorType[light] = 0
        BrickPiSetupSensors()
    except:
        pass

def pathFollow(waypoints, startPos, orientation):

    # initialising no of particles, x, y, theta
    START_X_POS = startPos[0]
    START_Y_POS = startPos[1]
    START_THETA_POS = math.radians(orientation) #eddie

    #Create particles and check current orientation
    particles = pt.initialise(NUM_OF_PARTICLES, START_X_POS, START_Y_POS, START_THETA_POS)

    #Narrow down starting location
    for i in range(3):
        particles = pt.move(particles,0)
        particles = pt.turn(particles,0)
        particles = measureNormaliseResample(particles)   
    

    # Break up waypoints into smaller waypoints
    #brokenUpWaypoints=waypoints #Don't break up the waypoints
    brokenUpWaypoints = breakUpWaypoints(waypoints)
    brokenUpWaypoints.pop(0)
    print("brokenUpWaypoints="+str(brokenUpWaypoints))

    # go to the waypoints in turn
    for w in brokenUpWaypoints:
        particles = navigateToWaypoint(w[0], w[1], particles)
        #time.sleep(1)

def breakUpWaypoints(waypoints):

    MAX_DISTANCE =  settings['MAX_DISTANCE']
    new_waypoints = []

    #initialising the first waypoint
    A = waypoints.pop(0)
    while len(waypoints) != 0:

        B = waypoints.pop(0)

        #getting the distance unit vector, ab
        delta_x = B[0] - A[0]
        delta_y = B[1] - A[1]
        distance = math.sqrt(delta_x**2 + delta_y**2)
        ab = (delta_x/distance, delta_y/distance)

        #the additional number waypoints needed to be created
        additional_waypoints = int(distance/MAX_DISTANCE)

        #append A, and then the intermediate points
        for i in range(int(additional_waypoints) + 1):
            new_waypoints.append((A[0] + i*ab[0]*MAX_DISTANCE, A[1] + i*ab[1]*MAX_DISTANCE))

        #updating the start point
        A = B

    #appending the final point
    new_waypoints.append(B)

    #new: for the first waypoint, always tentatively move 10cm, then move the rest
    #first_waypoint = new_waypoints[0]
    
    return new_waypoints
        

#####################
#       MAIN        #   
#####################

if __name__ == '__main__':

    print getThetaTarget(1.85, 84, 30, 126, 54)
