//hello final separate fn call


/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2012-2013 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#include <Windows.h>
#include<ShellAPI.h>
#include<math.h>
#include <WindowsX.h>
#include <commctrl.h>
#include<stdlib.h>
#include<iostream>
#include "resource.h"
#include "pxcgesture.h"
#include "pxcsmartptr.h"
using namespace std;
             int tek=0;
             int count=0;
			 int t3,t4;
			 int take=0;
        	 POINT pts;
			BOOL result;			
		

#define IDC_STATUS 10000
#define ID_DEVICEX 21000
#define ID_MODULEX 22000
SHELLEXECUTEINFO SheExecInfo = {0};

SHELLEXECUTEINFO ShExecInfo = {0};
 int cpt = 0 ;
                  int mode = GMMP_USE_DISPLAY_POINTS ;
				  

int hor,vert;
int px,py,cx,cy;
int dx=-1,dy=0,flg=0;
unsigned __int64 pt,ct,dt;
int when=0,i=0; int arr[5]={0};
 
void checkstat(){
HWND hwndDlg;
}

//the mouse
int threshold;
int w,h,new_x,new_y;
int c,dist,pip=0,dxflag=1,dyflag=1;

float sdp,slope=0;
int mod_dx,mod_dy;
float log_x;float log_y;//screen coordinates

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   hor=horizontal;
   vertical = desktop.bottom;
   vert=vertical;
}


HINSTANCE   g_hInst=0;
PXCSession *g_session=0;

/* Panel Bitmap */
HBITMAP     g_bitmap=0;

/* None Gesture */
HBITMAP     g_none=0;

/* Threading control */
volatile bool g_running=false;
volatile bool g_stop=true;

/* Control Layout */
int g_controls[]={ IDC_IGDGM, IDC_SCALE, IDC_MIRROR, IDC_GEONODE, IDC_GESTURE, IDC_WEB, IDC_GESTURE2, IDC_GESTURE1, ID_START, ID_STOP };
RECT g_layout[3+sizeof(g_controls)/sizeof(g_controls[0])];
int inert=2;


void mouse()
{   
    INPUT input;
    input.type=INPUT_MOUSE;
    input.mi.dx=0;
    input.mi.dy=0;
	input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP);
    input.mi.mouseData=0;
    input.mi.dwExtraInfo=NULL;
    input.mi.time=0;
    SendInput(1,&input,sizeof(INPUT));
	
   
}
 int flag =0,temp=0;
void mouse1()
{  
    INPUT input;
    input.type=INPUT_MOUSE;
    input.mi.dx=0;
    input.mi.dy=0;
	input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
    input.mi.mouseData=0;
    input.mi.dwExtraInfo=NULL;
    input.mi.time=0;
    SendInput(1,&input,sizeof(INPUT));
	flag++;
	if(flag % 2 == 0 )
	{input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
	SendInput(1,&input,sizeof(INPUT));temp=10;}
   
}


void scroll()
{INPUT in;
in.type = INPUT_MOUSE;
in.mi.dx = 0;
in.mi.dy = 0;
in.mi.dwFlags = (MOUSEEVENTF_WHEEL|MOUSEEVENTF_ABSOLUTE);
in.mi.time = 0;
in.mi.dwExtraInfo = 0;
in.mi.mouseData = WHEEL_DELTA*10;
SendInput(1,&in,sizeof(in));
}

void scrollup()
{INPUT in;
in.type = INPUT_MOUSE;
in.mi.dx = -15;
in.mi.dy = 0;
in.mi.dwFlags = (MOUSEEVENTF_WHEEL|MOUSEEVENTF_ABSOLUTE);
in.mi.time = 0;
in.mi.dwExtraInfo = 0;
in.mi.mouseData = WHEEL_DELTA*100;
SendInput(1,&in,sizeof(in));
}

void scrolld()
{INPUT in; 
in.type = INPUT_MOUSE;
in.mi.dx = 0;
in.mi.dy = -15;
in.mi.dwFlags = (MOUSEEVENTF_WHEEL|MOUSEEVENTF_ABSOLUTE);
in.mi.time = 0;
in.mi.dwExtraInfo = 0;
//for(int j=1;j<100;j+2)
{	//Sleep(1);
in.mi.mouseData = 80;}
SendInput(1,&in,sizeof(in));
}

void scrollr()
{INPUT in;
in.type = INPUT_MOUSE;
in.mi.dx = 0;
in.mi.dy = 0;
in.mi.dwFlags = (MOUSEEVENTF_WHEEL|MOUSEEVENTF_ABSOLUTE);
in.mi.time = 0;
in.mi.dwExtraInfo = 0;
in.mi.mouseData = -(WHEEL_DELTA*8);
SendInput(1,&in,sizeof(in));
}







void SaveLayout(HWND hwndDlg) {
	GetClientRect(hwndDlg,&g_layout[0]);
	ClientToScreen(hwndDlg,(LPPOINT)&g_layout[0].left);
	ClientToScreen(hwndDlg,(LPPOINT)&g_layout[0].right);
	GetWindowRect(GetDlgItem(hwndDlg,IDC_PANEL),&g_layout[1]);
	GetWindowRect(GetDlgItem(hwndDlg,IDC_STATUS),&g_layout[2]);
	for (int i=0;i<sizeof(g_controls)/sizeof(g_controls[0]);i++)
		GetWindowRect(GetDlgItem(hwndDlg,g_controls[i]),&g_layout[3+i]);
}

void RedoLayout(HWND hwndDlg) {
	RECT rect;
	GetClientRect(hwndDlg,&rect);

	/* Status */
	SetWindowPos(GetDlgItem(hwndDlg,IDC_STATUS),hwndDlg,
		0,
		rect.bottom-(g_layout[2].bottom-g_layout[2].top),
		rect.right-rect.left,
		(g_layout[2].bottom-g_layout[2].top),SWP_NOZORDER);

	/* Panel */
	SetWindowPos(GetDlgItem(hwndDlg,IDC_PANEL),hwndDlg,
		(g_layout[1].left-g_layout[0].left),
		(g_layout[1].top-g_layout[0].top),
		rect.right-(g_layout[1].left-g_layout[0].left)-(g_layout[0].right-g_layout[1].right),
		rect.bottom-(g_layout[1].top-g_layout[0].top)-(g_layout[0].bottom-g_layout[1].bottom),
		SWP_NOZORDER);

	/* Buttons & CheckBoxes */
	for (int i=0;i<sizeof(g_controls)/sizeof(g_controls[0]);i++) {
		SetWindowPos(GetDlgItem(hwndDlg,g_controls[i]),hwndDlg,
			rect.right-(g_layout[0].right-g_layout[3+i].left),
			(g_layout[3+i].top-g_layout[0].top),
			(g_layout[3+i].right-g_layout[3+i].left),
			(g_layout[3+i].bottom-g_layout[3+i].top),
			SWP_NOZORDER);
	}
}

static void PopulateDevice(HMENU menu) {
	DeleteMenu(menu,0,MF_BYPOSITION);

	PXCSession::ImplDesc desc;
	memset(&desc,0,sizeof(desc));
	desc.group=PXCSession::IMPL_GROUP_SENSOR;
	desc.subgroup=PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	HMENU menu1=CreatePopupMenu();
	for (int i=0,k=ID_DEVICEX;;i++) {
		PXCSession::ImplDesc desc1;
		if (g_session->QueryImpl(&desc,i,&desc1)<PXC_STATUS_NO_ERROR) break;
		PXCSmartPtr<PXCCapture> capture;
		if (g_session->CreateImpl<PXCCapture>(&desc1,&capture)<PXC_STATUS_NO_ERROR) continue;
		for (int j=0;;j++) {
			PXCCapture::DeviceInfo dinfo;
			if (capture->QueryDevice(j,&dinfo)<PXC_STATUS_NO_ERROR) break;
			AppendMenu(menu1,MF_STRING,k++,dinfo.name);
		}
	}
	CheckMenuRadioItem(menu1,0,GetMenuItemCount(menu1),0,MF_BYPOSITION);
	InsertMenu(menu,0,MF_BYPOSITION|MF_POPUP,(UINT_PTR)menu1,L"Device");
}

static int GetChecked(HMENU menu) {
	for (int i=0;i<GetMenuItemCount(menu);i++)
		if (GetMenuState(menu,i,MF_BYPOSITION)&MF_CHECKED) return i;
	return 0;
}

pxcCHAR* GetCheckedDevice(HWND hwndDlg) {
	HMENU menu=GetSubMenu(GetMenu(hwndDlg),0);	// ID_DEVICE
	static pxcCHAR line[256];
	GetMenuString(menu,GetChecked(menu),line,sizeof(line)/sizeof(pxcCHAR),MF_BYPOSITION);
	return line;
}

static void PopulateModule(HMENU menu) {
	DeleteMenu(menu,1,MF_BYPOSITION);

	PXCSession::ImplDesc desc, desc1;
	memset(&desc,0,sizeof(desc));
	desc.cuids[0]=PXCGesture::CUID;
	HMENU menu1=CreatePopupMenu();
	int i;
	for (i=0;;i++) {
		if (g_session->QueryImpl(&desc,i,&desc1)<PXC_STATUS_NO_ERROR) break;
		AppendMenu(menu1,MF_STRING,ID_MODULEX+i,desc1.friendlyName);
	}
	CheckMenuRadioItem(menu1,0,i,0,MF_BYPOSITION);
	InsertMenu(menu,1,MF_BYPOSITION|MF_POPUP,(UINT_PTR)menu1,L"Module");
}

pxcCHAR *GetCheckedModule(HWND hwndDlg) {
	HMENU menu=GetSubMenu(GetMenu(hwndDlg),1);	// ID_MODULE
	static pxcCHAR line[256];
	GetMenuString(menu,GetChecked(menu),line,sizeof(line)/sizeof(pxcCHAR),MF_BYPOSITION);
	return line;
}

static DWORD WINAPI ThreadProc(LPVOID arg) {
	void SimplePipeline(HWND hwndDlg);
	void AdvancedPipeline(HWND hwndDlg);

	if (GetMenuState(GetMenu((HWND)arg),ID_PIPELINE_SIMPLE,MF_BYCOMMAND)&MF_CHECKED) {
		SimplePipeline((HWND)arg);
	} else {
		AdvancedPipeline((HWND)arg);
	}

	PostMessage((HWND)arg,WM_COMMAND,ID_STOP,0);
	g_running=false;
	return 0;
}

void SetStatus(HWND hwndDlg, pxcCHAR *line) {
	HWND hwndStatus=GetDlgItem(hwndDlg,IDC_STATUS);
	SetWindowText(hwndStatus,line);
}

  bool GetLabelmapState(HWND hwndDlg) {
	return (Button_GetState(GetDlgItem(hwndDlg,IDC_IGDGM))&BST_CHECKED);
}

bool GetDepthState(HWND hwndDlg) {
	return (Button_GetState(GetDlgItem(hwndDlg,IDC_DEPTH))&BST_CHECKED);
}

bool GetPlaybackState(HWND hwndDlg) {
	return (GetMenuState(GetMenu(hwndDlg),ID_MODE_PLAYBACK,MF_BYCOMMAND)&MF_CHECKED)!=0;
}

bool GetRecordState(HWND hwndDlg) {
	return (GetMenuState(GetMenu(hwndDlg),ID_MODE_RECORD,MF_BYCOMMAND)&MF_CHECKED)!=0;
}

void DrawBitmap(HWND hwndDlg, PXCImage *image) {
    if (g_bitmap) {
        DeleteObject(g_bitmap);
		g_bitmap=0;
    }

    PXCImage::ImageInfo info;
    image->QueryInfo(&info);
    PXCImage::ImageData data;
    if (image->AcquireAccess(PXCImage::ACCESS_READ,PXCImage::COLOR_FORMAT_RGB32, &data)>=PXC_STATUS_NO_ERROR) {
		HWND hwndPanel=GetDlgItem(hwndDlg,IDC_PANEL);
        HDC dc=GetDC(hwndPanel);
		BITMAPINFO binfo;
		memset(&binfo,0,sizeof(binfo));
		binfo.bmiHeader.biWidth=info.width;
		binfo.bmiHeader.biHeight= - (int)info.height;
		binfo.bmiHeader.biBitCount=32;
		binfo.bmiHeader.biPlanes=1;
		binfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		binfo.bmiHeader.biCompression=BI_RGB;
        g_bitmap=CreateDIBitmap(dc, &binfo.bmiHeader, CBM_INIT, data.planes[0], &binfo, DIB_RGB_COLORS);
        ReleaseDC(hwndPanel, dc);
		image->ReleaseAccess(&data);
    }
}

HBITMAP ResizeBitmap(HWND hwnd, HBITMAP bitmap, bool scale=true, bool mirror=false) {
	RECT rect;
	GetClientRect(hwnd,&rect);

	BITMAP bm;
	GetObject(bitmap,sizeof(BITMAP),&bm);

	if (!scale) {
		rect.right=bm.bmWidth;
		rect.bottom=bm.bmHeight;
	}

	HDC dc=GetDC(hwnd);
	HDC dc2=CreateCompatibleDC(dc);
	SelectObject(dc2,bitmap);

	HBITMAP bitmap2=CreateCompatibleBitmap(dc,rect.right,rect.bottom);
	HDC dc3=CreateCompatibleDC(dc);
	SelectObject(dc3,bitmap2);
	ReleaseDC(hwnd,dc);

	SetStretchBltMode(dc3, HALFTONE);
	if (mirror) {
		StretchBlt(dc3,rect.right-1,0,-rect.right,rect.bottom,dc2,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	} else {
		if (!scale) {
			BitBlt(dc3,0,0,rect.right,rect.bottom,dc2,0,0,SRCCOPY);
		} else {
			StretchBlt(dc3,0,0,rect.right,rect.bottom,dc2,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		}
	}
	if (!scale) SetPixel(dc3,0,0,RGB(0,0,0)); // WORKAROUND for SS_CENTERIMAGE

	DeleteObject(dc3);
	DeleteObject(dc2);
	return bitmap2;
}

void UpdatePanel(HWND hwndDlg) {
	if (!g_bitmap) return;

	bool scale=Button_GetState(GetDlgItem(hwndDlg,IDC_SCALE))&BST_CHECKED;
	bool mirror=Button_GetState(GetDlgItem(hwndDlg,IDC_MIRROR))&BST_CHECKED;

	HWND panel=GetDlgItem(hwndDlg,IDC_PANEL);
	HBITMAP bitmap=ResizeBitmap(panel,g_bitmap,scale,mirror);
	DeleteObject(g_bitmap);
	g_bitmap=bitmap;
	SendMessage(panel,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)g_bitmap);
	InvalidateRect(panel,0,TRUE);
}
int x,y,xw,yw,sum=0,xsum=0,ysum=0;


void MoveMouse(HWND hwndDlg,PXCGesture::GeoNode nodes[11]){
	     float xw=nodes[0].positionWorld.x;
	     float yw=nodes[0].positionWorld.y;
	     float zw=nodes[0].positionWorld.z;
	     int f= nodes[0].openness;
	     int xb=(int)nodes[2].positionImage.x;
	     int yb=(int)nodes[2].positionImage.y;
	     unsigned __int64 stamp=nodes[0].timeStamp;

	    int logb_x=1,logb_y=1;MOUSEMOVEPOINT pts ;
		ct=stamp; cx=xb; cy=yb; if(!flg) {pt=ct;px=xb;py=cy;flg=1;}
	  int dx=0,dy=0;float spd=0.65;
	  if(xb==0 || yb==0 ||px==0 ||py==0) {dx=0;dy=0;flg=0;}

	  if(xb!=0 && yb!=0) 
	  {
		dy=yb-py;dt=ct-pt;
	   
			  if(i<4)
			  {arr[i]=xb;i++;}
			  else
			  {i=0;arr[i]=xb;}

			  dx=cx-px;//arr[0]-arr[1];
			  
			  mod_dx=dx,mod_dy=dy;
			  if(mod_dx<0) mod_dx*=-1;
			  if(mod_dy<0) mod_dy*=-1;

    	      int threshold=1;   
			  int max_threshold=20;
			  
	  if( (mod_dx>threshold || mod_dy>threshold) && (mod_dx<max_threshold && mod_dy<max_threshold) && yw<0.40 )
	   {
		   dxflag==-1;dyflag==-1;
		 
		 if(dx<=0) dxflag=1;
		 else dxflag=-1;
		
		 if(dy<=0) dyflag=1;
		 else dyflag=-1;
		
				  unsigned int p1=(unsigned int)(dx*dx);
					unsigned int p2=(unsigned int)(dy*dy);
	//				unsigned int dist=(p1+p2)/2;
					float sq=sqrtf((float)p1+p2);
					float spd=(sq*1000)/dt;

					
					//pow((float)(p1+p2)
					if(spd*100>5) spd=5/100;//max speed restricting

	POINT pt={0},pos;
	MOUSEMOVEPOINT mp_in ;
    

int cpt = 0 ;
int mode = GMMP_USE_HIGH_RESOLUTION_POINTS ;

ZeroMemory(&mp_in, sizeof(mp_in)) ;
GetCursorPos(&pos);
mp_in.x = ((int)pos.x) & 0x0000FFFF;  //Ensure that this number will pass through.
mp_in.y = ((int)pos.y) & 0x0000FFFF;

	cpt = GetMouseMovePointsEx(sizeof(mp_in),&mp_in,&pts, 1, mode) ;
	w=65535;h=65535;
			
			int spd_scl=spd*100;
			int ky=1,kx=1;

			//if(result)
			{  
				if(spd_scl<0.45) {kx=7*100;}
				else if(spd_scl<0.75) {kx=15*100;}
				else if(spd_scl>=0.75 && spd_scl<1) {kx=15*100;}
				else {kx=25*100;}
				if(spd_scl>0.95) {kx=35*100;}

				if(spd_scl<0.65) {ky=5*100;}
				else 
					if(spd_scl<0.75){ky=7*100;}
				else if(spd_scl>=0.75 && spd_scl<1) {ky=15*100;}
				else {ky=20*100;}
				if(spd_scl>1) {ky=30*100;}

     if(mod_dx>2) {
			
			slope=(float)dy/dx;

			if(abs(slope) < 1)//use kx==horizontal move.... else use ky==vertical move
			{
             if( dxflag==1 ) log_x= pts.x + kx;  //k*spd_scl; //pts.x + dx * 4.2;//pts.x + spd_scl*10; //(spd_scl*20*w/320);//(spd_scl*spd_scl);  //10;
			 else log_x= pts.x  - kx;  //pts.x - dx * 4.2;//pts.x - spd_scl*10;      //(spd_scl*20*w/320);//-(spd*100)*100;
			
			 logb_x=log_x;
			 if(logb_x > w) logb_x=w; 
			 if(logb_x < 0) logb_x=0;
		
			  //if(slope!=0)
			  {
				 log_y=(slope * (pts.x-logb_x) + pts.y);
			  }
			  /*else 
			  {
				if(dx%2!=0) dx=dx-(dx%2);
				if(dy%2!=0) dy=dy-(dy%2);

			  }*/
			  logb_y=log_y;
			  if(logb_y<0) logb_y=0; if(logb_y>h) logb_y=h;
			 //else log_y= pts.y;
			  //if(dy==5) logb_x=;
			}
		else 
		{
			 if( dxflag==1 ) log_x= pts.x + ky;  //k*spd_scl; //pts.x + dx * 4.2;//pts.x + spd_scl*10; //(spd_scl*20*w/320);//(spd_scl*spd_scl);  //10;
			 else log_x= pts.x  - ky;  //pts.x - dx * 4.2;//pts.x - spd_scl*10;      //(spd_scl*20*w/320);//-(spd*100)*100;
			
			 logb_x=log_x;
			 if(logb_x > w) logb_x=w; 
			 if(logb_x < 0) logb_x=0;
		
			  log_y=(slope * (pts.x-logb_x) + pts.y);
			  logb_y=log_y;
			  if(logb_y<0) logb_y=0; if(logb_y>h) logb_y=h;
		 
			}
		}
			/*
			if(dyflag==1) log_y=pts.y + kx;
			else log_y=pts.y - kx;

			logb_y=log_y;
			if(logb_y<0) logb_y=0; if(logb_y>h) logb_y=h;
			
			
			log_x=((pts.y-logb_y)/slope + pts.x);
			  logb_x=log_y;
			  if(logb_x<0) logb_x=0; if(logb_x>w) logb_x=w;//else case of slope<1
			*/
	        else {    
						logb_x=pts.x;
						logb_y=pts.y;
						if(dy>0)
							log_y=pts.y + kx;
						else if(dy<0)
							log_y=pts.y-kx;
						logb_y=log_y;
						if(logb_y<0) logb_y=0; if(logb_y>h) logb_y=h;
				}
			

			  //dy=100;dx=100;
				HWND ltt=GetDlgItem(hwndDlg,IDC_LIST2);
	               wchar_t d[250];//Mult1iByteToWideChar //smsgA
				  //f=100.99; 
			  //swprintf(d,250,L"currentpos(%d,%d)  newpos::(%d,%d) dxflag,dyflag::(%d,%d) slope::%d  dx,y:(%d,%d)     p:(%d,%d) factor=%f c:(%d,%d)  dist=%f ",pts.x,pts.y,logb_x,logb_y,  dxflag,dyflag,slope,  dx,dy ,px,py, spd, cx,cy,sq);
				swprintf(d,100,L"factor::%f   dx,dy(%d,%d)  slope::%f  c(%d,%d) p(%d,%d)", spd*100,  dx,dy,  slope , cx,cy,px,py);
				   //swprintf(d,90,L"factor::%f", spd*100);

					SendMessage(ltt,LB_DELETESTRING,0,(LPARAM)d);
				  SendMessage(ltt,LB_ADDSTRING,0,(LPARAM)d);
				
						INPUT  inp={0};
						inp.type = INPUT_MOUSE;
						inp.mi.dwFlags  = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
						inp.mi.dx = logb_x; 
						inp.mi.dy = logb_y;
						SendInput(1,&inp,sizeof(INPUT));
			
			   }
			}
			   px=cx; py=cy;pt=ct;
			   
			  } //entry cndn xb!=0

               
	 HWND listt=GetDlgItem(hwndDlg,IDC_LIST1);
	 wchar_t data[200];//Mult1iByteToWideChar //smsgA
	 swprintf(data,200,L" x:%d|,y:%d log_p::(%f,%f) cntpos::(%d,%d) sdp=%f pip:%d",xb,yb, logb_x,logb_y,  pts.x,pts.y,  sdp,pip);
	
	 SendMessage(listt,LB_DELETESTRING,0,(LPARAM)data);
	 SendMessage(listt,LB_ADDSTRING,0,(LPARAM)data);
     
	 inert++;

}

int sendinpflag,ck=0;
PXCGesture::GeoNode nodes[2][11];
void DrawGeoNode(HWND hwndDlg, PXCGesture::GeoNode nodes[2][11]) {
	  
	   
       bool ch = Button_GetState(GetDlgItem(hwndDlg,IDC_IGDGM))&BST_CHECKED;
     //  bool ch1 =Button_GetState(GetDlgItem(hwndDlg,IDC_IGDGM))&BST_UNCHECKED;
       if(ch && ck==0){
	                ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
                    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
                    ShExecInfo.hwnd = NULL;
                    ShExecInfo.lpVerb = NULL;
                    ShExecInfo.lpFile = L"C:\\face.exe";      
                    ShExecInfo.lpParameters = L"";   
                    ShExecInfo.lpDirectory = NULL;
                    ShExecInfo.nShow = SW_SHOW;
                    ShExecInfo.hInstApp = NULL; 
                    ShellExecuteEx(&ShExecInfo);
					ck=10;
					Sleep(500);
} 
     bool ch1 =Button_GetState(GetDlgItem(hwndDlg,IDC_IGDGM))&BST_UNCHECKED;
	 if(ch1) 
      {GetProcessId(ShExecInfo.hProcess);
       TerminateProcess(ShExecInfo.hProcess, 1);
        ck=0; }

	sum++;
	checkstat();
	if (!g_bitmap) return;
   // bool params=Button_GetState(GetDlgItem(hwndDlg,IDC_PARAMS))&BST_CHECKED;
	bool geonode=Button_GetState(GetDlgItem(hwndDlg,IDC_GEONODE))&BST_CHECKED;

	HWND hwndPanel=GetDlgItem(hwndDlg,IDC_PANEL);
	HDC dc=GetDC(hwndPanel);
	HDC dc2=CreateCompatibleDC(dc);
	SelectObject(dc2,g_bitmap);

	BITMAP bm;
	GetObject(g_bitmap,sizeof(bm),&bm);

 //   if (params)
	{
		HPEN cyan=CreatePen(PS_SOLID,3,RGB(255,255,0));
		SelectObject(dc2,cyan);

		if (nodes[0][0].body > 0) {
			MoveToEx(dc2, 3, bm.bmHeight-1,0);
			int y=(100 - nodes[0][0].openness) * (bm.bmHeight-1) / 100;
			if (y>=0 && y<bm.bmHeight) LineTo(dc2,3, y);
		}
        if (nodes[1][0].body > 0) {
			MoveToEx(dc2,bm.bmWidth-3,bm.bmHeight-1,0);
			int y=(100 - nodes[1][0].openness)*(bm.bmHeight-1) / 100;
            if (y>=0 && y<bm.bmHeight) LineTo(dc2,bm.bmWidth-3, y);
        }
		DeleteObject(cyan);
    } //eo params

	if (geonode) {
		HPEN red=CreatePen(PS_SOLID,3,RGB(255,0,0));
		HPEN green=CreatePen(PS_SOLID,3,RGB(0,255,0));

		for (int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 11; j++) 
			{
			   if (nodes[i][j].body <= 0) continue;
			   int sz = (j == 0) ? 10 : ((nodes[i][j].radiusImage>5)?(int)nodes[i][j].radiusImage:5);
			   int x=(int)nodes[i][j].positionImage.x;
			   int y=(int)nodes[i][j].positionImage.y;
	
			   SelectObject(dc2,red);
	            		    
			   SelectObject(dc2,(j<6 || j==10)?red:green);
			   MoveToEx(dc2, x, y, 0);
			   Arc(dc2,x-sz,y-sz,x+sz,y+sz,x+sz,y+sz,x+sz,y+sz);
			
			}
		}	       		      
	      /* if(tek > 0)	   
	        {
				if(nodes[0][2].positionWorld.y < nodes[1][2].positionWorld.y || nodes[1][2].positionWorld.y == 99 )
		             {//MessageBoxW(0,L"Initiate RIGHT click",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
						 if(sendinpflag!=1){
							sendinpflag=1;
							INPUT  inp={0};
							inp.type = INPUT_MOUSE;
							inp.mi.dwFlags  = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
							inp.mi.dx = 30000; 
							inp.mi.dy = 35000;
							SendInput(1,&inp,sizeof(INPUT));
						 }
						 MoveMouse(hwndDlg,nodes[0]);
					 }
				else if(nodes[0][2].positionWorld.y > nodes[1][2].positionWorld.y || nodes[0][2].positionWorld.y == 99)
						{
							//MessageBoxW(0,L"Initiate left click",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
						if(sendinpflag!=2){
							sendinpflag=2;
							INPUT  inp={0};
							inp.type = INPUT_MOUSE;
							inp.mi.dwFlags  = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
							inp.mi.dx = 30000; 
							inp.mi.dy = 35000;
							SendInput(1,&inp,sizeof(INPUT));
						 }
						 
							MoveMouse(hwndDlg,nodes[1]);} */
		 //  }

		   //if(tek)	
				MoveMouse(hwndDlg,nodes[0]);
		DeleteObject(red);
		DeleteObject(green);
	}//eo geonode

	
	DeleteObject(dc2);
	ReleaseDC(hwndPanel,dc);
}//eo fn 
 
 HWND hConsoleWnd;
void DrawGesture(HWND hwndDlg,PXCGesture::Gesture gestures[2]) {
	//checkstat();
	static int gesture_panels[2]={ IDC_GESTURE1, IDC_GESTURE2 };
	
	static struct { int label,id; HBITMAP bitmap; } bitmaps[]={
		{ PXCGesture::Gesture::LABEL_POSE_THUMB_UP,     IDB_THUMB_UP    , 0},
		{ PXCGesture::Gesture::LABEL_POSE_THUMB_DOWN,	IDB_THUMB_DOWN  , 0},
		{ PXCGesture::Gesture::LABEL_POSE_PEACE,        IDB_PEACE       , 0},
		{ PXCGesture::Gesture::LABEL_POSE_BIG5,         IDB_BIG5        , 0},
		{ PXCGesture::Gesture::LABEL_HAND_WAVE,			IDB_WAVE		, 0},
		{ PXCGesture::Gesture::LABEL_HAND_CIRCLE,		IDB_CIRCLE		, 0},
		{ PXCGesture::Gesture::LABEL_NAV_SWIPE_LEFT,    IDB_SWIPE_LEFT  , 0},
		{ PXCGesture::Gesture::LABEL_NAV_SWIPE_RIGHT,   IDB_SWIPE_RIGHT , 0},
		{ PXCGesture::Gesture::LABEL_NAV_SWIPE_UP,      IDB_SWIPE_UP    , 0},
		{ PXCGesture::Gesture::LABEL_NAV_SWIPE_DOWN,    IDB_SWIPE_DOWN  , 0},
	};

	if (!(Button_GetState(GetDlgItem(hwndDlg,IDC_GESTURE))&BST_CHECKED)) 
	{bitmaps[3].id==103;return;}

	for (int i=0;i<2;i++) {
		if (gestures[i].body<=0) continue;

		HWND hwndPanel=GetDlgItem(hwndDlg,gesture_panels[i]);
		
		for (int j=0;j<sizeof(bitmaps)/sizeof(bitmaps[0]);j++) {
			if (bitmaps[j].label!=gestures[i].label){/*MessageBoxW(0,L"Initiate left click",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);*/ continue;}

		
			  if(bitmaps[j].id==111 )
				{/*MessageBoxW(0,L"Initiate left click",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);*/if(temp<1)mouse1();temp--;}
				if(bitmaps[j].id==110)
				{mouse();}


				//right scr
				if(bitmaps[j].id==107)
				{/*MessageBoxW(0,L"Initiate left",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);*///scrollr();Sleep(500);
				     BYTE keyState[256];  GetKeyboardState((LPBYTE)&keyState);
      
         {
           // Simulate a key press
           for(int i=0;i<25;i++) {Sleep(10); keybd_event( VK_RIGHT,0x27,KEYEVENTF_EXTENDEDKEY | 0,0 );}
		   
          // Simulate a key release
		   { keybd_event( VK_RIGHT,0x27,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);}} }


          

				if(bitmaps[j].id==108)
				{/*MessageBoxW(0,L"Initiate right",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);*///scroll();Sleep(500);}
				 BYTE keyState[256];

                   GetKeyboardState((LPBYTE)&keyState);
      
         {// Simulate a key press
           for(int i=0;i<25;i++) {Sleep(10); keybd_event( VK_LEFT,
                      0x25,
                      KEYEVENTF_EXTENDEDKEY | 0,
					  0 );}

          // Simulate a key release
		   { keybd_event( VK_LEFT,0x25,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);}}}
			
				
				
				
				
				//down
				if(bitmaps[j].id==106){//MessageBoxW(PXCGesture::Gesture::timeStamp ,L"Initiate right",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);/*scrolld()*/
					 BYTE keyState[256];

                   GetKeyboardState((LPBYTE)&keyState);
      
         {
           // Simulate a key press
           for(int i=0;i<50;i++) {Sleep(10); keybd_event( VK_DOWN,
                      0x28,
                      KEYEVENTF_EXTENDEDKEY | 0,
					  0 );}

          // Simulate a key release
		   { keybd_event( VK_DOWN,0x28,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);}}}




		 //up
		

				if(bitmaps[j].id==109){

                 /*
					int SB_LINEUP = 0; 
                    ptrWparam = new IntPtr(SB_LINEUP);
                    ptrLparam = new IntPtr(0);
                    SendMessage(txtArea.Handle, WM_VSCROLL, ptrWparam, ptrLparam);*/
					
					
					   
					 BYTE keyState[256];

                   GetKeyboardState((LPBYTE)&keyState);
      
         {
           // Simulate a key press
           for(int i=0;i<50;i++) {Sleep(10); keybd_event( VK_UP, 0x26, KEYEVENTF_EXTENDEDKEY | 0,0 );}

          // Simulate a key release
		   { keybd_event( VK_UP,0x26,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);}} }








				if(bitmaps[j].id==105){
					//tek=0;//ck=0;
					if(ck>0)
					{GetProcessId(ShExecInfo.hProcess);
                     TerminateProcess(ShExecInfo.hProcess, 1);
					 CheckDlgButton(hwndDlg,IDC_IGDGM,BST_UNCHECKED);ck=0;}
					if(tek>0)
					{GetProcessId(SheExecInfo.hProcess);
                     TerminateProcess(SheExecInfo.hProcess, 1);
					 //CheckDlgButton(hwndDlg,IDC_KEYB,BST_UNCHECKED);
					 tek=0;}
					//MessageBoxW(0,L"Initiate right",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
					/*DWORD dwPID = NULL; 
                    HANDLE hProc = INVALID_HANDLE_VALUE; 
                    GetWindowThreadProcessId(FindWindow(NULL,TEXT("On-Screen Keyboard")), &dwPID); 
                    
					hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPID);
                    //if (NULL != hProc)
                    // {	//MessageBoxW(0,L"Initiate right",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
                      TerminateProcess(hProc, 0);
                    // }
				    CloseHandle(hProc);*/ 
					//DestroyWindow(hConsoleWnd);
									}
				if(bitmaps[j].id==103){
				/*HWND hwnd;
				 ShellExecute(hwnd, L"open", L"K:\face.exe", NULL, NULL, SW_SHOWNORMAL);*/
					//SHELLEXECUTEINFO SheExecInfo = {0};
					SheExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
                    SheExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
                    SheExecInfo.hwnd = NULL;
                    SheExecInfo.lpVerb = NULL;
                    SheExecInfo.lpFile = L"C:\\Windows\\System32\\osk.exe";      
                    SheExecInfo.lpParameters = L"";   
                    SheExecInfo.lpDirectory = NULL;
                    SheExecInfo.nShow = SW_SHOW;
                    SheExecInfo.hInstApp = NULL; 
                    ShellExecuteEx(&SheExecInfo);
					CheckDlgButton(hwndDlg,IDC_KEYB,BST_CHECKED);
                   
				 tek=10;

				// HWND hwnd1 = GetConsoleWindow();
				// DestroyWindow(hwnd1);
				

				//if(set<0){set=0;}
				//if(set<1 ){set = 1000;
             /*   BYTE keyState[256];
                GetKeyboardState((LPBYTE)&keyState);
      
      {
      // Simulate a key press
        { keybd_event( VK_LWIN,
                      0x5B,
                      KEYEVENTF_EXTENDEDKEY | 0,
					  0 );}

      // Simulate a key release
       { keybd_event( VK_LWIN,
                      0x5B,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
					  0);}*/
	  
	   //  Sleep(600); 

	  
			
   } 
HBITMAP bmp=LoadBitmap(g_hInst,MAKEINTRESOURCE(bitmaps[j].id));
			if (!bitmaps[j].bitmap) {
				HBITMAP bmp=LoadBitmap(g_hInst,MAKEINTRESOURCE(bitmaps[j].id));
				
				bitmaps[j].bitmap=ResizeBitmap(hwndPanel,bmp);
				
			LoadBitmap(g_hInst,MAKEINTRESOURCE(113));
				DeleteObject(bmp);
			
			
			} 
				
			SendMessage(hwndPanel,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)bitmaps[j].bitmap);
			SetTimer(hwndDlg,gesture_panels[i],3000,0);
			InvalidateRect(hwndPanel,0,FALSE);
			break;
		}
		

	}
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM) { 
	HMENU menu=GetMenu(hwndDlg);
	HMENU menu1;

    switch (message) { 
    case WM_INITDIALOG:
		{
		HWND listt=GetDlgItem(hwndDlg,IDC_LIST1);
		SendMessage(listt,LB_ADDSTRING,0,(LPARAM)L"hello");
		//SendMessage(listt,LB_ADDSTRING,1,(LPARAM)L"hello2");
		//ListBox_AddString(hwndDlg,L"hello2");
		//ListBox_AddString(hwndDlg,L"hello3");
		CheckDlgButton(hwndDlg,IDC_DEPTH,BST_CHECKED);
		CheckDlgButton(hwndDlg,IDC_MIRROR,BST_CHECKED);
		CheckDlgButton(hwndDlg,IDC_GEONODE,BST_CHECKED);
		CheckDlgButton(hwndDlg,IDC_GESTURE,BST_CHECKED);
		CheckDlgButton(hwndDlg,IDC_WEB,BST_CHECKED);
		PopulateDevice(menu);
		PopulateModule(menu);
		g_none=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_NONE));
		SaveLayout(hwndDlg);
		//MessageBoxW(0,L"Initiate left click",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
        return TRUE; 
		}

	case WM_COMMAND: 
		{menu1=GetSubMenu(menu,0);
		if (LOWORD(wParam)>=ID_DEVICEX && LOWORD(wParam)<ID_DEVICEX+GetMenuItemCount(menu1)) {
			CheckMenuRadioItem(menu1,0,GetMenuItemCount(menu1),LOWORD(wParam)-ID_DEVICEX,MF_BYPOSITION);
			return TRUE;
		}

		}//EO SWITCH

		menu1=GetSubMenu(menu,1);
		if (LOWORD(wParam)>=ID_MODULEX && LOWORD(wParam)<ID_MODULEX+GetMenuItemCount(menu1)) {
			CheckMenuRadioItem(menu1,0,GetMenuItemCount(menu1),LOWORD(wParam)-ID_MODULEX,MF_BYPOSITION);
			return TRUE;
		}
        switch (LOWORD(wParam)) {
        case IDCANCEL:
			g_stop=true;
			if (g_running) {
				PostMessage(hwndDlg,WM_COMMAND,IDCANCEL,0);
			} else {
				DestroyWindow(hwndDlg); 
				PostQuitMessage(0);
			}
            return TRUE;
		case IDC_DEPTH:
			//CheckDlgButton(hwndDlg,IDC_DEPTH,BST_CHECKED);
			//CheckDlgButton(hwndDlg,IDC_LABELMAP,BST_UNCHECKED);
			return TRUE;
		case IDC_IGDGM:
			//CheckDlgButton(hwndDlg,IDC_DEPTH,BST_UNCHECKED);
			//CheckDlgButton(hwndDlg,IDC_LABELMAP,BST_CHECKED);
			return TRUE;
		case ID_PIPELINE_SIMPLE:
			CheckMenuItem(menu,ID_PIPELINE_SIMPLE,MF_CHECKED);
			CheckMenuItem(menu,ID_PIPELINE_ADVANCED,MF_UNCHECKED);
			return TRUE;
		case ID_PIPELINE_ADVANCED:
			CheckMenuItem(menu,ID_PIPELINE_SIMPLE,MF_UNCHECKED);
			CheckMenuItem(menu,ID_PIPELINE_ADVANCED,MF_CHECKED);
			return TRUE;
		case ID_START:
			Button_Enable(GetDlgItem(hwndDlg,ID_START),false);
			Button_Enable(GetDlgItem(hwndDlg,ID_STOP),true);
			for (int i=0;i<GetMenuItemCount(menu);i++)
				EnableMenuItem(menu,i,MF_BYPOSITION|MF_GRAYED);
			DrawMenuBar(hwndDlg);
			g_stop=false;
			g_running=true;
			CreateThread(0,0,ThreadProc,hwndDlg,0,0);
			Sleep(0);
			return TRUE;
		case ID_STOP:
			g_stop=true;
			if (g_running) {
				PostMessage(hwndDlg,WM_COMMAND,ID_STOP,0);
			} else {
				for (int i=0;i<GetMenuItemCount(menu);i++)
					EnableMenuItem(menu,i,MF_BYPOSITION|MF_ENABLED);
				DrawMenuBar(hwndDlg);
				Button_Enable(GetDlgItem(hwndDlg,ID_START),true);
				Button_Enable(GetDlgItem(hwndDlg,ID_STOP),false);
			}
			return TRUE;
		case ID_MODE_LIVE:
			CheckMenuItem(menu,ID_MODE_LIVE,MF_CHECKED);
			CheckMenuItem(menu,ID_MODE_PLAYBACK,MF_UNCHECKED);
			CheckMenuItem(menu,ID_MODE_RECORD,MF_UNCHECKED);
			return TRUE;
		case ID_MODE_PLAYBACK:
			CheckMenuItem(menu,ID_MODE_LIVE,MF_UNCHECKED);
			CheckMenuItem(menu,ID_MODE_PLAYBACK,MF_CHECKED);
			CheckMenuItem(menu,ID_MODE_RECORD,MF_UNCHECKED);
			return TRUE;
		case ID_MODE_RECORD:
			CheckMenuItem(menu,ID_MODE_LIVE,MF_UNCHECKED);
			CheckMenuItem(menu,ID_MODE_PLAYBACK,MF_UNCHECKED);
			CheckMenuItem(menu,ID_MODE_RECORD,MF_CHECKED);
			return TRUE;
        } 
		break;
	case WM_TIMER:
		SendMessage(GetDlgItem(hwndDlg,wParam),STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)g_none);
		KillTimer(hwndDlg,wParam);
		return TRUE;
	case WM_SIZE:
		RedoLayout(hwndDlg);
		return TRUE;
    } 
    return FALSE; 
} 

#pragma warning(disable:4706) /* assignment within conditional */
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int) {

	SetCursorPos(683,384);
	InitCommonControls();
	g_hInst=hInstance;

	pxcStatus sts=PXCSession_Create(&g_session);
	if (sts<PXC_STATUS_NO_ERROR) {
        MessageBoxW(0,L"Failed to create an SDK session",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
        return 1;
    }

    HWND hWnd=CreateDialogW(hInstance,MAKEINTRESOURCE(IDD_MAINFRAME),0,DialogProc);
    if (!hWnd)  {
        MessageBoxW(0,L"Failed to create a window",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
        return 1;
    }


	HWND hWnd2=CreateStatusWindow(WS_CHILD|WS_VISIBLE,L"OK",hWnd,IDC_STATUS);
	if (!hWnd2) {
        MessageBoxW(0,L"Failed to create a status bar",L"Gesture Viewer",MB_ICONEXCLAMATION|MB_OK);
        return 1;
	}

	UpdateWindow(hWnd);

	/*char *lpbuffer=new char[200];
	strcpy_s(lpbuffer,200,"test\0");
	SendMessage(hWnd,LB_ADDSTRING,NULL,(LPARAM)lpbuffer);
	delete [] lpbuffer;*/


    MSG msg;
	for (int sts;(sts=GetMessageW(&msg,hWnd,0,0));) {
        if (sts == -1) return sts;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	g_stop=true;
	while (g_running) Sleep(5);
	g_session->Release();
    return (int)msg.wParam;

	return 0;
}

pxcCHAR* GetPlaybackFile(void) {
	static pxcCHAR file[1024]={0};

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.lpstrFilter=L"All Files\0*.*\0\0";
	ofn.lpstrFile=file; file[0]=0;
	ofn.nMaxFile=sizeof(file)/sizeof(pxcCHAR);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	if (!GetOpenFileName(&ofn)) return 0;
	return file;
}

pxcCHAR* GetRecordFile(void) {
	static pxcCHAR file[1024]={0};

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.lpstrFilter=L"All Files\0*.*\0\0";
	ofn.lpstrFile=file; file[0]=0;
	ofn.nMaxFile=sizeof(file)/sizeof(pxcCHAR);
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	if (!GetSaveFileName(&ofn)) return 0;
	return file;
}

