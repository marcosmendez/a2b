#include <time.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <CGameDevice.h>
#include <clock64.h>

int64_t CGameDevice::CLOCKS_PER_SECOND = 0; 
double CGameDevice::INVERSE_CLOCKS_PER_SECOND = 0;

CFont CGameDevice::FONT ("../data/jag.fnt");

int CGameDevice::MOUSE_X = 0, CGameDevice::MOUSE_Y = 0;
int CGameDevice::MOUSE_LMB = 0, CGameDevice::MOUSE_RMB = 0;
double CGameDevice::MOUSE_XFACTOR = 0, CGameDevice::MOUSE_YFACTOR = 0;

unsigned int CGameDevice::KEY = 0, CGameDevice::REPEAT_KEY = 0;
unsigned int CGameDevice::KEYS[] = {0};

int CGameDevice::WINDOW_WIDTH = 0, CGameDevice::WINDOW_HEIGHT = 0;
int CGameDevice::SCREEN_WIDTH = CGameDevice::WINDOW_WIDTH, CGameDevice::SCREEN_HEIGHT = CGameDevice::WINDOW_HEIGHT;
int CGameDevice::XMAX = 0, CGameDevice::YMAX = 0;

CBox CGameDevice::ARENA;

const color CGameDevice::weakGray = color (0.3, 0.3, 0.3);
const color CGameDevice::gray = color (0.75, 0.75, 0.75);
const color CGameDevice::weakGreen = color (0, 0.3, 0);
const color CGameDevice::green = color (0, 1, 0);
const color CGameDevice::red = color (1, 0, 0);
const color CGameDevice::yellow = color (1, 1, 0);
const color CGameDevice::gold = color (0.75, 0.75, 0);
