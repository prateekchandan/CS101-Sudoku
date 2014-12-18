#include <simplecpp>
#include <string>

struct button {
	int x,y;
	int length,width;
	button(int centre_x,int centre_y,int l,int w) {
		x=centre_x;
		y=centre_y;
		length=l;
		width=w;
	}
	button() {}
	void create() {
		Rectangle s(x,y,length,width);
		s.imprint();
	}
	bool clicked_on(int click_pos) {
		return(abs(click_pos/65536-x)<=length/2)&&(abs(click_pos%65536-y)<=width/2);
	}
	void put_text(string t) {
		Text info(x,y,t);
		info.imprint();
	}
};

/* function for drawing the squares of the rubik's cube */
void draw_rec(Rectangle r[][3][3], int face, int pos_x, int pos_y)
{
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			r[face][i][j].reset(pos_x+25+50*j,pos_y+25+50*i,48,48);
			r[face][i][j].imprint();
		}
	}
}

/*function for drawing the bounadaries of cube */
void draw_bound()
{
	Line l(0,0,0,0);

	for (int i=1; i<11; i++) {
		if (i<4 || i>7) {
			l.reset(250,150+50*(i-1),400,150+50*(i-1));
		} else {
			l.reset(100,300+50*(i-4),700,300+50*(i-4));
		}

		l.imprint();
	}

	for (int j=1; j<14; j++) {
		if (j<4 || j>7) {
			l.reset(100+50*(j-1),300,100+50*(j-1),450);
		} else {
			l.reset(100+50*(j-1),150,100+50*(j-1),600);
		}

		l.imprint();
	}
}

/*function for directly rotating a face */
/* used inside the functions for making various moves of the rubik's cube */
void rotate(int face, int sense, int rubik[][3][3])
{
	if (sense==1) { //clockwise rotation of face
		for (int j=0; j<2; j++) {
			int temp=rubik[face][0][j];
			rubik[face][0][j]=rubik[face][2-j][0];
			rubik[face][2-j][0]=rubik[face][2][2-j];
			rubik[face][2][2-j]=rubik[face][j][2];
			rubik[face][j][2]=temp;
		}
	} else { //anti-clockwise rotation of face
		for (int j=0; j<2; j++) {
			int temp=rubik[face][j][2];
			rubik[face][j][2]=rubik[face][2][2-j];
			rubik[face][2][2-j]=rubik[face][2-j][0];
			rubik[face][2-j][0]=rubik[face][0][j];
			rubik[face][0][j]=temp;
		}
	}
}

/* function for colouring the squares of the rubik's cube based on the value present in the corresponding element of the array 'rubik' */
void color(int rubik[][3][3], Rectangle r[6][3][3])
{
	beginFrame();

	for (int m=0; m<6; m++) {
		for (int j=0; j<3; j++) {
			for (int k=0; k<3; k++) {
				switch (rubik[m][j][k]) {
				case 0:
					r[m][j][k].setFill();
					r[m][j][k].setColor(COLOR("red"));
					break;
				case 1:
					r[m][j][k].setFill();
					r[m][j][k].setColor(COLOR("yellow"));
					break;
				case 2:
					r[m][j][k].setFill();
					r[m][j][k].setColor(COLOR("blue"));
					break;
				case 3:
					r[m][j][k].setColor(COLOR(255,255,255));
					break;
				case 4:
					r[m][j][k].setFill();
					r[m][j][k].setColor(COLOR(155,127,80));
					break;
				case 5:
					r[m][j][k].setFill();
					r[m][j][k].setColor(COLOR("green"));
				}

//				r[m][j][k].setFill(1);
			}
		}
	}

	endFrame();
}

//Following are functions for various movements of rubik's cube

/*eg. right means clockwise rotation of right-side face...
 And right_inv means anti-clockwise rotation */

/* The faces are decided(whether right or top or front or...) keeping the front face fixed */

/* left=0; front=1; right=2; back=3; top=4; down=5 */

void right(int rubik[][3][3])
{
	rotate(2,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[1][i][2];
		rubik[1][i][2]=rubik[5][i][2];
		rubik[5][i][2]=rubik[3][2-i][0];
		rubik[3][2-i][0]=rubik[4][i][2];
		rubik[4][i][2]=temp;
	}
}

void right_inv(int rubik[][3][3])
{
	rotate(2,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[4][i][2];
		rubik[4][i][2]=rubik[3][2-i][0];
		rubik[3][2-i][0]=rubik[5][i][2];
		rubik[5][i][2]=rubik[1][i][2];
		rubik[1][i][2]=temp;
	}
}

void left(int rubik[][3][3])
{
	rotate(0,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[1][i][0];
		rubik[1][i][0]=rubik[4][i][0];
		rubik[4][i][0]=rubik[3][2-i][2];
		rubik[3][2-i][2]=rubik[5][i][0];
		rubik[5][i][0]=temp;
	}
}

void left_inv(int rubik[][3][3])
{
	rotate(0,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[5][i][0];
		rubik[5][i][0]=rubik[3][2-i][2];
		rubik[3][2-i][2]=rubik[4][i][0];
		rubik[4][i][0]=rubik[1][i][0];
		rubik[1][i][0]=temp;
	}
}

void up(int rubik[][3][3])
{
	rotate(4,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[1][0][i];
		rubik[1][0][i]=rubik[2][0][i];
		rubik[2][0][i]=rubik[3][0][i];
		rubik[3][0][i]=rubik[0][0][i];
		rubik[0][0][i]=temp;
	}
}

void up_inv(int rubik[][3][3])
{
	rotate(4,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[0][0][i];
		rubik[0][0][i]=rubik[3][0][i];
		rubik[3][0][i]=rubik[2][0][i];
		rubik[2][0][i]=rubik[1][0][i];
		rubik[1][0][i]=temp;
	}
}

void down(int rubik[][3][3])
{
	rotate(5,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[0][2][i];
		rubik[0][2][i]=rubik[3][2][i];
		rubik[3][2][i]=rubik[2][2][i];
		rubik[2][2][i]=rubik[1][2][i];
		rubik[1][2][i]=temp;
	}
}

void down_inv(int rubik[][3][3])
{
	rotate(5,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[1][2][i];
		rubik[1][2][i]=rubik[2][2][i];
		rubik[2][2][i]=rubik[3][2][i];
		rubik[3][2][i]=rubik[0][2][i];
		rubik[0][2][i]=temp;
	}
}

void front(int rubik[][3][3])
{
	rotate(1,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[4][2][i];
		rubik[4][2][i]=rubik[0][2-i][2];
		rubik[0][2-i][2]=rubik[5][0][2-i];
		rubik[5][0][2-i]=rubik[2][i][0];
		rubik[2][i][0]=temp;
	}
}

void front_inv(int rubik[][3][3])
{
	rotate(1,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[2][i][0];
		rubik[2][i][0]=rubik[5][0][2-i];
		rubik[5][0][2-i]=rubik[0][2-i][2];
		rubik[0][2-i][2]=rubik[4][2][i];
		rubik[4][2][i]=temp;
	}
}

void back(int rubik[][3][3])
{
	rotate(3,1,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[4][0][i];
		rubik[4][0][i]=rubik[2][i][2];
		rubik[2][i][2]=rubik[5][2][2-i];
		rubik[5][2][2-i]=rubik[0][2-i][0];
		rubik[0][2-i][0]=temp;
	}
}

void back_inv(int rubik[][3][3])
{
	rotate(3,0,rubik);

	for (int i=0; i<3; i++) {
		int temp=rubik[0][2-i][0];
		rubik[0][2-i][0]=rubik[5][2][2-i];
		rubik[5][2][2-i]=rubik[2][i][2];
		rubik[2][i][2]=rubik[4][0][i];
		rubik[4][0][i]=temp;
	}
}
// end of rotation functions //

/*function which takes in a random value b/w 1 to 12 and does a
	move in the rubik's cube correspondingly */
void move(int move_index,int rubik[][3][3])
{
	switch (move_index) {
	case 1:
		right(rubik);
		break;
	case 2:
		right_inv(rubik);
		break;
	case 3:
		left(rubik);
		break;
	case 4:
		left_inv(rubik);
		break;
	case 5:
		up(rubik);
		break;
	case 6:
		up_inv(rubik);
		break;
	case 7:
		down(rubik);
		break;
	case 8:
		down_inv(rubik);
		break;
	case 9:
		front(rubik);
		break;
	case 10:
		front_inv(rubik);
		break;
	case 11:
		back(rubik);
		break;
	case 12:
		back_inv(rubik);
	}

//	cout<<move_index<<"\n";
}


int main()
{
	initCanvas("Rubik's Cube in 2-D",1000,1000);
	int rubik[6][3][3];
	int move_seq[20];
	Rectangle r[6][3][3];
	string command[12]={"R","R_I","L","L_I","U","U_I","D","D_I","F","F_I","B","B_I"};
	beginFrame();

	for (int face=0; face<6; face++) {
		for (int i=0; i<3; i++) {
			for (int j=0; j<3; j++) {
				rubik[face][i][j]=face;
			}
		}

		if (face<4) {
			draw_rec(r,face,100+face*150,300);
		} else if (face==4) {
			draw_rec(r,4,250,150);
		} else {
			draw_rec(r,5,250,450);
		}
	}

	draw_bound();
	color(rubik,r);
	button b[12];
	beginFrame();

	for (int i=0; i<12; i++) {
		b[i]=button(50+i*80,100,60,60);
		b[i].create();
		b[i].put_text(command[i]);
	}

	button quit(800,500,80,20),jumble(800,400,80,20);
	quit.create();
	jumble.create();
	quit.put_text("QUIT");
	jumble.put_text("JUMBLE");
	endFrame();
	endFrame();

	while (true) {
		int click_pos=getClick();
		int l;

		for (l=0; l<12; l++) {
			if (b[l].clicked_on(click_pos)==true) {
				move(l+1,rubik);
				color(rubik,r);
				break;
			}
		}

		if (l!=12)continue;

		if (jumble.clicked_on(click_pos)==true) {
			time_t *t;
			t=0;
			int i=0;
			repeat(20) {
				srand(time(t));
				int fro=(randuv(1.00,12.99));
				move(fro,rubik);
				move_seq[i]=fro;
				color(rubik,r);
				i++;
				wait(1);
			}
			continue;
		} else if (quit.clicked_on(click_pos)==true) {
			break;
		}

		/*	for(int i=0;i<20;i++){
				if(move[i]%2==1)random_move(move[i]+1,rubik);
				else random_move(move[i]-1,rubik);
				color(rubik,r);
			}
		*/
	}

	closeCanvas();
	return 0;
}


