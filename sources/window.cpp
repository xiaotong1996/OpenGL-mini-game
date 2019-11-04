

#include <windows.h>
#include "type.h"
#include "window.h"




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
*  constructeur, initialise quelques variables, ne cr�e pas de fen�tre     *
*                                                                          *
\**************************************************************************/
WINDOW::WINDOW()
{
  fen_ok     = false;
  nbbits     = 24;
  freq       = 60;
  fullscreen = true;
  Xres       = 640;
  Yres       = 480;
  Xpos       = 0;
  Ypos       = 0;

  visible    = false;
  focus      = false;

  handle     = 0;
  context    = 0;
}



/**************************************************************************\
*                                                                          *
*  destructeur, d�truit la fen�tre si elle existe encore                   *
*                                                                          *
\**************************************************************************/
WINDOW::~WINDOW()
{
  if (fen_ok)
    {
      close();
    }

}



/**************************************************************************\
*                                                                          *
*  cr�e une fen�tre, change de mode vid�o si "fullscreen = true"           *
*                                                                          *
\**************************************************************************/
bool WINDOW::create(int width, int height, int nbbits, int freq, bool fullscreen)
{

  this->freq       = freq;
  this->Xres       = width;
  this->Yres       = height;
  this->nbbits     = nbbits;
  this->fullscreen = fullscreen;



  extern LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

  // on commence par changer de mode graphique
  if (fullscreen)
    {
      DEVMODE dmScreenSettings;
      memset(&dmScreenSettings,0,sizeof(dmScreenSettings));

      dmScreenSettings.dmSize             = sizeof(dmScreenSettings);
      dmScreenSettings.dmPelsWidth        = Xres;
      dmScreenSettings.dmPelsHeight       = Yres;
      dmScreenSettings.dmBitsPerPel       = nbbits;
      dmScreenSettings.dmDisplayFrequency = freq;
      dmScreenSettings.dmFields           = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;

      if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) return false;
    }


  // puis on cr�e la fen�tre
  HINSTANCE hInstance;
  WNDCLASS  wc;
  dword     flag;

  if (handle != NULL) return false; // d�j� cr��e

  hInstance         = GetModuleHandle(NULL);
  wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // redessine sur un sizing et doit poss�der son DC
  wc.lpfnWndProc    = (WNDPROC) WndProc;
  wc.cbClsExtra     = 0;                                   // No Extra Window Data
  wc.cbWndExtra     = 0;                                   // No Extra Window Data
  wc.hInstance      = hInstance;
  wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground  = NULL;                                // No Background
  wc.lpszMenuName   = NULL;
  wc.lpszClassName  = "GLClass";

  if (!RegisterClass(&wc))                                 // enregistre la boucle des messages
    {
      return false;
    }

  flag = fullscreen ? WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN : WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
  handle = CreateWindowEx(WS_EX_APPWINDOW,                 // style �tendu
                          "GLClass",
                          "titre � changer dans windows.cpp",
                          flag,
                          0, 0,
                          Xres, Yres,
                          NULL,                           // pas de fen�tre m�re 
                          NULL,                           // pas menu
                          hInstance,
                          NULL);                          // ne passe pas de param�tre par WM_CREATE

  if ( handle == NULL)
    {
      return false;
    }

  SetClassLong(handle, GCL_HICON, (LONG)wc.hIcon);        // "active" l'ic�ne avec un warning de merd..

  if (!fullscreen)                                        // recentre la fen�tre et ajuste sa zone cliente
    {
      RECT r1,r2;
      SystemParametersInfo(SPI_GETWORKAREA,0,&r2,0);
      GetClientRect(handle,&r1);
      Xpos = ((r2.right  - r2.left) - (r1.right  - r1.left))/2;
      Ypos = ((r2.bottom - r2.top ) - (r1.bottom - r1.top ))/2;
      SetWindowPos( handle,
                    HWND_TOP,
                    Xpos, Ypos,
                    Xres+Xres-(r1.right - r1.left), Yres+Yres-(r1.bottom - r1.top),
                    0
                  );
    }
  else
    {
      Xpos = 0;
      Ypos = 0;
    }

  context = GetDC(handle);
  
  //ShowCursor(FALSE);
  ShowWindow(handle,SW_SHOW);
  SetForegroundWindow(handle);
  SetFocus(handle);
  fen_ok = true;

  return true;
}



/**************************************************************************\
*                                                                          *
*  ferme la fen�tre si elle existe, restaure le mode vid�o d'origine       *
*                                                                          *
\**************************************************************************/
bool WINDOW::close()
{

  if (!fen_ok) return false;


  ReleaseDC(handle,context);
  if (handle) DestroyWindow(handle);
  UnregisterClass("KouaikeClass",GetModuleHandle(NULL));
  handle  = NULL;
  context = NULL;
  if (fullscreen) ChangeDisplaySettings(NULL,0);   // revient au mode vid�o standard
  //ShowCursor(TRUE);                                // affiche la souris

  fen_ok = false;

  return true;
}



/**************************************************************************\
*                                                                          *
*  Met � jour le titre de la fen�tre (mode non fullscreen)                 *
*                                                                          *
\**************************************************************************/
void WINDOW::set_title(char *s)
{
  SetWindowText(handle,s);

}




