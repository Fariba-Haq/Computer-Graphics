#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define PI 3.14159265358979323846
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 750
#define ANIMATION_INTERVAL 50

// ==========================================
// GLOBAL STATE & NAVIGATION
// ==========================================
// 0: Instruction, 1: Nature, 2: Wildlife, 3: Wedding, 4: Street (Car)
int currentScene = 0;

void updateAudio() {
    PlaySound(NULL, 0, 0); // Stop all sounds

    if (currentScene == 1) {
        PlaySound(TEXT("nature.wav"), NULL, SND_ASYNC | SND_FILENAME);
    } else if (currentScene == 2) {
        PlaySound(TEXT("project_sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    } else if (currentScene == 3) {
        PlaySound(TEXT("Wedding.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    } else if (currentScene == 4) {
        PlaySound(TEXT("mixkit-cars-starting-1561.wav"), NULL, SND_ASYNC | SND_LOOP);
    }
}

// ==========================================
// SCENE 0: INSTRUCTION PAGE
// ==========================================
void renderBitmapString(float x, float y, float z, void *font, const char *string) {
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void displayInstruction() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    renderBitmapString(-0.12f, 0.90f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Instruction");

    // Navigation Info
    renderBitmapString(-0.92f, 0.80f, 0.0f, GLUT_BITMAP_HELVETICA_18, ">> Use LEFT / RIGHT Arrow Keys to switch between screens <<");

    // 1. Nature
    renderBitmapString(-0.92f, 0.70f, 0.0f, GLUT_BITMAP_HELVETICA_18, "1. Nature Scenario:");
    renderBitmapString(-0.88f, 0.64f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   Press SPACE BAR   ->  Move clouds");
    renderBitmapString(-0.88f, 0.58f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   Mouse Click       ->  Flash + Take picture");

    // 2. Wildlife
    renderBitmapString(-0.92f, 0.48f, 0.0f, GLUT_BITMAP_HELVETICA_18, "2. Wildlife Scenario:");
    renderBitmapString(-0.88f, 0.42f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   UP / DOWN ARROW   -> Increase/Decrease speed");
    renderBitmapString(-0.88f, 0.36f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   R: Reset | SPACE: Pause | A: Animals | C: Clouds | D: Day/Night");

    // 3. Wedding
    renderBitmapString(-0.92f, 0.26f, 0.0f, GLUT_BITMAP_HELVETICA_18, "3. Wedding Scenario:");
    renderBitmapString(-0.88f, 0.20f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   UP / DOWN         -> Increase / Decrease speed");
    renderBitmapString(-0.88f, 0.14f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   J / L             -> Rotate photographer");
    renderBitmapString(-0.88f, 0.08f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   R: Reset | SPACE: Pause | M: Mute");

    // 4. Street / Car
    renderBitmapString(-0.92f, -0.02f, 0.0f, GLUT_BITMAP_HELVETICA_18, "4. Street Scenario:");
    renderBitmapString(-0.88f, -0.08f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   Press 'M' or 'm'  ->  Mute/Unmute sound");
    renderBitmapString(-0.88f, -0.14f, 0.0f, GLUT_BITMAP_HELVETICA_18, "   Car will start automatically after 8 seconds");

    renderBitmapString(-0.92f, -0.30f, 0.0f, GLUT_BITMAP_HELVETICA_18, "Press ESC to exit the program.");
    glFlush();
}

// ==========================================
// SCENE 1: NATURE SCENARIO
// ==========================================
GLfloat n_position = 0.00f;
GLfloat n_speed = 0.00f;
int n_photoCount = 0;
float n_sun = 0.7f;

void n_cloud(float cx, float cy, float rX, float rY) {
    int triangleAmount = 50;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= triangleAmount; i++) {
        float angle = i * 2.0f * PI / triangleAmount;
        glVertex2f(cx + rX * cos(angle), cy + rY * sin(angle));
    }
    glEnd();
}

void n_drawCloudSet() {
    glColor3f(0.97f, 0.97f, 0.97f);
    n_cloud(-0.70f, 0.76f, 0.11f, 0.05f); n_cloud(-0.54f, 0.79f, 0.13f, 0.06f); n_cloud(-0.38f, 0.76f, 0.11f, 0.05f); n_cloud(-0.54f, 0.72f, 0.18f, 0.03f);
    n_cloud(0.10f, 0.83f, 0.12f, 0.05f); n_cloud(0.25f, 0.86f, 0.14f, 0.06f); n_cloud(0.40f, 0.83f, 0.12f, 0.05f); n_cloud(0.25f, 0.79f, 0.20f, 0.03f);
    n_cloud(0.65f, 0.72f, 0.10f, 0.05f); n_cloud(0.78f, 0.75f, 0.12f, 0.06f); n_cloud(0.91f, 0.72f, 0.10f, 0.05f);
    n_cloud(-0.10f, 0.66f, 0.08f, 0.04f); n_cloud(0.02f, 0.69f, 0.10f, 0.05f); n_cloud(0.14f, 0.66f, 0.08f, 0.04f);
}

void n_circle(float cx, float cy, float r) {
    int triangleAmount = 50;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= triangleAmount; i++) {
        float angle = i * 2.0f * PI / triangleAmount;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void n_flower(float x, float y, float s, float pr, float pg, float pb) {
    glLineWidth(2.0f); glColor3f(0.10f, 0.55f, 0.15f);
    glBegin(GL_LINES); glVertex2f(x, y); glVertex2f(x, y + 0.06f*s); glEnd();
    glLineWidth(1.0f);
    float cx = x; float cy = y + 0.075f*s;
    glColor3f(pr, pg, pb);
    n_circle(cx + 0.016f*s, cy, 0.010f*s); n_circle(cx - 0.016f*s, cy, 0.010f*s);
    n_circle(cx, cy + 0.016f*s, 0.010f*s); n_circle(cx, cy - 0.016f*s, 0.010f*s);
    n_circle(cx + 0.012f*s, cy + 0.012f*s, 0.010f*s); n_circle(cx - 0.012f*s, cy + 0.012f*s, 0.010f*s);
    glColor3f(1.0f, 0.85f, 0.10f); n_circle(cx, cy, 0.008f*s);
}

void displayNature() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.5f, 0.1f); glVertex2f(-1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
    glColor3f(0.2f, 0.4f, 0.8f); glVertex2f(1.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON); glColor3f(0.55f, 0.40f, 0.25f);
    glVertex2f(-1.0f, -1.0f); glVertex2f(-1.0f, -0.18f); glVertex2f(-0.49f, -0.08f);
    glVertex2f(-0.21f, -0.15f); glVertex2f(0.0f, -0.05f); glVertex2f(0.28f, -0.14f);
    glVertex2f(0.49f, -0.07f); glVertex2f(1.0f, -0.12f); glVertex2f(1.0f, -1.0f);
    glEnd();

    GLfloat radius = 0.08f; int triangleAmount = 150;
    glColor3f(1.0f, 0.8f, 0.2f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(0.7f, n_sun);
    for (int i = 0; i <= triangleAmount; i++) {
        float angle = i * 2.0f * PI / triangleAmount;
        glVertex2f(0.7f + radius * 0.5f * cos(angle), n_sun + radius * sin(angle));
    }
    glEnd();

    glColor3f(0.65f, 0.85f, 0.55f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.05f, 0.0f); glVertex2f(-0.82f, 0.38f); glVertex2f(-0.60f, 0.0f);
    glVertex2f(-0.68f, 0.0f); glVertex2f(-0.35f, 0.5f); glVertex2f(-0.07f, 0.0f);
    glVertex2f(-0.14f, 0.0f); glVertex2f(0.14f, 0.6f); glVertex2f(0.42f, 0.0f);
    glVertex2f(0.28f, 0.0f); glVertex2f(0.56f, 0.45f); glVertex2f(0.84f, 0.0f);
    glVertex2f(0.70f, 0.0f); glVertex2f(0.9f, 0.33f); glVertex2f(1.0f, 0.0f);
    glEnd();

    glColor3f(0.3f, 0.7f, 0.4f);
    for (float x = -1.0f; x < 1.0f; x += 0.35f) {
        glBegin(GL_TRIANGLE_FAN); glVertex2f(x, -0.1f);
        for (int i = 0; i <= 60; i++) {
            float angle = i * PI / 60;
            glVertex2f(x + 0.3f * cos(angle), -0.1f + 0.2f * sin(angle));
        } glEnd();
    }

    glColor3f(0.15f, 0.75f, 0.30f);
    for (float x = -1.0f; x <= 1.0f; x += 0.06f) {
        glBegin(GL_QUADS);
        glVertex2f(x, -1.0f); glVertex2f(x + 0.02f, -1.0f); glVertex2f(x + 0.05f, -0.90f); glVertex2f(x + 0.03f, -0.90f);
        glEnd();
    }

    glPushMatrix();
    glTranslatef(-0.65f, -0.25f, 0.0f);
    glColor3f(0.2f, 0.4f, 0.8f);
    glBegin(GL_QUADS); glVertex2f(-0.08f, 0.15f); glVertex2f(0.08f, 0.15f); glVertex2f(0.08f, 0.38f); glVertex2f(-0.08f, 0.38f); glEnd();
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_LINES); glVertex2f(-0.08f, 0.32f); glVertex2f(-0.18f, 0.25f); glVertex2f(0.08f, 0.32f); glVertex2f(0.18f, 0.25f); glEnd();
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS); glVertex2f(-0.06f, -0.15f); glVertex2f(-0.01f, -0.15f); glVertex2f(-0.01f, 0.15f); glVertex2f(-0.06f, 0.15f); glVertex2f(0.01f, -0.15f); glVertex2f(0.06f, -0.15f); glVertex2f(0.06f, 0.15f); glVertex2f(0.01f, 0.15f); glEnd();
    glColor3f(0.5f, 0.3f, 0.5f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(0.0f, 0.45f);
    for (int i = 0; i <= 100; i++) glVertex2f(0.0f + 0.07f * cos(i * 2.0f * PI / 100), 0.45f + 0.07f * sin(i * 2.0f * PI / 100));
    glEnd();
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS); glVertex2f(-0.12f, 0.26f); glVertex2f(0.12f, 0.26f); glVertex2f(0.12f, 0.34f); glVertex2f(-0.12f, 0.34f); glEnd();
    glPopMatrix();

    glPushMatrix(); glTranslatef(n_position, 0.0f, 0.0f); n_drawCloudSet(); glPopMatrix();
    glPushMatrix(); glTranslatef(n_position - 2.0f, 0.0f, 0.0f); n_drawCloudSet(); glPopMatrix();

    glFlush();
}

void nature_update(int value) {
    if (currentScene == 1) {
        if (n_position > 1.0f) n_position = -1.2f;
        n_position += n_speed;
        glutPostRedisplay();
    }
    glutTimerFunc(100, nature_update, 0);
}

void nature_key(unsigned char key, int x, int y) {
    if (key == ' ') n_speed = 0.02f;
}

void nature_mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && n_photoCount < 3) {
        n_photoCount++;
        glutPostRedisplay();
    }
}

// ==========================================
// SCENE 2: WILDLIFE SCENARIO
// ==========================================
GLfloat wild_giraffe1PosX = -0.3f, wild_giraffe2PosX = 0.1f, wild_zebraPosX = -0.8f, wild_birdPos = 0.0f, wild_cloudPos = 0.0f, wild_speed = 0.01f;
bool wild_isDay = true, wild_animalsMoving = true, wild_cloudsMoving = true, wild_globalPaused = false;
const float rayAngles[12] = {0.0f, 30.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f, 210.0f, 240.0f, 270.0f, 300.0f, 330.0f};

void wild_drawText(float x, float y, const char* text) {
    glColor3f(0.0f, 0.0f, 0.0f); glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

void wild_drawCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int triangleAmount = 100; GLfloat twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(x, y);
    for(int i = 0; i <= triangleAmount; i++) glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
    glEnd();
}

void wild_drawTree(GLfloat x, GLfloat baseY) {
    glColor3f(0.45f, 0.3f, 0.15f);
    glBegin(GL_QUADS); glVertex2f(x - 0.02f, baseY); glVertex2f(x + 0.02f, baseY); glVertex2f(x + 0.015f, baseY + 0.15f); glVertex2f(x - 0.015f, baseY + 0.15f); glEnd();
    glColor3f(0.2f, 0.5f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.08f, baseY + 0.2f); glVertex2f(x - 0.07f, baseY + 0.25f); glVertex2f(x - 0.03f, baseY + 0.26f); glVertex2f(x + 0.03f, baseY + 0.26f);
    glVertex2f(x + 0.07f, baseY + 0.25f); glVertex2f(x + 0.08f, baseY + 0.2f); glVertex2f(x + 0.05f, baseY + 0.15f); glVertex2f(x - 0.05f, baseY + 0.15f);
    glEnd();
}

void wild_drawGiraffe(GLfloat x, GLfloat y) {
    glColor3f(0.82f, 0.65f, 0.35f);
    glBegin(GL_QUADS); glVertex2f(x - 0.05f, y + 0.08f); glVertex2f(x + 0.06f, y + 0.08f); glVertex2f(x + 0.06f, y + 0.2f); glVertex2f(x - 0.05f, y + 0.2f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 0.03f, y + 0.2f); glVertex2f(x + 0.055f, y + 0.2f); glVertex2f(x + 0.06f, y + 0.5f); glVertex2f(x + 0.035f, y + 0.5f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 0.03f, y + 0.5f); glVertex2f(x + 0.08f, y + 0.5f); glVertex2f(x + 0.08f, y + 0.56f); glVertex2f(x + 0.03f, y + 0.56f); glEnd();
    glColor3f(0.0f, 0.0f, 0.0f); wild_drawCircle(x + 0.045f, y + 0.53f, 0.007f); wild_drawCircle(x + 0.065f, y + 0.53f, 0.007f);
    glColor3f(0.82f, 0.65f, 0.35f);
    glBegin(GL_QUADS); glVertex2f(x + 0.01f, y); glVertex2f(x + 0.025f, y); glVertex2f(x + 0.025f, y + 0.08f); glVertex2f(x + 0.01f, y + 0.08f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 0.035f, y); glVertex2f(x + 0.05f, y); glVertex2f(x + 0.05f, y + 0.08f); glVertex2f(x + 0.035f, y + 0.08f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 0.035f, y); glVertex2f(x - 0.02f, y); glVertex2f(x - 0.02f, y + 0.08f); glVertex2f(x - 0.035f, y + 0.08f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 0.01f, y); glVertex2f(x + 0.005f, y); glVertex2f(x + 0.005f, y + 0.08f); glVertex2f(x - 0.01f, y + 0.08f); glEnd();
}

void wild_drawPhotographer() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS); glVertex2f(-0.88f, -0.4f); glVertex2f(-0.72f, -0.4f); glVertex2f(-0.75f, -0.3f); glVertex2f(-0.85f, -0.3f); glEnd();
    glColor3f(0.65f, 0.55f, 0.4f);
    glBegin(GL_QUADS); glVertex2f(-0.83f, -0.3f); glVertex2f(-0.77f, -0.3f); glVertex2f(-0.77f, -0.2f); glVertex2f(-0.83f, -0.2f); glEnd();
    glColor3f(0.75f, 0.6f, 0.5f); wild_drawCircle(-0.8f, -0.16f, 0.035f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(4.0f); glBegin(GL_LINES);
    glVertex2f(-0.73f, -0.25f); glVertex2f(-0.78f, -0.4f); glVertex2f(-0.73f, -0.25f); glVertex2f(-0.68f, -0.4f); glVertex2f(-0.73f, -0.25f); glVertex2f(-0.73f, -0.4f); glEnd();
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS); glVertex2f(-0.76f, -0.25f); glVertex2f(-0.7f, -0.25f); glVertex2f(-0.7f, -0.21f); glVertex2f(-0.76f, -0.21f); glEnd();
}

void wild_drawCloud(GLfloat x, GLfloat y) {
    if (wild_isDay) glColor3f(0.95f, 0.95f, 0.95f); else glColor3f(0.7f, 0.7f, 0.8f);
    wild_drawCircle(x, y, 0.05f); wild_drawCircle(x + 0.04f, y, 0.04f); wild_drawCircle(x - 0.04f, y, 0.04f);
    wild_drawCircle(x + 0.02f, y + 0.03f, 0.035f); wild_drawCircle(x - 0.02f, y + 0.03f, 0.035f);
}

void wild_drawBird(GLfloat x, GLfloat y) {
    glColor3f(0.2f, 0.2f, 0.2f); glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.02f, y); glVertex2f(x - 0.01f, y + 0.01f); glVertex2f(x, y); glVertex2f(x + 0.01f, y + 0.01f); glVertex2f(x + 0.02f, y);
    glEnd();
}

void wild_drawZebra(GLfloat x, GLfloat y) {
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS); glVertex2f(x - 0.06f, y + 0.05f); glVertex2f(x + 0.04f, y + 0.05f); glVertex2f(x + 0.04f, y + 0.15f); glVertex2f(x - 0.06f, y + 0.15f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 0.02f, y + 0.15f); glVertex2f(x + 0.045f, y + 0.15f); glVertex2f(x + 0.05f, y + 0.28f); glVertex2f(x + 0.025f, y + 0.28f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 0.025f, y + 0.28f); glVertex2f(x + 0.065f, y + 0.28f); glVertex2f(x + 0.065f, y + 0.33f); glVertex2f(x + 0.025f, y + 0.33f); glEnd();
    glColor3f(0.0f, 0.0f, 0.0f); wild_drawCircle(x + 0.035f, y + 0.305f, 0.005f); wild_drawCircle(x + 0.055f, y + 0.305f, 0.005f);
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS); glVertex2f(x + 0.01f, y); glVertex2f(x + 0.022f, y); glVertex2f(x + 0.022f, y + 0.05f); glVertex2f(x + 0.01f, y + 0.05f); glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 0.03f, y); glVertex2f(x - 0.018f, y); glVertex2f(x - 0.018f, y + 0.05f); glVertex2f(x - 0.03f, y + 0.05f); glEnd();
    glColor3f(0.0f, 0.0f, 0.0f); glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.05f, y + 0.06f); glVertex2f(x - 0.05f, y + 0.14f); glVertex2f(x - 0.03f, y + 0.06f); glVertex2f(x - 0.03f, y + 0.14f); glVertex2f(x - 0.01f, y + 0.06f); glVertex2f(x - 0.01f, y + 0.14f);
    glVertex2f(x + 0.03f, y + 0.06f); glVertex2f(x + 0.03f, y + 0.14f); glEnd();
}

void wild_drawBush(GLfloat x, GLfloat y) {
    glColor3f(0.3f, 0.5f, 0.25f); wild_drawCircle(x, y, 0.04f); wild_drawCircle(x - 0.03f, y - 0.01f, 0.035f); wild_drawCircle(x + 0.03f, y - 0.01f, 0.035f); wild_drawCircle(x, y + 0.03f, 0.03f);
}

void wild_drawFlowers(GLfloat x, GLfloat y) {
    glColor3f(0.2f, 0.6f, 0.2f); glLineWidth(2.0f); glBegin(GL_LINES); glVertex2f(x, y); glVertex2f(x, y + 0.04f); glEnd();
    glColor3f(1.0f, 0.4f, 0.6f); wild_drawCircle(x, y + 0.04f, 0.01f); wild_drawCircle(x - 0.008f, y + 0.045f, 0.008f); wild_drawCircle(x + 0.008f, y + 0.045f, 0.008f); wild_drawCircle(x - 0.008f, y + 0.035f, 0.008f); wild_drawCircle(x + 0.008f, y + 0.035f, 0.008f);
    glColor3f(1.0f, 0.8f, 0.0f); wild_drawCircle(x, y + 0.04f, 0.005f);
}

void displayWildlife() {
    if (wild_isDay) glClearColor(0.6f, 0.82f, 0.95f, 1.0f); else glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    if (wild_isDay) glColor3f(0.6f, 0.82f, 0.95f); else glColor3f(0.1f, 0.1f, 0.2f);
    glVertex2f(-1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    if (wild_isDay) glColor3f(0.8f, 0.92f, 1.0f); else glColor3f(0.2f, 0.2f, 0.3f);
    glVertex2f(1.0f, -0.4f); glVertex2f(-1.0f, -0.4f);
    glEnd();

    if (wild_isDay) glColor3f(1.0f, 0.9f, 0.2f); else glColor3f(0.9f, 0.9f, 0.95f);
    wild_drawCircle(0.75f, 0.75f, 0.08f);

    wild_drawCloud(fmod(wild_cloudPos, 2.0f) - 1.0f, 0.7f); wild_drawCloud(fmod(wild_cloudPos + 0.8f, 2.0f) - 1.0f, 0.78f);
    wild_drawCloud(fmod(wild_cloudPos + 0.4f, 2.0f) - 1.0f, 0.65f); wild_drawCloud(fmod(wild_cloudPos + 1.2f, 2.0f) - 1.0f, 0.72f);

    wild_drawBird(fmod(wild_birdPos, 2.4f) - 1.2f, 0.6f); wild_drawBird(fmod(wild_birdPos + 0.05f, 2.4f) - 1.2f, 0.62f); wild_drawBird(fmod(wild_birdPos + 0.1f, 2.4f) - 1.2f, 0.58f);

    glColor3f(0.5f, 0.45f, 0.35f); glBegin(GL_TRIANGLES); glVertex2f(-1.0f, -0.4f); glVertex2f(-0.6f, 0.0f); glVertex2f(-0.2f, -0.4f); glEnd();
    glColor3f(0.55f, 0.5f, 0.4f); glBegin(GL_TRIANGLES); glVertex2f(-0.4f, -0.4f); glVertex2f(0.0f, 0.05f); glVertex2f(0.4f, -0.4f); glEnd();
    glColor3f(0.6f, 0.55f, 0.45f); glBegin(GL_TRIANGLES); glVertex2f(0.2f, -0.4f); glVertex2f(0.6f, -0.05f); glVertex2f(1.0f, -0.4f); glEnd();

    glColor3f(0.85f, 0.75f, 0.58f);
    glBegin(GL_QUADS); glVertex2f(-1.0f, -0.4f); glVertex2f(1.0f, -0.4f); glVertex2f(1.0f, -1.0f); glVertex2f(-1.0f, -1.0f); glEnd();

    wild_drawBush(-0.25f, -0.38f); wild_drawBush(0.55f, -0.38f); wild_drawBush(-0.75f, -0.38f); wild_drawBush(0.85f, -0.38f);
    wild_drawFlowers(-0.9f, -0.4f); wild_drawFlowers(-0.85f, -0.4f); wild_drawFlowers(0.6f, -0.4f); wild_drawFlowers(0.65f, -0.4f);

    wild_drawTree(-0.55f, -0.4f); wild_drawTree(0.82f, -0.4f); wild_drawTree(0.95f, -0.4f); wild_drawTree(-0.15f, -0.4f); wild_drawTree(-0.95f, -0.4f); wild_drawTree(0.3f, -0.4f);
    wild_drawPhotographer();

    GLfloat zebraX = fmod(wild_zebraPosX + 1.5f, 3.0f) - 1.5f; wild_drawZebra(-0.5f + zebraX, -0.4f);
    GLfloat giraffe1X = fmod(wild_giraffe1PosX + 1.2f, 2.4f) - 1.2f; wild_drawGiraffe(0.15f + giraffe1X, -0.4f);
    GLfloat giraffe2X = fmod(wild_giraffe2PosX + 1.3f, 2.6f) - 1.3f; wild_drawGiraffe(0.45f + giraffe2X, -0.4f);

    char statusText[256];
    snprintf(statusText, sizeof(statusText), "CONTROLS: Up/Down=Speed | Space=Pause | R=Reset");
    wild_drawText(-0.95f, 0.95f, statusText);
    snprintf(statusText, sizeof(statusText), "A=Animals %s | C=Clouds %s | D=%s", wild_animalsMoving ? "ON" : "OFF", wild_cloudsMoving ? "ON" : "OFF", wild_isDay ? "Day" : "Night");
    wild_drawText(-0.95f, 0.90f, statusText);

    glFlush();
}

void wild_update(int value) {
    if (currentScene == 2 && !wild_globalPaused) {
        if (wild_cloudsMoving) { wild_cloudPos += 0.005f; if(wild_cloudPos > 2.0f) wild_cloudPos -= 2.0f; }
        if (wild_animalsMoving) {
            wild_birdPos += 0.015f * wild_speed/0.01f; if(wild_birdPos > 2.4f) wild_birdPos -= 2.4f;
            wild_zebraPosX += wild_speed * 1.2f; if(wild_zebraPosX > 3.0f) wild_zebraPosX -= 3.0f;
            wild_giraffe1PosX += wild_speed; if(wild_giraffe1PosX > 2.4f) wild_giraffe1PosX -= 2.4f;
            wild_giraffe2PosX += wild_speed * 0.7f; if(wild_giraffe2PosX > 2.6f) wild_giraffe2PosX -= 2.6f;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(ANIMATION_INTERVAL, wild_update, 0);
}

void wild_special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) wild_speed += 0.005f;
    if (key == GLUT_KEY_DOWN && wild_speed > 0.005f) wild_speed -= 0.005f;
    glutPostRedisplay();
}

void wild_key(unsigned char key, int x, int y) {
    switch(key) {
        case 'r': case 'R': wild_giraffe1PosX = -0.3f; wild_giraffe2PosX = 0.1f; wild_zebraPosX = -0.8f; wild_birdPos = 0.0f; wild_cloudPos = 0.0f; wild_speed = 0.01f; wild_isDay = true; wild_animalsMoving = true; wild_cloudsMoving = true; wild_globalPaused = false; break;
        case ' ': wild_globalPaused = !wild_globalPaused; break;
        case 'a': case 'A': wild_animalsMoving = !wild_animalsMoving; break;
        case 'c': case 'C': wild_cloudsMoving = !wild_cloudsMoving; break;
        case 'd': case 'D': wild_isDay = !wild_isDay; break;
    }
    glutPostRedisplay();
}

// ==========================================
// SCENE 3: WEDDING SCENARIO
// ==========================================
float w_groomPos = 0.0f, w_bridePos = 0.0f, w_moveSpeed = 0.0011f, w_fanAngle = 0.0f, w_lightTime = 0.0f, w_flashTimer = 0.05f, w_photographerRotation = 0.0f;
bool w_cameraFlash = false, w_musicOn = true;

void w_drawFilledCircle(float cx, float cy, float r, int segments = 100) {
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) { float a = 2.0f * PI * i / segments; glVertex2f(cx + r * cosf(a), cy + r * sinf(a)); }
    glEnd();
}

void w_drawSemiCircle(float cx, float cy, float r, bool top = true, int segments = 50) {
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    float start = top ? 0.0f : PI; float end = top ? PI : 2.0f * PI;
    for (int i = 0; i <= segments; i++) { float a = start + (end - start) * i / segments; glVertex2f(cx + r * cosf(a), cy + r * sinf(a)); }
    glEnd();
}

void w_drawStandFan(float x, float y) {
    glColor3ub(80,80,80); w_drawFilledCircle(x, y, 0.05f);
    glColor3ub(120,120,120); glBegin(GL_QUADS); glVertex2f(x-0.01f,y); glVertex2f(x+0.01f,y); glVertex2f(x+0.01f,y+0.25f); glVertex2f(x-0.01f,y+0.25f); glEnd();
    glColor3ub(100,100,100); w_drawFilledCircle(x, y+0.27f, 0.03f);
    glColor3ub(180,180,180); w_drawFilledCircle(x, y+0.27f, 0.08f);
    glPushMatrix(); glTranslatef(x, y+0.27f, 0); glRotatef(w_fanAngle,0,0,1);
    glColor3ub(255,215,0);
    glBegin(GL_QUADS);
    glVertex2f(-0.01f,0); glVertex2f(0.01f,0); glVertex2f(0.01f,0.06f); glVertex2f(-0.01f,0.06f);
    glVertex2f(-0.01f,0); glVertex2f(0.01f,0); glVertex2f(0.01f,-0.06f); glVertex2f(-0.01f,-0.06f);
    glVertex2f(0,-0.01f); glVertex2f(0,0.01f); glVertex2f(0.06f,0.01f); glVertex2f(0.06f,-0.01f);
    glVertex2f(0,-0.01f); glVertex2f(0,0.01f); glVertex2f(-0.06f,0.01f); glVertex2f(-0.06f,-0.01f);
    glEnd(); glPopMatrix();
}

void displayWedding() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS); glColor3ub(167, 185, 209); glVertex2f(-1.0f, -0.1f); glVertex2f(1.0f, -0.1f); glVertex2f(1.0f, 1.0f); glVertex2f(-1.0f, 1.0f); glEnd();
    glBegin(GL_QUADS); glColor3ub(139, 90, 43); glVertex2f(-1.0f, 0.1f); glVertex2f(1.0f, 0.1f); glVertex2f(0.9f, 0.35f); glVertex2f(-0.9f, 0.35f); glEnd();
    glBegin(GL_QUADS); glColor3ub(196, 209, 167); glVertex2f(-0.9f, 0.35f); glVertex2f(0.9f, 0.35f); glVertex2f(0.9f, 1.0f); glVertex2f(-0.9f, 1.0f); glEnd();

    glLineWidth(4.0f); glColor3ub(139, 115, 85);
    glBegin(GL_LINE_STRIP); for (float x = -0.8f; x <= 0.8f; x += 0.05f) glVertex2f(x, 0.88f + 0.03f * sinf(x * 12.0f)); glEnd();
    for (int i = 0; i < 10; i++) {
        float x = -0.75f + i * 0.167f; float y = 0.88f + 0.03f * sinf(x * 12.0f);
        float glow = (sinf(w_lightTime * 3.0f + i) + 1.0f) * 0.5f;
        glColor3ub(200 + int(55 * glow), 200 + int(55 * glow), 120); w_drawFilledCircle(x, y, 0.05f);
        glColor3ub(255, 255, 200); w_drawFilledCircle(x, y, 0.025f);
    }

    w_drawStandFan(-0.83f, 0.28f); w_drawStandFan(0.83f, 0.28f);

    glColor3ub(15, 43, 84);
    glBegin(GL_QUADS); glVertex2f(-0.30f, 0.50f); glVertex2f(0.30f, 0.50f); glVertex2f(0.50f, 0.55f); glVertex2f(-0.50f, 0.55f); glVertex2f(-0.30f, 0.55f); glVertex2f(0.30f, 0.55f); glVertex2f(0.45f, 0.70f); glVertex2f(-0.45f, 0.70f); glEnd();

    float gx = -0.2f + w_groomPos; float g_base = 0.45f;
    glColor3ub(0, 0, 0); glBegin(GL_QUADS); glVertex2f(gx - 0.06f, g_base); glVertex2f(gx + 0.06f, g_base); glVertex2f(gx + 0.06f, g_base - 0.10f); glVertex2f(gx - 0.06f, g_base - 0.10f); glEnd();
    glColor3ub(0, 0, 0); glBegin(GL_QUADS); glVertex2f(gx - 0.09f, g_base); glVertex2f(gx + 0.09f, g_base); glVertex2f(gx + 0.09f, g_base + 0.20f); glVertex2f(gx - 0.09f, g_base + 0.20f); glEnd();
    glColor3ub(255, 220, 180); w_drawFilledCircle(gx, g_base + 0.25f, 0.046f);
    glColor3ub(0, 0, 0); w_drawSemiCircle(gx, g_base + 0.25f, 0.050f, true);

    float bx = 0.20f + w_bridePos; float b_base = 0.45f;
    glColor3ub(255, 255, 255); glBegin(GL_POLYGON); glVertex2f(bx - 0.10f, b_base); glVertex2f(bx + 0.10f, b_base); glVertex2f(bx + 0.14f, b_base + 0.07f); glVertex2f(bx + 0.12f, b_base + 0.17f); glVertex2f(bx, b_base + 0.20f); glVertex2f(bx - 0.12f, b_base + 0.17f); glVertex2f(bx - 0.14f, b_base + 0.07f); glEnd();
    glColor3ub(255, 220, 180); w_drawFilledCircle(bx, b_base + 0.25f, 0.046f);
    glColor3ub(139, 69, 19); w_drawSemiCircle(bx, b_base + 0.25f, 0.054f, true);

    glColor3ub(139, 90, 43); glBegin(GL_QUADS); glVertex2f(-1.0f, -1.0f); glVertex2f(-1.0f, 0.1f); glVertex2f(1.0f, 0.1f); glVertex2f(1.0f, -1.0f); glEnd();
    glColor3ub(170, 0, 0); glBegin(GL_QUADS); glVertex2f(-0.18f, -1.0f); glVertex2f( 0.18f, -1.0f); glVertex2f( 0.12f,  0.30f); glVertex2f(-0.12f,  0.30f); glEnd();
    glColor3ub(245, 245, 245); glBegin(GL_QUADS); glVertex2f(-0.05f, -1.0f); glVertex2f( 0.05f, -1.0f); glVertex2f( 0.03f,  0.30f); glVertex2f(-0.03f,  0.30f); glEnd();

    float ph_x = 0.0f; float ph_y = -0.65f;
    glPushMatrix(); glTranslatef(ph_x, ph_y + 0.15f, 0); glRotatef(w_photographerRotation, 0, 0, 1); glTranslatef(-ph_x, -(ph_y + 0.15f), 0);
    glColor3ub(30, 30, 30); glBegin(GL_QUADS); glVertex2f(ph_x - 0.08f, ph_y + 0.15f); glVertex2f(ph_x + 0.08f, ph_y + 0.15f); glVertex2f(ph_x + 0.08f, ph_y + 0.30f); glVertex2f(ph_x - 0.08f, ph_y + 0.30f); glEnd();
    glColor3ub(80, 80, 80); w_drawFilledCircle(ph_x, ph_y + 0.23f, 0.04f);
    glLineWidth(6.0f); glColor3ub(40, 40, 40);
    glBegin(GL_LINES); glVertex2f(ph_x - 0.05f, ph_y + 0.15f); glVertex2f(ph_x - 0.25f, ph_y - 0.30f); glVertex2f(ph_x, ph_y + 0.15f); glVertex2f(ph_x, ph_y - 0.30f); glVertex2f(ph_x + 0.05f, ph_y + 0.15f); glVertex2f(ph_x + 0.25f, ph_y - 0.30f); glEnd();
    glColor3ub(70, 130, 180); glBegin(GL_QUADS); glVertex2f(ph_x - 0.10f, ph_y); glVertex2f(ph_x + 0.10f, ph_y); glVertex2f(ph_x + 0.10f, ph_y + 0.22f); glVertex2f(ph_x - 0.10f, ph_y + 0.22f); glEnd();
    glColor3ub(255, 220, 180); w_drawFilledCircle(ph_x, ph_y + 0.25f, 0.055f);
    glPopMatrix();

    if (w_cameraFlash) {
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1,1,1,0.8f); glBegin(GL_QUADS); glVertex2f(-1,-1); glVertex2f( 1,-1); glVertex2f( 1, 1); glVertex2f(-1, 1); glEnd();
        glDisable(GL_BLEND); w_cameraFlash = false;
    }
    glFlush();
}

void wed_moveCouple(int value) {
    if (currentScene == 3) {
        if (w_groomPos < 0.08f) w_groomPos += w_moveSpeed;
        if (w_bridePos > -0.08f) w_bridePos -= w_moveSpeed;
        glutPostRedisplay();
    }
    glutTimerFunc(30, wed_moveCouple, 0);
}

void wed_Idle() {
    if (currentScene == 3) {
        w_fanAngle += 2.0f; if (w_fanAngle > 360.0f) w_fanAngle -= 360.0f;
        w_lightTime += 0.002f;
        w_flashTimer += 0.02f;
        if (w_flashTimer > 6.0f) { w_cameraFlash = true; w_flashTimer = 0.0f; }
        glutPostRedisplay();
    }
}

void wed_special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) w_moveSpeed += 0.0003f;
    if (key == GLUT_KEY_DOWN && w_moveSpeed > 0.0003f) w_moveSpeed -= 0.0003f;
    glutPostRedisplay();
}

void wed_key(unsigned char key, int x, int y) {
    static bool paused = false;
    switch(key) {
        case 'r': case 'R': w_groomPos = 0.0f; w_bridePos = 0.0f; w_moveSpeed = 0.0011f; w_photographerRotation = 0.0f; break;
        case 'j': case 'J': w_photographerRotation -= 2.0f; break;
        case 'l': case 'L': w_photographerRotation += 2.0f; break;
        case 'm': case 'M':
            if (w_musicOn) { PlaySound(NULL, 0, 0); w_musicOn = false; }
            else { PlaySound(TEXT("Wedding.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP); w_musicOn = true; } break;
        case ' ': paused = !paused; if (paused) w_moveSpeed = 0.0f; else w_moveSpeed = 0.0011f; break;
    }
    glutPostRedisplay();
}

// ==========================================
// SCENE 4: STREET SCENARIO (Car Moving)
// ==========================================
GLfloat s_position = 0.0f;
GLfloat s_speed = 0.05f;
bool s_soundPlaying = true;

void s_drawText(float x, float y, const char *text) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void displayStreet() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3ub(135, 206, 250);
    glVertex2f(-1.0f, -0.1f); glVertex2f(1.0f, -0.1f);
    glVertex2f(1.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-1.0f, -1.0f); glVertex2f(-1.0f, 0.1f);
    glVertex2f(1.0f, 0.1f); glVertex2f(1.0f, -1.0f);
    glEnd();

    glBegin(GL_POLYGON); glColor3ub(210, 180, 140);
    glVertex2f(-1.0f, 0.0f); glVertex2f(-0.50f, 0.0f); glVertex2f(-0.5f, 0.5f); glVertex2f(-1.0f, 0.50f); glEnd();

    glBegin(GL_POLYGON); glColor3ub(240, 230, 140);
    glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, 0.30f); glVertex2f(-0.50f, 0.30f); glVertex2f(-0.50f, 0.0f); glEnd();

    glBegin(GL_POLYGON); glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(0.0f, 0.0f); glVertex2f(0.50f, 0.0f); glVertex2f(0.50f, 0.40f); glVertex2f(0.0f, 0.40f); glEnd();

    glBegin(GL_POLYGON); glColor3ub(240, 230, 140);
    glVertex2f(0.74f, 0.0f); glVertex2f(1.0f, 0.0f); glVertex2f(1.0f, 0.20f); glVertex2f(0.74f, 0.20f); glEnd();
    s_drawText(0.8f, 0.22f, "CoCo's Pet Shop");

    glPushMatrix();
    glTranslatef(s_position, 0.0f, 0.0f);
    glBegin(GL_POLYGON); glColor3f(0.7f, 0.0f, 0.0f);
    glVertex2f(-0.50f, -0.20f); glVertex2f(0.0f, -0.20f); glVertex2f(0.0f, -0.30f); glVertex2f(-0.50f, -0.30f); glEnd();
    glBegin(GL_POLYGON); glColor3f(0.6f, 0.8f, 0.95f);
    glVertex2f(-0.30f, -0.10f); glVertex2f(-0.20f, -0.10f); glVertex2f(-0.10f, -0.20f); glVertex2f(-0.40f, -0.20f); glEnd();
    glPopMatrix();

    glBegin(GL_POLYGON); glColor3f(0.1f, 0.1f, 0.1f);
    glVertex2f(0.34f, -0.52f); glVertex2f(0.44f, -0.50f); glVertex2f(0.46f, -0.56f); glVertex2f(0.37f, -0.60f); glEnd();
    glBegin(GL_POLYGON); glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.42f, -0.54f); glVertex2f(0.48f, -0.54f); glVertex2f(0.48f, -0.60f); glVertex2f(0.42f, -0.60f); glEnd();
    glBegin(GL_POLYGON); glColor3f(0.0f, 0.0f, 0.8f);
    glVertex2f(0.40f, -0.60f); glVertex2f(0.50f, -0.60f); glVertex2f(0.50f, -0.70f); glVertex2f(0.40f, -0.70f); glEnd();

    glFlush();
}

void street_update(int value) {
    if (currentScene == 4) {
        if (s_position > 1.0) s_position = -1.0f;
        s_position = s_position + s_speed;
        glutPostRedisplay();
    }
    glutTimerFunc(100, street_update, 0);
}

void street_key(unsigned char key, int x, int y) {
    if (key == 'm' || key == 'M') {
        if (s_soundPlaying) { PlaySound(NULL, 0, 0); s_soundPlaying = false; }
        else { PlaySound(TEXT("mixkit-cars-starting-1561.wav"), NULL, SND_ASYNC | SND_LOOP); s_soundPlaying = true; }
    }
}

// ==========================================
// MASTER ROUTING & MAIN FUNCTION
// ==========================================
void display() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // For Wildlife scene (Scene 2), apply specific aspect ratio mapping
    if (currentScene == 2) {
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        float aspect = (float)width / height;
        if(aspect > 1.6) gluOrtho2D(-1.0 * aspect/1.6, 1.0 * aspect/1.6, -1.0, 1.0);
        else gluOrtho2D(-1.0, 1.0, -1.0/(aspect*0.625), 1.0/(aspect*0.625));
    } else {
        gluOrtho2D(-1, 1, -1, 1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch(currentScene) {
        case 0: displayInstruction(); break;
        case 1: displayNature(); break;
        case 2: displayWildlife(); break;
        case 3: displayWedding(); break;
        case 4: displayStreet(); break;
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC to exit everywhere

    if (currentScene == 1) nature_key(key, x, y);
    else if (currentScene == 2) wild_key(key, x, y);
    else if (currentScene == 3) wed_key(key, x, y);
    else if (currentScene == 4) street_key(key, x, y);
}

void handleSpecialKeys(int key, int x, int y) {
    // Navigation mapping
    if (key == GLUT_KEY_RIGHT) {
        currentScene = (currentScene + 1) % 5;
        updateAudio();
        glutPostRedisplay();
    } else if (key == GLUT_KEY_LEFT) {
        currentScene = (currentScene - 1 + 5) % 5;
        updateAudio();
        glutPostRedisplay();
    } else {
        // Pass Up/Down to active scenes
        if (currentScene == 2) wild_special(key, x, y);
        if (currentScene == 3) wed_special(key, x, y);
    }
}

void handleMouse(int button, int state, int x, int y) {
    if (currentScene == 1) nature_mouse(button, state, x, y);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(160, 50);
    glutCreateWindow("Merged CG Project - Use Left/Right Arrows to Switch");

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeys);
    glutMouseFunc(handleMouse);
    glutIdleFunc(wed_Idle); // Only runs logic if currentScene == 3

    // Start Scene Timers
    glutTimerFunc(100, nature_update, 0);       // Scene 1
    glutTimerFunc(ANIMATION_INTERVAL, wild_update, 0); // Scene 2
    glutTimerFunc(100, wed_moveCouple, 0);      // Scene 3
    glutTimerFunc(0, street_update, 0);         // Scene 4

    updateAudio();
    glutMainLoop();
    return 0;
}
