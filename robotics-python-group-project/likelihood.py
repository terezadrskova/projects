##################### 
#yuan_likelihood.py
#####################

from BrickPi import * #import BrickPi.py file to use BrickPi operations
from initialise import *
import movementLibrary as mv
import particles as pt
import particleDataStructures as pds
import math
import random

def calculate_likelihood(x, y, theta, z):
# x,y,theta: coordinates of a particle
# z: sonar measurement
	
	DIST_FROM_WHEEL_TO_SONAR = 0 #cm
	#print '------START---'
	z = z + DIST_FROM_WHEEL_TO_SONAR
	#Get expected distance to wall
	m = expected_depth(x,y,theta,pds.mymap.walls)	#print("Expected dist to wall: "+str(m))
	#print("Sonar reading:"+str(z))
	#calculate the likelihood
	likelihood = k_sqr_exp(z,m)
	#print("Likelihood="+str(likelihood))
	#pds.mymap.draw();
	#print '------END---'
	return likelihood
#end calculate_likelihood

def k_sqr_exp(z,m):

	K = 0.01 
	SIGMA = 2.7
	
	num = -(z-m)**2
	den = 2*(SIGMA**2)
	exponent = float(num)/den
	return 1000*math.exp(exponent) + K

#end k_sqr_exp

def expected_depth(x, y, theta, walls):

	expectedDepth = float("inf")
	i=0
	for wall in walls:
		i=i+1
		#print("i="+str(i))
		Ax = float(wall[0])
		Ay = float(wall[1])
		Bx = float(wall[2])
		By = float(wall[3])
		
		num = (By-Ay)*(Ax-x)-(Bx-Ax)*(Ay-y)
		den = (By-Ay)*math.cos(theta) - (Bx-Ax)*math.sin(theta)
		
		#if walls is parallel to robot
		if float(den) == 0:
			continue
		else:
			m = float(num)/den #Distance to a particular wall 
			#print("m="+str(m))
			if(m>0 and m<=expectedDepth and between(m,x,y,theta,wall)):
				expectedDepth = m		
	
	#print("expectedDepth="+str(expectedDepth))
	return expectedDepth
#end expected_depth

def between(m,x,y,theta,wall):
		
		Ax = int(wall[0])
		Ay = int(wall[1])
		Bx = int(wall[2])
		By = int(wall[3])

		X = int(math.ceil(x + m*math.cos(theta)))
		Y = int(math.ceil(y + m*math.sin(theta)))
		
		#print("AX="+str(Ax)+" AY="+str(Ay))
		#print("BX="+str(Bx)+" BY="+str(By))
		#print("X="+str(X)+" Y="+str(Y))
		if(((X>=Ax and X<=Bx) and (Y>=Ay and Y<=By) ) or \
		   ((X<=Ax and X>=Bx) and (Y<=Ay and Y>=By))):
			#print("Returning true")
			return True
		else:
			#print("Returning false")
			return False
#end between

def normalise(particles):
	total_weight = 0

	for p in particles:
		total_weight += p[3]

	new_particles = []

	for i in range(len(particles)):
		new_particles.append((particles[i][0],particles[i][1],particles[i][2], \
								particles[i][3]/total_weight))

	return new_particles

#need to adjust the particle library for weights; particles as classes?
def normalise_weight(particles, z):
	
	new_particles = []

	for p in particles:
		X = p[0]
		Y = p[1]
		theta = p[2]
		w = p[3]
		new_particles.append((X,Y,theta,calculate_likelihood(X,Y,theta,z)*w))

	#Print to web interface
	
	new_particles = normalise(new_particles)
	pt.drawParticles(new_particles)
 
	return new_particles
#end normalise

def resample(particles):
	
	NUMBEROFPARTICLES = len(particles)
	updated_particles = []
	
	#cumulative probability array
	weights = [particles[0][3]]
	for i in range(1, NUMBEROFPARTICLES):
		weights.append(particles[i][3] + weights[i-1])

	# total 'probability' aka cumulative likelihood
	total = weights[-1]

	for i in range(NUMBEROFPARTICLES):
		rand = random.uniform(0, total)
		index = 0
		while rand > weights[index]:
			index += 1

		#print i, index, rand, weights, NUMBEROFPARTICLES
		updated_particles.append((particles[index][0], particles[index][1], particles[index][2], float(1)/NUMBEROFPARTICLES))
	
	#pt.drawParticles(particles)

	return updated_particles

#For debug
def print_particles(particles,opt):
	
	total_weight = 0
	
	for i in length(particles):
		X = particles[i][0]
		Y = particles[i][1]
		theta = particles[i][2]
		weight = particles[i][3]
		total_weight += weight
		if(opt):
			print("particle "+str(i)+" X="+str(X)+" Y="+str(Y)+" theta="+str(theta))
		print("total_weight="+str(total_weight))
	
#############
#	MAIN	#
#############

#Likelihood function testing
#z=80
#m=79
#print("z="+str(z)+" m="+str(m))
#print k_sqr_exp(z,m)
#z=80
#m=77
#print("z="+str(z)+" m="+str(m))
#print k_sqr_exp(z,m)
#z=80
#m=75
#print("z="+str(z)+" m="+str(m))
#print k_sqr_exp(z,m)
#z=80
#m=70
#print("z="+str(z)+" m="+str(m))
#print k_sqr_exp(z,m)
