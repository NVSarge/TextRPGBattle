// TextRPGBattle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>   
#include <stdlib.h>
#include <chrono>
#include<thread>
#include <Windows.h>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

#define ENEMY_DICES_HIDE false
#define ENEMY_DICES_SHOW true
enum tcolor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void textcolor(int color)
{
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleHandle, color);
}


void wait(int mils)
{
	sleep_until(system_clock::now() + chrono::milliseconds(mils));
}

int xd6(int x, bool show = ENEMY_DICES_SHOW)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		int R = rand() % 6;
		for (int j = 0; j < rand() % 8 + 8; j++)
		{

			if (show)
			{
				cout << R + 1;
			}
			wait(30);
			R = rand() % 6;
			if (show)
			{
				cout << "\b";
			}

		}
		if (show)
		{
			cout << R + 1;
		}
		retval += R + 1;
	}
	return retval;
}

int xdy(int x, int y)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		retval += rand() % y + 1;
	}
	return retval;
}

struct Item
{
	string name;
	int Att;
	int Def;
	int BDY;
	Item()
	{
		Att = Def = BDY = 0;
		name = "dust";
	}
	Item(int a, int d, int x)
	{
		Att = a;
		Def = d;
		BDY = x;
		name = "smth";
	}
	Item(int a, int d, int x, string s)
	{
		Att = a;
		Def = d;
		BDY = x;
		name = s;
	}
};

struct RageDicePool
{
	int amount;
	RageDicePool()
	{
		amount = 0;
	}
	void Push()
	{
		amount++;
	}
	void Pop()
	{
		amount--;
	}

	bool Acquire()
	{
		bool retval = true;
		if (amount > 0)
		{
			textcolor(0x74);
			cout << "\nRed dice:";
			wait(600);
			if (xd6(1) <= amount)
			{
				retval = false;
				amount = 0;
				cout << endl;
				for (int i = 0; i < 5; i++)
				{

					cout << "RAAAAAGE!";
					wait(200);
					cout << "\r         \r";
					wait(200);
				}
			}
			else
			{
				textcolor(Green);
				cout << "Cool!";
				wait(300);
				amount--;
			}
		}
		cout << endl;
		textcolor(0x0F);
		return retval;
	}
};


string GetClashText()
{
	string Texts[7] = {
		" none",
		" clang!",
		" bang! ",
		" hoosh!",
		" swish!",
		" bat!  ",
		" wooze!"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];
}

string GetWoundText()
{
	string Texts[7] = {
		" none",
		" pierce lung",
		" cut leg",
		" cut arm",
		" wound head",
		" scratched eye",
		" broke teeth"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];
}

string GetBrokeText()
{
	string Texts[7] = {
		" none",
		" stepped on the dog's shit",
		" stepped in the mud",
		" sneezed",
		" heavy farted",
		" hicced up fluorishly",
		" coughed with blood"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}

struct DNA
{
	DNA()
	{
		isBDYReroll = 0;
	}
	int isBDYReroll;//0-10 amount of RDP use for reroll BDY
	bool CheckBDY()
	{

		return xdy(1, 10) <= isBDYReroll;
	}
};

struct Unit
{
	vector<string> wounds;
	Item Base;
	int HP;
	int Stamina;
	RageDicePool RDP;
	bool isRaged;
	int SubDicePool;
	DNA dna;
	string name;
	bool useRage()
	{
		bool ret = RDP.Acquire();
		if (!ret)
		{
			Stamina = 0;
			isRaged = true;
		}
		return ret;
	}
	Unit(int a, int d, int x, string s)
	{
		SubDicePool = 0;
		Base.Att = a;
		Base.Def = d;
		Base.BDY = x;
		HP = 4 + xdy(Base.BDY * 2 - 2, 3);
		Stamina = Base.BDY;
		name = s;
		dna.isBDYReroll = 5;
	}
	Unit()
	{
		SubDicePool = 0;
		HP = 1;
		Stamina = 1;
		name = "";
		dna.isBDYReroll = 5;
	}
	Unit(string s, int CR)
	{
		SubDicePool = 0;
		Base.Att = rand() % CR + 1;
		Base.Def = rand() % CR + 1;
		Base.BDY = CR;
		HP = 4 + 2 * xdy((CR - 1) * 2, 3);
		Stamina = Base.BDY;
		name = s;
		dna.isBDYReroll = 5;
	}
};



struct Option
{
	string Text;
	Item AddDamage;
	Option(string T, Item *i)
	{
		Text = T;
		AddDamage.Att = i->Att;
		AddDamage.Def = i->Def;
		AddDamage.BDY = i->BDY;
	}
};

/*
int DoDamageV1(Unit *p, Unit *f,Option *o,Option *l)
{
	int retval = 10;
	int sum = 0;
	for (int i = 0; i < p->Base.BDY; i++)
	{
		if (rand() % 6 + 1 > f->Base.Def)
		{
			sum++;
		}
	}
	if(sum>0)
	{
		//cout <<p->name<<" hit";
		int Dmg = (p->Base.Att + o->AddDamage.Att);
		Dmg = rand() % Dmg+1;
		retval = Dmg*100;
	}
	else
	{
		//cout << p->name << " miss";
	}
	if(retval>=0)
	{
		f->HP -= retval;
	}
	else
	{
		retval = 0;
	}
	p->Stamina += o->AddDamage.BDY;

	return retval;
}


void Version1_0()
{
	Unit *Foe;
	Unit *Player;
	vector<Item> *Inventory;
	vector<vector<Option>> *PlayerOptions;
	vector<Option> *FoeOptions;
	while (Foe->HP > 0 && Player->HP > 0)
	{

		//cout << "\n==============================" << endl;
		//cout << Player->name << " is " << Player->HP << " s:" << Player->Stamina << " [" << Player->Base.Att << ", " << Player->Base.Def << ", " << Player->Base.BDY << "]" << endl;
		//cout << Foe->name << " is " << Foe->HP << " [" << Foe->Base.Att << ", " << Foe->Base.Def << ", " << Foe->Base.BDY << "]" << endl;
		//cout << "==============================" << endl;
		int i = 1;
		int j = 1;
		int PSel = 1;
		int FSel = 0;
		if (Player->Stamina > 0)
		{
			for (Option o : PlayerOptions->at(0))
			{
				//cout << i++ << ". " << o.Text << endl;
			}
			cin >> i;
			if (i >= 1 && i <= PlayerOptions->size())
			{
				PSel = i;
				i = 1;
				for (Option o : PlayerOptions->at(PSel))
				{

					//cout << " *" << i++ << ". " << o.Text << "[" << o.AddDamage.BDY << "]" << endl;
				}
				cin >> j;



			}
			else
			{

				PSel = 4;
				j = 1;
			}

		}
		else
		{
			PSel = 4;
			j = 1;
		}
		system("CLS");

		int d = DoDamageV1(Player,Foe,&((PlayerOptions->at(PSel)).at(j - 1)), &FoeOptions->at(FSel));

		//cout << "\n\nYou " << PlayerOptions->at(0).at(PSel - 1).Text << " " << (PlayerOptions->at(PSel)).at(j - 1).Text << " for " << d << " damage." << endl;
		FSel = rand() % FoeOptions->size();
		d = DoDamageV1(Foe, Player, &(FoeOptions->at(FSel)), &((PlayerOptions->at(PSel)).at(j - 1)));
		//cout << "\n\n" << Foe->name << " " << (FoeOptions->at(i - 1)).Text << "s for " << d << " damage." << endl;

		if (Player->Stamina > Player->Base.BDY)
		{
			Player->Stamina = Player->Base.BDY;
		}


	}
	//cout << "Battle is over " << endl;
	system("PAUSE");
}
/**/

enum DColor
{
	WHITE,
	BLACK,
	RED
};



struct ColoredDice
{
	ColoredDice(DColor dc)
	{
		color = dc;
	}
	DColor color;
	int NUM;
	void Throw()
	{
		NUM = xd6(1);
	}
};


bool CheckDices(ColoredDice CD1, ColoredDice CD2)
{
	return (CD1.NUM < CD2.NUM);
}




void ShowStats(Unit Player, Unit Foe)
{

	cout << "\n==============================" << endl << endl;

	cout << Player.name << " HP :";
	textcolor(0x02);
	cout << Player.HP << endl;
	textcolor(0x0F);
	cout << " [BDY:" << Player.Base.BDY << ",  weapon " << Player.Base.Att << ", armor " << Player.Base.Def << "]" << endl << " RageT: " << Player.RDP.amount << endl;
	cout << "Additional dices:" << Player.SubDicePool << endl << endl;
	textcolor(Red);
	cout << "Wounds: ";
	for (string S : Player.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << "\n==============================" << endl << endl;

	cout << Foe.name << " HP :";
	textcolor(0x02);
	cout << Foe.HP << endl;
	textcolor(0x0F);
	cout << " [BDY:" << Foe.Base.BDY << ", weapon " << Foe.Base.Att << ", armor " << Foe.Base.Def << "]" << endl << " RageT: " << Foe.RDP.amount << endl;
	cout << "Additional dices:" << Foe.SubDicePool << endl << endl;
	textcolor(Red);
	cout << "Wounds: ";
	for (string S : Foe.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << "\n==============================" << endl;
	wait(500);
}


void MakeActions(Unit *Player, Unit *Foe, bool isAI, bool isShowDices = true)
{
	cout << endl << Player->name << " wins " << Player->Stamina << " actions," << Foe->name << " get 1 RT" << endl;
	Foe->RDP.Push();
	while (Player->Stamina > 0)
	{
		int n;
		int R = 0;
		char c = 'n';
		if (!isAI)
		{

			cout << "\nSelect(" << Player->Stamina << " ap left):\n1: Attack(1 ap)" << endl;
			cout << "2: Retreat(1 ap)" << endl;
			cout << "3: Strafe(1 ap)" << endl;
			cout << "4: Defend(" << Player->Stamina << " ap)" << endl;
			wait(500);
			cin >> n;
			cout << "\r             \r";
		}
		else
		{
			n = rand() % 4 + 1;
		}
		cout << endl << Player->name;
		switch (n)
		{
		case 1:
			Player->Stamina--;
			cout << " attacks: ";
			R = xd6(1, isShowDices);

			if (Player->RDP.amount > 0)
			{
				while (c != 'n'&&Player->RDP.amount > 0)
				{
					if (!isAI)
					{
						cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
						cin >> c;
					}
					else
					{
						//
					}
					if (c != 'n')
					{
						if (Player->RDP.Acquire())
						{
							R = xd6(1);
						}
						else
						{
							textcolor(Magenta);
							GetBrokeText();
							Player->Stamina = 0;
							///????
							Player->HP -= 1;
							Foe->HP -= 1;
							textcolor(White);
						}
					}
				}
			}
			R = R + Player->Base.Att / 2;
			if (R > Foe->Base.Def)
			{
				Foe->HP -= Player->Base.Att;
				cout << endl << Player->name << " attacks for " << Player->Base.Att << " and";
				Foe->wounds.push_back(GetWoundText());
				cout << " of " << Foe->name << ",  HP:" << Foe->HP << endl;

			}
			else if (R == Foe->Base.Def)
			{

				Foe->HP -= Player->Base.Att * 2;
				cout << endl << Player->name << " heavy attacks for " << Player->Base.Att * 2 << " and";
				Foe->wounds.push_back("heavy " + GetWoundText());
				cout << " of " << Foe->name << ",  HP:" << Foe->HP << endl;
			}
			else
			{
				cout << endl << Player->name << " attacks and";
				GetBrokeText();
				cout << endl;
			}
			break;
		case 2:
			Player->Stamina--;
			cout << " moves: ";
			R = xd6(1, isShowDices);
			if (Player->RDP.amount > 0)
			{
				while (c != 'n'&&Player->RDP.amount > 0)
				{
					if (!isAI)
					{
						cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
						cin >> c;
					}
					else
					{
						//
					}
					if (c != 'n')
					{
						if (Player->RDP.Acquire())
						{
							R = xd6(1);
						}
						else
						{
							textcolor(Magenta);
							GetBrokeText();
							Player->SubDicePool--;
							textcolor(White);
							break;
						}
					}
				}
			}
			cout << endl << Player->name << " moves back and ";
			if (Player->RDP.amount < R&&Player->RDP.amount>0)
			{
				Player->RDP.amount--;

				cout << Player->name << " loses one rage token." << endl;
			}
			else
			{
				GetBrokeText();
				cout << endl;
			}
			break;
		case 3:
			Player->Stamina--;
			cout << " strafes: ";
			R = xd6(1, isShowDices);
			if (Player->RDP.amount > 0)
			{
				while (c != 'n'&&Player->RDP.amount > 0)
				{
					if (!isAI)
					{
						cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
						cin >> c;
					}
					else
					{
						//
					}
					if (c != 'n')
					{
						if (Player->RDP.Acquire())
						{
							R = xd6(1);
						}
						else
						{
							textcolor(Magenta);
							GetBrokeText();
							Player->SubDicePool--;
							textcolor(White);
						}
					}
				}
			}
			cout << endl << Player->name << " strafes back and ";
			if (Foe->RDP.amount < R)
			{
				Foe->RDP.amount++;
				cout << Foe->name << " gets one rage token." << endl;
			}
			else
			{
				GetBrokeText();
				cout << endl;
			}
			break;
		case 4:
			cout << endl << " stands ground!" << endl;
			Player->SubDicePool += Player->Stamina;
			Player->Stamina = 0;
			break;
		default:
			break;
		}
	}
}

int main()
{
	srand(time(NULL));
	vector<Item> Inventory;
	vector<vector<Option>> PlayerOptions;
	vector<Option> FoeOptions;
	vector<Unit> Enemies;
	Enemies.push_back(Unit("Rat", 1));
	Enemies.push_back(Unit("Goblin", 2));
	Enemies.push_back(Unit("HobGoblin", 3));
	Enemies.push_back(Unit("Orc", 4));
	Enemies.push_back(Unit("Orc Chief", 5));
	Enemies.push_back(Unit("Troll", 6));
	Enemies.push_back(Unit("Owlbear", 7));
	Enemies.push_back(Unit("Sabertooth", 8));
	Enemies.push_back(Unit("Dragon", 9));

	Unit Foe = Enemies[rand() % Enemies.size()];
	Unit Player;
	cout << unitbuf;
	cout << "Hello stranger, i am " << Foe.name << "["<<Foe.Base.BDY<<"], state your name!: ";
	cin >> Player.name;
	cout << "Tell me," << Player.name << " your level: ";
	int cr;
	cin >> cr;
	Player = Unit(Player.name, cr);
	cout << Player.name << " you ows me money! Time to fight!" << endl;

	Player.RDP.amount = 1;
	Foe.RDP.amount = 1;

	ShowStats(Player, Foe);
	system("PAUSE");

	while (Player.HP > 0 && Foe.HP > 0)
	{
		system("CLS");
		ShowStats(Player, Foe);
		cout << "Fighting for initiative:" << endl;
		if (Foe.RDP.amount >= 6)
		{
			cout << Foe.name << " RAGED!" << endl;
			Foe.RDP.amount = 0;
			Foe.Stamina = 0;
			Foe.isRaged;
			Foe.SubDicePool = -Foe.Base.BDY;
		}
		if (Player.RDP.amount >= 6)
		{
			cout << Player.name << " RAGED!" << endl;
			Player.RDP.amount = 0;
			Player.Stamina = 0;
			Player.isRaged;
			Player.SubDicePool = -Player.Base.BDY;
		}

		Player.isRaged = false;
		Foe.isRaged = false;
		Player.Stamina = 0;
		Foe.Stamina = 0;
		vector<ColoredDice>BDYDices;

		int Z = Player.Base.BDY + Player.SubDicePool + Foe.Base.BDY + Foe.SubDicePool;
		int zP = 0;
		int zF = 0;
		for (int i = 0; i < Z; i++)
		{
			int side = rand() % 2;
			if (zP >= Player.Base.BDY + Player.SubDicePool)
			{
				side = 0;
			}
			if (zF >= Foe.Base.BDY + Foe.SubDicePool)
			{
				side = 1;
			}
			cout << endl;
			GetClashText();
		    if (side == 1)
			{

				textcolor(0xF0);
				ColoredDice white(WHITE);
				white.Throw();
				BDYDices.push_back(white);
				zP++;
			}
			else
			{
				textcolor(0x0F);
				ColoredDice black(BLACK);
				black.Throw();
				BDYDices.push_back(black);
				zF++;
			}
			textcolor(0x0F);
			cout << "    ";
		}


		Player.SubDicePool = 0;
		Foe.SubDicePool = 0;

		sort(BDYDices.begin(), BDYDices.end(), CheckDices);
		int ActionsNum = 0;
		cout << "\n====TOTAL:===" << endl;
		for (int i = 0; i < BDYDices.size(); i++)
		{

			textcolor((BDYDices[i].color == WHITE) ? (0xF0) : (0x0F));
			cout << i + 1 << ":" << BDYDices[i].NUM;
			textcolor(0x0F);
			cout << "   ";
		}

		wait(300);
		if (Player.RDP.amount > 0)
		{
			cout << "\nreroll "<<Player.RDP.amount<<" rt (input numbers e.g. 1,4,7 0-none): ";
			string rr;
			cin >> rr;
			//getline(std::cin , rr);
			stringstream ss(rr);
			vector<int> rerolldices;
			int i;
			while (ss >> i && rerolldices.size() < Player.RDP.amount)
			{
				if (i == 0)
				{
					break;
				}
				rerolldices.push_back(i - 1);
				if (ss.peek() == ',')
				{
					ss.ignore();
				}
			}
			for (int rt = 0; rt < rerolldices.size(); rt++)
			{
				if (Player.useRage())
				{
					BDYDices[rerolldices[rt]].Throw();
					cout << endl;
					GetClashText(); 
					cout<< rerolldices[rt] + 1 << " => " << BDYDices[rerolldices[rt]].NUM << endl;
				}
				else
				{
					cout << endl << Player.name;
					GetBrokeText();
					cout << " and loses his initiative" << endl;
					Foe.Stamina = 1;
					rt = rerolldices.size() + 1;
				}

			}
		}
		wait(300);
		if (Foe.dna.CheckBDY() && Foe.RDP.amount > 0)
		{
			cout << endl << Foe.name << " rerolls:";
			if (Foe.useRage())
			{
				int r = rand() % BDYDices.size();
				BDYDices[r].Throw();
				cout << endl;
				GetClashText();
				cout << r + 1 << " => " << BDYDices[r].NUM << endl;
			}
			else
			{
				cout << endl << Foe.name;
				GetBrokeText();
				cout << " and loses his initiative" << endl;
				Player.Stamina = 1;
			}
		}



		if (!Player.isRaged && !Foe.isRaged)
		{
			sort(BDYDices.begin(), BDYDices.end(), CheckDices);	
			DColor c;
			int lastDice;
			int lastChunk = 1;
			c = BDYDices.back().color;
			lastDice = BDYDices.back().NUM;
			for (int i = BDYDices.size() - 2; i >= 0; i--)
			{

				if (BDYDices[i].color != c)
				{
					if (BDYDices[i].NUM != lastDice)
					{
						lastChunk = 0;
					}
					i = -1;
				}
				else
				{
					if (BDYDices[i].NUM == lastDice)
					{
						lastChunk++;
					}
					else
					{
						lastChunk = 0;
					}

					lastDice = BDYDices[i].NUM;
					ActionsNum++;
				}

			}

			ActionsNum -= lastChunk - 1;
			if (BDYDices.back().color == WHITE)
			{
				Foe.Stamina = 0;
				Player.Stamina = ActionsNum;
			}
			else {
				Player.Stamina = 0;
				Foe.Stamina = ActionsNum;
			}
			cout << endl;
			for (int i = 0; i <= BDYDices.size() - ActionsNum - 1; i++)
			{

				textcolor(0x87);
				cout << i + 1 << ":" << BDYDices[i].NUM;
				textcolor(0x0F);
				cout << "   ";
			}

			for (int i = BDYDices.size() - ActionsNum; i < BDYDices.size(); i++)
			{

				textcolor((BDYDices[i].color == WHITE) ? (0xF0) : (0x0F));
				cout << i + 1 << ":" << BDYDices[i].NUM;
				textcolor(0x0F);
				cout << "   ";
			}
		}		
		system("PAUSE");
		system("CLS");		
		if (Player.Stamina > 0)
		{
			ShowStats(Player, Foe);
			MakeActions(&Player, &Foe, false, true);

		}
		else if (Foe.Stamina > 0)
		{
			ShowStats(Player, Foe);
			MakeActions(&Foe, &Player, true,false);
		}
		else
		{
			cout << "Draw! Sides retreat! Tuche!" << endl;
		}
		
		system("PAUSE");
	}


	cout << "Battle is over " << endl;
	system("PAUSE");

	return 0;
}

