#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* mkdir */
#include <sys/stat.h>
#include <sys/types.h>

#include <vector>
#include <fstream>
#include <thread>
#include "Frame.h"

#define npng
#ifdef png
#include "png.h"
#endif

constexpr double FPS = 120;

bool leftBtnDown = false;

GLfloat viewScaleL = 2.3;

GLfloat viewScale = 1;
GLfloat g_fViewDistance = 3 * 1.5f;
GLfloat g_nearPlane = 1;
GLfloat g_farPlane = 1000;

int width = 1600;
int height = 900;

float g_lightPos[4] = { 10, 10, -100, 1 };

float g_RotateX = 60;
float g_RotateY = 0;

Real L;

timeval last_idle_time;

std::vector<Frame> frames;
size_t f_pos = -1;
double d_pos = 0;

bool run = true;
bool info = true;
bool showSphere = false;

int frame_num = 0;
int last_t = 0;
int anim_num = 0;
int o_i = 0;
float ttime = 0;

#ifdef png
bool screen_rec = true;
float time_mul = .2;
#else
bool screen_rec = false;
float time_mul = 1;
#endif



#ifdef png
void shot(std::string file_name)
{
    png_structp png_ptr;
    png_infop info_ptr;

    FILE *fp = fopen(file_name.c_str(), "wb");

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr));

    png_init_io(png_ptr, fp);
    setjmp(png_jmpbuf(png_ptr));

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    setjmp(png_jmpbuf(png_ptr));

    png_byte row[3*width + 1];

    GLubyte pixels[width*height*3];
    glReadPixels(0, 0, width, height, GL_RGB,
                 GL_UNSIGNED_BYTE, &pixels);

    GLubyte *ptr = pixels + width * 3 * (height - 1);
    for (int y = 0 ;y < height; y++) {
        for (int x = 0; x < width; x++) {
            row[x*3] = *ptr++;
            row[x*3 + 1] = *ptr++;
            row[x*3 + 2] = *ptr++;
        }
        png_write_row(png_ptr, row);
        ptr -= width * 3 * 2;
    }

    setjmp(png_jmpbuf(png_ptr));

    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
}
#endif

void PrintString(const std::string &ref, float y) {
    y /= 15;

    glMatrixMode( GL_PROJECTION );
    glPushMatrix(); // save
    glLoadIdentity();// and clear
    glMatrixMode( GL_MODELVIEW ) ;
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_DEPTH_TEST ) ;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, y);

    for (size_t i = 0; i < ref.size(); ++i)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ref.data()[i]);

    glEnable(GL_LIGHTING);
    glEnable( GL_DEPTH_TEST ) ;

    glMatrixMode( GL_PROJECTION ) ;
    glPopMatrix() ; // revert back to the matrix I had before.
    glMatrixMode( GL_MODELVIEW ) ;
    glPopMatrix();

}

std::string toString(int i) {
    char r[5] = {'0', '0', '0', '0', 0};
    int p = 3;
    while (i) {
        r[p] = i % 10 + '0';
        i /= 10;
        p--;
    }
    return r;
}

void RenderObjects(void)
{
    float colorBlue[4]       = { 0.0, 0.0, 0.8, 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);

    auto &frame = frames.at(d_pos);

    for (auto &at : frame.at()) {
        glPushMatrix();
        glTranslatef(at.x(), at.y(), at.z());
        glutSolidSphere(.2, 16, 10);
        glPopMatrix();
    }

    if (showSphere) {
        glPushMatrix();
        glutWireSphere(L, 20, 10);
        glPopMatrix();
    }

    frame_num++;

    if (info) {
        float tpos = 15;
        PrintString(frame.t_str, --tpos);
        PrintString(frame.T_str, --tpos);
    }


#ifdef png
    if(last_t != static_cast<int>(d_pos) && screen_rec) {
        last_t = static_cast<int>(d_pos);
        std::cout << last_t << std::endl;
        shot("/tmp/out/" + toString(o_i++) + ".png");
    }
#endif


}

void display(void)
{
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0, 0, -g_fViewDistance * viewScale,
              0, 0, -1,
              0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);

    glRotatef( g_RotateX, 0, 1, 0 );
    glRotatef( g_RotateY, 0, 0, 1 );

    RenderObjects();

    glutSwapBuffers();
}

void reshape(GLint width, GLint height)
{
    width = width;
    height = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (float)width / height, g_nearPlane, g_farPlane);
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
        if (!leftBtnDown && state == GLUT_DOWN) {
            oldX = x;
            oldY = y;
        }
        leftBtnDown = (state == GLUT_DOWN);
    }
}

void MouseMotion(int x, int y)
{
    if (leftBtnDown)
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

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(333 / FPS)));

    gettimeofday(&time_now, NULL);

    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
            1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);

    if (run) {
        d_pos += 40 * dt * time_mul;
        if (d_pos > frames.size()) {
            run = false;
            d_pos = 0;
        }
    }

    ttime += dt;
    anim_num++;

    last_idle_time = time_now;
}

void Keyboard(unsigned char key, int /* x */, int /* y */)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'r':
        run = !run;
        break;
    case ' ':
        showSphere = !showSphere;
        break;
    case '+':
        time_mul *= 1.2;
        break;
    case '-':
        time_mul /= 1.2;
        break;
    case '0':
        time_mul = 1;
        break;
    case 'i':
        info = !info;
        break;
    }
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

void readFile() {
    std::ifstream file("dat.xyz", std::ios_base::in);
    Frame frame;
    while (true) {
        file >> frame;
        if (frame.at().size() == 0)
            break;
        frames.push_back(std::move(frame));
    }

    std::ifstream conf("parm.conf", std::ios_base::in);
    if (conf.is_open()) {
        std::string drop;
        drop.resize(100);
        Real rdrop;
        conf >> drop >> rdrop;
        conf >> drop >> rdrop;
        conf >> drop >> L;
    }

    viewScale =  L / viewScaleL;
}



int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);
    readFile();

    if(screen_rec) {
        mkdir("/tmp/out", 0777);
    }

    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Ar simulation");

    InitGraphics();

    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (Keyboard);
    glutMouseFunc (MouseButton);
    glutMotionFunc (MouseMotion);
    glutIdleFunc (AnimateScene);
    glutTimerFunc(1000 / FPS, timer, 0);

    gettimeofday (&last_idle_time, nullptr);

    glutMainLoop ();

    return 0;
}




