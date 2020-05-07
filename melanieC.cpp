//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.cpp
//Author: Melanie Corral
//Date: 2020
//Last modified 5 May 2020
//
//This file contains the code for most of the
//Enemy functionality and rendering
//Including Enemy movement, Enemy rendering
//Enemy Player Detection, and EnemyAttack
//
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include "fonts.h"
#include "melanieC.h"
#include "rainforest.h"
#define MAXBUTTONS 1

void MC_Credits(int yres)
{
    Rect r;
    unsigned int c = 0x00ff0000;
    r.bot = yres - 165;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "Melanie Corral");
}
typedef struct t_button {
	Rect r;
	char text[32];
	unsigned int text_color;
} Button;

void menu()
{
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Rect r;
        

        //size, position, & color
        r.width = 160;
        r.height = 60;
        r.left = 220;
        r.bot = 70;
        r.right =
         r.left + r.width;
        r.top = r.bot +
         r.height;
        r.centerx = (r.left +
         r.right) / 2;
        r.centery = (r.bot +
         r.top) / 2;
        int text_color = 0x36C6FF;

                glColor3f(1.0f, 1.0f, 0.0f);

                glBegin(GL_QUADS);
                        glVertex2i(r.left,  r.bot);
                        glVertex2i(r.left,  r.top);
                        glVertex2i(r.right, r.top);
                        glVertex2i(r.right, r.bot);
                glEnd();

                r.left = r.centerx;
                r.bot  = r.centery-8;
                r.center = 1;

                ggprint16(&r, 16, text_color, "Press B");

}


void Enemy::drawEnemy()
{
    glDisable(GL_TEXTURE_2D);
    glColor3ub(163, 31, 106);
    if (isHit == false) {
	    if (isDead) {
    		glEnable(GL_TEXTURE_2D);
		return;
	    }
    	glBegin(GL_TRIANGLES);
    	glVertex2f(pos[0], pos[1] + esize); //top vertex
    	glVertex2f(pos[0] - esize, pos[1] - esize); //left vertex
    	glVertex2f(pos[0] + esize, pos[1] - esize); //right vertex

    	glEnd();
    }
    glEnable(GL_TEXTURE_2D);

}
Enemy::Enemy(int enemyHP, int enemySize, Vec enemyPos)
{
    ehp = enemyHP;
    pos[0] = enemyPos[0];
    pos[1] = enemyPos[1];
    pos[2] = enemyPos[2];

    esize = enemySize;

    vel[0] = 0.5;
    vel[1] = 0;
    vel[2] = 0;

    isGrounded = false;
    isHit = false;
    isDead = false;

    /*isWalking = true;	
      if (isWalking == true) {
      enemyPos[0] += -.5; //left
      enemyPos[0] += .5; //right

      }*/
}
void Enemy::meleeBehavior(Player player, Platform ground)
{
    int distToPlayer = player.pos[0] - pos[0];
    int dirToPlayer = 0;
    float runningVel = 0;
    if (distToPlayer <= 0)
        dirToPlayer = -1;
    if (distToPlayer > 0)
        dirToPlayer = 1;
    if ((distToPlayer >= -150 && distToPlayer <= 150) || seesPlayer) {
        if (distToPlayer >= -25 && distToPlayer <= 25)
            runningVel = 0;
        else
            runningVel = dirToPlayer * 5;
        pos[0] += runningVel;
        //seesPlayer = true;
    } else if (!seesPlayer)
        movement(ground);
}

void Enemy::Attack(Player player)
{
        int Pright = player.pos[0] + player.size;
        int Pleft = player.pos[0] - player.size;
        int Pbottom = player.pos[1] - player.size;
        int Ptop = player.pos[1] + player.size;
        int Eleft = pos[0] - esize;
        int Eright = pos[0] + esize;
        int Etop = pos[1] + esize;
        int Ebottom = pos[1] - esize;
        //int HP = ehp;
        if ( Pright >= Eleft && Pleft <= Eright && Pbottom <= Etop && Ptop >= Ebottom ) {
            isAttacking = true;
        } else
            isAttacking = false;
}

void Enemy::CollisonGround(Platform ground)
{

    int EnemyBottom = pos[1] - esize;
    int EnemyTop = pos[1] + esize;
    int EnemyRight = pos[0] + esize;
    int EnemyLeft = pos[0] - esize;
    
    if ( EnemyTop < ground.bottom)
	    return;

    if (EnemyBottom <= ground.top && EnemyRight >= ground.left &&
	    EnemyLeft <= ground.right) {
	isGrounded = true;
	vel[1] = 0;
	if ((EnemyRight > ground.left || EnemyLeft < ground.right) 
             && EnemyBottom >= ground.bottom)
            pos[1] = ground.top + esize;
    } else { 
	isGrounded = false;
	vel[1] -= 1;
	pos[1] += vel[1];
    }

}
void Enemy::CheckBullet(Bullet *plBullet)
{

    int EnemyBottom = pos[1] - esize;
    int EnemyRight = pos[0] + esize;
    int EnemyLeft = pos[0] - esize;
    int EnemyTop = pos[1] + esize;

    if (plBullet->pos[0] <= EnemyLeft || plBullet->pos[0] >= EnemyRight || 
		    plBullet->pos[1] >= EnemyTop || plBullet->pos[1] <= EnemyBottom) {
	isHit = false;
    }
     else { 
	isHit = true;
	isDead = true;
	
    }
    if (isHit == true) {
      	pos[0] = vel[0]*0;
      	pos[1] = vel[1]*0;
	plBullet->vel[0] = 0;
	plBullet->vel[1] = 0;
    }
}

void Enemy::movement(Platform ground)
{	
    pos[0] += vel[0]*2;
    pos[1] += vel[1];

    int EnemyRight = pos[0] + esize;
    int EnemyLeft = pos[0] - esize;
    
    if ((EnemyLeft < ground.left + 200 && vel[0] < 0.0) ||
	    (EnemyRight > ground.right && vel[0] > 0.0))
	vel[0] = -vel[0];

    /*	if(((pos[0] - esize) < ground.right+200 && vel[1] < 0.0) || 
	(pos[1]+esize >= ground.right && vel[1] > 0.0))
	vel[0] = -vel[0] *2; 
     if ((pos[0] < -140.0 && vel[0] < 0.0) ||
     (pos[0] >= esize + 140.0 &&
     vel[0] > 0.0)) {

     vel[0] = -vel[0] * 5;

     }
     if ((pos[1] < 150.0 && vel[1] < 0.0) ||
     (pos[1] >= esize && vel[1] > 0.0)) {

     vel[1] = -vel[1] * 5;

     }
     */
}
void Enemy::PlatformMovement(Platform ground)
{	
    ground.pos[0] += ground.vel[0]*2;
    ground.pos[1] += ground.vel[1]*2;
    int PlatBottom = ground.pos[1] - ground.size;
    int PlatTop = ground.pos[1] + ground.size;

    if ((PlatBottom < 10 && ground.vel[1] < 0.0) ||
	    (PlatTop > 400 && ground.vel[1] > 0.0))
	ground.vel[1] -= ground.vel[1];
}
/////////////////////////lab3.cpp functions///////////////////////////

BIO *ssl_setup_bio(void)
{
    //Setup the ssl BIO, basic I/O abstraction.
    //https://www.openssl.org/docs/man1.1.0/man3/bio.html
    BIO *bio = NULL;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    bio = BIO_new(BIO_s_file());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    return bio;
}

void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname)
{
    //Display ssl certificate data here.
    //Get the remote certificate into the X509 structure
    printf("--------------------------------------------------------------\n");
    printf("Certificate data...\n");
    X509 *cert;
    X509_NAME *certname;
    printf("calling SSL_get_peer_certificate(ssl)\n");
    cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL)
	printf("Error: Could not get a certificate from: %s.\n", hostname);
    else
	printf("Retrieved the server's certificate from: %s.\n", hostname);
    //extract various certificate information
    certname = X509_NAME_new();
    certname = X509_get_subject_name(cert);
    //display the cert subject here
    if (BIO_printf(outbio, "Displaying the certificate subject data:\n") < 0)
	fprintf(stderr, "ERROR: BIO_printf\n");
    X509_NAME_print_ex(outbio, certname, 0, 0);
    if (BIO_printf(outbio, "\n\n") < 0)
	fprintf(stderr, "ERROR: BIO_printf\n");
    printf("--------------------------------------------------------------\n");
}

void set_to_non_blocking(const int sock)
{
    //Set a socket to be non-blocking.
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
	perror("ERROR: fcntl(F_GETFL)");
	exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
	perror("ERROR: fcntl(O_NONBLOCK)");
	exit(EXIT_FAILURE);
    }
}

/////////////////////////////////////////////////////////////////////
