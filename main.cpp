//
//Class:            Software Engineering 3350
//Program:          rainforest.cpp
//Author:           Gordon Griesel
//Date:             2013 to 2018
//
//Modified by:      Melanie Corral and Adrian Telan
//Last modified:    5 May 2020
//
//This program demonstrates the use of OpenGL and XWindows
//
//Texture maps are displayed.
//Press B to see bigfoot roaming his forest.
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include "adrianT.h"
#include "melanieC.h"
#include "log.h"
//#include "ppm.h"
#include "fonts.h"

using namespace std;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1
#define PI 3.141592653589793
//-----------------------------------------------------------------------------
//Setup timers
//clock_gettime(CLOCK_REALTIME, &timePause);
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
const int MAX_BULLETS = 5;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------
//Credits Page Method Prototypes
extern void CreditsTitle(int);
extern void MC_Credits(int);
extern void AT_Credits(int);

//Leaderboards Page Method Prototypes
int argc = 3;
char argvh[] = {"odin.cs.csub.edu"};                    //host
char argvpr[] = {"/~mcorral/3350/scores.php?param=Scores"};  //read from page
char argvpw[] = {"/~mcorral/3350/scores.php?param=Scores"};  //write to page
char* argvr[] = {NULL, argvh, argvpr};
char* argvw[] = {NULL, argvh, argvpw};
extern char *serverHandling(int argc, char* argv[], char score[]);

//Controls Scheme Method Prototypes
extern void PrintControls(int);

//Class Quiz
extern void PrintTasks(int);

//Methods from 'adrianT.cpp'
extern void DrawSquare(int);
//-----------------------------------------------------------------------------

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};
Image img[4] = {
"./images/bigfoot.png",
"./images/background.png",
"./images/cuteashell.png",
"./images/umbrella.png" };

class Global {
public:
    int credits;
int nbullets;

//adrian: shooting related variables and structs
bool reload;

struct timespec bulletTimer;
struct timespec reloadTimer;
Bullets *barr;

	int done;
	int xres, yres;
	GLuint bigfootTexture;
	GLuint silhouetteTexture;
	GLuint forestTexture;
	GLuint forestTransTexture;
	GLuint umbrellaTexture;
	int showBigfoot;
	int forest;
	int silhouette;
	int trees;
	int showRain;
	int showUmbrella;
	int deflection;
	Global() {
		logOpen();
		done=0;
		xres=1280;
		yres=720;
		showBigfoot=0;
		forest=1;
		silhouette=1;
		trees=1;
		showRain=0;
		showUmbrella=0;
		deflection=0;
	}
	~Global() {
		logClose();
	}
} g;

class Bigfoot {
public:
	Vec pos;
	Vec vel;
} bigfoot;

class Raindrop {
public:
	int type;
	int linewidth;
	int sound;
	Vec pos;
	Vec lastpos;
	Vec vel;
	Vec maxvel;
	Vec force;
	float length;
	float color[4];
	Raindrop *prev;
	Raindrop *next;
} *rainhead = NULL;
int ndrops=1;
int totrain=0;
int maxrain=0;
void deleteRain(Raindrop *node);
void cleanupRaindrops(void);
//
#define UMBRELLA_FLAT  0
#define UMBRELLA_ROUND 1
class Umbrella {
public:
	int shape;
	Vec pos;
	Vec lastpos;
	float width;
	float width2;
	float radius;
} umbrella;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setupScreenRes(1280, 720);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
							StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		setTitle();
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setTitle() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "3350 - CUTE as HELL");
	}
	void setupScreenRes(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshapeWindow(int width, int height) {
		//window has been resized.
		setupScreenRes(width, height);
		//
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		setTitle();
	}
	void checkResize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshapeWindow(xce.width, xce.height);
		}
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
} x11;

//*****Player Class Instantiation*****
//Note: Hey I changed your players spawn position
//you originally had him in the middle (2.0)
Vec playerPos = {g.xres/7.0 , g.yres/2.0, 0.0};
Vec healthPos = {(float) g.xres, (float) g.yres, 0.0};
Vec gameOverScr = {g.xres/2.0, g.yres/2.0, 0.0};
Vec ammoUIPos = {g.xres - 80.0, g.yres - 100.0, 0.0};
Player player(100, 25, playerPos);

//*****Bullet Class Instantiations*****
Vec bulletVel = {0.0, 0.0, 0.0};
Bullet plBullet(5, playerPos, bulletVel);

//plBullet.pos[0] = player.pos[0];
//plBullet.pos[1] = player.pos[1];

//*****Enemy Class Instantiations*****
//Note to Melanie:
//Array for enemies on screen? For example:
//Enemy dps[numEnemiesOnScreen];
//Enemy healers[numEnemiesOnScreen];
Vec enemyPos = {g.xres/1.5, g.yres/2.0, 0.0};
Vec enemyPos1 = {g.xres/3.1, g.yres/2.0, 0.0};
Enemy enemy(50, 25, enemyPos);
Enemy enemy1(50, 25, enemyPos1);


//****Platform Class Instantiation*****
Vec groundPos = {g.xres/10.0, g.yres/10.0, 0.0};
Vec groundVel = {0.0 ,0.0 ,0.0};
int groundSize = 20;
Platform ground(groundSize, groundPos, groundVel);

Vec cielPos = {g.xres/10.0 + 150, g.yres/10.0 + 120, 0.0};
Platform ciel(20, cielPos, groundVel);

Vec startPos = {g.xres/10.0 - 10, g.yres/10.0 + 230, 0.0};
Platform start(20, startPos, groundVel);

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);
extern void menu();
extern int nbuttons;
//extern Button button[];

//score variables
int Scores = 0; 
char score[256];
char *ptrScore;
int main()
{
//int nbullets = 0;
//int barr = new Bullets[MAX_BULLETS];
    ptrScore = serverHandling(argc, argvr, score);
    g.credits = 0;
	initOpengl();
	init();
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;

	while (!done) {
		while (x11.getXPending()) {
			//XEvent e;
			//XNextEvent(dpy, &e);
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		//
		//Below is a process to apply physics at a consistent rate.
		//1. Get the current time.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while (physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		render();
		x11.swapBuffers();
	}
	//cleanupXWindows();
	cleanup_fonts();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
		*(ptr+3) = (a|b|c);
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
//	bigfootImage     = ppm6GetImage("./images/bigfoot.ppm");
//	forestImage      = ppm6GetImage("./images/forest.ppm");
//	forestTransImage = ppm6GetImage("./images/forestTrans.ppm");
//	umbrellaImage    = ppm6GetImage("./images/umbrella.ppm");
	//create opengl texture elements
	glGenTextures(1, &g.bigfootTexture);
	glGenTextures(1, &g.silhouetteTexture);
	glGenTextures(1, &g.forestTexture);
	glGenTextures(1, &g.umbrellaTexture);
	//-------------------------------------------------------------------------
	//bigfoot
	//
	int w = img[0].width;
	int h = img[0].height;
	//
	glBindTexture(GL_TEXTURE_2D, g.bigfootTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	//-------------------------------------------------------------------------
	//
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(&img[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	//-------------------------------------------------------------------------
	//
	//umbrella
	//
	glBindTexture(GL_TEXTURE_2D, g.umbrellaTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	silhouetteData = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	//-------------------------------------------------------------------------
	//
	//forest
	glBindTexture(GL_TEXTURE_2D, g.forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		img[1].width, img[1].height,
		0, GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
	//-------------------------------------------------------------------------
	//
	//forest transparent part
	//
	glBindTexture(GL_TEXTURE_2D, g.forestTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = img[2].width;
	h = img[2].height;
	unsigned char *ftData = buildAlphaData(&img[2]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	//-------------------------------------------------------------------------
}

void initSounds()
{

}

void init() {
	umbrella.pos[0] = 220.0;
	umbrella.pos[1] = (double)(g.yres-200);
	VecCopy(umbrella.pos, umbrella.lastpos);
	umbrella.width = 200.0;
	umbrella.width2 = umbrella.width * 0.5;
	umbrella.radius = (float)umbrella.width2;
	umbrella.shape = UMBRELLA_FLAT;
	MakeVector(-150.0,180.0,0.0, bigfoot.pos);
	MakeVector(6.0,0.0,0.0, bigfoot.vel);
}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	//static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	    if (savex != e->xbutton.x || savey != e->xbutton.y) {
		    //Mouse moved
		    savex = e->xbutton.x;
		    savey = e->xbutton.y;
	    }
    }
}

int checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift = 0;
    static int a = 0;
    static int d = 0;
    //static bool reload = false;
    //Bullet *b = &player.ammo[player.mag];

    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;

	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift = 0;

        if (key == XK_a || key == XK_d) {
            if (key == XK_a)
                a = 0;
            if (key == XK_d)
                d = 0;
            if (a == 0 && d == 0)
                player.run(0);
        }

        if (key == XK_k)
            player.isShooting = false;

        if (key ==XK_l) {
            g.reload = false;
            player.isReloading = false;
        }

		return 0;
	}

	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift = 1;
		return 0;
	}

	switch (key) {

        //adrian: Skeleton for Movement Controls (W,A,S,D)
        case XK_w:
            break;
        case XK_a:
            a = 1;
            player.run(-10);
            break;
        case XK_s:
            break;
        case XK_d:
            d = 1;
            player.run(10);
            break;

        //adrian: Skeleton for Shooting
        case XK_k:
           //Bullet *b = &player.ammo[player.mag];
            /*
            if (player.mag < player.magMax) {
                player.shoot(b);
                player.mag++;
            }
            */
            player.isShooting = true;
/*            timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            ts = timeDiff(&g.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&g.bulletTimer, &bt);
                CheckShot(&player);
            }
*/
            break;
        //adrian: skeleton for reloading
        case XK_l:
            //player.mag = 0;
            g.reload = true;
            player.isReloading = true;
            //CheckReload(&player, reload);
            break;

        //adrian: Skeleton for Jumping
        case XK_space:
            player.jump(15);
            break;

		case XK_b:
			g.showBigfoot ^= 1;
			if (g.showBigfoot) {
			    bigfoot.pos[0] = -250.0;
			}
			break;
		case XK_j:
			break;
		//case XK_f:
			//g.forest ^= 1;
			//break;
		case XK_o:
			g.silhouette ^= 1;
			printf("silhouette: %i\n", g.silhouette);
			break;
		case XK_t:
			g.trees ^= 1;
			break;
		//case XK_u:
			//g.showUmbrella ^= 1;
			//break;
		case XK_p:
			umbrella.shape ^= 1;
			break;
		//case XK_r:
			//g.showRain ^= 1;
			//break;
        case XK_c:
            if (g.credits == 1) {
                g.credits = 0;
            } else {
                g.credits = 1;
            }
		case XK_Left:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] -= 10.0;
			break;
		case XK_Right:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] += 10.0;
			break;
		case XK_Up:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] += 10.0;
			break;
		case XK_Down:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] -= 10.0;
			break;
		case XK_equal:
			if (++ndrops > 40)
				ndrops=40;
			break;
		case XK_minus:
			if (--ndrops < 0)
				ndrops = 0;
			break;
		case XK_n:
			break;
		case XK_i:
			if (shift) {
				//shrink the umbrella
				umbrella.width *= (1.0 / 1.05);
			} else {
				//enlarge the umbrella
				umbrella.width *= 1.05;
			}
			//half the width
			umbrella.width2 = umbrella.width * 0.5;
			umbrella.radius = (float)umbrella.width2;
			break;
		case XK_Escape:
			return 1;
			//break;
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void cleanupRaindrops()
{
	Raindrop *s;
	while (rainhead) {
		s = rainhead->next;
		free(rainhead);
		rainhead = s;
	}
	rainhead=NULL;
}

void deleteRain(Raindrop *node)
{
	//remove a node from linked list
	//Log("deleteRain()...\n");
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//Log("only 1 item in list.\n");
			rainhead = NULL;
		} else {
			//Log("at beginning of list.\n");
			node->next->prev = NULL;
			rainhead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//Log("at end of list.\n");
			node->prev->next = NULL;
		} else {
			//Log("in middle of list.\n");
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	free(node);
	node = NULL;
}

void moveBigfoot()
{
	//move bigfoot...
	int addgrav = 1;
	//Update position
	bigfoot.pos[0] += bigfoot.vel[0];
	bigfoot.pos[1] += bigfoot.vel[1];
	//Check for collision with window edges
	if ((bigfoot.pos[0] < -140.0 && bigfoot.vel[0] < 0.0) ||
		(bigfoot.pos[0] >= (float)g.xres+140.0 &&
		bigfoot.vel[0] > 0.0))
	{
		bigfoot.vel[0] = -bigfoot.vel[0];
		addgrav = 0;
	}
	if ((bigfoot.pos[1] < 150.0 && bigfoot.vel[1] < 0.0) ||
		(bigfoot.pos[1] >= (float)g.yres && bigfoot.vel[1] > 0.0)) {
		bigfoot.vel[1] = -bigfoot.vel[1];
		addgrav = 0;
	}
	//Gravity?
	if (addgrav)
		bigfoot.vel[1] -= 0.75;
}


void createRaindrop(const int n)
{
	//create new rain drops...
	int i;
	for (i=0; i<n; i++) {
		Raindrop *node = (Raindrop *)malloc(sizeof(Raindrop));
		if (node == NULL) {
			Log("error allocating node.\n");
			exit(EXIT_FAILURE);
		}
		node->prev = NULL;
		node->next = NULL;
		node->sound=0;
		node->pos[0] = rnd() * (float)g.xres;
		node->pos[1] = rnd() * 100.0f + (float)g.yres;
		VecCopy(node->pos, node->lastpos);
		node->vel[0] = 
		node->vel[1] = 0.0f;
		node->color[0] = rnd() * 0.2f + 0.8f;
		node->color[1] = rnd() * 0.2f + 0.8f;
		node->color[2] = rnd() * 0.2f + 0.8f;
		node->color[3] = rnd() * 0.5f + 0.3f; //alpha
		node->linewidth = random(8)+1;
		//larger linewidth = faster speed
		node->maxvel[1] = (float)(node->linewidth*16);
		node->length = node->maxvel[1] * 0.2f + rnd();
		//put raindrop into linked list
		node->next = rainhead;
		if (rainhead != NULL)
			rainhead->prev = node;
		rainhead = node;
		++totrain;
	}
}

void checkRaindrops()
{
	if (random(100) < 50) {
		createRaindrop(ndrops);
	}
	//
	//move rain droplets
	Raindrop *node = rainhead;
	while (node) {
		//force is toward the ground
		node->vel[1] += gravity;
		VecCopy(node->pos, node->lastpos);
		node->pos[0] += node->vel[0] * timeslice;
		node->pos[1] += node->vel[1] * timeslice;
		if (fabs(node->vel[1]) > node->maxvel[1])
			node->vel[1] *= 0.96;
		node->vel[0] *= 0.999;
		node = node->next;
	}
	//}
	//
	//check rain droplets
	int n=0;
	node = rainhead;
	while (node) {
		n++;
		#ifdef USE_SOUND
		if (node->pos[1] < 0.0f) {
			//raindrop hit ground
			if (!node->sound && play_sounds) {
				//small chance that a sound will play
				int r = random(50);
				if (r==1) {
					//play sound here...


				}
				//sound plays once per raindrop
				node->sound=1;
			}
		}
		#endif //USE_SOUND
		//collision detection for raindrop on umbrella
		if (g.showUmbrella) {
			if (umbrella.shape == UMBRELLA_FLAT) {
				if (node->pos[0] >= (umbrella.pos[0] - umbrella.width2) &&
					node->pos[0] <= (umbrella.pos[0] + umbrella.width2)) {
					if (node->lastpos[1] > umbrella.lastpos[1] ||
						node->lastpos[1] > umbrella.pos[1]) {
						if (node->pos[1] <= umbrella.pos[1] ||
							node->pos[1] <= umbrella.lastpos[1]) {
							if (node->linewidth > 1) {
								Raindrop *savenode = node->next;
								deleteRain(node);
								node = savenode;
								continue;
							}
						}
					}
				}
			}
			if (umbrella.shape == UMBRELLA_ROUND) {
				float d0 = node->pos[0] - umbrella.pos[0];
				float d1 = node->pos[1] - umbrella.pos[1];
				float distance = sqrt((d0*d0)+(d1*d1));
				//Log("distance: %f  umbrella.radius: %f\n",
				//							distance,umbrella.radius);
				if (distance <= umbrella.radius &&
										node->pos[1] > umbrella.pos[1]) {
					if (node->linewidth > 1) {
						if (g.deflection) {
							//deflect raindrop
							double dot;
							Vec v, up = {0,1,0};
							VecSub(node->pos, umbrella.pos, v);
							VecNormalize(v);
							node->pos[0] =
								umbrella.pos[0] + v[0] * umbrella.radius;
							node->pos[1] =
								umbrella.pos[1] + v[1] * umbrella.radius;
							dot = VecDot(v,up);
							dot += 1.0;
							node->vel[0] += v[0] * dot * 1.0;
							node->vel[1] += v[1] * dot * 1.0;
						} else {
							Raindrop *savenode = node->next;
							deleteRain(node);
							node = savenode;
							continue;
						}
					}
				}
			}
		}
		if (node->pos[1] < -20.0f) {
			//rain drop is below the visible area
			Raindrop *savenode = node->next;
			deleteRain(node);
			node = savenode;
			continue;
		}
		node = node->next;
	}
	if (maxrain < n)
		maxrain = n;
}

void physics()
{
    player.checkPlatfColl(ground);
    player.checkPlatfColl(ciel);
    player.checkPlatfColl(start);
    player.applyGravity(1.5); 
    player.pos[0] += player.vel[0];
    enemy.PlatformMovement(start);

    //player.isDamaged(enemy);

    //adrian: bullet updates
    //plBullet.pos[0] += plBullet.vel[0];
    /*
    for (int i = 0; i < player.mag; i++) {
        Bullet *b = &player.ammo[i];
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
    }
    */
    if (player.isShooting) {
        timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        double ts = timeDiff(&g.bulletTimer, &bt);
        if (ts > 0.1) {
            timeCopy(&g.bulletTimer, &bt);
            CheckShot(&player);
        }
    }
    UpdateBulletPhysics(&player);
    //adrian: timing on the reload
    static int counter = 2;
    if (!player.isReloading)
        counter = 2;
    if (player.isReloading) {
        timespec rt;
        clock_gettime(CLOCK_REALTIME, &rt);
        double ts = timeDiff(&g.reloadTimer, &rt);
        CheckReload(&player, g.reload);
        if (ts > 0.5) {
            timeCopy(&g.reloadTimer, &rt);
            counter--;
            if (counter == 0)
                player.mag = 0;
        }
    }

    //adrian: bullet collision
    for (int i = 0; i < player.magMax; i++) {
        Bullet *b = &player.ammo[i];
        b->checkBulletColl(&player.ammo[i], ground);
        b->checkBulletColl(&player.ammo[i], ciel); 
        b->checkBulletColl(&player.ammo[i], start); 
        b->checkBulletColl(&player.ammo[i], enemy);
        b->checkBulletColl(&player.ammo[i], enemy1);
    }

    plBullet.checkBulletColl(&plBullet, ciel);
    //plBullet.checkBulletColl(&plBullet, start);


    //Enemy: Ground and Bullet collison
    enemy.CollisonGround(ground);
    enemy1.CollisonGround(ciel);
    enemy1.CollisonGround(ground);
    enemy.movement(ground);
    enemy.meleeBehavior(player,ground);
    enemy1.meleeBehavior(player,ciel);
    enemy.Attack(player);
    enemy1.Attack(player);

    if (enemy.isAttacking == true || enemy1.isAttacking == true) {
	    if (player.hp > 0) {
		    player.hp -=1;
	    }
    }
    
    for (int i = 0; i < player.mag; i++) {
    	enemy.CheckBullet(&player.ammo[i]);
    	enemy1.CheckBullet(&player.ammo[i]);
    }

    UpdatePlayerFacing(&player, &plBullet); 

        

    if (g.showBigfoot)
	if (g.showRain)
		checkRaindrops();
}

void drawUmbrella()
{
	//Log("drawUmbrella()...\n");
	if (umbrella.shape == UMBRELLA_FLAT) {
		glColor4f(1.0f, 0.2f, 0.2f, 0.5f);
		glLineWidth(8);
		glBegin(GL_LINES);
			glVertex2f(umbrella.pos[0]-umbrella.width2, umbrella.pos[1]);
			glVertex2f(umbrella.pos[0]+umbrella.width2, umbrella.pos[1]);
		glEnd();
		glLineWidth(1);
	} else {
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glPushMatrix();
		glTranslatef(umbrella.pos[0],umbrella.pos[1],umbrella.pos[2]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBindTexture(GL_TEXTURE_2D, g.umbrellaTexture);
		glBegin(GL_QUADS);
			float w = umbrella.width2;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}
}

void drawRaindrops()
{
	Raindrop *node = rainhead;
	while (node) {
		glPushMatrix();
		glTranslated(node->pos[0],node->pos[1],node->pos[2]);
		glColor4fv(node->color);
		glLineWidth(node->linewidth);
		glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, node->length);
		glEnd();
		glPopMatrix();
		node = node->next;
	}
	glLineWidth(1);
}

void render()
{
	Rect r;

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	//draw a quad with texture
	float wid = 120.0f;
	glColor3f(1.0, 1.0, 1.0);
	if (g.forest) {
		glBindTexture(GL_TEXTURE_2D, g.forestTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd();
	}
	if (g.showBigfoot) {
		menu();
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
		if (!g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.bigfootTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}
		glBegin(GL_QUADS);
			if (bigfoot.vel[0] > 0.0) {
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
			} else {
				glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
			}
		glEnd();
		glPopMatrix();
		//
		if (g.trees && g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.forestTransTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
			glEnd();
		}
		glDisable(GL_ALPHA_TEST);
	}

	glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
	//	glVertex2i(10,10);
	//	glVertex2i(10,60);
	//	glVertex2i(60,60);
	//	glVertex2i(60,10);
	//glEnd();
	//return;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	if (g.showRain)
		drawRaindrops();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	//
	if (g.showUmbrella)
		drawUmbrella();
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//
	unsigned int c = 0x00ffff44;
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "B - Title");
	//ggprint8b(&r, 16, c, "F - Forest");
	//ggprint8b(&r, 16, c, "O - Silhouette");
	//ggprint8b(&r, 16, c, "T - Trees");
	//ggprint8b(&r, 16, c, "U - Umbrella");
	//ggprint8b(&r, 16, c, "R - Rain");
	//ggprint8b(&r, 16, c, "N - Sounds");


//-----------------------------------------------------------------------------
//Our space to test entity rendering
    ggprint8b(&r, 16, c, "Move - WASD");
    ggprint8b(&r, 16, c, "Space - Jump");
    ggprint8b(&r, 16, c, "Shoot - K");
    ggprint8b(&r, 16, c, "Reload - L (Hold)");
    ggprint8b(&r, 16, c, "C - Credits");
    ggprint8b(&r, 16, c, "Score: %i sec", Scores/100);

    enemy.drawEnemy();
    enemy1.drawEnemy();

    //Vec playerPos = {g.xres/2.0 , g.yres/2.0, 0.0};
    //Player player(100, playerPos);

    //Vec groundPos = {g.xres/10.0, g.yres/10.0, 0.0};
    //Vec groundVel = {0.0 ,0.0 ,0.0};
    //Platform ground(20, groundPos, groundVel);

    //Platform test(20, groundPos, groundVel); 

    if (g.credits) {
        DrawSquare(g.yres);
        CreditsTitle(g.yres);
        MC_Credits(g.yres);
        AT_Credits(g.yres);
        //PrintTasks(g.yres);
        //PrintControls(g.yres);
    	//serverHandling(argc, argvr);   

        //Rect board;
        //board.bot = g.yres - 240;
        //board.left = 10;
        //board.center = 0;
	    //ggprint8b(&board, 16, 0x00ff0000, ptrScore);
    }

    ground.pos[0] = groundPos[0];
    ground.pos[1] = groundPos[1];
    ground.size = groundSize;
    ground.drawPlatf(20);
    
    ciel.pos[0] = cielPos[0];
    ciel.pos[1] = cielPos[1];
    ciel.size = 20;
    ciel.drawPlatf(10);
    
    start.pos[0] = startPos[0];
    start.pos[1] = startPos[1];
    start.size = 20;
    start.drawPlatf(5);
    
    player.drawPlayer();

    if (player.isDead == false)
	    Scores++;
    //plBullet.pos[0] = playerPos[0];
    //plBullet.pos[1] = playerPos[1];
    plBullet.size = 5;
    plBullet.inContact = false;
    plBullet.drawBullet(player);
    //test.drawPlatf(10);

    //adrian: drawing bullets
    /*
    for (int i = 0; i < player.mag; i++) {
        Bullet *b = &player.ammo[i];
        b->drawBullet();
    }
    */ 
    UpdateBulletRendering(&player);
    player.drawHealthBar(healthPos);
    if (player.pos[1] < 0) {
        player.isDead = true;
        player.hp = 0;
    }
    if (player.hp == 0) {
        player.isDead = true;
    }
    PrintGameOverScreen(&player, gameOverScr); 
    player.drawAmmo(&player, ammoUIPos); 
    
}

