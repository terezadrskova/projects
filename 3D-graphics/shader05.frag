#version 150 compatibility

in vec3 origin, dir, point; 
out vec4 outcolour;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

#define USE_OWN_GEOMETRY 0

const int raytraceDepth = 42;
const int numSpheres = 6;

// my constants
const int MAX_DISTANCE = 1000;
const float epsilon = 0.001;
struct Ray
{
  vec3 origin;
  vec3 dir;
};
struct Sphere
{
  vec3 centre;
  float radius;
  vec3 colour;
};
struct Plane
{
  vec3 point;
  vec3 normal;
  vec3 colour;
};

struct Intersection
{
  // this should be the distance between the origin and intersect
  float t;
  // hit point
  vec3 point; 
  // normal to the intersection
  vec3 normal; 
  // can be used as boolean of there exists an intersection
  int hit;
  vec3 colour;
};

void shpere_intersect(Sphere sph,  Ray ray, inout Intersection intersect)
{
  //////////////////////////////////////
  //TODO Exercise 5
  //calculate the intersection ray-sphere here

  // setting the boolean of intersection back to zero
  intersect.hit = 0;

  float a = dot(ray.dir, ray.dir); 
  float b = 2.0 * dot(ray.origin - sph.centre, ray.dir);
  float c = dot(ray.origin - sph.centre, ray.origin - sph.centre) - sph.radius*sph.radius;
    
  float sqrtOf = b * b - (4.0 * a * c);
  if (sqrtOf > 0.0){
    
    float mu1 = (-b + sqrt(sqrtOf) ) / ( 2.0 * a );
    float mu2 = (-b - sqrt(sqrtOf) ) / ( 2.0 * a );

    if(min(mu1,mu2) <=0)
      intersect.t = max(mu1,mu2);
    else
      intersect.t = min(mu1,mu2);

    if(intersect.t > 0){
      // distance between the point of origin and intersection
      intersect.point = ray.origin + intersect.t * ray.dir;
      // normal to the intersection is calculated by normalising 
      // vector between point of intersection and the centre of sphere
      intersect.normal = normalize(intersect.point - sph.centre);
      // setting intersection boolean to 1 (true)
      intersect.hit = 1;
      intersect.colour = sph.colour;
      intersect.t = distance(ray.origin, intersect.point);

    }
  }
  //////////////////////////////////////
}

void plane_intersect(Plane pl, Ray ray, inout Intersection intersect)
{
  //////////////////////////////////////
  //TODO Exercise 5
  //calculate the intersection ray-plane here
  //generate a checkerboard pattern

  // change the boolean of intersection back to zero (false)
  intersect.hit = 0;


  float num = dot((pl.point - ray.origin), pl.normal);
  float denom = dot(ray.dir, pl.normal);

  if( denom != 0 && num != 0){
    // if there is no intersection
    intersect.t  = num / denom;

    if(intersect.t > 0){

      intersect.point = ray.origin + intersect.t  * ray.dir;
      intersect.normal = pl.normal;
      // setting intersection boolean to 1 (true)
      intersect.hit = 1;
      intersect.colour = pl.colour;

      // creating a checkboardlike floor
      if(int(floor(intersect.point.x * 2) + floor(intersect.point.z * 2)) % 2 == 0){
	intersect.colour = pl.colour;
      } else {
	intersect.colour = vec3(0.35,0.35,0.35);
      }
    }
      
  }
}

Sphere sphere[numSpheres];
Plane plane;
void Intersect(Ray r, inout Intersection i)
{
  //////////////////////////////////////
  //TODO Exercise 5
  //test the ray for intersections with all objects

  plane_intersect(plane, r, i);
  
  Intersection nearest = i;


  // finding the closest intersection
  for(int x = 0; x < numSpheres; x++){
    
    shpere_intersect(sphere[x], r, i);	
    if(i.hit == 1){

      // calculating the nearest intersection
      if(nearest.hit == 1 && i.t < nearest.t){
	nearest = i;
      }
	    
      if(nearest.hit == 0){
	nearest = i;
      }
    }
  }
 
  i = nearest;
}

int seed = 0;
float rnd()
{
  //////////////////////////////////////
  // you may use pseudo random number 
  // generator this to account for numerical errors
  // however, you don't need to
  seed = int(mod(float(seed)*1364.0+626.0, 509.0));
  return float(seed)/509.0;
}

vec3 computeShadow(in Intersection intersect)
{
  //////////////////////////////////////
  //TODO Exercise 5
  //compute the shadow of the objects 
  //using additional rays
	

  float randomNumber = rnd();
  float rndCoeff = 15;
  float rndX = randomNumber / rndCoeff; 
  float rndY = randomNumber / rndCoeff; 
  float rndZ = randomNumber / rndCoeff;

  // add value to limit distortion
  vec3 lightSource = vec3(6 + rndX, 4 + rndY, 3 + rndZ);
  Ray rayOfShadows;
  Intersection i;

  // direction to light source
  vec3 l = normalize(lightSource - intersect.point);

  rayOfShadows.dir = l;
  rayOfShadows.origin = intersect.point + l/180;

  Intersection shadowIntersection;
  // set the boolean to zero (false) to start with
  shadowIntersection.hit = 0;

  Intersect(rayOfShadows, shadowIntersection);
 
  // if there is no intersection - set the value to whatever it was before
  // otherwise set the value to vector of zeroes
  vec3 ia;
  vec3 id;
  vec3 is;
  if (shadowIntersection.hit == 0){
    // if it doesn't hit - return ambient + diffuse + specular

    //  =============== Phong shading ==============
    // suggested values from prof. Kainz email
    vec3 lightSource = vec3(6, 4, 3); 
    // normal vector of vertex
    vec3 n = normalize(intersect.normal);
    
    // normalized direction to camera
    vec3 e = normalize(-intersect.point);
    //reflected light vector
    vec3 r = normalize(reflect(-l, n));

    float mu = 0.3;
    float specularExponent = 100;

    float d = distance(lightSource, intersect.point);

    float attenuation = 1.0 /
      (gl_LightSource[0].constantAttenuation 
       + gl_LightSource[0].linearAttenuation * d 
       + gl_LightSource[0].quadraticAttenuation * d * d);

    //ambient color component
    ia = 0.1 * intersect.colour;
    // diffuse color component
    id = 0.5 * attenuation *  intersect.colour * max(dot(n,l),0.0);
    // speculor color component
    is = attenuation * intersect.colour * pow(max(dot(r,e),0.0),0.27 * specularExponent);

    // the final vertex colour
    vec3 outcol = ia + id + is;
    return outcol;
    
  }
  else{
    // if it hits - return ambient colour
    return 0.1 * intersect.colour;
  } 
  
  //////////////////////////////////////
	
}

void main()
{
  // please leave the scene definition unaltered for marking reasons
  // if you add your own geometry or scene, please use USE_OWN_GEOMETRY 1
  // and implement it wihtin preprocessor blocks 
  // #if USE_OWN_GEOMETRY
  // your scene
  // #endif //USE_OWN_GEOMETRY
  //scene definition:
  sphere[0].centre   = vec3(-2.0, 1.5, -3.5);
  sphere[0].radius   = 1.5;
  sphere[0].colour = vec3(0.8,0.8,0.8);
  sphere[1].centre   = vec3(-0.5, 0.0, -2.0);
  sphere[1].radius   = 0.6;
  sphere[1].colour = vec3(0.3,0.8,0.3);
  sphere[2].centre   = vec3(1.0, 0.7, -2.2);
  sphere[2].radius   = 0.8;
  sphere[2].colour = vec3(0.3,0.8,0.8);
  sphere[3].centre   = vec3(0.7, -0.3, -1.2);
  sphere[3].radius   = 0.2;
  sphere[3].colour = vec3(0.8,0.8,0.3);
  sphere[4].centre   = vec3(-0.7, -0.3, -1.2);
  sphere[4].radius   = 0.2;
  sphere[4].colour = vec3(0.8,0.3,0.3);
  sphere[5].centre   = vec3(0.2, -0.2, -1.2);
  sphere[5].radius   = 0.3;
  sphere[5].colour = vec3(0.8,0.3,0.8);
  plane.point = vec3(0,-0.5, 0);
  plane.normal = vec3(0, 1.0, 0);
  plane.colour = vec3(1, 1, 1);
  seed = int(mod(dir.x * dir.y * 39786038.0, 65536.0));
  //scene definition end

  //outcolour = vec4(1,1,1,1);

  //////////////////////////////////////
  //TODO Exercise 5
  //implement your ray tracing algorithm here
  //don't forget to finally integrate mouse-interaction

  // ============= initilise values =============

  
  Ray r;
  r.origin = (modelViewMatrix * vec4(origin,1)).xyz + vec3(0,0,2);
  //move origin back slightly as otherwise view starts off zoomed in too far
  r.dir = normalize((modelViewMatrix * vec4(dir,0)).xyz);

  vec3 colour = vec3(0,0,0);
  Intersection intersect;
  Ray reflectedRay = r;

  int i = 0;

  Intersect(reflectedRay,intersect);

  while(i < raytraceDepth && intersect.hit == 1 && intersect.t < MAX_DISTANCE){

    colour = colour +  (1.0/(i+1)) * computeShadow(intersect);

    reflectedRay.origin = intersect.point + (intersect.normal * epsilon);
    reflectedRay.dir = reflect(reflectedRay.dir,intersect.normal);
	    
    i = i + 1;
    Intersect(reflectedRay,intersect);
  }

  outcolour = vec4(colour,1);

  //////////////////////////////////////

}
