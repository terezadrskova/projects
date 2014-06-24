#import place_rec_bits as prb
import run_place_main as prb
import route as rt
import pathfollow as pf

#1 Identify location and orientation
index, orientation = prb.recognize_location()
print("startIndex="+str(index)+" orientation="+str(orientation))

#index = 1

#orientation = 90

startPosition = rt.getStartPos(index) #get X,Y coords

#2 Request waypoints
waypoints = rt.routepath(index)
print("Waypoints to traverse:"+str(waypoints))

#3 Traverse waypoints
pf.pathFollow(waypoints,startPosition,orientation)







