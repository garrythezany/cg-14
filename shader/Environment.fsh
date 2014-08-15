#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 o_color;

in VertexData { vec3 coord; }
inData;

void main() {
	o_color= vec4(0,1,0,1);
  vec3 c0 = vec3(0, 0, 1);
  vec3 c1 = vec3(0.75, 0.75, 1);
  vec3 c2 = vec3(0.75, 0.75, 1);
  vec3 c3 = vec3(0.75, 0.75, 1);
  vec3 c4 = vec3(0.75, 0.75, 1);
  vec3 c5 = vec3(0, 0, 0);

  vec3 planevec1 = inData.coord;
  planevec1.y = 0;

  float angle1 = acos(dot(inData.coord, planevec1) / (length(inData.coord) * length(planevec1)));

  if(angle1 >= PI)
    angle1 = 2 * PI - angle1;

  angle1 /= (PI / 2);
  if(angle1 <0)
  	angle1 =0;
  if(angle1 >1)
  	angle1 = 1;

  vec3 planevec2;
  float angle2;

  // // for c1,c2
  // if(inData.coord.x > 0 && inData.coord.z >= 0) {
  //   planevec2 = inData.coord;
  //   planevec2.z = 0;

  //   angle2 = acos(inData.coord * planevec2 / (length(inData.coord) * length(planevec2)));

  //   if(angle2 > PI)
  //     angle2 = 2 * PI - angle2;

  //   angle2 /= (PI / 2);


  //   vec3 res;

  //   res = inData.coord.z * angle2 * c2 + (1 - inData.coord.x * angle2) *c1;

  //   if(inData.coord.y < 0)
  //     res = -inData.coord.y * angle1 * c5 + (1 + inData.coord.y * angle1) * res;
  //   else
  //     res = inData.coord.y * angle1 * c0 + (1 - inData.coord.y * angle1) * res;
  //   o_color = vec4(res, 1);

  // }
  // // for c2,c3
  // else if(inData.coord.x <= 0 && inData.coord.z > 0) {

  // }
  // // for c3,c4
  // else if(inData.coord.x < 0 && inData.coord.z <= 0) {

  // }
  // // for c4,c1
  // else if(inData.coord.x >= 0 && inData.coord.z < 0) {
  // }




  vec3 res;
  if(inData.coord.y <= 0)
    // res = angle1 * c5 + (1 - angle1) * c2;
    res = mix(c2,c5, angle1);
  else if(inData.coord.y >= 0)
    res = mix(c2,c0, angle1);
    // res = angle1 * c0 + (1 - angle1) * c2;
	  o_color = vec4(res, 1);



//   if(angle1>1||angle1 < 0)
//   	o_color= vec4(1,0,0,1);
 }