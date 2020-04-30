//Class: Software Engineering 3350
//Game_name: Cute as Hell
//
//Program: melanieC.cpp
//Author: Melanie Corral
//Date: 2020
//Last modified 29 April 2020
//
//Completed:
//Credit implementation
//Added Lab3 functions
//To do list:
//Enemy AI attack/health
//Different Enemy types

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

void MC_Credits(int yres)
{
    Rect r;
    unsigned int c = 0x00ff0000;
    r.bot = yres - 165;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "Melanie Corral");
}
void Main_Menu(int yres)
{
    Rect r;
    unsigned int c = 0x00ff0000;
   /* GLuint cuteashell;

    Image image[1] = {
	    "./images/cuteashell.png"};

    glGenTextures(1, &r.cuteashell);
    glBindTexture(GL_TEXTURE_2D, r.cuteashell);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    w = image[0].width;
    h = image[0].height;
    unsigned char *ftData = buildAlphaData(&img[0]);
    glTestImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ftData);
    free(ftData);

*/

    glDisable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, r.cuteashell);
    glColor3f(0.7f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
       glVertex2f(250, yres - 200);//upper right
       glVertex2f(250, yres - 100);//upper left
       glVertex2f(380, yres - 100);//
       glVertex2f(380, yres - 200);//
    glEnd();
    glEnable(GL_TEXTURE_2D);
    r.bot = yres - 165;
    r.left = 275;
    r.center = 0;
    ggprint16(&r, 16, c, "test");
}
void Enemy::drawEnemy()
{
    glDisable(GL_TEXTURE_2D);
    glColor3ub(242, 83, 0);
    glBegin(GL_TRIANGLES);
    //glVertex2f(-10.0f, -10.0f);
    //glVertex2f(  0.0f, 20.0f);
    //glVertex2f( 10.0f, -10.0f);
    glVertex2f(pos[0], pos[1] + esize); //top vertex
    glVertex2f(pos[0] - esize, pos[1] - esize); //left vertex
    glVertex2f(pos[0] + esize, pos[1] - esize); //right vertex

    glEnd();
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

    /*isWalking = true;	
      if (isWalking == true) {
      enemyPos[0] += -.5; //left
      enemyPos[0] += .5; //right

      }*/
}

void Enemy::PlayerHit(Player play)
{
	int HP = ehp;
	if (play.isHit == true) {
	    HP -=5;
			
	}
}

void Enemy::CollisonGround(Platform ground)
{

    int EnemyBottom = pos[1] - esize;
    int EnemyRight = pos[0] + esize;
    int EnemyLeft = pos[0] - esize;

    if (EnemyBottom <= ground.top && EnemyRight >= ground.left &&
	    EnemyLeft <= ground.right) {
	isGrounded = true;
	vel[1] = 0;
    } else { 
	isGrounded = false;
	vel[1] = -6.80;
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
	plBullet->inContact = true;
    } else { 
	isHit = true;
	plBullet->inContact = false;
	plBullet->vel[0] = 0;
	plBullet->vel[1] = 0;
    }

    if(isHit == true) {
	    //NOT A DELETE
	    //This moves enemy off screen
	    pos[0] = vel[0]*0;
	    pos[1] = vel[1]*0;
	    //add score eventually
    }
}

void Enemy::movement(Platform ground)
{	
    pos[0] += vel[0]*2;
    pos[1] += vel[1];

    if(((pos[0] - esize) < ground.left + 200 && vel[0] < 0.0) ||
	    (pos[0] + esize > ground.right && vel[0] > 0.0))
	vel[0] = -vel[0];
    /*	if(((pos[0] - esize) < ground.right+200 && vel[1] < 0.0) || 
	(pos[1]+esize >= ground.right && vel[1] > 0.0))
	vel[0] = -vel[0] *2; 
     *	if ((pos[0] < -140.0 && vel[0] < 0.0) ||
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
