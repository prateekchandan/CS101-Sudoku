
#include <simplecpp>
#include <string.h>

void sudoku(); // The sudoku function

int main()
{
	sudoku();
}




//*********BEGINNING OF SUDOKU****************//
struct box {
	int real;
	bool val[9];
	int count;    // keeps track of how many possibilities are cancelled
	box() {
		real=0;
		int i;

		for (i=0; i<9; i++)
			val[i]=true;

		count=0;
	}
};

bool solve(box a[9][9],int mode=1 );   // solves a sudoku
bool logic2(box a[9][9]);   // applies logic two which fills a box even it is not completely cut
bool logic3(box a[9][9]); // applies a stronger logic to solve sudoku
bool logic4(box a[9][9]);  // Applies different testcases for solving sudoku
bool check_cut(box a[9][9],int set,int pos[][2],int count);
void passinto(box a[9][9],int i , int j);  // places the real value ad cuts all vertical and horizontal possibilities
void generate(box a[9][9],int difficulty);    // generates a sudoku of different difficulties
void randomize(box a[9][9]); // creates a random answer
bool checkvalid(box a[9][9]);  // checks a sudoku is valid or not
int menu();  // This functions calls the menu

// THESE ARE THE SETS OF FUNCTIONS WHICH ARE USED TO SOLVE THE SUDOKU //

// THE FIRST FUNCTION : It passes a value in to sudoku array and removes the possibility of a being a number in its row column and the subsequent box
void passinto(box a[9][9],int i , int j)
{
	a[i][j].count=9;
	int l,m;

	for (l=0,m=j; l<9; l++) {
		bool check;

		if (l!=i) {
			check=a[l][m].val[a[i][j].real-1];

			if (a[l][m].real==0&&check) {
				a[l][m].val[a[i][j].real-1]=false;
				a[l][m].count++;
			}
		}
	}

	for (l=i,m=0; m<9; m++) {
		bool check;

		if (m!=j) {
			check=a[l][m].val[a[i][j].real-1];

			if (a[l][m].real==0&&check) {
				a[l][m].val[a[i][j].real-1]=false;
				a[l][m].count++;
			}
		}
	}

	m=(j/3)*3;
	repeat(3) {
		l=(i/3)*3;
		repeat(3) {
			if (l!=i||m!=i) {
				bool check;
				check=a[l][m].val[a[i][j].real-1];

				if (a[l][m].real==0&&check) {
					a[l][m].val[a[i][j].real-1]=false;
					a[l][m].count++;
				}
			}

			l++;
		}
		m++;
	}
}

// SECOND FUNCTION : This applies a Second and a bit coplex algorithm to solve the sudoku
bool logic2(box a[9][9])
{
	bool solved=false;
	int count=0,pos[2]={0,0};

	for (int n=0; n<9; n++) {
		for (int i=0; i<9; i++) {
			count=0;
			pos[0]=0;
			pos[1]=0;

			for (int j=0; j<9; j++) {
				if (a[i][j].val[n]&&a[i][j].count<9) {
					count++;
					pos[0]=i;
					pos[1]=j;
				}
			}

			if (count==1) {
				a[pos[0]][pos[1]].real=n+1;
				a[pos[0]][pos[1]].count=9;
				passinto(a,pos[0],pos[1]);
				solved=true;
			}

			count=0;
			pos[0]=0;
			pos[1]=0;

			for (int j=0; j<9; j++) {
				if (a[j][i].val[n]&&a[j][i].count<9) {
					count++;
					pos[0]=j;
					pos[1]=i;
				}
			}

			if (count==1) {
				a[pos[0]][pos[1]].real=n+1;
				a[pos[0]][pos[1]].count=9;
				passinto(a,pos[0],pos[1]);
				solved=true;
			}

			count=0;
			pos[0]=0;
			pos[1]=0;
			int l=(i/3)*3;
			int m=(i%3)*3;
			repeat(3) {
				m=(i%3)*3;
				repeat(3) {
					if (a[l][m].val[n]&&a[l][m].count<9) {
						count++;
						pos[0]=l;
						pos[1]=m;
					}

					m++;
				}
				l++;
			}

			if (count==1) {
				a[pos[0]][pos[1]].real=n+1;
				a[pos[0]][pos[1]].count=9;
				passinto(a,pos[0],pos[1]);
				solved=true;
			}
		}
	}

	return solved;
}
// Third Function: APPLYING THIRD LOGIC
bool logic3(box a[9][9])
{
	bool solved=false;
	int count,pos[9][2];

	for (int i=0; i<9; i++) {
		count=0;

		for (int j=0; j<9; j++) {
			if (a[i][j].real==0&&a[i][j].count<9) {
				pos[count][0]=i;
				pos[count][1]=j;
				count++;
			}
		}

		for (int set=2; set<3; set++)
			solved=check_cut(a,set,pos,count) or solved;

		count=0;

		for (int j=0; j<9; j++) {
			if (a[j][i].real==0&&a[j][i].count<9) {
				pos[count][0]=j;
				pos[count][1]=i;
				count++;
			}
		}

		for (int set=2; set<3; set++)
			solved=check_cut(a,set,pos,count)||solved;

		count=0;
		int l=(i/3)*3;
		int m=(i%3)*3;
		repeat(3) {
			m=(i%3)*3;
			repeat(3) {
				if (a[l][m].real==0&&a[l][m].count<9) {
					pos[count][0]=l;
					pos[count][1]=m;
					count++;
				}

				m++;
			}
			l++;
		}

		for (int set=2; set<3; set++)
			solved=check_cut(a,set,pos,count)||solved;
	}

	return solved;
}
// FOURTH : A  function use to help third logic
bool check_cut(box a[9][9],int set,int pos[][2],int count)
{
	for (int i=0; i<count; i++) {
		for (int j=i+1; j<count; j++) {
			int uni[9],unicount=0;
			int x1=pos[i][0],y1=pos[i][1],x2=pos[j][0],y2=pos[j][1];

			for (int n=0; n<9; n++) {
				if (a[x1][y1].val[n]) {
					uni[unicount]=n;
					unicount++;
				}
			}

			for (int n=0; n<9; n++) {
				if (a[x2][y2].val[n]) {
					bool check=true;

					for (int m=0; m<unicount; m++) {
						if (uni[m]==n)
							check=false;
					}

					if (check) {
						uni[unicount]=n;
						unicount++;
					}
				}
			}

			if (unicount==set) {
				for (int n=0; n<count; n++) {
					int x=pos[n][0],y=pos[n][1];

					if (n!=i&&n!=j) {
						for (int m=0; m<unicount; m++) {
							if (a[x][y].val[uni[m]]) {
								a[x][y].val[uni[m]]=false;
								a[x][y].count++;
							}
						}
					} else {
						for (int m=0; m<9; m++) {
							bool check=false;

							for (int k=0; k<unicount; k++) {
								check=check||(uni[k]==m);
							}

							if (!check) {
								if (a[x][y].val[m]) {
									a[x][y].val[m]=false;
									a[x][y].count++;
								}
							}
						}
					}
				}

				return true;
			}
		}
	}

	return false;
}
//5 : Applying fourth logic
bool logic4(box a[9][9])
{
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (a[i][j].count==7&&a[i][j].real==0) {
				box b[9][9];

				for (int l=0; l<9; l++)
					for (int m=0; m<9; m++)
						b[l][m]=a[l][m];

				for (int l=0; l<9; l++) {
					if (a[i][j].val[l]) {
						b[i][j].real=l+1;

						if (solve(b)) {
							for (int k=0; k<9; k++)
								for (int m=0; m<9; m++)
									a[k][m]=b[k][m];

							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// sixth function : checks validity of a sudoku
bool checkvalid(box a[9][9])
{
	bool check=true;

	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			int l,m;

			if (a[i][j].real!=0) {
				for (l=0,m=j; l<9; l++)
					if (l!=i)
						check=check&&(a[i][j].real!=a[l][m].real);

				for (l=i,m=0; m<9; m++)
					if (m!=j)
						check=check&&(a[i][j].real!=a[l][m].real);

				m=(j/3)*3;
				repeat(3) {
					l=(i/3)*3;
					repeat(3) {
						if (l!=i&&m!=j)
							check=check&&(a[i][j].real!=a[l][m].real);

						l++;
					}
					m++;
				}
			}
		}
	}

	return check;
}
// seventh function : Checks whether a sudoku is solved or not
bool issolved(box a[9][9])
{
	bool check=true;

	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			check=check&&(a[i][j].real!=0);

	return check;
}

// Eigth and main funtion : This Function Solves The sudoku and returnd true if the sudoku is actually solved
bool solve(box a[9][9],int mode)
{
	int i,j;

	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {
			if (a[i][j].real!=0) {
				passinto(a,i,j);
			}
		}
	}

	// The loop is taking only 81 steps coz the logics are so that it should fill atleast one box in step
	repeat(81) {
		bool solved=false;

		for (i=0; i<9; i++) {
			for (j=0; j<9; j++) {
				if (a[i][j].count==8) {
					int k=0;

					for (k=0; k<9; k++)
						if (a[i][j].val[k]==true)
							break;

					k++;
					a[i][j].real=k;
					a[i][j].count=9;
					passinto(a,i,j);
					solved=true;
				}
			}
		}

		if (!solved) {
			if (issolved(a)) {
				return true;
			} else {
				// if a simple logic doesnt work then this logic is used
				solved=logic2(a);

				if (!solved&&checkvalid(a)) {
					solved=logic3(a);
				}

				if (!issolved(a)&&mode==0) {
					logic4(a);
				}
			}
		}
	}
	return false;
}

// *********** END OF THE SOLVING PART ********************//

// ********************* GEANERATING SUDOKU FUNCTION ************************************** //
// This function Is used Generate The sudoku
void generate(box a[9][9],int difficulty)
{ // Assumption that box a[9]9] contains a correct solution
	randomize(a);
	srand(time(NULL));
	int remove=0,maxremove;
	bool check=true,r[9][9];

	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			r[i][j]=true;

	switch (difficulty) {
	case(1):
		maxremove=28;
		break;
	case(2):
		maxremove=36;
		break;
	case(3):
		maxremove=44;
		break;
	default:
		maxremove=81;
		break;
	}

	int time1=time(NULL);

	while (check&&remove<maxremove) {
		box b[9][9];

		for (int i=0; i<9; i++)
			for (int j=0; j<9; j++)
				b[i][j]=a[i][j];

		int i=randuv(0.0,8.99);
		int j=randuv(0.0,8.99);

		if (r[i][j]) {
			b[i][j].real=0;
			b[i][j].count=0;
			r[i][j]=false;

			if (solve(b)) {
				a[i][j].real=0;
				a[i][j].count=0;
				remove++;
			} else {
				bool valid=false;

				for (int x=0; x<9; x++)
					for (int y=0; y<9; y++)
						valid=valid||r[x][y];

				check=valid;
			}
		}

		int time2=time(NULL);

		if (time2>time1+3)
			check=false;
	}
}

//*************** METHOD TO GENERATE A RANDOM ANSWER **********************//

//These are the the different operations used on a fixed answer
//function for image along middle row
void hor_image(int sudoku[9][9])
{
	for (int i=0; i<=3; i++) {
		for (int j=0; j<9; j++) {
			int temp=sudoku[i][j];
			sudoku[i][j]=sudoku[8-i][j];
			sudoku[8-i][j]=temp;
		}
	}
}

//function for image along middle column
void ver_image(int sudoku[][9])
{
	for (int i=0; i<9; i++) {
		for (int j=0; j<=3; j++) {
			int temp=sudoku[i][j];
			sudoku[i][j]=sudoku[i][8-j];
			sudoku[i][8-j]=temp;
		}
	}
}

//function for anti-clockwise rotation by 90
void left(int sudoku[][9])
{
	for (int i=0; i<4; i++) {
		for (int j=i; j<8-i; j++) {
			int temp=sudoku[8-j][i];
			sudoku[8-j][i]=sudoku[i][j];
			sudoku[i][j]=sudoku[j][8-i];
			sudoku[j][8-i]=sudoku[8-i][8-j];
			sudoku[8-i][8-j]=temp;
		}
	}
}

//to rotate the matrix by 180 degrees
void rotate_180(int sudoku[][9])
{
	left(sudoku);
	left(sudoku);
}

//to swap along principal diagonal
void swap_diag1(int sudoku[][9])
{
	for (int i=1; i<9; i++) {
		for (int j=0; j<i; j++) {
			int temp=sudoku[i][j];
			sudoku[i][j]=sudoku[j][i];
			sudoku[j][i]=temp;
		}
	}
}

//to swap along the other diagonal
void swap_diag2(int sudoku[][9])
{
	for (int i=0; i<8; i++) {
		for (int j=0; j<8-i; j++) {
			int temp=sudoku[i][j];
			sudoku[i][j]=sudoku[8-j][8-i];
			sudoku[8-j][8-i]=temp;
		}
	}
}

//to swap horizontal strips i,j<(0,1,2)
void hor_swap(int sudoku[][9], int i, int j)
{
	for (int m=0; m<3; m++) {
		for (int n=0; n<9; n++) {
			int temp=sudoku[3*i+m][n];
			sudoku[3*i+m][n]=sudoku[3*j+m][n];
			sudoku[3*j+m][n]=temp;
		}
	}
}

//to swap vertical strips i,j<(0,1,2)
void ver_swap(int sudoku[][9], int i, int j)
{
	for (int m=0; m<3; m++) {
		for (int n=0; n<9; n++) {
			int temp=sudoku[n][3*i+m];
			sudoku[n][3*i+m]=sudoku[n][3*j+m];
			sudoku[n][3*j+m]=temp;
		}
	}
}

// This function creates so many answers fron one fixed answer by using different symmetry operations
void randomize(box a[9][9])
{
	srand(time(NULL));
	int sudoku[9][9]={{7,3,4,6,8,2,1,5,9},{2,5,9,7,4,1,8,6,3},{1,6,8,9,5,3,2,4,7},{5,9,2,4,3,6,7,1,8},{3,4,1,8,2,7,6,9,5},{8,7,6,5,1,9,3,2,4},{4,1,3,2,9,8,5,7,6},{6,8,5,1,7,4,9,3,2},{9,2,7,3,6,5,4,8,1}};

	for (int i=0; i<50; i++) {
		int rd=int(randuv(0.0,8.99)), rd1=int(randuv(0.0,2.99)),rd2=int(randuv(0.0,2.99));

		switch (rd) {
		case 1:
			hor_image(sudoku);
			break;
		case 2:
			ver_image(sudoku);
			break;
		case 3:
			left(sudoku);
			break;
		case 4:
			rotate_180(sudoku);
			break;
		case 5:
			swap_diag1(sudoku);
			break;
		case 6:
			swap_diag2(sudoku);
			break;
		case 7:
			hor_swap(sudoku,rd2,rd1);
			break;
		case 8:
			ver_swap(sudoku, rd2, rd1);
			break;
		}
	}

	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			a[i][j].real=sudoku[i][j];
		}
	}
}
// **************  END Of GENERATION PART OF SUDOKU **********//

// HERE STARTS THE PROGRAMS FOR THE GUI  ************* //

void print_grid();// Prints the sudoku Grid
void print_all_box(int command=1);  // Prints the buttons and the numbers part onthe interface
void printinbox(box a[9][9],bool q[9][9]); // This functions Prints the sudoku on the screen
int seeEvent(int x,int y);   // Checks the event happening and returns the appropriate command
void setString(string s[]);  // This function just sets different strins for the messages
void setquest(box a[9][9],bool q[9][9]);
// BEGINNING OF MAIN SUDOKU FUNCTION
void sudoku()
{
	srand(time(NULL));

	while (1) {
		int menu_command=menu();
		closeCanvas();

		if (menu_command==0)
			break;

		box a[9][9];
		string str[45];
		setString(str);
		initCanvas("SUDOKU",1000,700);
		beginFrame();
		print_grid();
		print_all_box(menu_command);
		endFrame();
		XEvent event;
		Text t_min(0,0,""),t_sec(0,0,"");
		Text message;

		if (menu_command>=1&&menu_command<=4) {
			message.reset(600,660,"PLEASE WAIT !! WHILE SUDOKU GENERATES ");
			generate(a,menu_command);
		}

		bool quest[9][9];
		setquest(a,quest);
		message.reset(600,660,"ENJOY THE MIGHTY SUDOKU !! ");
		printinbox(a,quest);
		double t1=time(NULL);

		while (1) {
			double t2=time(NULL);
			t2-=t1;
			int min=(int)t2/60,sec=(int)t2%60;

			if (menu_command!=10) {
				t_min.reset(95,50,double(min));
				t_sec.reset(95,76,double(sec));
			}

			if (checkEvent(&event)) {
				if (mouseButtonPressEvent(&event)) {
					int command=seeEvent(event.xbutton.x,event.xbutton.y);

					if (command==400)
						break;

					if (command>=0&&command<81) {
						int x=command%9,y=command/9;
						int past=a[x][y].real;

						if (quest[x][y])
							continue;

						Rectangle rect(300+y*50,175+x*50,50,50),r(852,220,100,40);
						rect.setColor(COLOR(255,0,0));
						Text clear(852,220,"CLEAR");
						bool check=true;
						XEvent event1;

						while (check) {
							nextEvent(&event1);

							if (keyPressEvent(&event1)) {
								char c = charFromEvent(&event1);

								if (c>=48&&c<58) {
									check=false;
									a[x][y].real=int(c-'0');
								}

								if (a[x][y].real==0||past!=0) {
									beginFrame();
									print_grid();
									print_all_box(menu_command);
									printinbox(a,quest);
									endFrame();
								}
							}

							if (mouseButtonPressEvent(&event1)) {
								check=false;
								int comm=seeEvent(event1.xbutton.x,event1.xbutton.y);

								if (comm>=100&&comm<110) {
									a[x][y].real=comm-100;

									if (a[x][y].real==0||past!=0) {
										beginFrame();
										print_grid();
										print_all_box();
										endFrame();
									}
								}
							}
						}

						if (issolved(a)) {
							printinbox(a,quest);
							string cong;
							cong="CONGRATS! YOU SOLVED IN      min and      sec";
							message.reset(600,660,cong);
							message.imprint();
							message.reset(690,660,double(min));
							message.imprint();
							message.reset(820,660,double(sec));
							message.imprint();
							t_min.imprint();
							t_min.hide();
							t_sec.imprint();
							t_sec.hide();
							getClick();
							break;
						} else if (checkvalid(a)&&past!=a[x][y].real&&menu_command!=10)
							message.reset(600,660,str[int(randuv(0.0,25.99))]);
						else if (checkvalid(a)&&past!=a[x][y].real&&menu_command==10)
							message.reset(600,660,"ENTER YOU QUESTION AND PRESS SOLVE ");
						else if (!checkvalid(a))
							message.reset(600,660,"YOUR LAST MOVE IS INVALID CLEAR IT ");
						}

					if (command==200) {
						box bo2[9][9];

						for (int ll1=0; ll1<9; ll1++)
							for (int ll2=0; ll2<9; ll2++)
								bo2[ll1][ll2]=a[ll1][ll2];

						if (solve(bo2,0)) {
							message.reset(600,665,"YOUR SUDOKU IS SOLVED");
							printinbox(bo2,quest);
							getClick();
							break;
						} else
							message.reset(600,665,"THE SUDOKU IS NOT SOLVABLE");
					}

					if (command==300) {
						if (issolved(a)&&checkvalid(a)) {
							message.reset(600,665,"YOUR SUDOKU IS SOLVED");
							getClick();
							break;
						} else if (!issolved(a)&&checkvalid(a))
							message.reset(600,665,"YOUR SUDOKU IS UNSOLVED");
					}
				}

				printinbox(a,quest);
			}
		}

		closeCanvas();
	}
}

// FUNCTIONS USED FOR THE INTERFACE
// 1st
void print_grid()
{
	srand(time(NULL));
	Line ll(-1500,1500,1500,-1500);

	for (int i=0; i<2000; i++) {
		ll.setColor(COLOR(255,255*abs(sin(0.002*i)),randuv(200.0,255.0)*abs(cosine(0.002*(1000-i)))));
		ll.imprint();
		ll.move(1,0);
	}

	Line l(275,150,729,150),l1(275,150,275,605);

	for (int y=0; y<=9; y++) {
		l.imprint();
		l1.imprint();

		if (y%3==0) {
			l.move(0,1);
			l.imprint();
			l1.move(1,0);
			l1.imprint();
		}

		l.move(0,50);
		l1.move(50,0);
	}
}
//2nd : printing the elements on the interface
void print_all_box(int command)
{
	Text t(500,50,"$ |_| |) 0 K |_| ");
	t.setColor(COLOR(200,0,0));
	t.imprint();
	t.reset(500,78,"******************");
	t.imprint();
	t.setColor(COLOR(0,100,205));
	Rectangle r(132,250,200,70);
	t.reset(132,250,"SOLVE");
	r.imprint();
	r.reset(132,250,195,66);
	r.imprint();
	r.reset(132,250,190,62);
	r.imprint();
	t.imprint();
	r.reset(132,380,200,70);
	t.reset(132,380,"CHECK IT!!");
	r.imprint();
	r.reset(132,380,195,66);
	r.imprint();
	r.reset(132,380,190,62);
	r.imprint();
	t.imprint();
	r.reset(132,530,200,70);
	t.reset(132,530,"MAIN MENU");
	r.imprint();
	r.reset(132,530,195,66);
	r.imprint();
	r.reset(132,530,190,62);
	r.imprint();
	t.imprint();

	if (command!=10) {
		Text t_min(50,50,"Min : "),t_sec(50,75,"Sec : ");
		t_min.setColor(COLOR("brown"));
		t_sec.setColor(COLOR("brown"));
		t_min.imprint();
		t_sec.imprint();
	}

	int x=832,y=300;
	r.reset(x,y,50,50);
	t.setColor(COLOR(0,155,0));
	t.reset(x,y,"1");

	for (int i=0; i<20; i++) {
		Line l(-50,i,1000,i);
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
		l.imprint();
		l.reset(-50,700-i,1000,700-i);
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
		l.imprint();
		l.reset(1000-i,0,1000-i,1000);
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
		l.imprint();
		l.reset(i,0,i,1000);
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
		l.imprint();
	}

	for (double i=1; i<10; i++) {
		t.imprint();
		r.imprint();

		if (int(i)%2==0) {
			x-=50 ;
			y+=50;
		} else
			x+=50;

		t.reset(x,y,i+1);
		r.reset(x,y,50,50);
	}

	r.reset(600,655,680,50);
	r.imprint();
	r.reset(600,655,675,46);
	r.imprint();
	t.reset(150,655,"Message Box :");
	t.imprint();
}
//3rd : returns the cmmand for the event
int seeEvent(int x,int y)
{
	if (x>275&&x<725&&y>150&&y<600)
		return (((x-275)/50)*9)+((y-150)/50);
	else if (x>32&&x<232&&y>225&&y<275)
		return 200;
	else if (x>32&&x<232&&y>355&&y<405)
		return 300;
	else if (x>32&&x<232&&y>505&&y<555)
		return 400;
	else if (x>807&&x<907&&y>275&&y<475)
		return 100+(((x-807)/50)+1)+(((y-275)/50)*2);
	else if (x>807&&x<857&&y>475&&y<525)
		return 109;
	else if (x>802&&x<902&&y>190&&y<240)
		return 100;
	else return -1;
}
void printinbox(box a[9][9],bool q[9][9])
{
	beginFrame();

	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (a[i][j].real!=0) {
				Text t(300+50*j,175+50*i,double(a[i][j].real));

				if (q[i][j])
					t.setColor(COLOR(0,100,80));
				else
					t.setColor(COLOR(200,50,50));

				t.imprint();
			}
		}
	}

	endFrame();
}

void setString(string s[])
{
	s[0]="CONGOS!! YOU ARE GOING A GOOD WAY";
	s[1]="GREAT JOB BY YOU ";
	s[2]="WOW !! YOU DID WELL ";
	s[3]="GOOD WORK";
	s[4]="YOU ARE DOING GREAT";
	s[5]="YUP!! THAT WAS A PERFECT MOVE";
	s[6]="HEY GENIUS I THINK U R GR8";
	s[7]="THAT WAS SIMPLY AWSOME";
	s[8]="WELLDONE ! THE MOVE WAS PERFECT";
	s[9]="GODMAX MOVE :)";
	s[10]="MACHAYA .. CORRECT ";
	s[11]="THAT WAS A ROCKING MOVE.. REALLY ";
	s[12]="HEY. YOU REALLY ROCKING.";
	s[13]="FOD DIYA.. WAAH WAAH";
	s[14]="WHAT A LOVELY MOVE BY YOU !! ";
	s[15]="THIS WAS A PERFECT MOVE ";
	s[16]="EKDUM SAHI HAI YE :D";
	s[17]="WAAH WAAH ... PERFECT";
	s[18]="LOVELY..  GREAT WORK";
	s[19]="JIYO ....";
	s[20]="YOUR BRAIN MUSCLES ARE IN GOOD SHAPE";
	s[21]="KYA BAAT !! KYA BAAT !! KYA BAAT !!";
	s[22]="GReAT DoNe .. :D";
	s[23]="U r on ur way 2 Success";
	s[24]="RIght Choice";
	s[25]="Well Done";
}

void setquest(box a[9][9],bool q[9][9])
{
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++) {
			if (a[i][j].real!=0)
				q[i][j]=true;
			else
				q[i][j]=false;
		}
}

//End of the fumctions for the main interface

//********** FUNCTIONS FOR THE MENU SCREEN**************/////////////////

//Assigs The points of the sudoku displayed on the menu screen
void assign_sudoku(double a[6][11][2])
{
	a[0][0][0]=0; // ASSIGNS LETTER S
	a[0][0][1]=0;
	a[0][1][0]=0;
	a[0][1][1]=-20;
	a[0][2][0]=80;
	a[0][2][1]=-60;
	a[0][3][0]=0;
	a[0][3][1]=-100;
	a[0][4][0]=0;
	a[0][4][1]=-120;
	a[0][5][0]=120;
	a[0][5][1]=-170;
	a[0][6][0]=120;
	a[0][6][1]=-150;
	a[0][7][0]=40;
	a[0][7][1]=-110;
	a[0][8][0]=120;
	a[0][8][1]=-70;
	a[0][9][0]=120;
	a[0][9][1]=-50;
	a[0][10][0]=0;
	a[0][10][1]=0;

	for (int i=1; i<6; i+=4) {  // ASSIGNS LETTER U
		a[i][0][0]=0 ;
		a[i][0][1]=-160;
		a[i][1][0]=20;
		a[i][1][1]=-160;
		a[i][2][0]=20;
		a[i][2][1]=-40;
		a[i][3][0]=100;
		a[i][3][1]=-40;
		a[i][4][0]=100;
		a[i][4][1]=-160;
		a[i][5][0]=120;
		a[i][5][1]=-160;
		a[i][6][0]=120;
		a[i][6][1]=-20;
		a[i][7][0]=0;
		a[i][7][1]=-20;
		a[i][8][0]=0;
		a[i][8][1]=-160;
		a[i][9][0]=0;
		a[i][9][1]=-160;
		a[i][10][1]=0;
		a[i][10][1]=-60;
	}

	a[4][0][0]=0;  // ASSIGNS LETTER K
	a[4][0][1]=-160;
	a[4][1][0]=20;
	a[4][1][1]=-160;
	a[4][2][0]=20;
	a[4][2][1]=-100;
	a[4][3][0]=65;
	a[4][3][1]=-160;
	a[4][4][0]=80;
	a[4][4][1]=-140;
	a[4][5][0]=35;
	a[4][5][1]=-90;
	a[4][6][0]=90;
	a[4][6][1]=-40;
	a[4][7][0]=75;
	a[4][7][1]=-20;
	a[4][8][0]=20;
	a[4][8][1]=-75;
	a[4][9][0]=20;
	a[4][9][1]=-27;
	a[4][10][0]=00;
	a[4][10][1]=-22;

	for (int i=0; i<11; i++) { //ASSIGNS LETTER O
		a[3][i][0]= 50*cosine(i*(360.0/11.0))+50;
		a[3][i][1]= 75*sine(i*(360.0/11.0))-85;
	}

	a[2][0][0]=30 ;  //ASSIGNS LETTER D
	a[2][0][1]=0 ;
	a[2][1][0]=30 ;
	a[2][1][1]=-150 ;

	for (int i=2; i<11; i++) {
		a[2][i][0]= 30+75*cosine(90-i*(360.0/18.0));
		a[2][i][1]= -75-75*sine(90-i*(360.0/18.0));
	}
}
// Prints All the elements present on the screen
void print_menu_element()
{
	Rectangle r(500,380,250,50);
	r.setFill(1);
	r.setColor(COLOR(255,155,30));
	r.imprint();
	r.move(0,100);
	r.imprint();
	r.move(0,100);
	r.imprint();
	Text t(500,380,"SUDOKU SOLVER");
	t.imprint();
	t.reset(500,480,"THE SUDOKU GAME");
	t.imprint();
	t.reset(500,580,"QUIT");
	t.imprint();
}

// Main function for the menu
int menu()
{
	srand(time(NULL));
	initCanvas("MENU",1000,700);
	Line l(0,0,0,1000);
	beginFrame();
	int k=0;

	for (int i=0; i<=1000; i++) {
		l.move(1,0);

		if (i>500)
			k=1000;

		l.setColor(COLOR(0.5*abs(double((k-i))),0.5*abs(double(k-i)),0.5*abs(double(k-i))));
		l.imprint();
	}

	for (int i=0; i<=1000; i++) {
		int x=randuv(0,1000.0),y=randuv(0,1000.0);
		l.reset(x,0,x,randuv(10.0,45.0));
		l.imprint();
		l.reset(0,y,randuv(10.0,45.0),y);
		l.imprint();
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
		l.reset(x,700,x,700-randuv(10.0,45.0));
		l.imprint();
		l.reset(1000,y,1000-randuv(10.0,45.0),y);
		l.imprint();
		l.setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
	}

	endFrame();
	double a[6][11][2];
	assign_sudoku(a);
	Polygon p[6];
	print_menu_element();
	XEvent event;

	while (1) {
		beginFrame();

		for (int i=0; i<6; i++) {
			p[i].reset(100+125*i,200,a[i],11);
			p[i].setFill(true);
			p[i].setColor(COLOR(randuv(0.0,255.0),randuv(0.0,255.0),randuv(0.0,255.0)));
			p[i].move(randuv(-10.0,10.0),randuv(-20.0,20.0));
			p[i].left(randuv(-20.0,20.0));
		}

		endFrame();

		if (checkEvent(&event)) {
			if (mouseButtonPressEvent(&event)) {
				int x=event.xbutton.x , y=event.xbutton.y ;

				if (x>375&&y>355&&x<625&&y<405)
					return 10;
				else if (x>375&&y>555&&x<625&&y<605)
					return 0;
				else if (x>375&&y>455&&x<625&&y<505) {
					double a1[4][4][2]={{{375,555},{625,555},{625,555},{375,555}},{{375,555},{625,555},{625,555},{375,555}},{{375,455},{625,455},{625,455},{375,455}},{{375,455},{625,455},{625,455},{375,455}}};
					double b1[4][4][2]={{{750,300},{900,300},{900,350},{750,350}},{{750,400},{900,400},{900,450},{750,450}},{{750,500},{900,500},{900,550},{750,550}},{{750,600},{900,600},{900,650},{750,650}}};
					double dist[4][4][2];

					for (int i=0; i<4; i++)
						for (int j=0; j<4; j++) {
							dist[i][j][0]= a1[i][j][0] ;
							dist[i][j][1]= a1[i][j][1] ;
						}

					Polygon boxes[4];

					for (int i=0; i<4; i++)
						boxes[i].reset(0,0,a1[i],4);

					for (int loopcount=0; loopcount<50; loopcount++) { // ANimation is taking place
						beginFrame();

						for (int i=0; i<4; i++)
							for (int j=0; j<4; j++) {
								dist[i][j][0]+=((b1[i][j][0]-a1[i][j][0])/50);
								dist[i][j][1]+=((b1[i][j][1]-a1[i][j][1])/50);
							}

						for (int i=0; i<4; i++) {
							boxes[i].reset(0,0,dist[i],4);
							boxes[i].setFill(1);
							boxes[i].setColor(COLOR(20,10,80));
						}

						endFrame();
						usleep(500);
					}

					Text mm[4];

					for (int loop=0; loop<4; loop++)
						mm[loop].setColor(COLOR("yellow"));

					mm[0].reset(825,325,"VERY EASY");
					mm[1].reset(825,425,"EASY");
					mm[2].reset(825,525,"MEDIUM");
					mm[3].reset(825,625,"HARD");
					bool check=true;

					while (check) {
						int click=getClick();
						int xpos=click/65536 , ypos=click%65536;

						if (xpos>750&&xpos<900&&ypos>300&&ypos<350)
							return 1;
						else if (xpos>750&&xpos<900&&ypos>400&&ypos<450)
							return 2;
						else if (xpos>750&&xpos<900&&ypos>500&&ypos<550)
							return 3;
						else if (xpos>750&&xpos<900&&ypos>600&&ypos<650)
							return 4;
						else
							break;
					}
				}
			}
		}
	}

	return 0;
}

//**** END OF SUDOKU **********///
