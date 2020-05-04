//Class:            Software Engineering 3350
//Author:           Adrian Jay Telan
//Last Modified:    24 Apr 2020
//File:             adrianT.cpp
//Project:          cute_as_hell
//Main location:    main.cpp
//
//Contains a function which outputs my credits.
//
//Will contain function definitions of player-related methods.
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 443
#define USERAGENT "CMPS-3350"


#include <GL/glx.h>

#include "fonts.h"
#include "adrianT.h"
#include "rainforest.h"

//Method Prototypes
extern BIO *ssl_setup_bio(void);
extern void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
extern void set_to_non_blocking(const int sock);

//===============Class and Method definitions from 'adrianT.h'=================
//-----------------------------------------------------------------------------
//Bullet Class method definitions

//Public Methods
void Bullet::drawBullet(Player player) 
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 1.0f, 0.0f);
    //if (vel[0] != 0) {
    if (!inContact && player.hp > 0 && !player.isDead) {
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] - size);
            glVertex2f(pos[0] - size, pos[1] - size);
        glEnd();
    }
    glEnable(GL_TEXTURE_2D);
}

void Bullet::moveBullet(int bulletSpeed, float pVel)
{
    vel[0] = (float)bulletSpeed + pVel/2.0;
    vel[1] = (((((float) rand())/(float) RAND_MAX) - 0.5) * 5);
}

void Bullet::drawBulletTracer()
{

}

void Bullet::checkBulletColl(Bullet *bullet, Platform platf)
{
    if (bullet->pos[1] < platf.top && bullet->pos[1] > platf.bottom &&
            bullet->pos[0] > platf.left && bullet->pos[0] < platf.right) {
        bullet->vel[0] = 0;
        bullet->vel[1] = 0;
        bullet->inContact = true;
    } //else {
        //bullet->inContact = false;
    //}
}

void Bullet::checkBulletColl(Bullet *bullet, Enemy enem)
{
    int enemTop = enem.pos[1] + enem.esize;
    int enemSole = enem.pos[1] - enem.esize;
    int enemLeft = enem.pos[0] - enem.esize;
    int enemRight = enem.pos[0] + enem.esize;

    if (bullet->pos[1] < enemTop && bullet->pos[1] > enemSole &&
            bullet->pos[0] > enemLeft && bullet->pos[0] < enemRight) {
        bullet->vel[0] = 0;
        bullet->vel[1] = 0;
        bullet->inContact = true;
    } //else {
        //bullet->inContact = false;
    //}
}


//Constructors
Bullet::Bullet()
{
}

Bullet::Bullet(int bulletSize, Vec initPos, Vec initVel)
{
    size = bulletSize;

    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    vel[0] = initVel[0];
    vel[1] = initVel[1];
    vel[2] = initVel[2];
}

//-----------------------------------------------------------------------------
//Platform Class method definitions

//Public Methods
void Platform::drawPlatf(int length)
{
    top = pos[1] + size;
    center = pos[0] + ((length - 1) * size);
    left = center - ((length) * size);
    right = center + ((length) * size);
    bottom = pos[1] - size;
    
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.2f);    

    glBegin(GL_QUADS);
        glVertex2f(left, top);
        glVertex2f(right, top);
        glVertex2f(right, bottom);
        glVertex2f(left, bottom);
    glEnd();

    /*
    for (int i = 0; i < length; i++) {
        pos[0] += 2 * size;
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] - size);
            glVertex2f(pos[0] - size, pos[1] - size);
        glEnd();
    }
    */
    
   glEnable(GL_TEXTURE_2D);
}

//Constructors
Platform::Platform(int platfSize, Vec initPos, Vec initVel)
{
    size = platfSize;
     
    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    vel[0] = initVel[0];
    vel[1] = initVel[1];
    vel[2] = initVel[2];
}

//Accessors

//Mutators

//-----------------------------------------------------------------------------
//Player Class method definitions

//Public Methods
void Player::run(int runVel)
{
    if (runVel !=0) {
        faceDir = (float)runVel;
    }
    vel[0] = (float)runVel;
    faceRight(faceDir);
    faceLeft(faceDir);
    isRunning = true;
    if (isRolling || isHit) {
        isRunning = false;
        return;
    }
}

void Player::jump(int jumpVel) 
{
    //vel[1] = (float)jumpVel;
    if (isGrounded) {
        vel[1] = (float)jumpVel;
        pos[1] += vel[1];
        isJumping = false;
        return;
    } else if (!isGrounded) { 
        isJumping = true;
        //vel[1] -= (float)jumpVel;
        //vel[1] *= 1.1;
        //pos[1] -= 5 * vel[1];
        return;
    }
}

void Player::shoot(Bullet *plBullet)
{
    if (isRolling || isHit)
        return;
    if (facingLeft) {
        plBullet->pos[0] = pos[0] - size;
        plBullet->pos[1] = pos[1];
        plBullet->pos[2] = 0;
        
        plBullet->vel[0] = 0;
        plBullet->vel[1] = 0;
        plBullet->vel[2] = 0;
        
        plBullet->size = 2; 
        plBullet->top = plBullet->pos[1] + plBullet->size;
        plBullet->left = plBullet->pos[0] - plBullet->size;
        plBullet->right = plBullet->pos[0] + plBullet->size;
        plBullet->bottom = plBullet->pos[1] - plBullet->size;
        
        plBullet->inContact = false;

        plBullet->moveBullet(-20, vel[0]);
        return;
    }
    if (facingRight) {
        plBullet->pos[0] = pos[0] + size;
        plBullet->pos[1] = pos[1];
        plBullet->pos[2] = 0;
        
        plBullet->vel[0] = 0;
        plBullet->vel[1] = 0;
        plBullet->vel[2] = 0;
        
        plBullet->size = 2; 
        plBullet->top = plBullet->pos[1] + plBullet->size;
        plBullet->left = plBullet->pos[0] - plBullet->size;
        plBullet->right = plBullet->pos[0] + plBullet->size;
        plBullet->bottom = plBullet->pos[1] - plBullet->size;
        
        plBullet->inContact = false;
        
        plBullet->moveBullet(20, vel[0]);
        return;
    }
}

void Player::roll()
{
    if (isRolling)
        return;
}

void Player::isDamaged(Enemy enem)
{
    float distToEnem = pos[0] - enem.pos[0];
    if (enem.isAttacking && (distToEnem <= 100 || distToEnem >= -100))
        hp -= enem.damage;
    if (hp <= 0) {
        hp = 0;
        isDead = true;
    }
}

void Player::drawPlayer()
{
    if (!isDead & (hp > 0)) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.4f, 0.1f, 0.1f);
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - size, pos[1] - size);
            glVertex2f(pos[0] - size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] + size);
            glVertex2f(pos[0] + size, pos[1] - size);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

void Player::checkPlatfColl(Platform ground)
{
    int plSoles = pos[1] - size;
    int plRight = pos[0] + size;
    int plLeft = pos[0] - size;
    int plTop = pos[1] + size;

    if (plTop < ground.bottom)
        return;

    if (plTop <= ground.bottom + ground.size) {
        if (plTop >= ground.bottom && plRight >= ground.left &&
                plLeft <= ground.right) {
            pos[1] = ground.bottom - size;
            vel[1] = vel[1] / 2.0;
            isGrounded = false;
        }
        return;
    }

    if (plTop > ground.bottom + ground.size && plTop <= ground.top) {
        if (plLeft <= ground.right && !(plRight <= ground.right))
            pos[0] = ground.right + size;
        if (plRight >= ground.left && !(plLeft >= ground.left)) 
            pos[0] = ground.left - size;
    }

    if (plSoles <= ground.top && plRight >= ground.left && 
            plLeft <= ground.right && plTop > ground.top && 
            plSoles >= ground.bottom) {
        isGrounded = true;

        if (!(plRight <= ground.left && plLeft >= ground.right) && 
                plSoles >= ground.bottom)
            pos[1] = ground.top + size;

    } else {
        isGrounded = false;
    }

/*
    if (plRight >= ground.left && plSoles <= ground.bottom &&
            plTop >= ground.bottom) {
        pos[0] = ground.left - size;
        isGrounded = false;
    }
*/
}

void Player::applyGravity(float gravVel)
{
    if (isGrounded)
        vel[1] = 0;
    else if (!isGrounded) {
        vel[1] -= gravVel * gravVel;
        vel[1] *= 1.1;
        pos[1] += vel[1];
    }
}

void Player::faceLeft(float faceDir)
{
    if (faceDir >= 0) 
        facingLeft = false;
    else
        facingLeft = true;
}

void Player::faceRight(float faceDir)
{
    if (faceDir < 0) 
        facingRight = false;
    else
        facingRight = true;
}

void Player::drawHealthBar(Vec pos)
{
    //pos is the top right vertex
    //health bar outline
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - 55, pos[1] - 15);
        glVertex2f(pos[0] - 5, pos[1] - 15);
        glVertex2f(pos[0] - 5, pos[1] - 5 - (100 * 2) - 20);
        glVertex2f(pos[0] - 55, pos[1] - 5- (100 * 2) - 20);
    glEnd();
    //health pool
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - 50, pos[1] - 20);
        glVertex2f(pos[0] - 10, pos[1] - 20);
        glVertex2f(pos[0] - 10, pos[1] - (hp * 2) - 20);
        glVertex2f(pos[0] - 50, pos[1] - (hp * 2) - 20);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Player::drawAmmo(Player *player, Vec pos)
{
    //pos indicates center
    //ammo ui background
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos[0] - 20, pos[1] + 85);
        glVertex2f(pos[0] + 20, pos[1] + 85);
        glVertex2f(pos[0] + 20, pos[1] - 60);
        glVertex2f(pos[0] - 20, pos[1] - 60);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    for (int i = player->mag; i < player->magMax; i++) {    
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - 15, pos[1] - 45 - (-i * 10));
            glVertex2f(pos[0] + 15, pos[1] - 45 - (-i * 10));
            glVertex2f(pos[0] + 15, pos[1] - 50 - (-i * 10));
            glVertex2f(pos[0] - 15, pos[1] - 50 - (-i * 10));
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

int Player::applyPoison(int poisDam)
{
    if (poisDam == 0)
        return 0;
    else 
        return applyPoison(poisDam - 1);
}

//Constructor
Player::Player(int initHp, int playerSize, Vec initPos)
{
    hp = initHp;
    pos[0] = initPos[0];
    pos[1] = initPos[1];
    pos[2] = initPos[2];

    mag = 0; 
    magMax = 13;

    ammo = new Bullet[magMax + 1];

    //int speed = 0;
    size = playerSize;

    vel[0] = 0;
    vel[1] = 0;
    vel[2] = 0;

    isRunning = false;
    isJumping = false;
    isGrounded = false;
    isRolling = false;
    isShooting = false;
    isHit = false;
    facingLeft = false;
    facingRight = true;
    isPoisoned = false;
    isReloading = true;
    isDead = false;
}

//Accessors
int Player::getHp()
{
    return hp;
}

//Mutators
void Player::setHp(int newHp)
{
    hp = newHp;
}
//=============================================================================

//Uncategorized Methods
//Prints the word 'Credits' to the screen
void CreditsTitle(int yres)
{
    Rect TCredit;

    TCredit.bot = yres - 150;
    TCredit.left = 10;
    TCredit.center = 0;

    ggprint12(&TCredit, 16, 0x00ff0000, "Credits");
}

//Prints my name in the credits section
void AT_Credits(int yres)
{
    
    Rect name;
    //Rect credit;

    name.bot = yres - 180;
    name.left = 10;
    name.center = 0;

    //ggprint8b(&credit, 16, 0x00ff0000, "Character Design");
    ggprint8b(&name, 16, 0x00ff0000, "Adrian Jay Telan");
}

//Prints the current tasks I'm working on onto the screen
void PrintTasks(int yres)
{
    Rect taskList;
    Rect task[3];

    taskList.bot = yres - 320;
    taskList.left = 10;
    taskList.center = 0;

    ggprint12(&taskList, 16, 0x00ff0000, "TASKS");

    for (int i = 0; i < 3; i++) {
	    task[i].bot = yres - 350 - 20*i;
	    task[i].left = 10;
	    task[i].center = 0;

	    if (i == 0) {
	        ggprint8b(&task[i], 16, 0x00ff0000, 
                    "Analyze rainforest framework");
        }

        if (i == 1) {
            ggprint8b(&task[i], 16, 0x00ff0000, 
                    "Analyze walk framework");
        }

        if (i == 2) {
            ggprint8b(&task[i], 16, 0x00ff0000, "Get good at programming");
        }
    }
}

//Draws a square
void DrawSquare(int yres)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(0, yres - 120);
        glVertex2f(0, yres - 410);
        glVertex2f(200, yres - 410);
        glVertex2f(200, yres - 120);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void UpdatePlayerFacing(Player *player, Bullet *bullet)
{
    if (player->facingLeft) {
        bullet->pos[0] = player->pos[0] - player->size;
        bullet->pos[1] = player->pos[1];
    }

    if (player->facingRight) {
        bullet->pos[0] = player->pos[0] + player->size;
        bullet->pos[1] = player->pos[1];
    }
}

void CheckShot(Player *player)
{
    if (player->mag < player->magMax) {
        player->shoot(&player->ammo[player->mag]);
        player->mag++;
    } else {
        player->isShooting = false;
    }
}

void CheckReload(Player *player, bool pressed)
{
    bool canReload = !player->isShooting && pressed;
    if (canReload) 
        player->isReloading = true;
    else
        player->isReloading = false;
}

void UpdateBulletPhysics(Player *player)
{
    for (int i = 0; i < player->magMax; i++) {
        //player->ammo[i].drawBullet();
        player->ammo[i].pos[0] += player->ammo[i].vel[0];
        player->ammo[i].pos[1] += player->ammo[i].vel[1];
    }
}

void UpdateBulletRendering(Player *player)
{
    for (int i = 0; i < player->magMax; i++) {
        player->ammo[i].drawBullet(*player);
    }
}

void PrintGameOverScreen(Player *player, Vec pos)
{
    //pos values give the center of the game over popup
    if (player->isDead || player->hp <= 0) {
        glDisable(GL_TEXTURE_2D);
        //outline
        glColor3f(0.0f, 0.0f, 0.0f);    
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - 100, pos[1] + 50);
            glVertex2f(pos[0] + 100, pos[1] + 50);
            glVertex2f(pos[0] + 100, pos[1] - 50);
            glVertex2f(pos[0] - 100, pos[1] - 50);
        glEnd();
        //fill
        glColor3f(1.0f, 0.0f, 0.0f);    
        glBegin(GL_QUADS);
            glVertex2f(pos[0] - 95, pos[1] + 45);
            glVertex2f(pos[0] + 95, pos[1] + 45);
            glVertex2f(pos[0] + 95, pos[1] - 45);
            glVertex2f(pos[0] - 95, pos[1] - 45);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        Rect gameOver;
        gameOver.bot = pos[1];
        gameOver.left = pos[0];
        gameOver.center = 1;
        ggprint12(&gameOver, 16, 0x00000000, "Game Over");
    }
}

void PrintControls(int yres)
{
    Rect controlTitle;
    Rect cList[3];
    
    controlTitle.bot = yres - 320;
    controlTitle.left = 10;
    controlTitle.center = 0;    
    
    for (int i = 0; i < 3; i++) {
        cList[i].bot = yres - 350 - 15 * i;
        cList[i].left = 10;
        cList[i].center = 0;
    }
    
    ggprint12(&controlTitle, 16, 0x00ff0000, "Controls");
    ggprint8b(&cList[0], 16, 0x00ff0000, "A - Move Left");
    ggprint8b(&cList[1], 16, 0x00ff0000, "D - Move Right");
    ggprint8b(&cList[2], 16, 0x00ff0000, "Space - Jump");
}

//===================================LAB 7=====================================

const int MAX_READ_ERRORS = 100;


char *serverHandling(int argc, char *argv[], char score[])
{
    BIO *ssl_setup_bio(void);
    void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_to_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;


    //-------------------------------------------------------------------------
    //Fixed error: invalid conversion from 'const SSL_METHOD* to 'SSL_METHOD'
    //Previous code was:
    //      SSL_METHOD *method;
    //-------------------------------------------------------------------------
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[1000];
    int req_len;
    char hostname[256] = "www.google.com";
    char pagename[256] = "";
    int port = PORT;
    int bytes, nreads, nerrs;
    char buf[256];
    int ret;
    //Get any command-line arguments.
    if (argc > 1)
        strcpy(hostname, argv[1]);
    if (argc > 2)
        strcpy(pagename, argv[2]);
    //
    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0) 
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
    //next 2 lines of code are not currently needed.
    //SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
    //SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    //
    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio, 
                "%s: Cannot connect to host %s [%s] on port %d.\n",
                argv[0], hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //
    //Show the certificate data 
    show_cert_data(ssl, outbio, hostname);
    //
    //A non-blocking socket will make the ssl_read() not block.
    set_to_non_blocking(sd);
    //
    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);

    //-------------------------------------------------------------------------
    //Fixed warning: this 'if' clase does not guard this statement.
    //[-Wmisleading-indentation]
    //Previous code was:
    //      if (ret <= 0)
    //          fprintf(stderr, "ERROR: SSL_write\n"); fflush(stderr);
    //-------------------------------------------------------------------------
    if (ret <= 0)
        fprintf(stderr, "ERROR: SSL_write\n");
    fflush(stderr);
    //
    //Get data returned from the server.
    //First, do priming read.
    //We can take this approach because our socket is non-blocking.
    //Start with an error condition.
    bytes = -1; 
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        //A slight pause can cause fewer reads to be needed.
        usleep(10000);
    }
    //A successful priming read was accomplished.
    //Now read all the data.
    nreads = 1;
    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;


    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        write(STDOUT_FILENO, buf, bytes);
        //memset(buf, '\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if (bytes == 0) ++nerrs; else nerrs = 0;
        //A slight pause can cause fewer reads to be needed.
        usleep(20000); 
    }

    //char arr[] = "Test";
    string s(buf);
    cout << s;

    
    for (unsigned int i = 0; i < sizeof(buf); i++) 
        score[i] = buf[i];
    for (unsigned int i = 0; i < sizeof(buf) && buf[i] != 'G'; i++)
        score[i] = ' ';

    /*
    for (unsigned int i = 0; i < sizeof(buf); i++) 
        cout << buf[i] << endl;
    */

    printf("\nn calls to ssl_read(): %i\n", nreads); fflush(stdout);
    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
    return score;
}

