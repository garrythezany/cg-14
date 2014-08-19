#include "Environment.hpp"

#include <math.h>

Environment::Environment() : m_dayLength(10), m_time(0), m_day(1), visible(true) {}

void Environment::draw(Mat4f viewMat, Mat4f projMat)
{

	if(m_day % 2 != 0)
	{
		visible = true;
	}
	else
	{
		visible = false;
	}


	m_program.prime([&](HotProgram& hotprog)
	{

		viewMat.setColumn(3, Vec4f(0,0,0,1));

		hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.uniform["u_colorUp"] = getcUp();
		hotprog.uniform["u_colorWest"] = getcWest();
		hotprog.uniform["u_colorNorth"] = getcNorth();
		hotprog.uniform["u_colorEast"] = getcEast();
		hotprog.uniform["u_colorSouth"] = getcSouth();
		hotprog.uniform["u_colorDown"] = getcDown();

		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);

	});

  m_program2.prime([&](HotProgram& hotprog)
  {

    float a = m_time * 3.1415 * 2 / (m_dayLength);


    viewMat.setColumn(3, Vec4f(0,0,0,1));

    Mat4f rotMat = rotationMatrix(quaternionFromAxisAngle(Vec3f(0,0,1), a));

    if(visible)
    {

    	hotprog.uniform["u_color"] = Vec3f(0.75, 0.75, 0.75);
    	hotprog.uniform["u_transform"] = projMat * (viewMat * rotMat);
    	hotprog.draw(m_moon, PrimitiveType::TriangleFan);
    
    }
    
    hotprog.uniform["u_color"] = getSunColor();
    hotprog.uniform["u_transform"] = projMat * (viewMat * rotMat);
    hotprog.draw(m_sun, PrimitiveType::TriangleFan);

  });

}

void Environment::init()
{

	VShader vs;
	vs.compile(loadShaderFromFile("shader/Environment.vsh"));
	FShader fs;
	fs.compile(loadShaderFromFile("shader/Environment.fsh"));

	constexpr float PI = 3.1415926;

  // create program and link the two shaders
	m_program.create(vs, fs);

  m_sphere.create(3 + 2, 6, 13);
  m_sphere.prime<Vec3f, Vec2f>([](HotVertexSeq<Vec3f, Vec2f>& hot)
  {

  	hot.vertex[0].set(Vec3f(0, 1, 0),  Vec2f(0,0));
  	hot.vertex[1].set(Vec3f(1, 0, 0),  Vec2f(PI/2,0));
  	hot.vertex[2].set(Vec3f(0, 0, 1) , Vec2f(PI/2,0));
  	hot.vertex[3].set(Vec3f(-1, 0, 0), Vec2f(PI/2,0));
  	hot.vertex[4].set(Vec3f(0, 0, -1), Vec2f(PI/2,0));
  	hot.vertex[5].set(Vec3f(0, -1, 0), Vec2f(PI,0));

  	hot.index[0] = 0;
  	hot.index[1] = 2;
  	hot.index[2] = 1;
  	hot.index[3] = 5;
  	hot.index[4] = 4;
    hot.index[5] = 3;
    hot.index[6] = GLIndex::PrimitiveRestart;
  	hot.index[7] = 5;
  	hot.index[8] = 2;
  	hot.index[9] = 3;
  	hot.index[10] = 0;
  	hot.index[11] = 4;
  	hot.index[12] = 1;

  });

  VShader vs2;
  vs2.compile(loadShaderFromFile("shader/Sun.vsh"));
  FShader fs2;
  fs2.compile(loadShaderFromFile("shader/Sun.fsh"));

  // create program and link the two shaders
  m_program2.create(vs2, fs2);

  m_moon.create(3, 34);
  m_moon.prime<Vec3f>([](HotVertexSeq<Vec3f>& hot)
  {
  	// Mond
    hot.vertex[0]. set(Vec3f(                   0, 20, 0                  ));
    hot.vertex[1]. set(Vec3f(                   1, 20, 0                  ));
    hot.vertex[2]. set(Vec3f( cos(     3.1415/16), 20, sin(     3.1415/16)));
    hot.vertex[3]. set(Vec3f( cos( 2 * 3.1415/16), 20, sin( 2 * 3.1415/16)));
    hot.vertex[4]. set(Vec3f( cos( 3 * 3.1415/16), 20, sin( 3 * 3.1415/16)));
    hot.vertex[5]. set(Vec3f( cos( 4 * 3.1415/16), 20, sin( 4 * 3.1415/16)));
    hot.vertex[6]. set(Vec3f( cos( 5 * 3.1415/16), 20, sin( 5 * 3.1415/16)));
    hot.vertex[7]. set(Vec3f( cos( 6 * 3.1415/16), 20, sin( 6 * 3.1415/16)));
    hot.vertex[8]. set(Vec3f( cos( 7 * 3.1415/16), 20, sin( 7 * 3.1415/16)));   
    hot.vertex[9]. set(Vec3f( cos( 8 * 3.1415/16), 20, sin( 8 * 3.1415/16)));   
    hot.vertex[10].set(Vec3f( cos( 9 * 3.1415/16), 20, sin( 9 * 3.1415/16)));
    hot.vertex[11].set(Vec3f( cos(10 * 3.1415/16), 20, sin(10 * 3.1415/16)));
    hot.vertex[12].set(Vec3f( cos(11 * 3.1415/16), 20, sin(11 * 3.1415/16)));
    hot.vertex[13].set(Vec3f( cos(12 * 3.1415/16), 20, sin(12 * 3.1415/16)));
    hot.vertex[14].set(Vec3f( cos(13 * 3.1415/16), 20, sin(13 * 3.1415/16)));
    hot.vertex[15].set(Vec3f( cos(14 * 3.1415/16), 20, sin(14 * 3.1415/16)));
    hot.vertex[16].set(Vec3f( cos(15 * 3.1415/16), 20, sin(15 * 3.1415/16)));
    hot.vertex[17].set(Vec3f( cos(16 * 3.1415/16), 20, sin(16 * 3.1415/16)));   
    hot.vertex[18].set(Vec3f( cos(17 * 3.1415/16), 20, sin(17 * 3.1415/16)));
    hot.vertex[19].set(Vec3f( cos(18 * 3.1415/16), 20, sin(18 * 3.1415/16)));
    hot.vertex[20].set(Vec3f( cos(19 * 3.1415/16), 20, sin(19 * 3.1415/16)));
    hot.vertex[21].set(Vec3f( cos(20 * 3.1415/16), 20, sin(20 * 3.1415/16)));
    hot.vertex[22].set(Vec3f( cos(21 * 3.1415/16), 20, sin(21 * 3.1415/16)));
    hot.vertex[23].set(Vec3f( cos(22 * 3.1415/16), 20, sin(22 * 3.1415/16)));
    hot.vertex[24].set(Vec3f( cos(23 * 3.1415/16), 20, sin(23 * 3.1415/16)));
    hot.vertex[25].set(Vec3f( cos(24 * 3.1415/16), 20, sin(24 * 3.1415/16)));   
    hot.vertex[26].set(Vec3f( cos(25 * 3.1415/16), 20, sin(25 * 3.1415/16)));
    hot.vertex[27].set(Vec3f( cos(26 * 3.1415/16), 20, sin(26 * 3.1415/16)));
    hot.vertex[28].set(Vec3f( cos(27 * 3.1415/16), 20, sin(27 * 3.1415/16)));
    hot.vertex[29].set(Vec3f( cos(28 * 3.1415/16), 20, sin(28 * 3.1415/16)));
    hot.vertex[30].set(Vec3f( cos(29 * 3.1415/16), 20, sin(29 * 3.1415/16)));
    hot.vertex[31].set(Vec3f( cos(30 * 3.1415/16), 20, sin(30 * 3.1415/16)));
    hot.vertex[32].set(Vec3f( cos(31 * 3.1415/16), 20, sin(31 * 3.1415/16)));
    hot.vertex[33].set(Vec3f(                   1, 20, 0                  ));

  });

	m_sun.create(3, 34);
  m_sun.prime<Vec3f>([](HotVertexSeq<Vec3f>& hot)
  {
  	// Sonne
    hot.vertex[0]. set(Vec3f(                   0,-15, 0                  ));
    hot.vertex[1]. set(Vec3f(                   1,-15, 0                  ));
    hot.vertex[2]. set(Vec3f( cos(     3.1415/16),-15, sin(     3.1415/16)));
    hot.vertex[3]. set(Vec3f( cos( 2 * 3.1415/16),-15, sin( 2 * 3.1415/16)));
    hot.vertex[4]. set(Vec3f( cos( 3 * 3.1415/16),-15, sin( 3 * 3.1415/16)));
    hot.vertex[5]. set(Vec3f( cos( 4 * 3.1415/16),-15, sin( 4 * 3.1415/16)));
    hot.vertex[6]. set(Vec3f( cos( 5 * 3.1415/16),-15, sin( 5 * 3.1415/16)));
    hot.vertex[7]. set(Vec3f( cos( 6 * 3.1415/16),-15, sin( 6 * 3.1415/16)));
    hot.vertex[8]. set(Vec3f( cos( 7 * 3.1415/16),-15, sin( 7 * 3.1415/16)));   
    hot.vertex[9]. set(Vec3f( cos( 8 * 3.1415/16),-15, sin( 8 * 3.1415/16)));   
    hot.vertex[10].set(Vec3f( cos( 9 * 3.1415/16),-15, sin( 9 * 3.1415/16)));
    hot.vertex[11].set(Vec3f( cos(10 * 3.1415/16),-15, sin(10 * 3.1415/16)));
    hot.vertex[12].set(Vec3f( cos(11 * 3.1415/16),-15, sin(11 * 3.1415/16)));
    hot.vertex[13].set(Vec3f( cos(12 * 3.1415/16),-15, sin(12 * 3.1415/16)));
    hot.vertex[14].set(Vec3f( cos(13 * 3.1415/16),-15, sin(13 * 3.1415/16)));
    hot.vertex[15].set(Vec3f( cos(14 * 3.1415/16),-15, sin(14 * 3.1415/16)));
    hot.vertex[16].set(Vec3f( cos(15 * 3.1415/16),-15, sin(15 * 3.1415/16)));
    hot.vertex[17].set(Vec3f( cos(16 * 3.1415/16),-15, sin(16 * 3.1415/16)));   
    hot.vertex[18].set(Vec3f( cos(17 * 3.1415/16),-15, sin(17 * 3.1415/16)));
    hot.vertex[19].set(Vec3f( cos(18 * 3.1415/16),-15, sin(18 * 3.1415/16)));
    hot.vertex[20].set(Vec3f( cos(19 * 3.1415/16),-15, sin(19 * 3.1415/16)));
    hot.vertex[21].set(Vec3f( cos(20 * 3.1415/16),-15, sin(20 * 3.1415/16)));
    hot.vertex[22].set(Vec3f( cos(21 * 3.1415/16),-15, sin(21 * 3.1415/16)));
    hot.vertex[23].set(Vec3f( cos(22 * 3.1415/16),-15, sin(22 * 3.1415/16)));
    hot.vertex[24].set(Vec3f( cos(23 * 3.1415/16),-15, sin(23 * 3.1415/16)));
    hot.vertex[25].set(Vec3f( cos(24 * 3.1415/16),-15, sin(24 * 3.1415/16)));   
    hot.vertex[26].set(Vec3f( cos(25 * 3.1415/16),-15, sin(25 * 3.1415/16)));
    hot.vertex[27].set(Vec3f( cos(26 * 3.1415/16),-15, sin(26 * 3.1415/16)));
    hot.vertex[28].set(Vec3f( cos(27 * 3.1415/16),-15, sin(27 * 3.1415/16)));
    hot.vertex[29].set(Vec3f( cos(28 * 3.1415/16),-15, sin(28 * 3.1415/16)));
    hot.vertex[30].set(Vec3f( cos(29 * 3.1415/16),-15, sin(29 * 3.1415/16)));
    hot.vertex[31].set(Vec3f( cos(30 * 3.1415/16),-15, sin(30 * 3.1415/16)));
    hot.vertex[32].set(Vec3f( cos(31 * 3.1415/16),-15, sin(31 * 3.1415/16)));
    hot.vertex[33].set(Vec3f(                   1,-15, 0                  ));

  });

}

void Environment::update(float delta)
{

	m_time += delta;

	if(m_time > m_dayLength)
	{
		m_time -= m_dayLength;
		m_day++;
	}

}

void Environment::setDayLength(float sec) 
{ 

	m_dayLength = sec; 

}

Vec3f Environment::getSunColor()
{

	float r, g, b;
	float help;

	r=0;
	g=0;
	b=0;

	if(m_time > 0.75 * m_dayLength) 
	{

		help = m_time - 0.75 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1 - help;

	} 
	else if(m_time > 0.5 * m_dayLength) 
	{

		help = m_time - 0.5 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = 1 - help;
		b = 0.5 * (1 - help);

	} 
	else if(m_time > 0.25 * m_dayLength) 
	{

		help = m_time - 0.25 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = help;
		b= 0.5 * help;

	} 
	else 
	{

		help = m_time / (m_dayLength/4);
		r = help;

	}

	return Vec3f(r,g,b);

}

float Environment::getSunIntensity(){

	float help;

	help = m_time / m_dayLength;

	if(help>0.25 && help<0.75){

		help -= 0.25;
		if(help > 0.25){
			help = 0.5 - help;
		}

		return help*3;

	}
	return 0.0;
}

Vec3f Environment::getSunPos(){

	return Vec3f(sin(m_time*2*3.1415/m_dayLength),-cos(m_time*2*3.1415/m_dayLength),0);
}






Vec3f Environment::getcUp()
{

	float help = m_time;

	if(m_time > 0.5 * m_dayLength)
	{

		help = m_dayLength - m_time; 

	}

	return Vec3f(0, 0, 0.7 * (help/5));

}

Vec3f Environment::getcWest()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 + (0.25 * help), 0.75 * help, help);

  } 
  else 
  {
    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 * help, 0, 0);
  }

}

Vec3f Environment::getcNorth()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.5)
    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);

  }

}

Vec3f Environment::getcEast()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.5 * (1 - help), 0, 0);

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.25 * help), 0.75 - (0.75 * help), 1 - help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);

  }
}

Vec3f Environment::getcSouth()
{

	return getcNorth();

}

Vec3f Environment::getcDown()
{

	return Vec3f(0,0,0);

}