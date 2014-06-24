##################### 
#movementLibrary.py
#####################
from BrickPi import * 
from initialise import *

#Alex's move
def move(N,settings):
    
    motorLeft = settings['motorLeft']
    motorRight = settings['motorRight']
    FORWARD_CONSTANT = settings['FORWARD_CONSTANT']
    SPEED = settings['SPEED']
    WHEEL_SEPARATION = settings['WHEEL_SEPARATION']

    # N is distance to move in cm (+/-)
    if abs(N) < 0.1:
        return

    target = N * FORWARD_CONSTANT #dist to travel in half degrees
    travelled = 0

    while BrickPiUpdateValues():
        pass # Ensure that the update happens

    startLeft  = BrickPi.Encoder[motorLeft]
    startRight = BrickPi.Encoder[motorRight]
    travelledLeft  = 0
    travelledRight = 0
    deltaLeft  = 0
    deltaRight = 0

    startTime = time.time()
    elapsedTime = 0
    deltaTime = 0

    if N > 0: # moving forward
        BrickPi.MotorSpeed[motorLeft]  = -SPEED
        BrickPi.MotorSpeed[motorRight] = -SPEED
    else:
        BrickPi.MotorSpeed[motorLeft] =  SPEED
        BrickPi.MotorSpeed[motorRight] = SPEED

    while abs(travelled) < abs(target):
        if not BrickPiUpdateValues():

            # calculate incremental rotation of each wheel
            deltaLeft  = BrickPi.Encoder[motorLeft]   - travelledLeft  - startLeft
            deltaRight = BrickPi.Encoder[motorRight]  - travelledRight - startRight
            # update rotation since start
            travelledLeft  =  BrickPi.Encoder[motorLeft]  - startLeft
            travelledRight =  BrickPi.Encoder[motorRight] - startRight
            # update time
            deltaTime   = time.time() - startTime - elapsedTime
            elapsedTime = time.time() - startTime
            
            common = (deltaRight + deltaLeft) * (deltaRight - deltaLeft) / (2 * WHEEL_SEPARATION)
            dy_dt   = common / deltaTime
            y       = common / 2
            d2y_dt2 = common / (deltaTime ** 2)

            K_u = 0.011

            # # PID With some overshoot
            P_e = 0.33 * K_u
            P_i = 2 * P_e / deltaTime
            P_d = P_e * deltaTime / 3

            # No PID
            # P_e = P_i = P_d = 0

            # # PD Only
            # P_e = 0.8 * K_u
            # P_i = 0
            # P_d = P_i * deltaTime / 8

            # # 'Classic PID'
            # P_e = 0.6 * K_u
            # P_i = 2 * P_e / deltaTime
            # P_d = P_e * deltaTime / 8

            # # 'No Overshoot'
            # P_e = 0.2 * K_u
            # P_i = 2 * P_e / deltaTime
            # P_d = P_e * deltaTime / 3

            # # Pessen Integral Rule
            # P_e = 0.7 * K_u
            # P_i = 2.5 * P_e / deltaTime
            # P_d = 0.15* P_e * deltaTime

            correction = P_e * dy_dt + P_i * y + P_d * P_d * d2y_dt2

            if correction > 0:
                correction = min(correction, 3)
            else:
                correction = max(correction, -3)

            BrickPi.MotorSpeed[motorLeft]  -= int(correction)
            BrickPi.MotorSpeed[motorRight] += int(correction)  

            while BrickPiUpdateValues():
                pass # Ensure that the update happens

            travelled = min(travelledLeft, travelledRight)

        time.sleep(0.2)

    BrickPi.MotorSpeed[motorLeft] = 0
    BrickPi.MotorSpeed[motorRight] = 0
    while BrickPiUpdateValues():
        pass # Ensure that the update happens

#def move(N, settings):
## N is distance to move in cm (+/-)
#   print('Moving:'+str(N))
#   motorLeft = settings['motorLeft']
#   motorRight = settings['motorRight']
#   FORWARD_CONSTANT = settings['FORWARD_CONSTANT']
#   SPEED = settings['SPEED']
#
#   if(N > 0): #if moving forward
#
#       BrickPi.MotorSpeed[motorLeft] = -SPEED
#       BrickPi.MotorSpeed[motorRight] = int(-SPEED*1.05)
#
#   else:
#       BrickPi.MotorSpeed[motorLeft] = SPEED
#       BrickPi.MotorSpeed[motorRight] = int(SPEED*1.02)
#
#   BrickPiUpdateValues()
#
#   target = abs(N * FORWARD_CONSTANT) #dist to travel in half degrees
#   cumulatedTarget = 0
#   
#   offsetLeft = None
#   offsetRight = None
#   deltaLeft = 0
#   deltaRight = 0  
#   
#   while(cumulatedTarget < target): 
#       if not BrickPiUpdateValues():
#           if offsetLeft is None:
#               offsetLeft = BrickPi.Encoder[motorLeft]
#               offsetRight = BrickPi.Encoder[motorRight]
#           else:
#               deltaLeft = BrickPi.Encoder[motorLeft]-offsetLeft
#               deltaRight = BrickPi.Encoder[motorRight]-offsetRight
#               cumulatedTarget += min(abs(deltaLeft),abs(deltaRight))
#               
#               offsetLeft = BrickPi.Encoder[motorLeft]
#               offsetRight = BrickPi.Encoder[motorRight]
#               
#               #Gain control for the turn
#               Kp = float(1)/9000 #7200
#               err = float(deltaLeft - deltaRight)*Kp #Convert error to reasonable value
#               #print float(deltaLeft - deltaRight)
#               #print float(err)
#               if err > 0: #If left wheel turning too fast
#                   BrickPi.MotorSpeed[motorLeft] -= int(err)       
#                   BrickPi.MotorSpeed[motorRight] += int(err)      
#               else:
#                   BrickPi.MotorSpeed[motorLeft] += int(err)       
#                   BrickPi.MotorSpeed[motorRight] -= int(err)      
#       
#       time.sleep(0.01)
#
#   BrickPi.MotorSpeed[motorLeft] = 0
#   BrickPi.MotorSpeed[motorRight] = 0
#   BrickPiUpdateValues()
#   #time.sleep(0.5)

#Alex's turn
#def turn(theta,settings):
#    motorLeft = settings['motorLeft']
#    motorRight = settings['motorRight']
#    ROTATE_CONSTANT = settings['ROTATE_CONSTANT']
#    SPEED = settings['SPEED']
#    WHEEL_SEPARATION = settings['WHEEL_SEPARATION']
#    
#    if abs(theta) < 0.1:
#        return
#
#    if theta > 0: #if turning counterclockwise(left)
#        turnLeft = True
#        left_speed  =  SPEED
#        right_speed = -SPEED
#    else:
#        turnLeft = False
#        left_speed  = -SPEED
#        right_speed =  SPEED
#
#    BrickPi.MotorSpeed[motorLeft] =  left_speed
#    BrickPi.MotorSpeed[motorRight] = right_speed
#
#    while BrickPiUpdateValues():
#        pass # Ensure that the update happens
#
#    target = abs(theta * ROTATE_CONSTANT)  # dist to travel in half degrees
#    turned = 0
#
#    offsetLeft  = BrickPi.Encoder[motorLeft]
#    offsetRight = BrickPi.Encoder[motorRight]
#
#    # acceleration_factor = 1
#
#    while turned < target:
#
#        if not BrickPiUpdateValues():
#
#            # Truly horrible, I'm sorry. At low speeds turning does nothing!!
#            #if turned < (target * 0.25):
#            #    acceleration_factor = max(0.7, float(turned) / (target / 4))
#            #elif turned > (target * 0.75):
#            #    acceleration_factor = max(0.7, 1.75 - (float(turned) / target))
#            #else:
#            #    acceleration_factor = 1
#
#            deltaLeft  = BrickPi.Encoder[motorLeft]  - offsetLeft
#            deltaRight = BrickPi.Encoder[motorRight] - offsetRight
#
#            turned += min(abs(deltaLeft),abs(deltaRight))
#            
#            offsetLeft = BrickPi.Encoder[motorLeft]
#            offsetRight = BrickPi.Encoder[motorRight]
#
#            #GAIN CONTROL
#            Kp = float(1)/9000 #7200
#            err = float(abs(deltaLeft) + abs(deltaRight))*Kp #Convert error to reasonable value
#                #print float(deltaLeft - deltaRight)
#                #print 'error on rotate:'
#            # print int(err)
#            if(turnLeft):
#                    #print("err:"+str(err)+" dLeft:"+str(deltaLeft)+" dRight:"+str(deltaRight))
#                if err > 0: #If left wheel turning too fast
#                    BrickPi.MotorSpeed[motorLeft]  = left_speed  - int(err)
#                    BrickPi.MotorSpeed[motorRight] = right_speed + int(err)
#                else:
#                    BrickPi.MotorSpeed[motorLeft]  = left_speed  + int(err)
#                    BrickPi.MotorSpeed[motorRight] = right_speed - int(err)
#            else:
#                if err < 0: #If right wheel turning too fast
#                    BrickPi.MotorSpeed[motorLeft]  = left_speed  - int(err)
#                    BrickPi.MotorSpeed[motorRight] = right_speed + int(err)
#                else:
#                    BrickPi.MotorSpeed[motorLeft]  = left_speed  + int(err)
#                    BrickPi.MotorSpeed[motorRight] = right_speed - int(err)
#
#        time.sleep(0.01)
#
#    BrickPi.MotorSpeed[motorLeft]=0
#    BrickPi.MotorSpeed[motorRight]=0
#    while BrickPiUpdateValues():
#        pass # Ensure that the update happens

def turn(THETA,settings):
    print('Turning:'+str(THETA))

    motorLeft = settings['motorLeft']
    motorRight = settings['motorRight']
    LRC = settings['LEFT_ROTATE_CONSTANT']
    RRC = settings['RIGHT_ROTATE_CONSTANT']
    SPEED = settings['TURNSPEED']
    leftBoost = settings['LEFT_BOOST']
    rightBoost = settings['RIGHT_BOOST']
    
    if(THETA > 0): #if turning counterclockwise(left)
        turnLeft = True;
        BrickPi.MotorSpeed[motorLeft] = int(SPEED*leftBoost)
        BrickPi.MotorSpeed[motorRight] = -SPEED
        target = abs(THETA * LRC)
    else:
        turnLeft = False;
        BrickPi.MotorSpeed[motorLeft] = -SPEED
        BrickPi.MotorSpeed[motorRight] = int(SPEED*rightBoost)
        target = abs(THETA * RRC)

    BrickPiUpdateValues()

    cumulatedTarget = 0
    #cLeft = 0
    #cRight = 0
    offsetLeft = BrickPi.Encoder[motorLeft]
    offsetRight = BrickPi.Encoder[motorRight]

    while(cumulatedTarget < target):
        if not BrickPiUpdateValues():
            deltaLeft = BrickPi.Encoder[motorLeft]-offsetLeft
            deltaRight = BrickPi.Encoder[motorRight]-offsetRight
                
            #Take average of encoders
            cumulatedTarget += (abs(deltaLeft)+abs(deltaRight))/2

            offsetLeft = BrickPi.Encoder[motorLeft]
            offsetRight = BrickPi.Encoder[motorRight]
        time.sleep(0.01)

    BrickPi.MotorSpeed[motorLeft]= 0
    BrickPi.MotorSpeed[motorRight]= 0
    BrickPiUpdateValues()

if __name__ == '__main__':
    
    move(100,settings)

