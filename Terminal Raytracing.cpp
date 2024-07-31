//Includes
#include <iostream>
#include <cmath>


//Namespaces
using namespace std;


//Structs
struct Vec3D{ double xCom, yCom, zCom; };
struct DegreesAnglePair{double xAngle, yAngle;};
struct Directional3DVector { double xDir, yDir, zDir, magnitude; };

//Constants
const float FONT_ASPECT_RATIO = 0.47;
const int X_RESOLUTION = 250;
const int Y_RESOLUTION = X_RESOLUTION * FONT_ASPECT_RATIO; 

const double X_CAMERA_ANGLE_LIMIT = 90;
const double Y_CAMERA_ANGLE_LIMIT = 90;
const float CAMERA_RAY_MAX_LENGTH = 500;

const double SPHERE_RADIUS = 25;

const struct Vec3D SPHERE_POSITION_VEC = {0,0,0};
const struct Vec3D CAMERA_POSITION_VEC = {-50,0,0};
const struct Vec3D CAMERA_DIRECTION_VEC = {1,0,0};


//Arrays & Vectors
char displayArrayBuffer[X_RESOLUTION][Y_RESOLUTION];

//Global Variables


//Method Declarations
void debug_fill_display_buffer();
void print_display_buffer();
void ray_shooter();
double quadratic_equation_solver();
bool does_ray_intersect(Vec3D lineUnitVec, Vec3D lineOriginPositionVec, Vec3D spherePositionVec, double sphereRadius);
void debug_does_ray_intersect_test();
Vec3D convert_degrees_angle_to_unit_vector(DegreesAnglePair anglePair);
DegreesAnglePair convert_unit_vector_to_degrees_angle(Vec3D unitVector);


int main ()
{


    //debug_fill_display_buffer();
    //print_display_buffer();

    //debug_does_ray_intersect_test();

    ray_shooter();
    print_display_buffer();
    getchar();

    return 0;
}

void debug_fill_display_buffer()
{
    //Fills the entire display buffer with one character to show it works.
    for (int y = 0; y < Y_RESOLUTION; y++)
    {
        for (int x = 0; x < X_RESOLUTION; x++)
        {
            displayArrayBuffer[x][y] = '@';
        }  
    }
}

void print_display_buffer()
{
    //Print the display buffer to the terminal.

    for (int y = 0; y < Y_RESOLUTION; y++)
    {
        for (int x = 0; x < X_RESOLUTION; x++)
        {
            cout << displayArrayBuffer[x][y];
        }
        
        cout << endl;
    }
}

void ray_shooter()
{
    //Get how much difference in angle each ray has from those adjacent.
    //X and Y are seperate to allow for different display aspect ratios.
    double xAngleDiff = X_CAMERA_ANGLE_LIMIT / X_RESOLUTION;
    double yAngleDiff = Y_CAMERA_ANGLE_LIMIT / Y_RESOLUTION;

    //Get the angle of the central camera direction.
    DegreesAnglePair initialCameraAngle = convert_unit_vector_to_degrees_angle(CAMERA_DIRECTION_VEC);

    //Get angle of Top left most pixel.
    DegreesAnglePair currentCameraAngle;
    currentCameraAngle.xAngle = initialCameraAngle.xAngle - (X_CAMERA_ANGLE_LIMIT / 2);
    currentCameraAngle.yAngle = initialCameraAngle.yAngle - (Y_CAMERA_ANGLE_LIMIT / 2);

    for (int y = 0; y < Y_RESOLUTION; y++)
    {
        for (int x = 0; x < X_RESOLUTION; x++)
        {
            //convert current angles to vector.
            Vec3D lineUnitVec = convert_degrees_angle_to_unit_vector(currentCameraAngle);

            if (does_ray_intersect(lineUnitVec, CAMERA_POSITION_VEC, SPHERE_POSITION_VEC, SPHERE_RADIUS) == true)
            {
                displayArrayBuffer[x][y] = '@';
            }
            else
            {
                displayArrayBuffer[x][y] = ' ';
            }

            //Increment angle.
            currentCameraAngle.xAngle = currentCameraAngle.xAngle + xAngleDiff;
        }

        //Increment and set angles.
        currentCameraAngle.xAngle = currentCameraAngle.xAngle - X_CAMERA_ANGLE_LIMIT;
        currentCameraAngle.yAngle = currentCameraAngle.yAngle + yAngleDiff;
        
    }
}

double quadratic_equation_solver()
{
    //finds the position(s) of where the line intersects the sphere.

    double a, b, c;
    // a = (vector direction of line)^2
    // b = 2 * ( vector direction of line * (origin of the line ))
    // c =

    // i) z = -b
    // ii) y = b^2
    // iii) w = 4a
    // iv) v = w * c
    // v) u = y - v
    // vi) t = sqrt(v)
    // vii) s = z + t
    // viii) r = 2a
    // ix) x1 = s/r
    // x) s = z - t
    // xi) x2 = s/r

}

bool does_ray_intersect(Vec3D lineUnitVec, Vec3D lineOriginPosVec, Vec3D spherePosVec, double sphereRadius)
{
    double intersectResult,dotProduct, dotProductSquared, lineSpherePosDiffDotProduct, temp1;
    Vec3D lineSpherePosDiffVec;
    

    //intersectResult = (u * (o - c))^2 - ((o - c)^2 - r^2)

    // o - c
    lineSpherePosDiffVec.xCom = lineOriginPosVec.xCom - spherePosVec.xCom;
    lineSpherePosDiffVec.yCom = lineOriginPosVec.yCom - spherePosVec.yCom;
    lineSpherePosDiffVec.zCom = lineOriginPosVec.zCom - spherePosVec.zCom;

    // lineUnitVec * lineSpherePosDiffVec
    dotProduct = (lineUnitVec.xCom * lineSpherePosDiffVec.xCom) + (lineUnitVec.yCom * lineSpherePosDiffVec.yCom) + (lineUnitVec.zCom * lineSpherePosDiffVec.zCom);

    // dotProduct^2
    dotProductSquared = dotProduct * dotProduct;

    // lineSpherePosDiffVec^2
    lineSpherePosDiffDotProduct = (lineSpherePosDiffVec.xCom * lineSpherePosDiffVec.xCom) + (lineSpherePosDiffVec.yCom * lineSpherePosDiffVec.yCom) + (lineSpherePosDiffVec.zCom * lineSpherePosDiffVec.zCom);

    // lineSpherePosDiffDotProduct - (sphereRadius^2)
    temp1 = lineSpherePosDiffDotProduct - (sphereRadius * sphereRadius);

    // dotProductSquared - temp1
    intersectResult = dotProductSquared - temp1;

    //***If this actually works I will eat a shoe.

    if (intersectResult < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void debug_does_ray_intersect_test()
{

    Vec3D lineUnitVec = CAMERA_DIRECTION_VEC;
    Vec3D lineOriginPosVec = CAMERA_POSITION_VEC;
    Vec3D spherePosVec = SPHERE_POSITION_VEC;

    double sphereRadius = SPHERE_RADIUS;

    cout << does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius);

}

Vec3D convert_degrees_angle_to_unit_vector(DegreesAnglePair anglePair)
{
    Vec3D unitVector;
    double xAngleRad, yAngleRad;

    //Convert degrees to radians.
    xAngleRad = anglePair.xAngle * (M_PI/180);
    yAngleRad = anglePair.yAngle * (M_PI/180);

    //Convert radians to unit vector.
    unitVector.xCom = sin(xAngleRad) * cos(yAngleRad);
    unitVector.yCom = sin(xAngleRad) * sin(yAngleRad);
    unitVector.zCom = cos(xAngleRad);

    return unitVector;

}

DegreesAnglePair convert_unit_vector_to_degrees_angle(Vec3D unitVector)
{
    DegreesAnglePair Angle;
    double xAngleRad, yAngleRad;

    //Convert Vector to Radians.
    xAngleRad = acos(unitVector.zCom);
    yAngleRad = atan2(unitVector.yCom, unitVector.xCom);

    //Convert Radians to Degrees.
    Angle.xAngle = xAngleRad / (M_PI/180);
    Angle.yAngle = yAngleRad / (M_PI/180);

    return Angle;

}

