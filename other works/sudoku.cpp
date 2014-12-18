#include <simplecpp>
#include <assert.h>
// PROGRAM USES A BOX STRUCTURE FOR EACH BOX
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

bool solve(box a[9][9],int mode=1);   // solves a sudoku
bool logic2(box a[9][9]);   // applies logic two which fills a box even it is not completely cut
bool logic3(box a[9][9]); // applies a stronger logic to solve sudoku
bool logic4(box a[9][9]);
bool check_cut(box a[9][9],int set,int pos[][2],int count);
void print(box a[9][9]);  // prints a block
void passinto(box a[9][9],int i , int j);  // places the real value ad cuts all vertical and horizontal possibilities

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
// APPLYING THIRD LOGIC
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

bool logic4(box a[9][9])
{
	for (int i=0; i<9; i++) {
		for (int j=0; j<9; j++) {
			if (a[i][j].count==7&&a[i][j].real==0) {
				box b[9][9];

				for (int l=0; l<9; l++)
					for (int m=0; m<9; m++)
						b[l][m]=a[l][m];

				bool check=true;

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

void print(box a[9][9])
{
	int i,j;
	cout<<"\n\n +--------------------------+\n";

	for (i=0; i<9; i++) {
		cout<<" | ";

		for (j=0; j<9; j++) {
			if (a[i][j].real!=0)
				cout<<a[i][j].real<<" ";
			else
				cout<<". ";

			if ((j+1)%3==0) cout<<" | ";
		}

		cout<<endl;

		if ((i+1)%3==0)
			cout<<" +--------------------------+"<<endl;
	}
}
bool checkvalid(box a[9][9])    // checks validity of a sudoku
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
					}
				}
			}
		}
	}

	return check;
}
bool issolved(box a[9][9])
{
	bool check=true;

	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			check=check&&(a[i][j].real!=0);

	return check;
}
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


main_program {
	box a[9][9];
	int i,j;

	for (i=0; i<9; i++) {
		for (j=0; j<9; j++) {
			cin>>a[i][j].real;

			if (!(checkvalid(a))) {
				cout<<" you entered wrong sudoku ";
			}
		}
	}

	solve(a,0);
	cout<<endl;
	print(a);
	cout<<endl;

}

