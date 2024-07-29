//Includes
#include <iostream>


//Namespaces
using namespace std;


//Structs
struct ObjectCoords3D{ double xPos, yPos, zPos; };
struct Directional3DVector { double x, y, z; };

//Constants
const float FONT_ASPECT_RATIO = 0.47;
const int X_RESOLUTION = 250;
const int Y_RESOLUTION = X_RESOLUTION * FONT_ASPECT_RATIO; 

const double X_CAMERA_ANGLE_LIMIT = 90;
const double Y_CAMERA_ANGLE_LIMIT = 90; 
const double CAMERA_DIRECTION_ANGLE = 0;
const float CAMERA_RAY_MAX_LENGTH = 500;

const double SPHERE_RADIUS = 25;


//Arrays & Vectors
char displayArrayBuffer[X_RESOLUTION][Y_RESOLUTION];

//Global Variables


//Method Declarations
void initialise_structs();
void debug_fill_display_buffer();
void print_display_buffer();
double quadratic_equation_solver();
bool does_ray_intersect(Directional3DVector u, ObjectCoords3D o, ObjectCoords3D c);


int main ()
{

    initialise_structs();

    debug_fill_display_buffer();
    print_display_buffer();

    return 0;
}

void initialise_structs()
{
    //Create the 3D position objects.

    ObjectCoords3D sphere;
    sphere.xPos = 0;
    sphere.yPos = 0;
    sphere.zPos = 0;

    ObjectCoords3D camera;
    camera.xPos = 50;
    camera.yPos = 0;
    camera.zPos = 0;

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
    double x_ray_angle_diff = X_CAMERA_ANGLE_LIMIT / X_RESOLUTION;
    double y_ray_angle_diff = Y_CAMERA_ANGLE_LIMIT / Y_RESOLUTION;


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

bool does_ray_intersect(Directional3DVector u, ObjectCoords3D o, ObjectCoords3D c)
{
    double intersectResult;

    double o, c, r;
    

    //intersectResult = (u * (o - c))^2 - ((o - c)^2 - r^2)










    if (intersectResult < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}