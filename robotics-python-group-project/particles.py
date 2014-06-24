import time
import sys
import random
import math
import operator
import particleDataStructures as pds
#import numpy

#SIMULATED ERRORS
SIGMA_E = 0.025  #Straight line std dev
SIGMA_F = math.pi/180 * 1.83 *0.15 #Angle std dev err in radians
SIGMA_G = math.pi/180 * 2.48 *0.2 #Angle std dev err for turning

def errStraightLine(D): #Proportional error from moving in straight line
	FIXED_ERROR = 0
	return D*random.gauss(FIXED_ERROR, float(SIGMA_E))

def errStraightLineTheta(): #Angular error from moving in straight line
	return random.gauss(0, float(SIGMA_F))

def errTheta(THETA): #Proportional angular error from turning
	#THETA in radians
	return THETA*random.gauss(0, float(SIGMA_G))

def initialise(num_particles, init_x, init_y, init_theta):
    particles = [( init_x,  init_y, init_theta, float(1)/num_particles) for i in range(num_particles)]
    return particles

#MOVE()
def move(particles, D):

	updated_particles = []
	for i in range(len(particles)):
		e = errStraightLine(D)
		x = particles[i][0]
		y = particles[i][1]
		t = particles[i][2]
		w = particles[i][3]
		p = (x + (D + e)*math.cos(t),y + (D + e)*math.sin(t), ((t + errStraightLineTheta())+2*math.pi)%(2*math.pi), w)
		updated_particles.append(p)
	
	return updated_particles

#TURN()
def turn(particles, theta):
	
	updated_particles = []
	for i in range(len(particles)):
		g = errTheta(theta)
		x = particles[i][0]
		y = particles[i][1]
		t = particles[i][2]
		w = particles[i][3]
		p = (x, y, (t + theta + g +2*math.pi)%(2*math.pi), w)
		updated_particles.append(p)
	
	return updated_particles

#DRAWPARTICLES()
def drawParticles(particles):
	pds.canvas.drawParticles(particles)

#GETMEAN()
def getMean(particles):
	
	#For x,y averages
	avg_x = 0
	avg_y = 0
	
	#For unit angle average
	sum_unit_x = 0
	sum_unit_y = 0

	for p in particles:
	
		x = p[0]
		y = p[1]
		t = p[2]
		w = p[3]

		#adding in the weights
		avg_x += x*w
		avg_y += y*w
		
		sum_unit_x += math.cos(t) * w
		sum_unit_y += math.sin(t) * w

	avg_t = math.atan2(sum_unit_y,sum_unit_x)

	return [avg_x, avg_y, avg_t]


#############
#	MAIN	#
#############

#Error sampling testing
#for i in range(100):
#	print("err="+str(math.degrees(errTheta(math.radians(90)))))



