#include<iostream>
#include<vector>
#include<windows.h>
using namespace std;


struct Color{
	unsigned char r, g, b;
};

class Canvas{
	public:
		int width, height;
		vector<Color> buffer;
		
		Canvas(int w, int h) : width(w) , height(h) {
			buffer.resize(w*h); 
		}
		
		void clear(Color c){
			fill(buffer.begin(), buffer.end(), c);
		}
		
		void setPixel(int x, int y, Color c){
			if(x < 0 || x > width || y < 0 || y > height) return;
			buffer[y * width + x] = c;
		}
		
		Color getPixel(int x, int y){
			
			return buffer[y * width + x];
		}
		
		
		void displayASCII(){
			for(int y=0; y<height; y++){
				for(int x=0; x<width; x++){
					Color c = buffer[y*width + x];
					char ch;
					if((c.r + c.g + c.b) > 128){
						ch = '#';
					}else if((c.r + c.g + c.b) == 3){
						ch = 'x';
					}else{
						ch = '.';
					}
					cout  << ch ;
				}
				cout << '\n';
			}
		}
		
		
};


int main(){
	
	Canvas win(20, 20);
	win.displayASCII();
	
	int posx = 10, posy = 10;
	short enemyDir = 1;
	int i = 0;
	
	// Game Loop
	bool running = true;
	while(running){
		
		// Clear The Screen and Fill with .
		system("cls");
		win.clear({0,0,0});
		
		//Draw Enemies 
		win.setPixel(i,5, {1,1,1});
		win.setPixel(i,15, {1,1,1});
		win.setPixel(13,i, {1,1,1});
		
		if( i >= win.width-1){
			enemyDir = -1;
		}
		if( i <= 0){
			enemyDir = 1;
		}
		
		i += enemyDir;
		
		
		//Player Movement
		win.setPixel(posx, posy , {128,128,128});
		
		if( posx > 0 && (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000)){
			// Move Towards Left
			posx--;
			
		}
		if( posx < win.width-1 && (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000)){
			// Move Towards Right
			posx++;
		}
		if( posy > 0 && (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('w') & 0x8000)){
			// Move Towards Left
			posy--;
			
		}
		if( posy < win.height-1 && (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000)){
			// Move Towards Right
			posy++;
		}
		
		
		win.displayASCII();
		
		// Detect Collision with Enemy
		if( (posx == i && posy == 5) || (posx == i && posy == 15) || (posx == 13 && posy == i)){
			running = false;
			break;
		}
		
		Sleep(30);
	}	
	cout << "Game Over" << endl;
	return 0;
}