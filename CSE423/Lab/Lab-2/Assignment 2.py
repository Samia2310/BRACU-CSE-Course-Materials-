from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import random 

SCREEN_WIDTH = 650
SCREEN_HEIGHT = 800
FRAMES_PER_SECOND = 60

CATCHER_WIDTH = 170
CATCHER_HEIGHT = 20
DIAMOND_SIZE = 50
CATCHER_SPEED = 300
DIAMOND_INITIAL_SPEED = 100 
DIAMOND_ACCELERATION = 10

WHITE = (1.0, 1.0, 1.0)
BLACK = (0.0, 0.0, 0.0)
RED = (1.0, 0.0, 0.0)
TEAL = (0.251, 0.878, 0.816)
AMBER = (1.0, 0.75, 0.0)

score = 0
game_state = "PLAYING" 
catcher_x = SCREEN_WIDTH / 2 - CATCHER_WIDTH / 2
catcher_y = 20
diamond_x = 0
diamond_y = SCREEN_HEIGHT
diamond_speed = DIAMOND_INITIAL_SPEED
diamond_color = (1.0, 1.0, 1.0)

restart_button = [5, SCREEN_HEIGHT - 120, 100, 80] 
pause_button = [SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 120, 100, 80] 
exit_button = [SCREEN_WIDTH - 105, SCREEN_HEIGHT - 120, 100, 80] 

last_time = 0
time_step = 1000 / FRAMES_PER_SECOND 
catcher_direction = 0 

def DrawLineZone0(x1, y1, x2, y2, original_zone):
    dx = x2 - x1
    dy = y2 - y1
    d_init = 2 * dy - dx
    del_E = 2 * dy
    del_NE = 2 * (dy - dx)
    x = int(x1)
    y = int(y1)
    
    while x <= int(x2):
        ox, oy = convertFromZone0(x, y, original_zone)
        plotPixel(ox, oy) 
        if d_init > 0:
            d_init += del_NE
            y += 1
        else:
            d_init += del_E
        x += 1

def findZone(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    if abs(dx) >= abs(dy):
        if dx >= 0 and dy >= 0:
            return 0
        elif dx < 0 and dy >= 0:
            return 3
        elif dx < 0 and dy < 0:
            return 4
        else:
            return 7
    else:
        if dx >= 0 and dy >= 0:
            return 1
        elif dx < 0 and dy >= 0:
            return 2
        elif dx < 0 and dy < 0:
            return 5
        else:
            return 6

def convertToZone0(x, y, zone):
    if zone == 0: 
        return x, y
    elif zone == 1: 
        return y, x
    elif zone == 2: 
        return y, -x
    elif zone == 3: 
        return -x, y
    elif zone == 4: 
        return -x, -y
    elif zone == 5: 
        return -y, -x
    elif zone == 6: 
        return -y, x
    elif zone == 7: 
        return x, -y

def convertFromZone0(x, y, zone):
    if zone == 0: 
        return x, y
    elif zone == 1: 
        return y, x
    elif zone == 2: 
        return -y, x
    elif zone == 3: 
        return -x, y
    elif zone == 4: 
        return -x, -y
    elif zone == 5: 
        return -y, -x
    elif zone == 6: 
        return y, -x
    elif zone == 7: 
        return x, -y

def drawLine(x1, y1, x2, y2):
    zone = findZone(x1, y1, x2, y2)
    x1p, y1p = convertToZone0(x1, y1, zone)
    x2p, y2p = convertToZone0(x2, y2, zone)

    if x1p > x2p:
        x1p, y1p, x2p, y2p = x2p, y2p, x1p, y1p
    DrawLineZone0(x1p, y1p, x2p, y2p, zone)

def plotPixel(x, y):
    glBegin(GL_POINTS)
    for i in range(2): 
        for j in range(2): 
            glVertex2i(int(x + i), int(y + j))
    glEnd()

def drawCatcher():
    global catcher_x, catcher_y, CATCHER_WIDTH, CATCHER_HEIGHT
    if game_state == "GAME_OVER":
        glColor3f(*RED)
    else:
        glColor3f(*WHITE)

    inset = CATCHER_WIDTH * 0.1
    p_bl = (catcher_x + inset, catcher_y)
    p_br = (catcher_x + CATCHER_WIDTH - inset, catcher_y) 
    p_tl = (catcher_x, catcher_y + CATCHER_HEIGHT)
    p_tr = (catcher_x + CATCHER_WIDTH, catcher_y + CATCHER_HEIGHT)

    drawLine(p_bl[0], p_bl[1], p_br[0], p_br[1]) 
    drawLine(p_br[0], p_br[1], p_tr[0], p_tr[1])
    drawLine(p_tr[0], p_tr[1], p_tl[0], p_tl[1])
    drawLine(p_tl[0], p_tl[1], p_bl[0], p_bl[1])

def drawDiamond():
    global diamond_x, diamond_y, DIAMOND_SIZE, diamond_color, game_state
    
    if game_state != "GAME_OVER":
        glColor3f(*diamond_color)

        v1 = (diamond_x, diamond_y + (DIAMOND_SIZE * 0.8) / 2)
        v2 = (diamond_x + (DIAMOND_SIZE * 0.5) / 2, diamond_y)
        v3 = (diamond_x, diamond_y - (DIAMOND_SIZE * 0.8) / 2) 
        v4 = (diamond_x - (DIAMOND_SIZE * 0.5) / 2, diamond_y)

        drawLine(v1[0], v1[1], v2[0], v2[1])
        drawLine(v2[0], v2[1], v3[0], v3[1])
        drawLine(v3[0], v3[1], v4[0], v4[1])
        drawLine(v4[0], v4[1], v1[0], v1[1])

def drawButtons():
    x, y, w, h = restart_button
    h = h * 1.5
    glColor3f(*TEAL)
    arrow_center_y = y + h / 2
    drawLine(x + w * 0.75, arrow_center_y, x + w * 0.25, arrow_center_y)
    drawLine(x + w * 0.25, arrow_center_y, x + w * 0.5, arrow_center_y + h * 0.25)
    drawLine(x + w * 0.25, arrow_center_y, x + w * 0.5, arrow_center_y - h * 0.25)
    
    x, y, w, h = pause_button
    h = h * 1.5 
    glColor3f(*AMBER)
    icon_top = y + h * 0.25
    icon_bottom = y + h * 0.75
    if game_state == "PLAYING":
        gap = w * 0.15
        center_x = x + w / 2
        left_bar_x = center_x - gap
        right_bar_x = center_x + gap
        drawLine(left_bar_x, icon_top, left_bar_x, icon_bottom)
        drawLine(right_bar_x, icon_top, right_bar_x, icon_bottom)
    else:
        tri_height = icon_bottom - icon_top
        tri_half = tri_height / 2
        left_x = x + w * 0.35
        tip_x = x + w * 0.65
        mid_y = y + h / 2
        drawLine(left_x, mid_y - tri_half, tip_x, mid_y)
        drawLine(tip_x, mid_y, left_x, mid_y + tri_half)
        drawLine(left_x, mid_y + tri_half, left_x, mid_y - tri_half)

    x, y, w, h = exit_button
    h = h * 1.5
    glColor3f(*RED)
    offset_w = w * 0.25
    offset_h = h * 0.25
    drawLine(x + offset_w, y + offset_h, x + w - offset_w, y + h - offset_h)
    drawLine(x + offset_w, y + h - offset_h, x + w - offset_w, y + offset_h)

def pointBoundary(px, py, box):
    bx, by, bw, bh = box
    return bx <= px <= bx + bw and by <= py <= by + bh

def dropDiamond():
    global diamond_x, diamond_y, diamond_speed, diamond_color
    diamond_x = random.uniform(DIAMOND_SIZE / 2, SCREEN_WIDTH - DIAMOND_SIZE / 2)
    diamond_y = SCREEN_HEIGHT
    diamond_speed = DIAMOND_INITIAL_SPEED
    diamond_color = (random.uniform(0.3, 1.0), random.uniform(0.3, 1.0), random.uniform(0.3, 1.0))

def hasCollide(box1, box2):
    return (box1[0] < box2[0] + box2[2] and
            box1[0] + box1[2] > box2[0] and
            box1[1] < box2[1] + box2[3] and
            box1[1] + box1[3] > box2[1])

def restartGame():
    global score, game_state, diamond_speed, catcher_x
    score = 0
    diamond_speed = DIAMOND_INITIAL_SPEED
    game_state = "PLAYING"
    catcher_x = SCREEN_WIDTH / 2 - CATCHER_WIDTH / 2
    print("Starting Over")
    dropDiamond()

def togglePause():
    global game_state
    if game_state == "PLAYING":
        game_state = "PAUSED"
    elif game_state == "PAUSED":
        game_state = "PLAYING"

def display():
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    drawCatcher()
    drawDiamond()
    drawButtons()
    glutSwapBuffers()

def idle():
    global diamond_y, diamond_speed, game_state, score, catcher_x, last_time, time_step, catcher_direction
    
    current_time = glutGet(GLUT_ELAPSED_TIME)
    delta_time = current_time - last_time
    
    if delta_time >= time_step:
        last_time = current_time

        if game_state == "PLAYING":
            diamond_y -= diamond_speed * (delta_time / 1000.0)
            diamond_speed += DIAMOND_ACCELERATION * (delta_time / 1000.0)

            if catcher_direction == 1: 
                catcher_x = max(0, catcher_x - CATCHER_SPEED * (delta_time / 1000.0))
            elif catcher_direction == 2:
                catcher_x = min(SCREEN_WIDTH - CATCHER_WIDTH, catcher_x + CATCHER_SPEED * (delta_time / 1000.0))

            diamond_box = [diamond_x - (DIAMOND_SIZE * 0.5) / 2, diamond_y - (DIAMOND_SIZE * 0.8) / 2,
                           (DIAMOND_SIZE * 0.5) / 2 * 2, (DIAMOND_SIZE * 0.8) / 2 * 2] 
            catcher_box = [catcher_x, catcher_y, CATCHER_WIDTH, CATCHER_HEIGHT]

            if hasCollide(catcher_box, diamond_box):
                score += 1
                print(f"Score: {score}")
                dropDiamond()
            elif diamond_y - DIAMOND_SIZE / 2 <= 0:
                game_state = "GAME_OVER"
                print(f"Game Over! Score: {score}")
    
    glutPostRedisplay()

def specialKeyListener(key, x, y):
    global catcher_direction
    if key == GLUT_KEY_LEFT:
        catcher_direction = 1
    elif key == GLUT_KEY_RIGHT:
        catcher_direction = 2

def specialKeyUpListener(key, x, y):
    global catcher_direction
    if key == GLUT_KEY_LEFT or key == GLUT_KEY_RIGHT:
        catcher_direction = 0

def mouseListener(button, state, x, y):
    global score
    if button == GLUT_LEFT_BUTTON and state == GLUT_DOWN:
        my = SCREEN_HEIGHT - y
        if pointBoundary(x, my, restart_button):
            restartGame()
        elif pointBoundary(x, my, pause_button):
            togglePause()
        elif pointBoundary(x, my, exit_button):
            print(f"Goodbye. Score: {score}")
            glutLeaveMainLoop()
            quit()

def main():
    global last_time
    glutInit()
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT)
    glutInitWindowPosition(0, 0)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutCreateWindow(b"Catch the Diamonds!")

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    glutDisplayFunc(display)
    glutIdleFunc(idle)
    glutSpecialFunc(specialKeyListener)
    glutSpecialUpFunc(specialKeyUpListener)
    glutMouseFunc(mouseListener)
    
    dropDiamond()

    last_time = glutGet(GLUT_ELAPSED_TIME)

    glutMainLoop()

if __name__ == "__main__":
    main()
