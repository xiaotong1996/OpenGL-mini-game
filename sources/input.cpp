
#include <windows.h>
#include <string.h>  // memcpy(), memset(), ...
#include "type.h"
#include "input.h"




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



/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/






/**************************************************************************\
*                                                                          *
* Constructeur                                                             *
*                                                                          *
\**************************************************************************/
MY_INPUT::MY_INPUT(WINDOW *win)
{
  int i;

  window = win;

  tmp_keys  = new bool[256];
  keys      = new bool[256];
  tmp_pulse = new bool[256];
  pulse     = new bool[256];

  for (i=0; i<256; i++)
    {
      tmp_keys [i] = false;
      keys     [i] = false;
      tmp_pulse[i] = false;
      pulse    [i] = false;
    }

  LBmouse_tmp = false;
  RBmouse_tmp = false;
  MBmouse_tmp = false;

  LBmouse = false;
  RBmouse = false;
  MBmouse = false;

  get_mouse_movement(); // initialise les coordonn�es relatives (centrage)
  get_mouse_movement(); // initialise les coordonn�es relatives (calibrage)
  get_mouse_pos();      // initialise les coordonn�es absolues

  Xmouse     = 0;
  Ymouse     = 0;
  Xrelmouse  = 0;
  Yrelmouse  = 0;

}




/**************************************************************************\
*                                                                          *
* Destructeur, vide la RAM                                                 *
*                                                                          *
\**************************************************************************/
MY_INPUT::~MY_INPUT()
{

  delete[] tmp_keys;
  delete[] keys;
  delete[] tmp_pulse;
  delete[] pulse;

}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_KEY_PRESS"              *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_key_down(dword key)
{
  tmp_keys [key] = true;
  tmp_pulse[key] = true;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_KEY_RELEASE"            *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_key_up(dword key)
{
  tmp_keys[key] = false;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_LEFT_DOWN"        *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_left_down()
{
  LBmouse_tmp = true;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_LEFT_UP"          *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_left_up()
{
  LBmouse_tmp = false;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_RIGHT_DOWN"       *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_right_down()
{
  RBmouse_tmp = true;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_RIGHT_UP"         *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_right_up()
{
  RBmouse_tmp = false;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_MIDDLE_DOWN"      *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_middle_down()
{
  MBmouse_tmp = true;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler sur un �v�nement du type "ON_MOUSE_MIDDLE_UP"        *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_middle_up()
{
  MBmouse_tmp = false;
}




/**************************************************************************\
*                                                                          *
*  Fonction � appeler UNE fois juste au d�but de la "Main Loop"            *
*                                                                          *
\**************************************************************************/
void MY_INPUT::refresh()
{
  memcpy( keys , tmp_keys , 256*sizeof(bool) );
  memcpy( pulse, tmp_pulse, 256*sizeof(bool) );

  LBmouse = LBmouse_tmp;
  RBmouse = RBmouse_tmp;
  MBmouse = MBmouse_tmp;

  for (int i=0; i<256; i++) tmp_pulse[i] = false; // la version puls�e est un "instantan�", donc faux
                                                  // au prochain tour
}




/**************************************************************************\
*                                                                          *
*  R�cup�re la position de la souris                                       *
*                                                                          *
\**************************************************************************/
void MY_INPUT::get_mouse_pos()
{

  POINT pt;
  GetCursorPos(&pt);
  Xmouse = (dword) pt.x;
  Ymouse = (dword) pt.y;

}




/**************************************************************************\
*                                                                          *
*  Positionne de la souris                                                 *
*                                                                          *
\**************************************************************************/
void MY_INPUT::set_mouse_pos(dword x, dword y)
{

  SetCursorPos(x,y);

}




/**************************************************************************\
*                                                                          *
*  R�cup�re les coordonn�es relatives de souris depuis le dernier appel    *
*  de cette fonction. Recentre la souris au milieu.                        *
*                                                                          *
\**************************************************************************/
void MY_INPUT::get_mouse_movement()
{

  POINT pt;
  GetCursorPos(&pt);
  Xrelmouse = (short) pt.x;
  Yrelmouse = (short) pt.y;
  Xrelmouse -= window->Xpos + ((window->Xres)>>1);
  Yrelmouse -= window->Ypos + ((window->Yres)>>1);
  SetCursorPos(window->Xpos+((window->Xres)>>1), window->Ypos+((window->Yres)>>1)); // recentre la souris


}







