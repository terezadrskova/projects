#!/usr/bin/env python
# By Jacek Zienkiewicz and Andrew Davison, Imperial College London, 2014
# Based on original C code by Adrien Angeli, 2009

import random
import os

from BrickPi import *
from initialise import *
import movementLibrary as mv
from particleDataStructures import Canvas
import route as rt
import time

NO_BINS = settings['NO_BINS']

# Location signature class: stores a signature characterizing one location
class LocationSignature:
    #def __init__(self, no_bins = 360):
        #self.sig = [0] * no_bins #Initialise array of zeroes
    def __init__(self, no_bins = NO_BINS):
        self.sig = [0] * no_bins #Initialise array of zeroes
    def print_signature(self):
        for i in range(len(self.sig)):
            print self.sig[i]

# --------------------- File management class ---------------
class SignatureContainer():
    def __init__(self, size = 5):
        self.size      = size; # max number of signatures that can be stored
        self.filenames = [];
        
        # Fills the filenames variable with names like loc_%%.dat 
        # where %% are 2 digits (00, 01, 02...) indicating the location number. 

        for i in range(self.size):
           self.filenames.append('loc_{0:02d}.dat'.format(i))

    # Get the index of a filename for the new signature. If all filenames are 
    # used, it returns -1;
    def get_free_index(self):
        n = 0
        while n < self.size:
            if (os.path.isfile(self.filenames[n]) == False):
                break
            n += 1
            
        if (n >= self.size):
            return -1;
        else:    
            return n;
 
    # Delete all loc_%%.dat files
    def delete_loc_files(self):
        print "STATUS:  All signature files removed."
        for n in range(self.size):
            if os.path.isfile(self.filenames[n]):
                os.remove(self.filenames[n])

    # delete loc_5
    def delete_loc_5(self):
        if os.path.isfile("loc_05.dat"):
            os.remove("loc_05.dat")
            print 'loc_05.dat deleted'
            
    # Writes the signature to the file identified by index (e.g, if index is 1
    # it will be file loc_01.dat). If file already exists, it will be replaced.
    def save(self, signature, index):
        filename = self.filenames[index]
        if os.path.isfile(filename):
            os.remove(filename)
            
        f = open(filename, 'w')

        for i in range(len(signature.sig)):
            s = str(signature.sig[i]) + "\n"
            f.write(s)
        f.close();

    # Read signature file identified by index. If the file doesn't exist
    # it returns an empty signature.
    def read(self, index):
        ls = LocationSignature()
        filename = self.filenames[index]
        if os.path.isfile(filename):
            f = open(filename, 'r')
            for i in range(len(ls.sig)):
                s = f.readline()
                if (s != ''):
                    ls.sig[i] = int(s)
            f.close();
        else:
            print "WARNING: Signature does not exist."
        
        return ls
    
       
# FILL IN: spin robot or sonar to capture a signature and store it in ls
def characterize_location(ls):
#spinning the robot for 360 degrees
######################################################################    
    motorSonar = settings['motorSonar']
    SRC = settings['SONAR_ROTATE_CONSTANT']
    SONAR_SPEED = settings['SONAR_SPEED']
    THETA = 360
    #MAX_SPEED = 40  #Top speed for sonar rotation
    #ANGULAR_VELOCITY = 40 #degrees per second
    SLEEPTIME = 0.01

    #target = abs(THETA * SRC)
    target = abs(THETA * SRC)
    #target = abs(THETA * SRC) + 20
    #Start the motor
    BrickPi.MotorSpeed[motorSonar] = SONAR_SPEED
    BrickPiUpdateValues()
    cumulatedTarget = 0
    offset = BrickPi.Encoder[motorSonar]

    #angle = 0 #angle at which to take readings    

    while(cumulatedTarget < target):
        if not BrickPiUpdateValues():
            delta = BrickPi.Encoder[motorSonar]-offset
                
            #If the sonar isn't turning, up speed
           # if(abs(float(delta))/SLEEPTIME < ANGULAR_VELOCITY*1.01 and \
           # (abs(float(delta))/SLEEPTIME > ANGULAR_VELOCITY*0.99)):
           #     break
           # elif(abs(float(delta))/SLEEPTIME < ANGULAR_VELOCITY): 
           #     BrickPi.MotorSpeed[motorSonar] += -1
           # else:
           #     BrickPi.MotorSpeed[motorSonar] -= -2

            cumulatedTarget += abs(delta)
            offset = BrickPi.Encoder[motorSonar]
            index = (int(cumulatedTarget*THETA/target)/(360/NO_BINS))%(NO_BINS)
            
            #if(index >= angle):
            reading = BrickPi.Sensor[sonar]
            if reading != -1:
                ls.sig[index] = reading
            #angle += 1

        time.sleep(SLEEPTIME)

    #Stop the sonar
    BrickPi.MotorSpeed[motorSonar] = 0
    BrickPiUpdateValues()
    cumulatedTarget = 0

    time.sleep(0.5)
    
    #Rotate the sonar back
    MAX_SPEED = 75
    target = abs(THETA * SRC)
    BrickPi.MotorSpeed[motorSonar] = -SONAR_SPEED
    BrickPiUpdateValues()
    offset = BrickPi.Encoder[motorSonar]
    while(cumulatedTarget < target):
        if not BrickPiUpdateValues():
            delta = BrickPi.Encoder[motorSonar]-offset
            #if(abs(delta) < 2 and abs(BrickPi.MotorSpeed[motorSonar])<MAX_SPEED): #If the sonar isn't turning, up speed
            #    BrickPi.MotorSpeed[motorSonar] += 1
            #else:
            #    BrickPi.MotorSpeed[motorSonar] -= 1
            cumulatedTarget += abs(delta)
            offset = BrickPi.Encoder[motorSonar]
        time.sleep(0.0005)
    
    #Stop the sonar
    BrickPi.MotorSpeed[motorSonar] = 0
    BrickPiUpdateValues()
    
######################################################################    
def to_histogram(ls):
    RANGE = 256
    histogram = [0]*RANGE
    
    for i in range(len(ls.sig)):
        histogram[ls.sig[i]] += 1

    return histogram

#def count_dist():
#count = 0
#return count

# FILL IN: compare two signatures
def compare_signatures(ls1, ls2):
    dist = 0

    histogram_ls1 = to_histogram(ls1)
    histogram_ls2 = to_histogram(ls2)
    
    if(len(histogram_ls1) != len(histogram_ls2)):
        print "Error: len(ls1)!=len(ls2)"
        return
    
    for i in range(len(histogram_ls1)):
        dist += (2*(histogram_ls1[i]-histogram_ls2[i]))**2
    
    return dist

# sends the info into the drawSonar function of the Canvas instance in particleDataStructure.py
def drawSonar(sig,X,Y):
    canvas = Canvas()
    canvas.drawSonar(sig,X,Y)

# not actually interpolating - just taking the previous non none value
def interpolateReadings(sig):
    latest_occupied = 0
    
    for i in xrange(len(sig)):
        if sig[i] != 0:
            latest_occupied = i
        else:
            sig[i] = sig[latest_occupied]
    

# This function characterizes the current location, and stores the obtained 
# signature into the next available file.
def learn_location():
    ls = LocationSignature()
    characterize_location(ls)
    #interpolateReadings(ls.sig)
    idx = signatures.get_free_index();

    if (idx == -1): # run out of signature files
        print "\nWARNING:"
        print "No signature file is available. NOTHING NEW will be learned and stored."
        print "Please remove some loc_%%.dat files.\n"
        return
    
    signatures.save(ls,idx)
    print "STATUS:  Location " + str(idx) + " learned and saved."

# This function tries to recognize the current location.
# 1.   Characterize current location# 2.   For every learned locations
# 2.1. Read signature of learned location from file
# 2.2. Compare signature to signature coming from actual characterization
# 3.   Retain the learned location whose minimum distance with
#      actual characterization is the smallest.
# 4.   Display the index of the recognized location on the screen
def recognize_location():
    ls_obs = LocationSignature()
    #print ("ls_obs")
    #ls_obs.print_signature()

    characterize_location(ls_obs)
    sigsize =  signatures.size #get change for 999, just for testing
    dist = [0] * sigsize
    temp = float('inf')
    minimumidx = -1 #initialise
    
    # FILL IN: COMPARE ls_read with ls_obs and find the best match
    for idx in xrange(sigsize):
        print "STATUS:  Comparing signature " + str(idx) + " with the observed signature."
        ls_read = signatures.read(idx);
        dist[idx] = compare_signatures(ls_obs, ls_read)
        print ("difference: "+ str(dist[idx]))
        if dist[idx] < temp:
            temp = dist[idx]
            minimumidx = idx
    ls_read = signatures.read(minimumidx)
    orientation = get_shift(ls_obs,ls_read)
       
        #print minimumidx,orientation
    return minimumidx,orientation



#def recognize_location3():
#    ls_obs = LocationSignature()
#    characterize_location(ls_obs)
#    sigsize =  signatures.size #get change for 999, just for testing
#    dist = [0] * sigsize
#    orientation = [0] * sigsize
#    temp = float('inf')
#    minimumidx = -1 #initialise
#   
#   # FILL IN: COMPARE ls_read with ls_obs and find the best match
#    for idx in xrange(sigsize):
#        print "STATUS:  Comparing signature " + str(idx) + " with the observed signature."
#        ls_read = signatures.read(idx);
#        dist[idx],orientation[idx] = get_shift(ls_obs,ls_read)
#       #dist[idx]    = compare_signatures(ls_obs, ls_read)
#        print ("difference: "+ str(dist[idx]))
#        if dist[idx] < temp:
#            temp = dist[idx]
#            minimumidx = idx
#    ls_read = signatures.read(minimumidx)
#   
#    print minimumidx,orientation
#    return minimumidx,orientation[minimumidx]


def recognize_location2(ls_obs):
    sigsize =  signatures.size #get change for 999, just for testing
    sigsize -= 1
    dist = [0] * sigsize
    temp = float('inf')

    minimumidx = -1 #initialise
    
    # FILL IN: COMPARE ls_read with ls_obs and find the best match
    for idx in xrange(sigsize):
        print "STATUS:  Comparing signature " + str(idx) + " with the observed signature."
        ls_read = signatures.read(idx);
        dist[idx]    = compare_signatures(ls_obs, ls_read)
        print ("difference: "+ str(dist[idx]))
        if dist[idx] < temp:
            temp = dist[idx]
            minimumidx = idx
    ls_read = signatures.read(minimumidx)

    orientation = get_shift(ls_obs,ls_read)
        
#       print minimumidx,orientation
    return minimumidx,orientation
    


def 




def get_shift(ls1,ls2):

    minDist = float("inf")
    orientation = -1
    
    for angle in xrange(NO_BINS):

        dist = 0

        for i in xrange(NO_BINS):
                        dist += (ls1.sig[(i+angle)%NO_BINS]-ls2.sig[i])**2
            #dist += (ls1[(i+angle)%60]-ls2[i])**2
        
        if(dist < minDist):
            minDist = dist
            orientation = angle*(360/NO_BINS)
        
    #return minDist, (360 - orientation) #In degrees
    return (360 - orientation)

# Prior to starting learning the locations, it should delete files from previous
# learning either manually or by calling signatures.delete_loc_files(). 
# Then, either learn a location, until all the locations are learned, or try to
# recognize one of them, if locations have already been learned.
signatures = SignatureContainer()

if __name__ == "__main__":
 
############
# learning
###########
#    learn_location()
#    time.sleep(0.5)
##############
# recognition and print
##############

#    signatures.delete_loc_5()
#    learn_location()
#    loc_05 = signatures.read(5)
#    index, orientation = recognize_location2(loc_05)
#    print index, orientation

###################
# recognition
##################
    index, orientation = recognize_location()
    print index, orientation

#################
# graphical
#################

#    ls = signatures.read(0)
#    drawSonar(ls.sig,84,30)
#    ls = signatures.read(1)
#    drawSonar(ls.sig,180,30)
#    ls = signatures.read(2)
#    drawSonar(ls.sig,126,54)
#    ls = signatures.read(3)
#    drawSonar(ls.sig,126,168)
#    ls = signatures.read(4)
#    drawSonar(ls.sig,30,54)
#    ls = signatures.read(5)
#    drawSonar(ls.sig,30,54)



