#include<iostream>
#include<vector>
#include<windows.h>
#include<cstdlib>
using namespace std;

long long int Score = 0;


const int ROAD_WIDTH = 10;
const int ENTITY_SIZE = 3;
const char GRASS_CHAR = '^';
const char PLAYER_CHAR = '#';
const char ENEMY_CHAR = 'x';
const int FRAME_DELAY_MS = 30;

class Canvas{
	public:
		int width, height;
		vector<char> buffer;
		
		Canvas(int w, int h) : width(w) , height(h) {
			buffer.resize(w*h); 
		}
		
		void clear(char c){
			fill(buffer.begin(), buffer.end(), c);
		}
		
		void setPixel(int x, int y, char c){
			if(x < 0 || x >= width || y < 0 || y >= height) return;
			buffer[y * width + x] = c;
		}
		
		char getPixel(int x, int y){
			
			return buffer[y * width + x];
		}
		
		
		void displayASCII(){
			for(int y=0; y<height; y++){
				for(int x=0; x<width; x++){
					char ch = buffer[y*width + x];
					cout  << ch ;
				}
				cout << '\n';
			}
		}
		
		
};


class Enemy{
	int ex;
	int ey;
	int ewidth;
	int eheight;
	char ch;
	public:
		
		Enemy(int x, int y) : ex(x), ey(y){
			this->ewidth = ENTITY_SIZE;
			this->eheight = ENTITY_SIZE;
			this->ch = ENEMY_CHAR;
		}
		
		int getEx() { return ex; }
		int getEy() { return ey; }
		int getWidth() { return ewidth; }
		int getHeight() { return eheight; }

		
		char getEnemyPixel(){
			return this->ch;
		}
		
		void drawEnemy(Canvas& c, int leftBoundary, int rightBoundary){
			
			// Drawing a enemy in canvas
			for(int rely = this->ey; rely < this->ey + eheight; rely++){
				for(int relx = this->ex; relx < this->ex + ewidth; relx++){
					if(rely <= c.height-1 )
						c.buffer[rely*c.width + relx] = this->ch;
				}
			}
			// Enemy Movement Logic
			this->ey++;
			if(this->ey >= c.height - 1){
				this->ex = leftBoundary + (rand() % (rightBoundary - ewidth - leftBoundary + 1));
				this->ey = 0;
				Score++;
			}
		}
	
};



class Player{
	
	public:
	
		int posx;
		int posy;
		int pwidth;
		int pheight;
		char ch;
	
		
		Player(int x, int y) : posx(x), posy(y){
			this->pwidth = ENTITY_SIZE;
			this->pheight = ENTITY_SIZE;
			this->ch = PLAYER_CHAR;
		}
		
		
		void drawPlayer(Canvas& win){
			
			// Drawing a player in canvas
			for(int rely = this->posy; rely < this->posy + pheight; rely++){
				for(int relx = this->posx; relx < this->posx + pwidth; relx++){
					if(rely <= win.height-1 )
						win.buffer[rely*win.width + relx] = this->ch;
				}
			}
			
			// Player Movement Logic
			win.setPixel(posx, posy , PLAYER_CHAR);
		
			if( win.getPixel(posx-1, posy) != GRASS_CHAR && ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('a') & 0x8000))){
				// Move Towards Left
				posx--;
				
			}
			if( win.getPixel(posx + pwidth, posy) != GRASS_CHAR && ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('d') & 0x8000))){
				// Move Towards Right
				posx++;
			}
			if( posy > 0 && ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('w') & 0x8000))){
				// Move Towards Left
				posy--;
				
			}
			if( posy < win.height-pheight && ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('s') & 0x8000))){
				// Move Towards Right
				posy++;
			}
			
			
		}
	
};

class GrassLand{
	int gx;
	int gwidth;
	char ch;
	
	public:
		GrassLand(int x, int w) : gx(x), gwidth(w){
			this->ch = GRASS_CHAR;
		}
		
		void drawGrassLand(Canvas& c){
			for(int y = 0; y < c.height; y++){
				for(int x = this->gx; x < this->gx + gwidth; x++){
					c.buffer[y*c.width + x] = ch;
				}
			}
			
		}
};



bool checkCollision(Player& p, Enemy& e) {
    return !(
        p.posx + p.pwidth  <= e.getEx() ||
        p.posx >= e.getEx() + e.getWidth() ||
        p.posy + p.pheight <= e.getEy() ||
        p.posy >= e.getEy() + e.getHeight()
    );
}


int main(){
	
	Canvas win(40, 20);
	win.displayASCII();
	
	// road boundary
	int leftBoundary = ROAD_WIDTH;
	int rightBoundary = win.width - ROAD_WIDTH;
	
	// Player Position
	Player p(19,15);
	
	
	// Enemy Setup
	Enemy e1(leftBoundary + 5, 0);
	char enemyPixel = e1.getEnemyPixel();
	
	Enemy e2(leftBoundary + 10, 0);
	
	
	// GrassLand Setup
	GrassLand lGrass(0,ROAD_WIDTH);
	GrassLand rGrass(rightBoundary, ROAD_WIDTH);
	
	
	// Difficulty level
	int difficulty = 0;
	
	
	// Game Loop
	bool running = true;
	while(running){
		
		// Clear The Screen and Fill with .
		system("cls");
		
		// Header Section
		cout << "Score : " << Score << endl; 
		
		win.clear('.');
		
		
		
		// Drawing Grass Land;
		lGrass.drawGrassLand(win);
		rGrass.drawGrassLand(win);
		
		// Draw Enemies 
		e1.drawEnemy(win, leftBoundary, rightBoundary);
		
		if(difficulty >= 50){
			e2.drawEnemy(win, leftBoundary, rightBoundary);
		}
		
		
		// Player
		p.drawPlayer(win);
		
		
		
		win.displayASCII();
		
		// Detect Collision with Enemy
		
		if (checkCollision(p, e1) || checkCollision(p, e2)) {
		    running = false;
		    break;
		}
		
		difficulty++;
		
		Sleep(FRAME_DELAY_MS);
	}	
	cout << "Game Over" << endl;
	return 0;
}