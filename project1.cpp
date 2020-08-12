#define UNICODE
#include<bits/stdc++.h>
using namespace std;

#include<Windows.h>
wstring tetromino[7];

// setting the playing area
int nFieldWidth=30;
int nFieldHeight=15;
unsigned char *pField= NULL;

//initialize the screen
int nScreenWidth = 80;
int nScreenHeight = 30;

// ROTATE function
int Rotate(int px,int py,int r){
 switch(r%4){
 	case 0: return((py * 4) + px);       //0   degree
 	case 1: return(12 + py) - (px*4);  //90  degree
 	case 2: return(15- (py * 4 )- px); //270 degree
 	case 3: return(3- py + (px*4));    //360 degree
  }	
  return 0;
}

bool DoesPieceFit(int nTetromino,int nRotation,int nPosx,int nPosy){
	for(int px=0;px<4;px++){
		for(int py=0;py<4;py++){
			//index into piece
			int pi=Rotate(px,py,nRotation);
			//get into field
			int fi=(nPosy+py)*nFieldWidth+(nPosx+px);
			if((nPosx+px)>=0 && (nPosx+px)<nFieldWidth){
			  if((nPosy+py)>=0 && (nPosy+py)<nFieldHeight){
				if(tetromino[nTetromino][pi] == L'X' && pField[fi]!=0){
					return false;//hit
				}
			  }	
			}
		}
	}
	return true;
}
int main(){
	//create
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	
	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");
	
	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

    tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");
		
	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");
		
	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");
	
	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");
	
	pField = new unsigned char[nFieldWidth*nFieldHeight];
	for(int x=0 ; x<nFieldWidth ; x++){
		for(int y=0 ; y<nFieldHeight ; y++){
			pField[(y*nFieldWidth) + x] = ((x==0) ||(x==nFieldWidth-1) || (y==nFieldHeight-1)) ? 9:0;
		}
	}	
	// now we are making the scrren
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for(int i=0 ; i<nScreenWidth*nScreenHeight ; i++){
		screen[i]=L' ';
	}
	HANDLE hConsole =CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0 ;
	
	bool bGameOver = false;
	
	bool bkey[4];
	
	int nCurrentPiece=1;
	int nCurrentRotation=0;
	int nCurrentX  =  (nFieldWidth)/2;
	int nCurrentY =  0;
	
	
	while(!bGameOver){
		//game timing
		Sleep(50);
		
		//input
		for(int k=0;k<4;k++){                                    //R   L   D Z
			bkey[k] = (0X8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k])))!=0;
		}	
	
		//game logic
		nCurrentX-= (bkey[1] && DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX-1,nCurrentY)) ? 1:0;
		nCurrentX+= (bkey[0] && DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX+1,nCurrentY)) ? 1:0;
		nCurrentY+= (bkey[2] && DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY+1)) ? 1:0;
		nCurrentRotation+= (bkey[3] && DoesPieceFit(nCurrentPiece,nCurrentRotation+1,nCurrentX,nCurrentY)) ? 1:0;
		//nCurrentRotation+= (bkey[3] && DoesPieceFit(nCurrentPiece,nCurrentRotation+1,nCurrentX,nCurrentY)) ? 1:0;
	
		//draw field
	    for(int x=0 ; x<nFieldWidth ; x++){
		for(int y=0 ; y<nFieldHeight ; y++){
    	screen[((y+2)*nScreenWidth )+(x+2)]= L" ABCDEFG=#"[pField[(y*nFieldWidth) + x]];
		  }
	    }
	  
	  //draw current piece
	  for(int px=0;px<4;px++){
	  	for(int py=0;py<4;py++){
	  		if(tetromino[nCurrentPiece][Rotate(px,py,nCurrentRotation)]==L'X'){
	  			screen[(nCurrentY+py+2)*nScreenWidth + (nCurrentX+px+2)]=nCurrentPiece+65;
			  }
		  }
	  }
	  
	  //display frame
	WriteConsoleOutputCharacter(hConsole, screen,nScreenWidth*nScreenHeight ,{0,0},&dwBytesWritten);
   }
	return 0;
}
