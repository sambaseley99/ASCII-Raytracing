//Includes
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>


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
const float CAMERA_SPIN_RADIUS = 50;
const int CAMERA_SPIN_STEPS = 360;

const double SPHERE_RADIUS = 25;

const struct Vec3D SPHERE_POSITION_VEC = {0,0,0};
struct Vec3D CAMERA_POSITION_VEC = {-50,0,0};   //removed const so can be moved.
struct Vec3D CAMERA_DIRECTION_VEC = {1,0,0};    //removed const so can be moved.
float CAMERA_SPIN_DEGREES = 0;
const struct Vec3D LIGHT_POSITION_VEC = {-500,-500,-500};

const string SHADING_MATERIAL_STRING = "@#HOo!:.";


//Arrays & Vectors
char displayArrayBuffer[X_RESOLUTION][Y_RESOLUTION];

//Global Variables

//Method Declarations
void debug_fill_display_buffer();
void print_display_buffer();
void ray_shooter();
Vec3D position_of_intersect(Vec3D lineUnitVec, Vec3D lineOriginPosVec, Vec3D spherePosVec, double sphereRadius);
double does_ray_intersect(Vec3D lineUnitVec, Vec3D lineOriginPositionVec, Vec3D spherePositionVec, double sphereRadius);
Vec3D unit_vector_calculator (Vec3D startPointPosVec, Vec3D toPointPosVec);
double normal_to_light_angle_calculator(Vec3D rayImpactPosVec, Vec3D lightPosVec, Vec3D spherePosVec);
double irradience_calculator (double incidenceAngle, Vec3D rayImpactPosVec, Vec3D lightPosVec);
Vec3D maximum_irradiance_pos_calculator (Vec3D sphereToLightUnitVec, double sphereRadius, Vec3D sphereCoordsPosVec);
char character_shading (double lightValue, double maxLightValue);
void rotate_camera ();
void camera_direction_correction ();

void debug_does_ray_intersect_test();

Vec3D convert_degrees_angle_to_unit_vector(DegreesAnglePair anglePair);
DegreesAnglePair convert_unit_vector_to_degrees_angle(Vec3D unitVector);


int main ()
{


    //debug_fill_display_buffer();
    //print_display_buffer();

    //debug_does_ray_intersect_test();

    //cout << "---Start of Log:---" << endl;

    //rotate_camera();
    ray_shooter();
    system("clear");
    rotate_camera();
    print_display_buffer();
    this_thread::sleep_for(chrono::milliseconds(10));
    getchar();
    main();

    //getchar();

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
    Vec3D rayImpactPosVec;
    double lightAngle, maxLightAngle, lightAngleSquare, maxLightAngleSquare;

    maxLightAngle = 180 * (M_PI/180);

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

    //Find the maximum irradiance position.
    Vec3D maximumIrradiancePosVec = maximum_irradiance_pos_calculator (unit_vector_calculator(SPHERE_POSITION_VEC, LIGHT_POSITION_VEC), SPHERE_RADIUS, SPHERE_POSITION_VEC);

    //Find the maximum irradiance.
    double maximumIrradiance = -(irradience_calculator(normal_to_light_angle_calculator(maximumIrradiancePosVec, LIGHT_POSITION_VEC, SPHERE_POSITION_VEC), maximumIrradiancePosVec, LIGHT_POSITION_VEC));

    //cout << "MaximumIrradiancePosition = " << maximumIrradiancePosVec.xCom << "," << maximumIrradiancePosVec.yCom << "," << maximumIrradiancePosVec.zCom  << endl;
    //cout << "MaximumIrradiance = " << maximumIrradiance << endl;

    for (int y = 0; y < Y_RESOLUTION; y++)
    {
        for (int x = 0; x < X_RESOLUTION; x++)
        {
            //convert current angles to vector.
            Vec3D lineUnitVec = convert_degrees_angle_to_unit_vector(currentCameraAngle);

            if (does_ray_intersect(lineUnitVec, CAMERA_POSITION_VEC, SPHERE_POSITION_VEC, SPHERE_RADIUS) > 0)
            {
                rayImpactPosVec =  position_of_intersect(lineUnitVec, CAMERA_POSITION_VEC, SPHERE_POSITION_VEC, SPHERE_RADIUS);
                //lightAngle = normal_to_light_angle_calculator(rayImpactPosVec, LIGHT_POSITION_VEC, SPHERE_POSITION_VEC);
                //Find the impact point's irradiance.
                double pointIrradiance = -(irradience_calculator(normal_to_light_angle_calculator(rayImpactPosVec, LIGHT_POSITION_VEC, SPHERE_POSITION_VEC), rayImpactPosVec, LIGHT_POSITION_VEC));


                //cout << pointIrradiance << endl;

                //lightAngleSquare = sqrt(lightAngle);
                //maxLightAngleSquare = sqrt(maxLightAngle);

                displayArrayBuffer[x][y] = character_shading( pointIrradiance, maximumIrradiance);

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

Vec3D position_of_intersect(Vec3D lineUnitVec, Vec3D lineOriginPosVec, Vec3D spherePosVec, double sphereRadius)
{
    //finds the position(s) of where the line intersects the sphere.

    double distanceAlongRay, tempDouble1;
    Vec3D tempVec1, impactPosVec;

    // d = -(u * (o - c)) +/- sqrt(does_ray_intersect())

    // o - c
    tempVec1.xCom = lineOriginPosVec.xCom - spherePosVec.xCom;
    tempVec1.yCom = lineOriginPosVec.yCom - spherePosVec.yCom;
    tempVec1.zCom = lineOriginPosVec.zCom - spherePosVec.zCom;

    // u * tempVec1
    tempDouble1 = (lineUnitVec.xCom * tempVec1.xCom) + (lineUnitVec.yCom * tempVec1.yCom) + (lineUnitVec.zCom * tempVec1.zCom);

    distanceAlongRay = -(tempDouble1) - sqrt(does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius));

    /*
    //Check if - or + sqrt is closer to the origin.
    if ( abs(-(tempDouble1) + sqrt(does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius))) < abs(-(tempDouble1) - sqrt(does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius))))
    {
        // d = -(tempDouble1) + sqrt(does_ray_intersect())
        distanceAlongRay = -(tempDouble1) + sqrt(does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius));

    }
    else
    {
        // d = -(tempDouble1) - sqrt(does_ray_intersect())
        distanceAlongRay = -(tempDouble1) - sqrt(does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius));

    }
    */

    //Get impact position
    impactPosVec.xCom = (lineUnitVec.xCom * distanceAlongRay) + lineOriginPosVec.xCom;
    impactPosVec.yCom = (lineUnitVec.yCom * distanceAlongRay) + lineOriginPosVec.yCom;
    impactPosVec.zCom = (lineUnitVec.zCom * distanceAlongRay) + lineOriginPosVec.zCom;

    return impactPosVec;

}

double does_ray_intersect(Vec3D lineUnitVec, Vec3D lineOriginPosVec, Vec3D spherePosVec, double sphereRadius)
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

    return intersectResult;
}

Vec3D unit_vector_calculator (Vec3D startPointPosVec, Vec3D toPointPosVec)
{

    Vec3D betweenPointsDirVec, unitVec;
    double betweenPointsMagnitude, betweenPointsMagnitudeSquared;

    //Get vector between specified points.
    betweenPointsDirVec.xCom = toPointPosVec.xCom - startPointPosVec.xCom;
    betweenPointsDirVec.yCom = toPointPosVec.yCom - startPointPosVec.yCom;
    betweenPointsDirVec.zCom = toPointPosVec.zCom - startPointPosVec.zCom;

    //Get normal magnitude.
    //betweenPointsMagnitudeSquared = (x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2;
    betweenPointsMagnitudeSquared = pow(toPointPosVec.xCom - startPointPosVec.xCom, 2) + pow(toPointPosVec.yCom - startPointPosVec.yCom, 2) + pow(toPointPosVec.zCom - startPointPosVec.zCom, 2);
    betweenPointsMagnitude = sqrt(betweenPointsMagnitudeSquared);

    //Unit vector = Direction Vector / Direction Vector Magnitude.
    unitVec.xCom = betweenPointsDirVec.xCom / betweenPointsMagnitude;
    unitVec.yCom = betweenPointsDirVec.yCom / betweenPointsMagnitude;
    unitVec.zCom = betweenPointsDirVec.zCom / betweenPointsMagnitude;

    return unitVec;

}

double normal_to_light_angle_calculator(Vec3D rayImpactPosVec, Vec3D lightPosVec, Vec3D spherePosVec)
{

    Vec3D lightDirVec, normalDirVec;
    double dotProduct, lightMag, normalMag, cosAngle, angle;

    //Make vectors to measure.
    lightDirVec.xCom = lightPosVec.xCom - rayImpactPosVec.xCom;
    lightDirVec.yCom = lightPosVec.yCom - rayImpactPosVec.yCom;
    lightDirVec.zCom = lightPosVec.zCom - rayImpactPosVec.zCom;

    normalDirVec.xCom = rayImpactPosVec.xCom - spherePosVec.xCom;
    normalDirVec.yCom = rayImpactPosVec.yCom - spherePosVec.yCom;
    normalDirVec.zCom = rayImpactPosVec.zCom - spherePosVec.zCom;

    //Dot product of vectors.
    dotProduct = (lightDirVec.xCom * normalDirVec.xCom) + (lightDirVec.yCom * normalDirVec.yCom) + (lightDirVec.zCom * normalDirVec.zCom);

    //Magnitude of vectors.
    lightMag = sqrt(pow(lightDirVec.xCom, 2) + pow(lightDirVec.yCom, 2) + pow(lightDirVec.zCom, 2));
    normalMag = sqrt(pow(normalDirVec.xCom, 2) + pow(normalDirVec.yCom, 2) + pow(normalDirVec.zCom, 2));

    //cout << "cosAngle = " << dotProduct << " / (" << lightMag << " * " << normalMag << ");" << endl;

    //Get the value equal to the cos of the angle difference between the vectors.
    cosAngle = dotProduct / (lightMag * normalMag);

    //Get the angle.
    angle = acos(cosAngle);

    //cout << "Angle: " << angle << endl;

    return angle;


}

double irradience_calculator (double incidenceAngle, Vec3D rayImpactPosVec, Vec3D lightPosVec)
{

    double irradiance, lightDistance, lightIntensity;

    if (incidenceAngle > M_PI_2)
    {
        return 0;
    }
    

    lightIntensity = -1;
    
    lightDistance = sqrt(pow((lightPosVec.xCom - rayImpactPosVec.xCom), 2) + pow((lightPosVec.yCom - rayImpactPosVec.yCom), 2) + pow((lightPosVec.zCom - rayImpactPosVec.zCom), 2));

    //cout << lightDistance << endl;

    irradiance = (lightIntensity * cos(incidenceAngle)) / pow( lightDistance, 2 );

    //cout << "Irradiance: " << irradiance << endl;

    return irradiance;

}

Vec3D maximum_irradiance_pos_calculator (Vec3D sphereToLightUnitVec, double sphereRadius, Vec3D sphereCoordsPosVec)
{

    Vec3D maximumIrradiance;

    maximumIrradiance.xCom = (sphereToLightUnitVec.xCom * sphereRadius) + sphereCoordsPosVec.xCom;
    maximumIrradiance.yCom = (sphereToLightUnitVec.yCom * sphereRadius) + sphereCoordsPosVec.yCom;
    maximumIrradiance.zCom = (sphereToLightUnitVec.zCom * sphereRadius) + sphereCoordsPosVec.zCom;

    return maximumIrradiance;

}

char character_shading (double lightValue, double maxLightValue)
{

    if ( abs(lightValue) > maxLightValue - (1 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(0); }
    else if ( abs(lightValue) > maxLightValue - (2 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(1); }
    else if ( abs(lightValue) > maxLightValue - (3 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(2); }
    else if ( abs(lightValue) > maxLightValue - (4 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(3); }
    else if ( abs(lightValue) > maxLightValue - (5 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(4); }
    else if ( abs(lightValue) > maxLightValue - (6 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(5); }
    else if ( abs(lightValue) > maxLightValue - (7 * (maxLightValue / 8)) ){ return SHADING_MATERIAL_STRING.at(6); }
    else { return SHADING_MATERIAL_STRING.at(7); }
}

void rotate_camera ()
{

    //Camera Position.
    CAMERA_SPIN_DEGREES = ((2 * M_PI) / CAMERA_SPIN_STEPS) + CAMERA_SPIN_DEGREES;

    CAMERA_POSITION_VEC.xCom = CAMERA_SPIN_RADIUS * (cos(CAMERA_SPIN_DEGREES));
    CAMERA_POSITION_VEC.zCom = CAMERA_SPIN_RADIUS * (sin(CAMERA_SPIN_DEGREES));

    cout << "Camera Pos: (" << CAMERA_POSITION_VEC.xCom << "," << CAMERA_POSITION_VEC.yCom << "," << CAMERA_POSITION_VEC.zCom << ")" << endl;

    //Camera Direction.
    CAMERA_DIRECTION_VEC = unit_vector_calculator(CAMERA_POSITION_VEC, SPHERE_POSITION_VEC);

    camera_direction_correction();

    cout << "Camera Dir: (" << CAMERA_DIRECTION_VEC.xCom << "," << CAMERA_DIRECTION_VEC.yCom << "," << CAMERA_DIRECTION_VEC.zCom << ")" << endl;

    /*
    //Correct rounding errors.
    if (abs(CAMERA_DIRECTION_VEC.xCom) + abs(CAMERA_DIRECTION_VEC.yCom) + abs(CAMERA_DIRECTION_VEC.zCom) != 1)
    {
        camera_direction_correction();
    }
    */
    
}

void camera_direction_correction ()
{
    //Find factor to multiply components by to make unit vector == 1.
    double unitVecSum = abs(CAMERA_DIRECTION_VEC.xCom) + abs(CAMERA_DIRECTION_VEC.yCom) + abs(CAMERA_DIRECTION_VEC.zCom);
    double multFactor = 1 / unitVecSum;

    //Multiply original values to get corrected values.
    CAMERA_DIRECTION_VEC.xCom = CAMERA_DIRECTION_VEC.xCom * multFactor;
    CAMERA_DIRECTION_VEC.yCom = CAMERA_DIRECTION_VEC.yCom * multFactor;
    CAMERA_DIRECTION_VEC.zCom = CAMERA_DIRECTION_VEC.zCom * multFactor;
}

void debug_does_ray_intersect_test()
{

    Vec3D lineUnitVec = CAMERA_DIRECTION_VEC;
    Vec3D lineOriginPosVec = CAMERA_POSITION_VEC;
    Vec3D spherePosVec = SPHERE_POSITION_VEC;

    double sphereRadius = SPHERE_RADIUS;

    //cout << does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius);

    if (does_ray_intersect(lineUnitVec, lineOriginPosVec, spherePosVec, sphereRadius) > 0 )
    {
        cout << true;
    }
    else
    {
        cout << false;
    }

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

