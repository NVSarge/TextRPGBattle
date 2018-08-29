// TextRPGBattle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>   
#include <stdlib.h>
#include <chrono>
#include<thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


void wait(int mils)
{
	sleep_until(system_clock::now() + chrono::milliseconds(mils));
}

int xd6(int x)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		int R = rand()%6;
		for (int j = 0; j < rand() % 5 + 5; j++)
		{
		
			cout<<R+1;
			wait(100);
			R = rand() % 6;
			cout<< "\b";
		}
		cout<<R+1;
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
			cout << "\nRed dice:"<<endl;
			wait(600);
			if (xd6(1) <= amount)
			{
				retval = false;
				amount = 0;	
				cout << "\r       \r";
				cout << "\nRAAAAAGE!" << endl;
			}
			else
			{
				cout << "\r       \r";
				amount--;
			}
		}
		return retval;
	}
};

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
	cout << "\n==============================" << endl;
	cout << Player.name << " HP is " << Player.HP << " [BDY:" << Player.Base.BDY << ",  weapon " << Player.Base.Att << ", armor " << Player.Base.Def << "]" << " RageT: " << Player.RDP.amount << endl;
	cout << Foe.name << " HP is " << Foe.HP << " [BDY:" << Foe.Base.BDY << ", weapon " << Foe.Base.Att << ", armor " << Foe.Base.Def << "]" << " RageT: " << Foe.RDP.amount << endl;
	cout << "==============================" << endl;
	wait(500);
}

int main()
{
	srand(time(NULL));
	vector<Item> Inventory;
	vector<vector<Option>> PlayerOptions;
	vector<Option> FoeOptions;
	vector<Unit> Enemies;
	Enemies.push_back(Unit("Goblin", 2));
	Enemies.push_back(Unit("HobGoblin", 3));
	Enemies.push_back(Unit("Rat", 1));
	Enemies.push_back(Unit("Owlbear", 4));


	vector<Option> T;
	T.push_back(Option("Attack", new Item(0, 0, 0)));
	T.push_back(Option("Move", new Item(0, 0, 0)));
	T.push_back(Option("Use", new Item(0, 0, 0)));
	T.push_back(Option("wait", new Item(0, 0, 0)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Cut", new Item(1, 0, -1)));
	T.push_back(Option("Block", new Item(0, 1, -1)));
	T.push_back(Option("Slash", new Item(2, 0, -2)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Lunge", new Item(1, 0, -2)));
	T.push_back(Option("Volte", new Item(0, 1, -1)));
	T.push_back(Option("Retreat", new Item(0, 2, -2)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Apple", new Item(-10, 0, 1)));
	T.push_back(Option("Potion", new Item(-10, 0, 2)));
	T.push_back(Option("Spell", new Item(-10, 5, 0)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("and wait", new Item(0, 0, 1)));
	PlayerOptions.push_back(T);

	FoeOptions.push_back(Option("Bite", new Item(1, 0, -1)));
	FoeOptions.push_back(Option("Slash", new Item(2, 0, -2)));
	FoeOptions.push_back(Option("Stud", new Item(0, 1, -2)));
	FoeOptions.push_back(Option("Heal", new Item(-10, 2, 2)));
	Unit Foe = Enemies[rand() % Enemies.size()];
	Unit Player;
	cout << unitbuf;
	cout << "Hello stranger, i am "<<Foe.name<<" state your name!: ";
	cin >> Player.name;
	cout << "Tell me," << Player.name << " your level: ";
	int cr;
	cin >> cr;
	Player = Unit(Player.name, cr);
	cout << Player.name << " you ows me money! Time to fight!" << endl;
	ShowStats(Player, Foe);
	system("PAUSE");

	Foe.RDP.amount = 5;
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
		cout << endl <<Player.name<<", white dices:" ;
		for (int i = 0; i < Player.Base.BDY+Player.SubDicePool; i++)
		{							
		
			ColoredDice white(WHITE);
			white.Throw();
			BDYDices.push_back(white);
			cout <<"    ";
		}
		cout << endl <<Foe.name<<", black dices:";
		for (int i = 0; i < Foe.Base.BDY+Foe.SubDicePool; i++)
		{
			ColoredDice black(BLACK);
			black.Throw();
			BDYDices.push_back(black);
			cout << "    ";

		}
		Player.SubDicePool = 0;
		Foe.SubDicePool = 0;
		
		sort(BDYDices.begin(), BDYDices.end(), CheckDices);
		int ActionsNum = 0;
		cout << "\n====TOTAL:===" << endl;
		for (int i = 0; i < BDYDices.size(); i++)
		{
			cout << i + 1 << ((BDYDices[i].color == WHITE) ? (" w") : (" b")) << ":" << BDYDices[i].NUM << "   ";
		}
		wait(2000);
		if (Player.RDP.amount > 0)
		{
			cout << "\nReroll how many(0-none): ";
			int rr = 0;
			cin >> rr;
			if (rr > 0 && Player.RDP.amount >= rr)
			{

				int num;
				for (int i = 0; i < rr; i++)
				{					
					cout << "Which one?:";
					cin >> num;
					if (Player.useRage())
					{
						BDYDices[num - 1].Throw();
						cout << "new value: " << BDYDices[num - 1].NUM << endl;
					}
					else
					{
						cout << endl << Player.name << " failed in reroll, loses his actions" << endl;
						Foe.Stamina = 1;						
						rr = -10;
					}
				}
			}
		}
		wait(500);
		if (Foe.dna.CheckBDY() && Foe.RDP.amount > 0)
		{
			cout << endl <<Foe.name<<" rerolls:";
			if (Foe.useRage())
			{
				int r = rand() % BDYDices.size();
				BDYDices[r].Throw();				
			}
			else
			{				
				cout << endl << Foe.name << " failed in reroll, loses his actions" << endl;
				Player.Stamina=1;
			}
		}

		
		if (!Player.isRaged && !Foe.isRaged)
		{
			sort(BDYDices.begin(), BDYDices.end(), CheckDices);
			wait(500);
			cout << "\n====Collecting dices" << endl;
			system("PAUSE");
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
				cout<< "Top dice is WHITE!"<<endl;
				Foe.Stamina=0;
				Player.Stamina = ActionsNum;
			}
			else {
				cout<< "Top dice is BLACK!" << endl;
				Player.Stamina = 0;
				Foe.Stamina = ActionsNum;
			}
			
		}
		wait(500);
		system("CLS");
		ShowStats(Player, Foe);
		if (Player.Stamina>0)
		{
			cout << endl<< "Player wins " << Player.Stamina << " actions" << endl;			
			Foe.RDP.Push();
			while (Player.Stamina > 0)
			{
				cout << "\nSelect("<<Player.Stamina<<" ap left):\n1: Attack(1 ap)"<<endl;
				cout << "2: Retreat(1 ap)" << endl;
				cout << "3: Strafe(1 ap)" << endl;
				cout << "4: Defend("<<Player.Stamina<<" ap)"<<endl;
				wait(500);
				int n;
				int R = 0;
				int c = 1;
				cin >> n;
				cout << "\r             \r";
				switch (n)
				{
				case 1:
					Player.Stamina--;
					R = xd6(1);
					cout << "You throw: " << R;
					if (Player.RDP.amount > 0)
					{
						while (c > 0)
						{
							cout << " reroll(0 -none, 1-RageToken)?" << endl;
							cin >> c;
							if (c != 0)
							{
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "You lose your grip and hit yourself! " << endl;
									ActionsNum = 0;
									Player.HP -= 1;
									Foe.HP -= 1;
								}
							}
						}
					}
					R=R+Player.Base.Att / 2;
					if (R > Foe.Base.Def)
					{
						Foe.HP -= Player.Base.Att;
						cout << " it's a Hit, " << Foe.name << " HP:"<<Foe.HP<<endl;
						
					}
					else if (R == Foe.Base.Def)
					{
						
						Foe.HP -= Player.Base.Att * 2;
						cout << " it's a Critical Hit,"<<Foe.name<<" HP: "<< Foe.HP << endl;
					}
					else
					{
						cout << " and Missed!" << endl;
					}
					break;
				case 2:
					Player.Stamina--;
					R = xd6(1);
					cout << "You throw: " << R;
					if (Player.RDP.amount > 0)
					{
						while (c > 0)
						{
							cout <<" reroll(0 -none, 1-RageToken)?" << endl;
							cin >> c;
							if (c != 0)
							{
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{	
									cout << "You stepped on the thorn and lose initiative!";
									Player.SubDicePool--;									
								}
							}
						}
					}
					if (Player.RDP.amount<R&&Player.RDP.amount>0)
					{
						Player.RDP.amount--;
						cout << " and retreated" << Player.name << " loses one rage token" << endl;
					}
					else
					{
						cout << " and missteped";
					}
					break;
				case 3:
					Player.Stamina--;
					R = xd6(1);
					cout << "You throw: " << R;
					if (Player.RDP.amount > 0)
					{
						while (c > 0)
						{
							cout << " reroll(0 -none, 1-RageToken)?" << endl;
							cin >> c;
							if (c != 0)
							{
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "You stepped on the thorn and lose initiative!";
									Player.SubDicePool--;
								}
							}
						}
					}
					if (Foe.RDP.amount<R)
					{
						Foe.RDP.amount++;
						cout << " and strafed" << Foe.name << " gets one rage token" << endl;
					}
					else
					{
						cout << " and missteped" << endl;
					}
					break;
				case 4:
					cout << "Stand Ground!" << endl;
					Player.SubDicePool += ActionsNum;
					Player.Stamina = 0;
					break;
				default:
					break;
				}
			}
			
		}
		else if (Foe.Stamina>0)
		{
			cout << Foe.name<<" wins " << Foe.Stamina << " actions" << endl;
			Player.RDP.Push();
			while (Foe.Stamina > 0)
			{
				cout << endl;
				int n;
				int R = 0;
				int c = 1;
				n = rand()%4 + 1;
				switch (n)
				{
				case 1:
					Foe.Stamina--;
					R = xd6(1);
					cout <<Foe.name <<" attack throw: " << R;
					if (Foe.RDP.amount > 0)
					{
					
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									Player.HP -= 1;
									Foe.HP -= 1;
								}
							
						
					}
					R = R + Player.Base.Att / 2;
					if (R > Player.Base.Def)
					{						
						Player.HP -= Foe.Base.Att;
						cout << " it's a Hit, " << Player.name << " HP:" << Player.HP << endl;
					}
					else if (R == Player.Base.Def)
					{
						
						Player.HP -= Foe.Base.Att * 2;
						cout << " it's a Critical Hit, " << Player.name << " HP:" << Player.HP << endl;
					}
					else
					{
						cout << " and Missed!" << endl;
					}
					break;
				case 2:
					Foe.Stamina--;
					R = xd6(1);
					cout << Foe.name<<" move throw: " << R;
					if (Foe.RDP.amount > 0)
					{
						
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{	
									cout <<Foe.name<<" stepped on the thorn and lose initiative!";
									Foe.SubDicePool--;
								}
							
						
					}
					if (Foe.RDP.amount<R&&Foe.RDP.amount>0)
					{
						Foe.RDP.amount--;
						cout << " and retreated" << Foe.name << " loses one rage token" << endl;
					}
					else
					{
						cout << " and missteped" << endl;
					}
					break;
				case 3:
					Foe.Stamina--;
					R = xd6(1);
					cout << Foe.name<<" move throw: " << R;
					if (Foe.RDP.amount > 0)
					{

						if (Player.RDP.Acquire())
						{
							R = xd6(1);
						}
						else
						{
							cout << Foe.name << " stepped on the thorn and lose initiative!";
							Foe.SubDicePool--;
						}


					}
					if (Player.RDP.amount<R)
					{
						Player.RDP.amount++;
						cout << " and strafed."<<Player.name<<" gets one rage token"<< endl;
					}
					else
					{
						cout << " and missteped!" << endl;
					}
					break;
				case 4:
					cout <<endl<<Foe.name<<" Stand Ground!" << endl;
					Foe.SubDicePool += ActionsNum;
					Foe.Stamina = 0;
					break;
				default:
					break;
				}
			}
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

