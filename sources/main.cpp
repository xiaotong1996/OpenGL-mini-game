                                                                                                     /*
/##################################################################################################\
# Description : #                                                                                  #
#################                                                                                  #
#                                                                                                  #
#  Ce fichier est le point d'entr�e du moteur ( le "main()" ).                                     #
#                                                                                                  #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "camera.h"
#include "maths_3d.h"
#include <math.h>



/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/
WINDOW    *win = NULL;
MY_INPUT     *inp = NULL;
TIMER     *tim = NULL;

float incrementAngleY = 0;

Texture  *chess_piece_texture = NULL;// chess piece material;
Model3D  *chess_piece_model = NULL; // main character model: chess piece

Texture  *texture_arbre = NULL;	
Texture  *texture_arrow = NULL;
Texture  *texture_blue = NULL;	

float chess_posX = 0;
float chess_posY = 5;
float chess_posZ = 0;

float chees_move_speed = 0.02f;
//store cube's vertexs' positions
static const GLfloat vertex_list[][3] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
};

//store vertexs' rendering orders for each face
static const GLint index_list[][4] = {
	0, 2, 3, 1,
	4, 6, 2, 0,
	4, 0, 1, 5,
	4, 6, 7, 5,
	5, 7, 3, 1,
	6, 2, 3, 7,
};

CAMERA *cam = NULL;
float posX = 0;
float posY = 30;
float posZ = 0;
float angleX = 0;
float angleY = 0;
float angleZ = 0;

GLuint DLPlatform;
GLuint DLFin;


static const int required_inputs[] = {
	KEY_CODE_2,
	KEY_CODE_8,
	KEY_CODE_6,
	KEY_CODE_4,
};

static int chess_current_index = 0;

bool jump = false;
bool falldown = false;

static const int chess_routes[][2] = {
	0,20,
	25,20,
	50,20,
	50,35,
};

/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/







/********************************************************************\
*                                                                    *
*  les formes                                               *
*                                                                    *
\********************************************************************/
void display_chess_piece_model()
{
	
	//glBindTexture(GL_TEXTURE_2D, chess_piece_texture->OpenGL_ID[0]);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on r�p�te la texture en cas de U,V > 1.0
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut m�langer la texture avec la couleur courante

	//// charge le tableau de la texture en m�moire vid�o et cr�e une texture mipmap

	//if (chess_piece_texture->isRGBA)
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, chess_piece_texture->img_color->lenx, chess_piece_texture->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, chess_piece_texture->img_all);
	//else
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, chess_piece_texture->img_color->lenx, chess_piece_texture->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, chess_piece_texture->img_color->data);

	//glEnable(GL_TEXTURE_2D);



	glScalef(10, 10, 10);


	glBegin(GL_TRIANGLES);
	for (int i = 0; i < chess_piece_model->nb_triangles; i++) {
		glColor3f(0.0f, 0.0f, 0.0f);

		//glTexCoord2f(chess_piece_model->points[chess_piece_model->faces[i].a].ucol, chess_piece_model->points[chess_piece_model->faces[i].a].vcol);
		glVertex3f(chess_piece_model->points[chess_piece_model->faces[i].a].x, chess_piece_model->points[chess_piece_model->faces[i].a].y, chess_piece_model->points[chess_piece_model->faces[i].a].z);

		//glTexCoord2f(chess_piece_model->points[chess_piece_model->faces[i].b].ucol, chess_piece_model->points[chess_piece_model->faces[i].b].vcol);
		glVertex3f(chess_piece_model->points[chess_piece_model->faces[i].b].x, chess_piece_model->points[chess_piece_model->faces[i].b].y, chess_piece_model->points[chess_piece_model->faces[i].b].z);

		//glTexCoord2f(chess_piece_model->points[chess_piece_model->faces[i].c].ucol, chess_piece_model->points[chess_piece_model->faces[i].c].vcol);
		glVertex3f(chess_piece_model->points[chess_piece_model->faces[i].c].x, chess_piece_model->points[chess_piece_model->faces[i].c].y, chess_piece_model->points[chess_piece_model->faces[i].c].z);
	}
	glEnd();

	/*glDisable(GL_TEXTURE_2D);*/
}

void display_cube_body() {
	glBindTexture(GL_TEXTURE_2D, texture_blue ->OpenGL_ID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (texture_blue->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_blue->img_color->lenx, texture_blue->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_blue->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_blue->img_color->lenx, texture_blue->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_blue->img_color->data);

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glBegin(GL_QUADS);
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 4; ++j) {
			switch (j)
			{
			case 0:
				glTexCoord2f(0, 0);
				break;
			case 1:
				glTexCoord2f(0, 1);
				break;
			case 2:
				glTexCoord2f(1, 1);
			case 3:
				glTexCoord2f(1, 0);
				break;
			}
			glVertex3fv(vertex_list[index_list[i][j]]);
		}
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

void display_cube_face() {
	glBindTexture(GL_TEXTURE_2D, texture_arrow->OpenGL_ID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (texture_arrow->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_arrow->img_color->lenx, texture_arrow->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_arrow->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_arrow->img_color->lenx, texture_arrow->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_arrow->img_color->data);

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glBegin(GL_QUADS);
	for (int j = 0; j < 4; ++j) {
		switch (j)
		{
		case 0:
			glTexCoord2f(0, 0);
			break;
		case 1:
			glTexCoord2f(0, 1);
			break;
		case 2:
			glTexCoord2f(1, 1);
		case 3:
			glTexCoord2f(1, 0);
			break;
		}
		glVertex3fv(vertex_list[index_list[5][j]]);
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}

void display_platform() {
	display_cube_body();
	display_cube_face();
}

void display_pyramide(float x, float y, float z, float h, float c) {

	glBegin(GL_TRIANGLES);
	glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z + c / 2);
	glColor3f(0, 1, 0); glVertex3f(x, y + h, z);
	glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z + c / 2);

	glColor3f(0, 1, 0); glVertex3f(x - c / 2, y, z + c / 2);
	glColor3f(0, 1, 0); glVertex3f(x, y + h, z);
	glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z - c / 2);

	glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z - c / 2);
	glColor3f(0, 1, 0); glVertex3f(x, y + h, z);
	glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z - c / 2);

	glColor3f(1, 0, 0); glVertex3f(x + c / 2, y, z - c / 2);
	glColor3f(0, 1, 0); glVertex3f(x, y + h, z);
	glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z + c / 2);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z + c / 2);
	glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z + c / 2);
	glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z - c / 2);
	glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z - c / 2);
	glEnd();
}

void display_pyramide(float h, float c) {

	glBegin(GL_TRIANGLES);
	glColor3f(0, 1, 0); glVertex3f(c / 2, 0, c / 2);
	glColor3f(0, 1, 0); glVertex3f(0, h, 0);
	glColor3f(1, 0, 0); glVertex3f(-c / 2, 0, c / 2);

	glColor3f(0, 1, 0); glVertex3f(-c / 2, 0, c / 2);
	glColor3f(0, 1, 0); glVertex3f(0, h, 0);
	glColor3f(1, 0, 0); glVertex3f(-c / 2, 0, -c / 2);

	glColor3f(1, 0, 0); glVertex3f(-c / 2, 0, -c / 2);
	glColor3f(0, 1, 0); glVertex3f(0, h, 0);
	glColor3f(0, 1, 0); glVertex3f(c / 2, 0, -c / 2);

	glColor3f(1, 0, 0); glVertex3f(c / 2, 0, -c / 2);
	glColor3f(0, 1, 0); glVertex3f(0, h, 0);
	glColor3f(0, 1, 0); glVertex3f(c / 2, 0, c / 2);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 1, 0); glVertex3f(c / 2, 0, c / 2);
	glColor3f(1, 0, 0); glVertex3f(-c / 2, 0, c / 2);
	glColor3f(1, 0, 0); glVertex3f(-c / 2, 0, -c / 2);
	glColor3f(0, 1, 0); glVertex3f(+c / 2, 0, -c / 2);
	glEnd();
}

GLuint generatePlatformDisplayList()
{
	GLuint id = 0;
	id = glGenLists(1);
	if (!id) return id;  // failed to create a list, return 0
	glNewList(id, GL_COMPILE);
	display_platform();
	glEndList();
	return id;
}

GLuint generateFinDisplayList()
{
	GLuint id = 0;
	id = glGenLists(1);
	if (!id) return id;  // failed to create a list, return 0
	glNewList(id, GL_COMPILE);
	display_pyramide(20, 10);
	glEndList();
	return id;
}


/********************************************************************\
*                                                                    *
*  D�marre l'application (avant la main_loop). Renvoie false si      *
*  il y a une erreur.                                                *
*                                                                    *
\********************************************************************/
bool start()
{
	// initialisations de base
	// -----------------------
	win = new WINDOW();									// pr�pare la fen�tre
	win->create(600, 800, 16, 60, false);			// cr�e la fen�tre

	tim = new TIMER();									// cr�e un timer
	cam = new CAMERA();									// create a camera


	inp = new MY_INPUT(win);								// initialise la gestion clavier souris
	create_context(*win);								// cr�e le contexte OpenGL sur la fen�tre
	init_font(*win, "Courier");							// initialise la gestion de texte avec OpenGL


	// initialisations d'OpenGL
	// ------------------------
	//glClearColor(0.0f,0.0f,0.0f,1.0f);					// set clear color for color buffer (RGBA, black)
	glClearColor(230.0f, 230.0f, 250.0f, 1.0f);					// set clear color for color buffer (RGBA, Lavender)
	glViewport(0, 0, win->Xres, win->Yres);				// zone de rendu (tout l'�cran)
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// mapping quality = best
	glFrontFace(GL_CCW);								// front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_LINE);					// back of a face is made of lines
	glCullFace(GL_BACK);								// cull back face only
	glDisable(GL_CULL_FACE);						    // disable back face culling


	win->set_title("Mon jeu");

	chess_piece_model = new Model3D();
	chess_piece_model->load_3d_model("resources/chess_piece.off");
	/*chess_piece_texture = new Texture();
	chess_piece_texture->load_texture("resources/texture_chess_piece.tga", NULL);
	glGenTextures(1, chess_piece_texture->OpenGL_ID);*/



	texture_blue = new Texture();
	//texture_blue->load_texture("resources/texture_blue.tga", NULL);
	texture_blue->load_texture("resources/texture_blue.tga", NULL);
	glGenTextures(1, texture_blue->OpenGL_ID);

	texture_arrow = new Texture();
	texture_arrow->load_texture("resources/texture_arrow.tga", NULL);
	glGenTextures(1, texture_arrow->OpenGL_ID);

	DLPlatform = generatePlatformDisplayList();
	DLFin = generateFinDisplayList();

	return true;
}

/********************************************************************\
*                                                                    *
*  Boucle principale, appel�e pour construire une image, g�rer les   *
*  entr�es.                                                          *
*                                                                    *
\********************************************************************/
void main_loop() 
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//				gestion des touches	et du temps													//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	inp->refresh();
	tim->update_horloge();


	if (inp->keys[KEY_CODE_ESCAPE]) 
	{	  
		PostMessage(win->handle,WM_CLOSE,0,0);	// Stoppe la "pompe ?message" en y envoyant le message "QUIT"
	}
	if (chess_current_index >= 4) {
		chess_current_index = 0;
		//tim->pause_ms(10000);
		debug("YOU WIN");
		//PostMessage(win->handle, WM_CLOSE, 0, 0);
	}


	// TODO
	if (fabs(chess_posX - chess_routes[3][0])<5 && fabs(chess_posY - chess_routes[3][1])<5)
	{
		debug("YOU WIN");
	}




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						�a commence ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// votre code OpenGL ici
	glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
	//composition d'une matrice de projection
	glLoadIdentity(); //on choisit la matrice identit?
	gluPerspective(60,(double)win->Xres/(double)win->Yres,10,30000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000


	//fixd view
	gluLookAt(25, 60, -40,		// position
		10,0,20,		// point cible
		0, 1, 1);		// vecteur up

	//follow view
	//posX = chess_posX + cos(45) * 0;
	//posY = chess_posY + cos(45) * 40;
	//posZ = chess_posZ - cos(45) * 40;
	//gluLookAt(posX, posY, posZ,		// position
	//	chess_posX, chess_posY, chess_posZ,		// point cible
	//	0, 1, 0);		// vecteur up


	gotoxy(33, -23);
	write_2_screen("Use w a s d to move on the platform.\n");
	gotoxy(33, -28);
	write_2_screen("Use arrow button to move to next platform.\n");
	gotoxy(33, -33);
	write_2_screen("Use space to jump.\n");
	

	

	if (tim->global_timer_25_Hz)				// augmente incrementAngleY tous les 20�me de seconde
		incrementAngleY += 1.0f;
	if (incrementAngleY >= 360) incrementAngleY -= 360;

	//down
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glScalef(10, 5, 10);
	//display_platform();
	glCallList(DLPlatform);
	glPopMatrix();

	//up
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0, 0, 20);
	glRotatef(180, 0, 1, 0);
	glScalef(10, 5, 10);
	//display_platform();
	glCallList(DLPlatform);
	glPopMatrix();

	//right
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(25, 0, 20);
	glRotatef(90, 0, 1, 0);
	glScalef(10, 5, 10);
	//display_platform();
	glCallList(DLPlatform);
	glPopMatrix();

	//left
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(50, 0, 20);
	glRotatef(-90, 0, 1, 0);
	glScalef(10, 5, 10);
	//display_platform();
	glCallList(DLPlatform);
	glPopMatrix();

	//end
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(50, 0, 35);
	glRotatef(incrementAngleY, 0, 1, 0);
	glCallList(DLFin);
	glPopMatrix();




	if (inp->keys[KEY_CODE_W]) {
		chess_posZ += chees_move_speed;
	}
	if (inp->keys[KEY_CODE_S]) {
		chess_posZ -= chees_move_speed;
	}
	if (inp->keys[KEY_CODE_A]) {
		chess_posX += chees_move_speed;
	}
	if (inp->keys[KEY_CODE_D]) {
		chess_posX -= chees_move_speed;
	}


	if (inp->keys[KEY_CODE_UP]) {
		if (required_inputs[chess_current_index] == KEY_CODE_8) {
			chess_posX = chess_routes[chess_current_index][0];
			chess_posZ = chess_routes[chess_current_index][1];
			chess_current_index++;
			//debug("%d\n", chess_current_index);
		}

	}
	if (inp->keys[KEY_CODE_DOWN]) {
		//debug("%d\n", chess_current_index);
		if (required_inputs[chess_current_index] == KEY_CODE_2) {
			chess_posX = chess_routes[chess_current_index][0];
			chess_posZ = chess_routes[chess_current_index][1];
			chess_current_index++;
			//debug("%d\n", chess_current_index);
		}
	}
	


	if (inp->keys[KEY_CODE_RIGHT]) {
		if (required_inputs[chess_current_index] == KEY_CODE_6) {
			chess_posX = chess_routes[chess_current_index][0];
			chess_posZ = chess_routes[chess_current_index][1];
			chess_current_index++;
			//debug("%d\n", chess_current_index);
		}
	}
	
	if (inp->keys[KEY_CODE_LEFT]) {
		if (required_inputs[chess_current_index] == KEY_CODE_4) {
			chess_posX = chess_routes[chess_current_index][0];
			chess_posZ = chess_routes[chess_current_index][1];
			chess_current_index++;
			//debug("%d\n", chess_current_index);
		}
	}


	if (inp->keys[KEY_CODE_SPACE]) {
		jump = true;
	}

	// can not respond to number button
	/*if (inp->keys[KEY_CODE_NUMPAD2]) {
		debug("%d\n", chess_current_index);
		if (required_inputs[chess_current_index] == KEY_CODE_2) {
			chess_posX = chess_routes[chess_current_index][0];
			chess_posZ = chess_routes[chess_current_index][1];
			chess_current_index++;
			debug("%d\n", chess_current_index);
		}
	}

	if (inp->keys[required_inputs[chess_current_index]]) {
		chess_posX = chess_routes[chess_current_index][0];
		chess_posZ = chess_routes[chess_current_index][1];
		chess_current_index++;
		debug("%d\n", chess_current_index);
	}*/
	
	
	if (jump) {
		if (tim->global_timer_25_Hz)		// augmente incrementAngleY tous les 20�me de seconde
		{
			if (!falldown) {
				chess_posY += 0.6f;
			}
			else {
				chess_posY -= 0.7f;
			}

			if (chess_posY > 20) falldown = true;
			if (chess_posY < 5) {
				chess_posY = 5;
				jump = false;
				falldown = false;
				//debug("end jump");
			}
		}
		
	}
	

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	//glTranslatef(-5, 5, -5);
	glTranslatef(chess_posX-5, chess_posY, chess_posZ-5);
	display_chess_piece_model();
	glPopMatrix();

	
	

	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						�a finit ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	swap_buffer(win);	// affiche l'image compos�e ?l'�cran
}


/********************************************************************\
*                                                                    *
* Arr�te l'application                                               *
*                                                                    *
\********************************************************************/
void stop()
{
	delete texture_arrow;
	delete texture_blue;
	delete chess_piece_model;
	delete chess_piece_texture;

	delete inp;
	delete tim;

	if (win)
	{
		kill_font();
		kill_context(*win);
		delete win;
	}
}


/********************************************************************\
*                                                                    *
* Point d'entr�e de notre programme pour Windows ("WIN32")           *
*                                                                    *
\********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	MSG  msg;

	win	= NULL;
	inp	= NULL;
	tim = NULL;

	if (start() == false)								// initialise la fen�tre et OpenGL
	{
		debug("start() : �chec !");
		stop();
		return 1;
	}

	// main loop //
	// --------- //
	//   __
	//  /  \_
	//  |  \/ 
	//  \__/

	while (true)
	{
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))	// s'il y a un message, appelle WndProc() pour le traiter
		{
			if (!GetMessage(&msg,NULL,0,0))				// "pompe ?message"
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else 
		{
			main_loop();								// sinon, appelle main_loop()
		}
	}


	stop();												// arr�te OpenGL et ferme la fen�tre

	return 0;
}



/********************************************************************\
*                                                                    *
*  Boucle des messages                                               *
*                                                                    *
\********************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch (msg)
    {

      case WM_MOVE:        win->Xpos = (dword) LOWORD(lParam);
                           win->Ypos = (dword) HIWORD(lParam);
                           return 0;

      case WM_CLOSE:       PostQuitMessage(0); // dit ?GetMessage() de renvoyer 0
                           return 0;

      case WM_SYSCOMMAND:  // �vite l'�conomiseur d'�cran
                           switch (wParam)
                             {
                               case SC_SCREENSAVE:
                               case SC_MONITORPOWER:
                               return 0;
                             }
                           break;
/*
      case WM_CHAR:        la touche est traduite dans ce msg 
                           return 0;
*/

      case WM_KEYDOWN:     inp->set_key_down ((dword)wParam);
                           return 0;

      case WM_KEYUP:       inp->set_key_up   ((dword)wParam);
                           return 0;

      case WM_LBUTTONDOWN: inp->set_mouse_left_down();
                           return 0;

      case WM_LBUTTONUP:   inp->set_mouse_left_up();
                           return 0;

      case WM_RBUTTONDOWN: inp->set_mouse_right_down();
                           return 0;

      case WM_RBUTTONUP:   inp->set_mouse_right_up();
                           return 0;

      case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
                           return 0;

      case WM_MBUTTONUP:   inp->set_mouse_middle_up();
                           return 0;

   }

  return DefWindowProc(hwnd,msg,wParam,lParam);
}

