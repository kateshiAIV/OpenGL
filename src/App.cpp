#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>

#define M_PI 3.14159265358979323846  /* pi */

using namespace std;


class DefaultSettings {
public:

    static int particlesCount;
    static double gravitationalConstant;
    static float boundary;
    static float particleSizeScale;
    static float epsilon;
};

// Simulation settings
int DefaultSettings::particlesCount = 500;
double DefaultSettings::gravitationalConstant = 1.0;
float DefaultSettings::boundary = 40.0f;
float DefaultSettings::particleSizeScale = 0.55f;
float DefaultSettings::epsilon = 0.1f;


class Particle {
public:
    float position[3];
    float velocity[3];
    float acceleration[3];
    float mass;
    friend class DefaultSettings;

    Particle(float x, float y, float z, float vx, float vy, float vz, float m) {
        position[0] = x;
        position[1] = y;
        position[2] = z;

        velocity[0] = vx;
        velocity[1] = vy;
        velocity[2] = vz;

        acceleration[0] = 0.0;
        acceleration[1] = 0.0;
        acceleration[2] = 0.0;

        mass = m;
    }

    void update(float deltaTime) {
        for (int i = 0; i < 3; i++) {
            position[i] += velocity[i] * deltaTime;
            velocity[i] += acceleration[i] * deltaTime;
        }

        // Check for boundary collision and reverse velocity if necessary
        if (position[0] < -DefaultSettings::boundary)
            if (velocity[0] < 0)
                velocity[0] = -velocity[0];

        if (position[0] > DefaultSettings::boundary)
            if (velocity[0] > 0)
                velocity[0] = -velocity[0];

        if (position[1] < -DefaultSettings::boundary)
            if (velocity[1] < 0)
                velocity[1] = -velocity[1];

        if (position[1] > DefaultSettings::boundary)
            if (velocity[1] > 0)
                velocity[1] = -velocity[1];

        if (position[2] < -DefaultSettings::boundary)
            if (velocity[2] < 0)
                velocity[2] = -velocity[2];

        if (position[2] > DefaultSettings::boundary)
            if (velocity[2] > 0)
                velocity[2] = -velocity[2];
    }

    void applyForce(float forcex, float forcey, float forcez) {
        acceleration[0] += forcex / mass;
        acceleration[1] += forcey / mass;
        acceleration[2] += forcez / mass;
    }

    void DrawCircle() {
        glColor3f(1.0f, 1.0f, 0.0f);
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glutSolidSphere(pow(mass, DefaultSettings::particleSizeScale) / 20, 20, 20); // Draw a solid sphere using FreeGLUT
        glPopMatrix();
    }
};


vector<Particle> particles;


void generateRandomParticles(int n) {
    // Ustawienia zakresów dla pozycji, prędkości i masy
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> posDist(-DefaultSettings::boundary, DefaultSettings::boundary);
    uniform_real_distribution<float> velDist(-1.0f, 1.0f);
    uniform_real_distribution<float> massDist(0.5f, 5.0f);

    for (int i = 0; i < n; ++i) {
        float x = posDist(gen);
        float y = posDist(gen);
        float z = posDist(gen);
        float vx = velDist(gen);
        float vy = velDist(gen);
        float vz = velDist(gen);
        float mass = massDist(gen);

        particles.push_back(Particle(x, y, z, vx, vy, vz, mass));
    }
}

float lastTime;

void drawBox() {


    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    // Front face
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);

    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);

    // Back face
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);

    // Connect front and back faces
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, -DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);

    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, DefaultSettings::boundary);
    glVertex3f(-DefaultSettings::boundary, DefaultSettings::boundary, -DefaultSettings::boundary);
    glEnd();

}


void setupLighting() {
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    //// Position the light at the sun's position
    //GLfloat light_position[] = { 1, 1, 1, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //// Set the ambient light properties
    //GLfloat ambient_light[] = { 0.2, 0.2, 0.2, 1.0 };
    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    //// Set the diffuse light properties
    //GLfloat diffuse_light[] = { 0.8, 0.8, 0.8, 1.0 };
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

    //// Set the specular light properties
    //GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
}

void setupViewport(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, DefaultSettings::boundary * 10);
    glMatrixMode(GL_MODELVIEW);
}

void calculateForces() {

    // Reset acceleration for all particles
    for (auto& p : particles) {
        p.acceleration[0] = 0.0;
        p.acceleration[1] = 0.0;
        p.acceleration[2] = 0.0;
    }

    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            double dx = particles[j].position[0] - particles[i].position[0];
            double dy = particles[j].position[1] - particles[i].position[1];
            double dz = particles[j].position[2] - particles[i].position[2];

            double distanceSquared = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);
            double distance = sqrt(distanceSquared + DefaultSettings::epsilon); // Add epsilon to avoid division by zero

            double forceMagnitude = (DefaultSettings::gravitationalConstant * particles[i].mass * particles[j].mass) / distanceSquared;

            double force[3];
            force[0] = forceMagnitude * dx / distance;
            force[1] = forceMagnitude * dy / distance;
            force[2] = forceMagnitude * dz / distance;

            particles[i].applyForce(force[0], force[1], force[2]);

            particles[j].applyForce(-force[0], -force[1], -force[2]);
        }
    }
}


float camera_distance = DefaultSettings::boundary * 4.0f;
float camera_angle_x = 0.0f;
float camera_angle_y = 0.0f;

float X = 0.0, Y = 0.0, Z = 0.0;
float posX = 0.0, posY = 1.0, posZ = 0.0;


void display() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;


    /* Update positions based on velocities */
    calculateForces();
    for (auto& p : particles) {
        p.update(deltaTime);
    }


    /* Update light position */
    setupLighting();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float eyeX = camera_distance * sin(camera_angle_x) * cos(camera_angle_y);
    float eyeY = camera_distance * sin(camera_angle_y);
    float eyeZ = camera_distance * cos(camera_angle_x) * cos(camera_angle_y);

    gluLookAt(eyeX, eyeY, eyeZ,  // Eye position
        X, Y, Z,    // Look-at point
        posX, posY, posZ);   // Up direction



    drawBox();


    for (auto& p : particles) {
        p.DrawCircle();
    }


    glutSwapBuffers();
}

void keyboard(int key, int x, int y) {
    float angle_step = 0.1f;
    switch (key) {
    case GLUT_KEY_LEFT:
        camera_angle_x -= angle_step;
        break;
    case GLUT_KEY_RIGHT:
        camera_angle_x += angle_step;
        break;
    case GLUT_KEY_UP:
        camera_angle_y += angle_step;
        if (camera_angle_y >= M_PI / 2.0f) camera_angle_y = -M_PI / 2.0f; // Limit the vertical angle
        break;
    case GLUT_KEY_DOWN:
        camera_angle_y -= angle_step;
        if (camera_angle_y <= -M_PI / 2.0f) camera_angle_y = M_PI / 2.0f; // Limit the vertical angle
        break;
    case GLUT_KEY_F1:
        posX++;
        break;
    case GLUT_KEY_F2:
        posX--;
        break;
    case GLUT_KEY_F3:
        posY++;
        break;
    case GLUT_KEY_F4:
        posY--;
        break;

    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == 3) { // Scroll up
            camera_distance -= DefaultSettings::boundary * 0.05;
            if (camera_distance < 2.0f) camera_distance = 2.0f; // Limit zoom in
        }
        else if (button == 4) { // Scroll down
            camera_distance += DefaultSettings::boundary * 0.05;
            if (camera_distance > DefaultSettings::boundary * 5) camera_distance = DefaultSettings::boundary * 5; // Limit zoom out
        }
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    setupViewport(w, h);
}





int main(int argc, char** argv) {
    char useDefaults;
    int particlesCount;
    double gravitationalConstant;
    float boundary;
    float particleSizeScale;

    cout << "Do you want to use default settings? (y/n): ";
    cin >> useDefaults;

    if (useDefaults == 'n' || useDefaults == 'N') {
        cout << "Enter the number of particles (default is 500): ";
        cin >> particlesCount;

        cout << "Enter the gravitational constant (default is 1.0): ";
        cin >> gravitationalConstant;

        cout << "Enter the boundary size (default is 40.0): ";
        cin >> boundary;

        cout << "Enter the particle size scale (default is 0.55): ";
        cin >> particleSizeScale;

        DefaultSettings::particlesCount = particlesCount;
        DefaultSettings::gravitationalConstant = gravitationalConstant;
        DefaultSettings::boundary = boundary;
        DefaultSettings::particleSizeScale = particleSizeScale;
    }

    generateRandomParticles(DefaultSettings::particlesCount);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Particle simulation");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    setupLighting(); // Set up lighting

    cout << glGetString(GL_VERSION) << endl;

    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutSpecialFunc(keyboard); // Register keyboard function for arrow keys
    glutMouseFunc(mouse); // Register mouse function for scrolling

    glutMainLoop();

    return 0;
}