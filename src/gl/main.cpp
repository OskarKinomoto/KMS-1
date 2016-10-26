#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <vector>
#include <fstream>
#include "Frame.h"

#define VIEWING_DISTANCE_MIN  3.0

bool g_bLightingEnabled = true;
bool g_bFillPolygons = true;
bool g_bTexture = false;
bool g_bButton1Down = false;

GLfloat g_fViewDistance = 3 * VIEWING_DISTANCE_MIN;
GLfloat g_nearPlane = 1;
GLfloat g_farPlane = 1000;

int g_Width = 600;                          // Initial window width
int g_Height = 600;                         // Initial window height

int g_yClick = 0;

float g_lightPos1[4] = { 10, 10, -100, 1 };  // Position of light
float g_lightPos2[4] = { 10, -10, 100, 1 };  // Position of light

float g_RotateX = 0;
float g_RotateY = 0;

Real L;

timeval last_idle_time;

std::vector<Frame> frames;
size_t f_pos = 0;
double d_pos = 0;

void PrintString(const std::string &ref) {
    glPushMatrix();
    glutWireSphere(L, 20, 10);
    glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPushMatrix(); // save
    glLoadIdentity();// and clear
    glMatrixMode( GL_MODELVIEW ) ;
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_DEPTH_TEST ) ;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9, 0.9f);

    for (size_t i = 0; i < ref.size(); ++i)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ref.c_str()[i]);

    glEnable(GL_LIGHTING);
    glEnable( GL_DEPTH_TEST ) ;

    glMatrixMode( GL_PROJECTION ) ;
    glPopMatrix() ; // revert back to the matrix I had before.
    glMatrixMode( GL_MODELVIEW ) ;
    glPopMatrix();

}

void RenderObjects(void)
{
    float colorBlue[4]       = { 0.0, 0.0, 0.8, 1.0 };
    float colorBlue2[4]       = { 0.0, 0.2, 1.0, 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue2);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    auto &frame = frames.at(d_pos);

    for (auto &at : frame.at()) {
        glPushMatrix();
        glTranslatef(at.x(), at.y(), at.z());
        glutSolidSphere(.2, 16, 10);
        glPopMatrix();
    }

    PrintString(std::to_string(frame.T));
}

void display(void)
{
    // Clear frame buffer and depth buffer
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up viewing transformation, looking down -Z axis
    glLoadIdentity();
    gluLookAt(0, 0, -g_fViewDistance,
              0, 0, -1,
              0, 1, 0);

    // Set up the stationary light
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos1);

    glRotatef( g_RotateX, 0, 1, 0 );
    glRotatef( g_RotateY, 0, 0, 1 );

    // Render the scene
    RenderObjects();

    // Make sure changes appear onscreen
    glutSwapBuffers();
}

void reshape(GLint width, GLint height)
{
    g_Width = width;
    g_Height = height;

    glViewport(0, 0, g_Width, g_Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (float)g_Width / g_Height, g_nearPlane, g_farPlane);
    glMatrixMode(GL_MODELVIEW);
}

void InitGraphics(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int oldY = 0;
int oldX = 0;

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (!g_bButton1Down && state == GLUT_DOWN) {
            oldX = x;
            oldY = y;
        }
        g_bButton1Down = (state == GLUT_DOWN) ? true : false;
        g_yClick = y - 3 * g_fViewDistance;
    }
}

void MouseMotion(int x, int y)
{
    if (g_bButton1Down)
    {
        g_RotateX -= oldX - x;
        g_RotateY += oldY - y;
        oldY = y;
        oldX = x;
    }
}

void AnimateScene(void)
{
    float dt;

    timeval time_now;

    gettimeofday(&time_now, NULL);

    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
            1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);

    d_pos = (d_pos + 40 * dt);
    if (d_pos > frames.size()) {
        d_pos = 0;
    }

    // Save time_now for next time
    last_idle_time = time_now;
}

void Keyboard(unsigned char key, int /* x */, int /* y */)
{
    switch (key)
    {
    case 27:
        exit (0);
        break;
    }
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/20, timer, 0);
}

int main(int argc, char** argv)
{
    std::ifstream file("dat.xyz", std::ios_base::in);
    Frame frame;
    while (true) {
        file >> frame;
        if (frame.at().size() == 0)
            break;
        frames.push_back(frame);
    }

    std::ifstream conf("parm.conf");
    if (conf.is_open()) {
        std::string drop;
        Real rdrop;
        conf >> drop >> rdrop;
        conf >> drop >> rdrop;
        conf >> drop >> L;
    }

    glutInit (&argc, argv);
    glutInitWindowSize (g_Width, g_Height);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("Ar simulation");

    // Initialize OpenGL graphics state
    InitGraphics();

    // Register callbacks:
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (Keyboard);
    glutMouseFunc (MouseButton);
    glutMotionFunc (MouseMotion);
    glutIdleFunc (AnimateScene);
    glutTimerFunc(1000/20, timer, 0);

    // Get the initial time, for use by animation
    gettimeofday (&last_idle_time, nullptr);

    // Turn the flow of control over to GLUT
    glutMainLoop ();
    return 0;
}




