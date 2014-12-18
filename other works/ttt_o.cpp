//FOR PROJECT
//The game of TIC TAC TOE with Intelligency
#include <simplecpp>
#include <assert.h>

void call_game();
void printboard(int x_size,int y_size);  //function to print game board
int game(bool chance);  // function of actual game
int ifwin(int gamepos[][3]); // function to chek the winning condition
int computer_play(int game[3][3],int &x,int &y);//Function in which computer checks the board and plays
void animateLine(int x1,int y1,int x2,int y2); // A simple Animation
void menu();//Menu screen
bool insidebox(int inx,int iny ,int x ,int y ,int height , int width);
int x_size=600,y_size=600,gametype=1;  // size of board

int abs(int x)
{
	if (x<0) x*=-1;

	return x;
}

int main()
{
	menu();
}
void menu()
{
	initCanvas("MENU",600,600);
	beginFrame();
	Circle c(0,0,0);
	c.setColor(COLOR(255,50,255));
	c.setFill(1);
	Line l(-600,0,600,1200);
	l.setColor(COLOR("yellow"));

	for (int i=0; i<2000; i++) {
		l.move(1,0);
		l.imprint();
		l.setColor(COLOR(255*abs(sine(0.5*i)),255,255*abs(cosine(0.5*i))));
	}

	for (int x=0; x<=600; x+=50) {
		c.reset(x,0,25);
		c.imprint();
		c.reset(x,600,25);
		c.imprint();
		c.reset(0,x,25);
		c.imprint();
		c.reset(600,x,25);
		c.imprint();
	}

	Rectangle choice;
	choice.setFill(1);
	choice.setColor(COLOR(0,0,0));
	choice.reset(300,110,380,100);
	choice.imprint();
	Text t(300,110,"TIC TAC TOE");
	t.setColor(COLOR("white"));
	t.imprint();
	t.setColor(COLOR("brown"));
	string str[4]={"1. vs. Computer(EASY)","2. vs. Computer(HARD)","3. Two-player Game","4. EXIT"};
	choice.setColor(COLOR("white"));

	for (int i=240; i<=500; i+=80) {
		choice.reset(300,i,360,60);
		choice.imprint();
		t.reset(300,i,str[(i-240)/80]);
		t.imprint();
	}

	endFrame();
	int user_choice=0;
	XEvent event;

	while (1) {
		nextEvent(&event);

		if (mouseButtonPressEvent(&event)) {
			for (int i=0; i<4; i++) {
				if (insidebox(event.xbutton.x, event.xbutton.y,300,240+(i*80),360,60)) {
					user_choice=i+1;
					break;
				}
			}

			if (user_choice==4) {
				closeCanvas();
				initCanvas("THANKING SCREEN",500,200);
				Text t(250,100,"THANX FOR PLAYING");
				t.setColor(COLOR("GREEN"));
				getClick();
				closeCanvas();
				assert(false);
			} else if (user_choice==1) {
				gametype=0;
				call_game();
			} else if (user_choice==2) {
				gametype=1;
				call_game();
			} else if (user_choice==3) {
				gametype=2;
				call_game();
			}
		}
	}

	closeCanvas();
}
void call_game()
{
	closeCanvas();
	string p1,p2="COMPUTER";

	if (gametype==2) {
		cout<<"\n\n\nENTER PLAYER 1 NAME : ";
		getline(cin,p1);
		cout<<"ENTER PLAYER 2 NAME : ";
		getline(cin,p2);
	} else {
		cout<<"\n\n\nENTER YOUR NAME : ";
		getline(cin,p1);
	}

	if (gametype==1)
		cout<<"\n You Are Playing hard Game so ts hard for you to win.. \n";

	cout<<"\n\nLET's PLAY \n";
	wait(1);
	bool chance=true;
	double p1score=0,p2score=0;

	while (1) {  // loop for continous games
		initCanvas("TIC TAC TOE",600,700);
		Text pchance;
		pchance.setColor(COLOR("pink"));
		string s;

		if (chance) {
			s="*"+p1+"  :  CROSS";
			pchance.reset(150,50,s);
			pchance.imprint();
			s=p2+"  :  ZERO";
			pchance.reset(450,50,s);
			pchance.imprint();
		} else {
			s=p1+"  :  CROSS";
			pchance.reset(150,50,s);
			pchance.imprint();
			s="*"+p2+"  :  ZERO";
			pchance.reset(450,50,s);
			pchance.imprint();
		}

		pchance.setColor(COLOR("brown"));
		pchance.reset(150,80,p1score);
		pchance.imprint();
		pchance.reset(450,80,p2score);
		pchance.imprint();
		int result=game(chance);
		Rectangle r(200,600,150,60);
		r.imprint();
		r.reset(500,600,100,60);
		r.imprint();
		Text t(200,600,"Play again");
		t.setColor(COLOR("green"));
		t.imprint();
		t.reset(500,600,"EXIT");
		t.setColor(COLOR("red"));
		t.imprint();
		int user_opinion=0;

		if (!chance&&result!=-1)
			result=!result;

		if (result<0) {
			pchance.reset(300,540,"GAME TIES .." );
		} else if (result!=chance) {
			pchance.reset(300,540,p2+"  wins .." );
			p2score++;
		} else {
			pchance.reset(300,540,p1+"  wins..");
			p1score++;
		}

		wait(1);

		while (user_opinion==0) {
			int click=getClick();
			int x,y;
			x=click/65536;
			y=click%65536;

			if (x<=275&&x>=125&&y<=630&&y>=570)
				user_opinion=1;
			else if (x<=550&&x>=450&&y<=630&&y>=570)
				user_opinion=-1;
		}

		closeCanvas();
		chance=!chance;

		if (user_opinion==-1)
			break;
	}

	menu();
}
int game(bool chance)
{
	bool gameend=false;
	int gamepos[3][3];
	int no=0,result=-1;
	printboard(x_size,y_size);

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			gamepos[i][j]=-1;

	while (!gameend&&no<9) {
		if (no%2!=chance) {   //control of players
			int clickpos,i,j,x,y;

			do {
				clickpos=getClick();
				x=clickpos/65536;
				y=clickpos%65536;
				i=(x-(x_size/5))/(x_size/5);
				j=(y-(y_size/5))/(y_size/5);
			} while (!(i<3&&i>=0&&j<3&&j>=0&&x>x_size/5&&y>y_size/5&&gamepos[i][j]==-1));

			x=x_size/5;
			y=y_size/5;
			Line l1((i+1)*x+15,(j+1)*y+15,(i+2)*x-15,(j+2)*y-15)	;
			l1.setColor(COLOR("green"));
			l1.imprint();
			l1.reset((i+1)*x+15,(j+2)*y-15,(i+2)*x-15,(j+1)*y+15);
			l1.imprint();
			gamepos[i][j]=1;
			no++;
		} else {
			int clickpos,i,j,x,y;

			if (gametype!=2)
				computer_play(gamepos,i,j);
			else {
				do {
					clickpos=getClick();
					x=clickpos/65536;
					y=clickpos%65536;
					i=(x-(x_size/5))/(x_size/5);
					j=(y-(y_size/5))/(y_size/5);
				} while (!(i<3&&i>=0&&j<3&&j>=0&&x>x_size/5&&y>y_size/5&&gamepos[i][j]==-1));
			}

			Circle c(((i+1)*(x_size/5))+(x_size/5)/2,((j+1)*(y_size/5))+(y_size/5)/2,(x_size/5)/2-15);
			c.setColor(COLOR("blue"));
			c.imprint();
			gamepos[i][j]=0;
			no++;
		}

		result=-1;
		result=ifwin(gamepos);

		if (result>=0)
			gameend=true;
	}

	if (!gameend)
		return -1;
	else
		return result;
}

void printboard(int x_size,int y_size)
{
	beginFrame();
	int x=x_size/5;
	int y=y_size/5;
	Line l1,l2;
	l1.reset(x,y,4*x,y);
	l2.reset(x,y,x,4*y);
	l1.setColor(COLOR("red"));
	l2.setColor(COLOR("red"));

	for (int i=0; i<4; i++) {
		l1.imprint();
		l2.imprint();
		l1.move(0,y);
		l2.move(x,0);
	}

	endFrame();
}

int ifwin(int a[][3])
{
	int win=-1;

	for (int i=0; i<3; i++) {
		if (a[i][0]==a[i][1]&&a[i][0]==a[i][2]) {  // Horizontal line checked
			win=a[i][0];

			if (win!=-1) {
				animateLine((2*(i+1)+1)*x_size/10,y_size/5,(2*(i+1)+1)*x_size/10,4*y_size/5);
				return win;
			}
		}

		if (a[0][i]==a[1][i]&&a[0][i]==a[2][i]) {   // Vertical lines checked
			win=a[0][i];

			if (win!=-1) {
				animateLine(x_size/5,(2*(i+1)+1)*y_size/10,4*x_size/5,(2*(i+1)+1)*y_size/10);
				return win;
			}
		}
	}

	if (a[0][0]==a[1][1]&&a[1][1]==a[2][2]) {   // Main diagonal checked
		win=a[0][0];

		if (win!=-1) {
			animateLine(x_size/5,y_size/5,4*x_size/5,4*y_size/5);
			return win;
		}
	}

	if (a[0][2]==a[1][1]&&a[1][1]==a[2][0]) {   // The other diagonal is checked
		win=a[0][2];

		if (win!=-1) {
			animateLine(4*x_size/5,y_size/5,x_size/5,4*y_size/5);
			return win;
		}
	}

	return win;
}

void animateLine(int x1,int y1,int x2,int y2)
{
	Line li(x1,y1,x1,y1);

	for (int k=x1,l=y1; abs(x2-k)>0||abs(y2-l>0);) {
		usleep(10000);
		li.reset(x1,y1,k,l);

		if (abs(x2-k)>0) k+=((x2-k)/abs(x2-k));

		if (abs(y2-l)>0) l+=((y2-l)/abs(y2-l));
	}

	li.imprint();
}

int computer_play(int game[3][3],int &x ,int &y)
{
	// THE FIRST LOOP CHECKS THE INITIAL CONDITION OF OBIVIOUS
	for (int mycheck=0; mycheck<2; mycheck++) {
		for (int i=0; i<3; i++) {
			for (int j=0; j<3; j++) {
				if ((game[i][j]==game[i][(j+1)%3])&&game[i][(j+2)%3]==-1&&game[i][j]==mycheck) {
					game[i][(j+2)%3]=0;
					x=i;
					y=(j+2)%3;
					return 0;
				}

				if ((game[j][i]==game[(j+1)%3][i])&&game[j][i]==mycheck&&game[(j+2)%3][i]==-1) {
					game[(j+2)%3][i]=0;
					x=(j+2)%3;
					y=i;
					return 0;
				}
			}

			if (game[i][i]==game[(i+1)%3][(i+1)%3]&&game[i][i]==mycheck&&game[(i+2)%3][(i+2)%3]==-1) {
				game[(i+2)%3][(i+2)%3]=0;
				x=(i+2)%3;
				y=(i+2)%3;
				return 0;
			}

			if (game[i][2-i]==game[(i+1)%3][2-((i+1)%3)]&&game[i][2-i]==mycheck&&game[(i+2)%3][2-((i+2)%3)]==-1) {
				game[(i+2)%3][2-((i+2)%3)]=0;
				x=(i+2)%3;
				y=2-((i+2)%3);
				return 0;
			}
		}
	}

	// HERE STARTS THE CODE CONTAINING THE TRICKS FOR GAME
	if (gametype==1) {
		int count=0,fieldpos[9][2];

		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				if (game[i][j]!=-1) {
					fieldpos[count][0]=i;
					fieldpos[count][1]=j;
					count++;
				}

		// COUNT IS NO OF BOXES IN THE ARENA FILLED
		if (count==0) {
			x=2;
			y=2;
			game[2][2]=0;
			return 0;
		}

		if (count==1) {
			if (game[1][1]==-1) {
				x=1;
				y=1;
				game[1][1]=0;
				return 0;
			} else {
				x=0;
				y=2;
				game[0][2]=0;
				return 0;
			}
		}

		if (count==2) {
			if ((fieldpos[0][0]==0&&fieldpos[0][1]==0)||(fieldpos[0][0]==0&&fieldpos[0][1]==1)||(fieldpos[0][0]==2&&fieldpos[0][1]==0)||(fieldpos[0][0]==2&&fieldpos[0][1]==1)) {
				x=0;
				y=2;
				game[0][2]=0;
				return 0;
			}

			if ((fieldpos[0][0]==1&&fieldpos[0][1]==0)||(fieldpos[0][0]==0&&fieldpos[0][1]==2)||(fieldpos[0][0]==1&&fieldpos[0][1]==2)) {
				x=2;
				y=0;
				game[2][0]=0;
				return 0;
			}
		}

		if (count==3) {
			if (game[0][0]==1&&game[2][1]==1) {
				x=2;
				y=0;
				game[2][0]=0;
				return 0;
			}

			if ((game[0][0]==1&&game[2][2]==1)||(game[2][0]==1&&game[0][2]==1)) {
				x=1;
				y=2;
				game[1][2]=0;
				return 0;
			}

			if ((game[1][2]==1&&game[2][1]==1)||(game[2][0]==1&&game[1][2]==1)) {
				x=2;
				y=2;
				game[2][2]=0;
				return 0;
			}
		}

		if (count==4) {
			if (game[2][2]==0&&game[0][2]==0&&game[0][1]==1) {
				x=2;
				y=0;
				game[2][0]=0;
				return 0;
			}

			if (game[2][2]==0&&game[2][0]==0&&game[1][0]==1) {
				x=0;
				y=2;
				game[0][2]=0;
				return 0;
			}

			if (game[2][2]==0&&game[0][2]==0&&game[0][0]==1) {
				x=2;
				y=0;
				game[2][0]=0;
				return 0;
			}
		}
	}

	// END OF HARD GAME
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			if (game[j][i]==-1) {
				game[j][i]=0;
				x=j;
				y=i;
				return 0;
			}
		}
	}

	return 0;
}
bool insidebox(int inx,int iny ,int x ,int y ,int height , int width)
{
	if ((inx<=(x+height/2))&&(inx>=(x-height/2))&&(iny<=(y+width/2))&&(iny>=(y-width/2))) {
		return true;
	} else return false;
}
