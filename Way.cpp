#include "TXLib.h"
#include <cstdlib>
#include <cmath>

struct Vector
{
    float X;
    float Y;
};

struct Sphere
{
    Vector position;
    Vector velocity;
    int Radius;
    int redRate;
    int greenRate;
    int blueRate;
    int Gradient;
};

void drawSphere(Sphere* sphere)
{
    for (int i = 0; i < sphere->Gradient; i++)
    {
        int X = sphere->position.X + sphere->Radius * i / 3 / sphere->Gradient;
        int Y = sphere->position.Y + sphere->Radius * i / 2 / sphere->Gradient;

        txSetFillColor(RGB(i * sphere->redRate / sphere->Gradient, i * sphere->greenRate / sphere->Gradient, i * sphere->blueRate / sphere->Gradient));
        txSetColor    (RGB(i * sphere->redRate / sphere->Gradient, i * sphere->greenRate / sphere->Gradient, i * sphere->blueRate / sphere->Gradient));
        txCircle(sphere->position.X, sphere->position.Y, sphere->Radius - sphere->Radius * i / sphere->Gradient);
    }
}

void moveSphere(Sphere *sphere, float dt)
{
    sphere->position.X += sphere->velocity.X * dt;
    sphere->position.Y += sphere->velocity.Y * dt;
}

bool checkCollisionTwoSpheres(Sphere* sphere1, Sphere* sphere2)
{
    if (sphere1->Radius + sphere2->Radius > sqrt(pow(sphere1->position.X - sphere2->position.X, 2)+ pow(sphere1->position.Y - sphere2->position.Y, 2)))
    {
        return true;
    }
    return false;
}

void collisionSphere(Sphere* sphere, int screenSizeX, int screenSizeY)
{
    if (sphere->position.X > screenSizeX - sphere->Radius || sphere->position.X < sphere->Radius)
        {
            sphere->velocity.X = - sphere->velocity.X;
        }

    if (sphere->position.Y > screenSizeY - sphere->Radius || sphere->position.Y < sphere->Radius)
        {
            sphere->velocity.Y = - sphere->velocity.Y;
        }
}


float projectionVector( float vectorX, float vectorY, float axisX, float axisY)
{
    float scalarComposition = vectorX * axisX + vectorY * axisY;
    float moduleAxis = pow (axisX * axisX + axisY * axisY, 0.5 );
    float projection = scalarComposition / moduleAxis;
    return projection;
}

void changeVelocitySphere(Sphere* sphere1, Sphere* sphere2)
{
    float axisX = sphere1->position.X - sphere2->position.X;
    float axisY = sphere1->position.Y - sphere2->position.Y;
    float moduleAxis = pow (axisX * axisX + axisY * axisY, 0.5 );
    float projectionVelocity1 = projectionVector( sphere1->velocity.X, sphere1->velocity.Y, axisX, axisY);
    float projectionVelocity2 = projectionVector( sphere2->velocity.X, sphere2->velocity.Y, axisX, axisY);
    float dv = projectionVelocity1 - projectionVelocity2;

    if (dv < 0)
    {
        sphere2->velocity.X +=   dv * axisX / moduleAxis;
        sphere2->velocity.Y +=   dv * axisY / moduleAxis;
        sphere1->velocity.X += - dv * axisX / moduleAxis;
        sphere1->velocity.Y += - dv * axisY / moduleAxis;
    }
}

int main()
{
    int screenSizeX = 1000;
    int screenSizeY = 600;


    float dt = 1;
    Sphere sphere1 = {Vector{100, 200}, Vector{2.0, 2.0}, 20, 300, 0, 300, 300};
    Sphere sphere2 = {Vector{200, 200}, Vector{2.0, 2.0}, 20, 0, 300, 300, 300};
    Sphere sphere3 = {Vector{300, 200}, Vector{2.0, 2.0}, 20, 300, 300, 0, 300};

    txCreateWindow(screenSizeX, screenSizeY);
    txSetColor(RGB(0, 0, 0));

    double mouseX = 0;
    double mouseY = 0;
    double mouseOldX = 0;
    double mouseOldY = 0;

    while(true)
    {
        mouseOldX = mouseX;
        mouseOldY = mouseY;
        mouseX = txMouseX();
        mouseY = txMouseY();

        if (txMouseButtons() > 0 )
        {
            sphere1.position.X = mouseX;
            sphere1.position.Y = mouseY;
            sphere1.velocity.X = (mouseX - mouseOldX)/dt;
            sphere1.velocity.Y = (mouseY - mouseOldY)/dt;
        }

        moveSphere(&sphere1, dt);
        moveSphere(&sphere2, dt);
        moveSphere(&sphere3, dt);

        txBegin();
        drawSphere(&sphere1);
        drawSphere(&sphere2);
        drawSphere(&sphere3);
        txEnd();

        txClear();

        collisionSphere(&sphere1, screenSizeX, screenSizeY);
        collisionSphere(&sphere2, screenSizeX, screenSizeY);
        collisionSphere(&sphere3, screenSizeX, screenSizeY);

        if (checkCollisionTwoSpheres(&sphere1, &sphere2))
        {
            changeVelocitySphere(&sphere1, &sphere2);
        }
        if (checkCollisionTwoSpheres(&sphere1, &sphere3))
        {
            changeVelocitySphere(&sphere1, &sphere3);
        }
        if (checkCollisionTwoSpheres(&sphere2, &sphere3))
        {
            changeVelocitySphere(&sphere2, &sphere3);
        }
    }

return 0;
}
