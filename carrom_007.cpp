#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define eps 0.0002
#define PI 3.141592653589

// Function Declarations
void drawScene();
void update(int value);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void mymotion(int x,int y);

// Global Variables
float theta=90,power =0;
int flag=0,points=30,mark[9]={0},marks=0,counts[3]={0},upflag=0,temp,flags=0;

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(r,g,b);
	glVertex3f(x1, y1, 0.0f);
	glColor3f(r,g,b);
	glVertex3f(x2, y2, 0.0f);
	glColor3f(r,g,b);
	glVertex3f(x3, y3, 0.0f);
	glEnd();
}

class Board {
	int type;
	public:
	float len;
	Board(float x, int t) {
		len=x;
		type=t; 
	}
	void make() {
		if(type)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glVertex2f(-len / 2, -len / 2);
		glVertex2f(len / 2, -len / 2);
		glVertex2f(len / 2, len / 2);
		glVertex2f(-len / 2, len / 2);
		glEnd();
	}
	void drawMiddleCircles(){
		float rad=0.8f,r=0.5f,g=0.5f,b=0.5f;
		glLineWidth(2.0f);
		glColor3f(r,g,b);
		glBegin(GL_LINE_LOOP);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
		glEnd();
		glLineWidth(5.0f);
		rad=0.87f;
		glColor3f(r,g,b);
		glBegin(GL_LINE_LOOP);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
			glEnd();
		glLineWidth(1.0f);
	}
	void drawCenterCircles(){
		float rad=0.15f,r=1.0f,g=0.0f,b=0.0f;
		glColor3f(r,g,b);
		glBegin(GL_LINE_LOOP);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
			glEnd();
		//drawFilledBall(rad,r,g,b,x,y,z);
	}
	void drawCenterDesigns(){
		int i;
		float x1;float x2;float x3;
		float y1;float y2;float y3;
		float r=1.0f;
		float g=0.0f;
		float b=0.0f;
		// white triangles
		r=0.45f;g=0.9f;b=0.0f;
		x1=0.17863f;x2=0.8f;x3=0.19;
		y1=0.0f;y2=0.0;y3=0.15;
		glPushMatrix();
		for(i=0;i<4;i++){
			drawTriangle(x1,y1,x2,y2,x3,y3,r,g,b);
			glRotatef(90.0,0.0,0.0,1.0);
		}
		glPopMatrix();
		r=1.0f;g=1.0f;b=1.0f;
		x1=0.17863f;x2=0.8f;x3=0.19;
		y1=0.0f;y2=0.0;y3=-0.15;
		glPushMatrix();
		for(i=0;i<4;i++){
			drawTriangle(x1,y1,x2,y2,x3,y3,r,g,b);
			glRotatef(90.0,0.0,0.0,1.0);
		}
		glPopMatrix();
		r=0.66f;g=0.66f;b=0.66f;
		x1=0.10606f;x2=0.56568f;x3=0.0;
		y1=0.10606f;y2=0.56568f;y3=0.17863;
		glPushMatrix();
		for(i=0;i<4;i++){
			drawTriangle(x1,y1,x2,y2,x3,y3,r,g,b);
			glRotatef(90.0,0.0,0.0,1.0);
		}
		glPopMatrix();
		r=0.3f;g=0.7f;b=0.7f;
		x1=0.10606f;x2=0.56568f;x3=0.10606f;
		y1=0.10606f;y2=0.56568f;y3=-0.10606f;
		glPushMatrix();
		for(i=0;i<4;i++){
			drawTriangle(x1,y1,x2,y2,x3,y3,r,g,b);
			glRotatef(90.0,0.0,0.0,1.0);
		}
		glPopMatrix();
	}
};
Board innerbox(4.0f, 1);
Board outerbox(4.4f, 1);
Board innersquare(2.8f, 0);

class Hole {
	public:
		float rad;
		Hole(float x) {
			rad=x;
		}
		void make() {
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
			glEnd();
		}
};
Hole hole(0.13f);
float hole_pos = (innerbox.len/2) - hole.rad;

class Power {
	int type;
	public:
	float len_x;
	float len_y;
	float r;
	float g;
	void set_coordinates(float x, float y, int t) {
		len_x=x;
		len_y=y;
		type=t;
		r=0.0f;
		g=0.0f;
	}
	void make() {
		if(type)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glVertex2f(-len_x / 2, 0);
		glVertex2f(len_x / 2, 0);
		glVertex2f(len_x / 2, len_y);
		glVertex2f(-len_x / 2, len_y);
		glEnd();
	}
};
Power powermeter;
Power powerlayer;

class Line {
	public:
		float len;
		float theta;
		Line(float l) {
			len=l;
			theta=90;
		}
		void make() {
			glBegin(GL_LINES);
			glVertex2f(0, 0);
			glVertex2f(len*cos(DEG2RAD(theta)), len*sin(DEG2RAD(theta)));
			glEnd();
		}
};
Line dirmeter(3.0f);

class Coin {
	public:
		float rad;
		float x;
		float y;
		float theta;
		float power;
		float vel_x;
		float vel_y;
		float mass;
		float pocketed;
		int point;
		void set_coordinates(float xp, float yp, float m, float r) {
			rad=r;
			x=xp;
			y=yp;
			vel_x=0;
			vel_y=0;
			theta=90;
			power=0;
			mass=m;
			pocketed=0;
		}
		void make(int p) {
			point =p;
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
			glEnd();
		}
		float get_direction()
		{
			if(abs(vel_x) > eps)
				return atan(vel_y/vel_x);
			else
				return PI/2;
		}
};
Coin strike,coins[9],samplecoin[3];

int main(int argc, char **argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w;
	int windowHeight = h;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Carrom_007");  // Setup the window
	initRendering();
	powermeter.set_coordinates(0.5f, 6.0f, 0);
	powerlayer.set_coordinates(0.5f, 0.0f, 1);
	strike.set_coordinates(0.0f, -innersquare.len/2, 20.0f, 0.12f);
	for(int i=0;i<8;i++) 
		coins[i].set_coordinates(0.3f*cos(DEG2RAD(45*i)),0.3f*sin(DEG2RAD(45*i)), 10.0f, 0.10f);
	coins[8].set_coordinates(0.0f, 0.0f,10.0f, 0.10f);
	for(int i=0;i<3;i++)
		samplecoin[i].set_coordinates(-3.8+(i*0.5),0.0f,10.0f,0.10f);
	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(Mouse);
	glutMotionFunc(mymotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(1, update, 0);
	glutMainLoop();
	return 0;
}

// Function to draw objects on the screen
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	//Draw outer box
	glTranslatef(0.0f, 0.0f, -6.0f);
	glColor3f(0.80f, 0.52f, 0.25f);
	outerbox.make();

	// Draw inner box
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.6f);
	innerbox.make();

	//Draw inner square
	glPushMatrix();
	glLineWidth(5.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.80f, 0.52f, 0.0f);
	innersquare.make();
	innersquare.drawMiddleCircles();
	innersquare.drawCenterCircles();
	innersquare.drawCenterDesigns();
	glPopMatrix();

	//Draw power layer
	glPushMatrix();
	glTranslatef(6.0f, -3.0f, -6.0f);
	glColor3f(powerlayer.r, powerlayer.g, 0.0f);
	powerlayer.make();
	glPopMatrix();

	//Draw power meter
	glPushMatrix();
	glTranslatef(6.0f, -3.0f, -6.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	powermeter.make();
	glPopMatrix();

	//Draw direction meter
	glPushMatrix();
	glTranslatef(strike.x, strike.y, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	dirmeter.make();
	glPopMatrix();

	int ar[2] = {-1,1};
	// Draw Holes
	for(int i=0; i<2; i++)
		for(int j=0; j<2; j++) {
			glPushMatrix();
			glTranslatef((-1)*ar[i]*hole_pos, (-1)*ar[j]*hole_pos, 0.0f);
			glColor3f(0.54f, 0.27f, 0.074f);
			hole.make();
			glPopMatrix();
		}

	glPushMatrix();
	glTranslatef(strike.x, strike.y, 0.0f);
	glColor3f(0.04f, 0.27f, 0.074f);
	strike.make(0);
	glPopMatrix();

	for(int i=0;i<8;i++) {

		glPushMatrix();
		glTranslatef(coins[i].x, coins[i].y, 0.0f);
		if(i%2)
		{
			glColor3f(0.9f,0.9f,0.0f);
			coins[i].make(10);
		}
		else
		{
			glColor3f(0.0f,0.0f,0.0f);
			coins[i].make(-5);
		}
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(coins[8].x,coins[8].y,0.0f);
	glColor3f(1.0f,0.0f,0.0f);
	coins[8].make(50);
	glPopMatrix();

	for(int i=0;i<3;i++)
	{
		glPushMatrix();
		glTranslatef(samplecoin[i].x, samplecoin[i].y, 0.0f);
		if(i==0)
			glColor3f(1.0f,0.0f,0.0f);
		else if(i==1)
			glColor3f(0.9f,0.9f,0.0f);
		else
			glColor3f(0.0f,0.0f,0.0f);
		samplecoin[i].make(0);
		glPopMatrix();
	}

	glPushMatrix();
	for(int i=0;i<3;i++)
	{
		if(i==0)
			glColor3f(1.0f,0.0f,0.0f);
		else if(i==1)
			glColor3f(0.9f,0.9f,0.0f);
		else
			glColor3f(0.0f,0.0f,0.0f);
		glRasterPos2f(-3.86f+(i*0.5), -0.3f);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
	}
	glPopMatrix();

	glPushMatrix();
	for(int i=0;i<3;i++)
	{
		if(i==0)
			glColor3f(1.0f,0.0f,0.0f);
		else if(i==1)
			glColor3f(0.9f,0.9f,0.0f);
		else
			glColor3f(0.0f,0.0f,0.0f);
		glRasterPos2f(-3.85f+(i*0.5), -0.5f);
		int temp=counts[i],j=0,a[10];
		while(temp)
		{
			a[j++]=(temp%10)+48;
			temp/=10;
		}
		if(!j)
			a[j++]=48;
		for (int k=j-1;k>=0;k--) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, a[k]);
		}
	}
	glPopMatrix();
	char score[6]="SCORE";
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos2f(-3.5f, 1.8f);
	for(int i=0;i<5;i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[i]);
	glPopMatrix();
	temp=points-temp;
	int i=0,a[10],m=0;
	if(temp<0)
		m=1;
	temp=fabs(temp);
	while(temp)
	{
		a[i++]=(temp%10)+48;
		temp/=10;
	}
	if(m)
		a[i++]='-';
	if(!i)
		a[i++]=48;
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos2f(-3.3f, 1.4f);
	int len=i;
	if(flags>12)
		for (int i=len-1;i>=0;i--) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, a[i]);
		}
	flags=(flags+1)%30;
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

void pocket(Coin &coin)
{
	int a[2]={1,-1};
	for (int i=0;i<2;i++)
	{
		for(int j=0;j<2;j++)
			if (sqrt((pow(coin.x-(a[i]*hole_pos), 2)+pow(coin.y-(a[j]*hole_pos), 2))) <= (hole.rad))
				coin.pocketed=1;
	}
}
void checkwall(Coin &wallcoin)
{
	float e_wall=0.8f;
	if(wallcoin.x+wallcoin.rad>=innerbox.len/2 || wallcoin.x-wallcoin.rad<=-innerbox.len/2)
	{
		wallcoin.vel_x*=-e_wall;
		if(wallcoin.x+wallcoin.rad >= innerbox.len/2)
			wallcoin.x=(innerbox.len/2)-wallcoin.rad;
		else
			wallcoin.x=-(innerbox.len/2)+wallcoin.rad;
	}
	if(wallcoin.y+wallcoin.rad>=innerbox.len/2 || wallcoin.y-wallcoin.rad<=-innerbox.len/2)
	{
		wallcoin.vel_y*=-e_wall;
		if (wallcoin.y+wallcoin.rad >= innerbox.len/2)
			wallcoin.y=(innerbox.len/2)-wallcoin.rad;
		else
			wallcoin.y=-(innerbox.len/2)+wallcoin.rad;
	}
}
void applyfriction(Coin &frictioncoin)
{
	float fk=0.00009f;
	float angle;
	if(frictioncoin.vel_x!=0)
		angle=(atan(abs(frictioncoin.vel_y/frictioncoin.vel_x)));
	else
		angle=DEG2RAD(90);
	if(abs(frictioncoin.vel_x)-fk*cos((angle)) > 0)
	{
		if (frictioncoin.vel_x > 0)
			frictioncoin.vel_x-=fk*cos((angle));
		else
			frictioncoin.vel_x+=fk*cos((angle));
	}
	else
		frictioncoin.vel_x=0;
	if (abs(frictioncoin.vel_y)-fk*sin((angle)) > 0)
	{
		if (frictioncoin.vel_y>0)
			frictioncoin.vel_y-=fk*sin((angle));
		else
			frictioncoin.vel_y+=fk*sin((angle));
	}
	else
		frictioncoin.vel_y=0;
}
void ballCollision(Coin &c1,Coin &c2)
{
	if (sqrt(((c1.x-c2.x)*(c1.x-c2.x))+((c1.y-c2.y)*(c1.y-c2.y))) <= (c1.rad+c2.rad))
	{
		if ((abs(c1.x-c2.x)) != 0)
			theta=PI-atan((c1.y-c2.y)/(c1.x-c2.x));
		else
			theta=DEG2RAD(90);
		float e_ball=0.8f;
		float initVel_x1=(c1.vel_x*cos(theta))-(c1.vel_y*sin(theta));
		float initVel_y1=(c1.vel_y*cos(theta))+(c1.vel_x*sin(theta));
		float initVel_x2=(c2.vel_x*cos(theta))-(c2.vel_y*sin(theta));
		float initVel_y2=(c2.vel_y*cos(theta))+(c2.vel_x*sin(theta));
		float finVel_x1=((c2.mass*(initVel_x2-(e_ball*(initVel_x1-initVel_x2)))+c1.mass*(initVel_x1))/(c1.mass+c2.mass));//w1
		float finVel_x2=finVel_x1+(e_ball*(initVel_x1-initVel_x2));
		c1.vel_x=finVel_x1*cos(theta)+initVel_y1*sin(theta);
		c1.vel_y=-finVel_x1*sin(theta)+initVel_y1*cos(theta);
		c2.vel_x=finVel_x2*cos(theta)+initVel_y2*sin(theta);
		c2.vel_y=-finVel_x2*sin(theta)+initVel_y2*cos(theta);
		float diff=(c1.rad+c2.rad)-(sqrt(pow(c1.x-c2.x, 2)+pow(c1.y-c2.y, 2)));
		diff*=2;
		float angle=abs(atan((c1.y-c2.y)/(c1.x-c2.x)));
		float diff_x=diff*cos(angle);
		float diff_y=diff*sin(angle);
		if(diff>0)
		{
			if(c1.x<c2.x)
			{
				c1.x-=diff_x/2;
				c2.x+=diff_x/2;
			}
			else
			{
				c1.x+=diff_x/2;
				c2.x-=diff_x/2;
			}

			if(c1.y<c2.y)
			{
				c1.y-=diff_y/2;
				c2.y+=diff_y/2;
			}
			else
			{
				c1.y+=diff_y/2;
				c2.y-=diff_y/2;
			}
		}
	}
	}int f=0;
	// Function to handle all calculations in the scene
	// updated evry 10 milliseconds
	void update(int value) {
		if(!upflag)
		{
			if(counts[0]+counts[1]+counts[2]<9)
				temp=glutGet(GLUT_ELAPSED_TIME)/1000;
			// Update position of ball
			for (int i = 0; i < 9; i++)
			{
				coins[i].x+=coins[i].vel_x;
				coins[i].y+=coins[i].vel_y;
			}
			strike.x+=strike.vel_x;
			strike.y+=strike.vel_y;
			f=0;
			// Handle ball collisions with box
			for (int i = 0; i < 9; i++)
			{
				checkwall(coins[i]);
				applyfriction(coins[i]);
				pocket(coins[i]);
				if(coins[i].pocketed)
				{
					coins[i].x=10.0f;
					coins[i].y=10.0f;
					if(!mark[i])
					{
						mark[i]=1;
						points+=coins[i].point;
						if(i%2&&i<8)
							counts[1]++;
						else if(i%2==0 && i<8)
							counts[2]++;
						else
							counts[0]++;
					}
				}
			}
			checkwall(strike);
			applyfriction(strike);
			pocket(strike);
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (i != j)
						ballCollision(coins[i], coins[j]);
				}
				ballCollision(coins[i], strike);
			}

			if(fabs(strike.vel_x)<eps && fabs(strike.vel_y)<eps && flag ==1)
			{
				for(int i=0;i<9;i++)
					if(fabs(coins[i].vel_x)>eps||fabs(coins[i].vel_y)>eps)
					{
						f=1;
						break;
					}
			}
			else{
				f=1;
			}
			if(!f)
			{
				flag=0;
				strike.power=0;
				dirmeter.theta=90;
				strike.theta=90;
				powerlayer.len_y=0;
				strike.x=0;
				strike.y=-innersquare.len/2;
				dirmeter.len=3.0f;    
			}
		}
		glutTimerFunc(1, update, 0);
	}

	void drawTriangle() {

		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}

	// Initializing some openGL 3D rendering options
	void initRendering() {

		glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
		glEnable(GL_COLOR_MATERIAL);    // Enable coloring
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
	}

	// Function called when the window is resized
	void handleResize(int w, int h) {

		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void handleKeypress1(unsigned char key, int x, int y) {

		if (key == 27) {
			exit(0);     // escape key is pressed
		}
		if(key==32&&!flag)
		{
			flag=1;
			dirmeter.len=0;
			strike.vel_x=(2*strike.power/strike.mass)*cos(DEG2RAD(strike.theta));
			strike.vel_y=(2*strike.power/strike.mass)*sin(DEG2RAD(strike.theta));
		}
		if(key=='a'&&!flag) 
		{
			strike.theta+=1;
			dirmeter.theta+=1;
		}
		if(key=='c'&&!flag)
		{
			strike.theta-=1;
			dirmeter.theta-=1;
		}
		if(key=='p')
			upflag=(upflag+1)%2;
	}

	void handleKeypress2(int key, int x, int y) {

		if (key == GLUT_KEY_LEFT&&strike.x>-((innersquare.len/2)-strike.rad)&&!flag)
			strike.x -= 0.01f;
		if (key == GLUT_KEY_RIGHT&&strike.x<(innersquare.len/2)-strike.rad&&!flag)
			strike.x += 0.01f;
		if(key==GLUT_KEY_UP&&strike.power<1.0f&&!flag)
		{
			strike.power+=0.1f;
			powerlayer.len_y+=0.6f;
			powerlayer.r=(strike.power);
			powerlayer.g=(1.0f-strike.power);
		}
		if(key==GLUT_KEY_DOWN&&strike.power>0.09f&&!flag)
		{
			strike.power-=0.1f;
			powerlayer.len_y-=0.6f;
			powerlayer.r=(strike.power);
			if(strike.power>0.09f)
				powerlayer.g=(1.0f-strike.power);
			else
				powerlayer.g=0.0f;
		}
	}
	int drag;
	GLdouble ox=0.0,oy=0.0,oz=0.0;
	void Mouse(int button,int state,int x,int y) {
		GLint viewport[4];
		GLdouble modelview[16],projection[16];
		GLfloat wx=x,wy,wz;
		if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN&&!flag){
			glGetIntegerv(GL_VIEWPORT,viewport);
			y=viewport[3]-y;
			wy=y;
			glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
			glGetDoublev(GL_PROJECTION_MATRIX,projection);
			glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
			gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
			glutPostRedisplay();

			float temp1=((ox*60-strike.x));
			float temp2=((oy*60-strike.y));
			if(temp1<=0)
				theta=(PI+atan(temp2/temp1)) * 180 / PI;
			else
				theta=atan(temp2/temp1) * 180 / PI;
		}
		if(button==GLUT_LEFT_BUTTON && state==GLUT_UP&&!flag){
			strike.power=(oy*60+2.0f)/2;
			powerlayer.len_y=(oy*60+2.0f)/2*3;
			powerlayer.r=(strike.power/2);
			powerlayer.g=(1.0f-strike.power/2);
			dirmeter.len=0;
			dirmeter.theta=theta;
			strike.vel_y=(2*strike.power/strike.mass)*sin(DEG2RAD(theta)); 
			strike.vel_x=(2*strike.power/strike.mass)*cos(DEG2RAD(theta)); 
			flag=1;
		}
		if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN&&!flag){
			drag=1;
		}
		else
			drag=0;
	}

	void mymotion(int x,int y){
		GLint viewport[4];
		GLdouble modelview[16],projection[16];
		GLfloat wx=x,wy,wz;
		if(drag==1&&!flag){
			glGetIntegerv(GL_VIEWPORT,viewport);
			y=viewport[3]-y;
			wy=y;
			glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
			glGetDoublev(GL_PROJECTION_MATRIX,projection);
			glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
			gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
			glutPostRedisplay();
			strike.x=ox*60;
		}
	}
