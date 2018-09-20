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
#include <fcntl.h>
#include <io.h>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
stringstream Buffer;
streambuf *old;
bool isGodmode;
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
			wait(5);
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


void pd(string M, int pause = 10)
{

	for (int m = 0; m < M.length(); m++)
	{
		/*if (rand() % 5 == 4&&m>0)
		{
			cout << (char)(rand() % 30+40);
			wait(50);
			cout << "\b";
		}*/
		cout << M[m];
		wait(pause);
	}
}

void flushText(int pause = 10)
{
	cout.rdbuf(old);
	pd(Buffer.str(), pause);
	old = cout.rdbuf(Buffer.rdbuf());
	Buffer.str(string());
}

void textcolor(int color)
{
	flushText(20);
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleHandle, color);
}

struct Item
{
	string name;
	int Att;
	int Def;
	int BDY;
	int SubDice;
	int HP;
	int RC;
	void Use()
	{
		SubDice = 0;
		HP = 0;
		RC = 0;
		Att = Def = BDY = 0;
		name = "";
	}
	Item()
	{
		SubDice = 0;
		HP = 0;
		RC = 0;
		Att = Def = BDY = 0;
		name = "";
	}
	Item(int att, int def, int bd, string nm, int hp = 0, int rt = 0, int sd = 0)
	{
		Att = att;
		Def = def;
		BDY = bd;
		name = nm;
		SubDice = sd;
		HP = hp;
		RC = rt;
	}
};

struct RageDicePool
{
	int amount;
	RageDicePool()
	{
		amount = 0;
	}
	void Push(int a = 1)
	{
		amount += 1;
		if (amount > 6)
			amount = 6;
	}
	void Pop(int a = 1)
	{
		amount -= a;
		if (amount < 0)
			amount = 0;
	}

	bool Acquire()
	{
		bool retval = true;
		if (amount > 0)
		{
			textcolor(Red);
			cout << "\nRed dice:";
			wait(100);
			if (xd6(1) <= amount)
			{
				retval = false;
				amount = 0;
				cout << endl;
				textcolor(Magenta);
				for (int i = 0; i < 5; i++)
				{

					cout << "RAAAAAGE!";
					wait(50);
					cout << "\r         \r";
					wait(50);
				}
			}
			else
			{
				cout << endl;
				textcolor(Green);
				amount--;
				for (int i = 0; i < 5; i++)
				{

					cout << "Cool!";
					wait(50);
					cout << "\r         \r";
					wait(50);
				}
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
		" pat!  ",
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


string GetFriendRoomText()
{
	string Texts[7] = {
		" none",
		" a shady dormitory, beds are almost broken, \nbut one is still has bedroll and hay pillow. ",
		" a small mramored room with small basin in the\n middle, lightss dancing and flickers on the walls and ceilng",
		" an abandoned tavern, fire pit is still hot, \nthere are some smoked meat and a barrel of ale under the counter",
		" a library, books are everythere, even laying\n in heaps on the floor, candels are dangerously near dried scrolls",
		" a dusty ceremonial hall, throne is empty, some\n armor and swords hanging from the walls, banners with lions and griffons silently hangs from poles",
		" an abandoned armory, weapon racks are empty, \nguards lost their rations on the table and a ring with keys."
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}

string GetFoeRoomText()
{
	string Texts[7] = {
		" none",
		" dusty webs and gossamers are hanging around, \nthere is something lurking in the corner",
		" a room full of bleached bones, dried blood \nand tattered rags",
		" an abandoned tavern, broken chairs, dusty \ntables, kitchen utensils and tableware laying around",
		" a torture room, embers glowing in the fire\n pit, chains, racks, and a big skeleton in the rusty cage",
		" a mostly destroyed by magic explosion ceremonial hall,\n human-like shadows on the floor, burned banners hanging from ceilng, rusted and melted weapons laying on the floor.",
		" a former prison hall, cells are opened, \nflooded mud soaking on the floor, moss and dipping water covering walls"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}
enum DNAType
{
	Chaotic,
	Attacker,
	Defender,
	Strafer,
	Charger,
	Rager
};

enum Perk
{
	None,
	Cold,
	Fiechter,
	Dancer,
	Tactitian
};

string Perks[5]
{
	"None",
	"Cold",
	"Fiechter",
	"Dancer",
	"Tactitian"
};

struct DNA
{
	DNA(DNAType d = DNAType::Attacker)
	{
		dt = d;
	}
	DNAType dt;
	bool Rerollprob(int RDPamnt)
	{
		if (RDPamnt == 0)
		{
			return false;
		}
		if (dt == Rager || dt == Attacker)
		{
			return true;
		}
		if (RDPamnt > 3)
		{
			return dt == Rager;
		}
		return false;
	}
	int GetAction(int RDPamnt, int HP, int MaxHP, int Stamina)
	{
		int retval = 1;
		switch (dt)
		{
		case Chaotic:
			retval = rand() % 5 + 1;
			break;
		case Attacker:
			if (HP > MaxHP / 3)
			{
				retval = 1;
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Defender:
			if (HP > MaxHP / 3)
			{
				retval = 2;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Strafer:
			if (HP > MaxHP / 3)
			{
				retval = 3;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Charger:
			if (HP > MaxHP / 3 && Stamina > 1)
			{
				retval = 4;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Rager:
			if (RDPamnt > 2)
			{
				retval = 1;
			}
			else
			{
				retval = rand() % 2 + 1;
			}
			break;
		}
		return retval;
	}

};

struct ColoredDice
{
	ColoredDice(tcolor dc)
	{
		color = dc;
	}
	ColoredDice(tcolor dc, int n)
	{
		color = dc;
		NUM = n;
	}
	tcolor color;
	int NUM;
	void Throw(bool isShow = true)
	{
		NUM = xd6(1, isShow);
	}
};


bool CheckDices(ColoredDice CD1, ColoredDice CD2)
{
	return (CD1.NUM < CD2.NUM);
}

struct DicePool
{
	vector<ColoredDice> Dices;
	void Clear()
	{
		Dices.clear();
	}
	void MakeThrow(int n, tcolor c, bool isHidden = false)
	{
		for (int i = 0; i < n; i++)
		{
			textcolor(c << 4);
			ColoredDice D(c);
			D.Throw();
			Dices.push_back(D);
		}
		textcolor(White);
	}

	ColoredDice GetMaxColoredSeq()
	{
		int ActionsNum = 0;
		sort(Dices.begin(), Dices.end(), CheckDices);
		tcolor c = Dices.back().color;
		int lastDice = Dices.back().NUM;
		int lastChunk = 0;
		for (int i = Dices.size() - 1; i >= 0; i--)
		{

			if (Dices[i].color != c)
			{
				if (Dices[i].NUM != lastDice)
				{
					lastChunk = 0;
				}
				break;
			}
			else
			{
				if (Dices[i].NUM == lastDice)
				{
					lastChunk++;
				}
				else
				{
					lastChunk = 1;
				}
				if (i == 0)
				{
					lastChunk = 0;
				}
				lastDice = Dices[i].NUM;
				ActionsNum++;
			}

		}

		ActionsNum -= lastChunk;
		return ColoredDice(Dices.back().color, ActionsNum);
	}
	void ShowPool(bool isShaded = false, int s = 0)
	{
		sort(Dices.begin(), Dices.end(), CheckDices);
		textcolor(0x0F);
		cout << "\n=========DICE POOL==================" << endl;
		cout << endl;
		if (!isShaded)
		{
			for (int i = 0; i < Dices.size(); i++)
			{

				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
		}
		else
		{
			for (int i = 0; i < Dices.size(); i++)
			{

				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
			cout << "\r";
			s = min(Dices.size() - s, Dices.size());
			for (int i = 0; i < s; i++)
			{
				wait(300);
				textcolor(DarkGray << 4 | DarkGray);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
			for (int i = s; i < Dices.size(); i++)
			{
				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
		}
		cout << endl;
		textcolor(0x0F);
		cout << "\n==================================" << endl;
		textcolor(0x0F);
	}

};


struct Unit
{
	vector<string> wounds;
	Item Base;
	Item InHand;
	Item Used;
	Perk myPerk;
	vector<Item> Inventory;
	vector<Item> Belt;
	int HP;
	int MaxHP;
	int Stamina;
	RageDicePool RDP;
	bool isRaged;
	int SubDicePool;
	DNA dna;
	string name;

	void PushRt(int a)
	{
		if (myPerk != Cold)
		{
			RDP.Push(a);
		}
		else {
			cout << " cold perk saves!";
		}
	}

	int AskForReroll(int oldR, bool isAI, bool isRageEnable, DicePool *DP = NULL)
	{
		char c = 'y';
		vector<int> rerolldices;
		int R = oldR;
		if (!isAI)
		{

			while (c != 'n'&&RDP.amount > 0)
			{

				cout << " reroll(y/n)?" << endl;
				flushText();
				cin >> c;

				if (c != 'n')
				{
					if (DP != NULL)
					{
						cout << RDP.amount << "RT, input numbers e.g. 1,4,7: ";
						flushText();
						string rr;
						cin >> rr;
						stringstream ss(rr);

						int i;
						while (ss >> i && rerolldices.size() < RDP.amount)
						{
							if (i > 0 && i <= DP->Dices.size())
							{
								rerolldices.push_back(i - 1);
							}
							if (ss.peek() == ',')
							{
								ss.ignore();
							}
						}
						for (int rt = 0; rt < rerolldices.size(); rt++)
						{
							R = DP->Dices[rerolldices[rt]].NUM;
							if (RDP.Acquire())
							{
								int F = xd6(1);
								if (myPerk == Tactitian)
								{
									textcolor(Green);
									cout << " tactical desicion (" << R << "<->" << F << ") :";
									if (DP->Dices[rerolldices[rt]].color == White)
									{
										R = max(R, F);
									}
									else
									{
										R = min(R, F);
									}
									cout << R << endl;
								}
								else {
									R = F;
								}
								DP->Dices[rerolldices[rt]].NUM = R;
							}
							else if (isRageEnable)
							{
								isRaged = true;
								textcolor(Magenta);
								GetBrokeText();
								Stamina = 0;
								R = 0;
								break;
							}
							else
							{
								R = 0;
								textcolor(Green);
								cout << " perks saves!" << endl;
								break;
							}
						}
					}
					else
					{
						if (RDP.Acquire())
						{
							int F = xd6(1);
							if (myPerk == Tactitian)
							{
								cout << " tactical desicion (" << R << "<->" << F << ") :";
								textcolor(Green);
								R = max(R, F);
								cout << R << endl;
							}
							else {
								R = F;
							}
						}
						else if (isRageEnable)
						{
							isRaged = true;
							textcolor(Magenta);
							GetBrokeText();
							Stamina = 0;
							R = 0;
							break;
						}
						else
						{
							R = 0;
							textcolor(Green);
							cout << " perks saves!" << endl;
							break;
						}
					}

				}
				flushText();

			}

		}
		else
		{
			if (DP != NULL)
			{

				int numtoreroll = rand() % RDP.amount;
				if (numtoreroll > 0)
				{
					cout << name << " choose to reroll: ";
				}
				for (int i = 0; i < numtoreroll; i++)
				{
					rerolldices.push_back(rand() % DP->Dices.size());
					cout << rerolldices.back() + 1 << ",  ";
				}


				for (int rt = 0; rt < rerolldices.size(); rt++)
				{
					R = DP->Dices[rerolldices[rt]].NUM;
					if (RDP.Acquire())
					{
						int F = xd6(1);
						if (myPerk == Tactitian)
						{
							textcolor(Green);
							cout << " tactical desicion (" << R << "<->" << F << ") :";
							if (DP->Dices[rerolldices[rt]].color != White)
							{
								R = max(R, F);
							}
							else
							{
								R = min(R, F);
							}
							cout<< R << endl;
						}
						else {
							R = F;
						}
						DP->Dices[rerolldices[rt]].NUM = R;
						DP->ShowPool();
					}
					else if (isRageEnable)
					{
						isRaged = true;
						textcolor(Magenta);
						GetBrokeText();
						Stamina = 0;
						R = 0;
						break;
					}
					else
					{
						R = 0;
						textcolor(Green);
						cout << " perks saves!" << endl;
						break;
					}
				}
			}
			else
			{
				if (RDP.Acquire())
				{
					int F = xd6(1);
					if (myPerk == Tactitian)
					{
						textcolor(Green);
						cout << " tactical desicion (" << R << "<->" << F << ") :" << R << endl;
						R = max(R, F);
						cout << R << endl;
					}
					else {
						R = F;
					}
				}
				else if (isRageEnable)
				{
					isRaged = true;
					textcolor(Magenta);
					GetBrokeText();
					Stamina = 0;
					R = 0;
				}
				else
				{
					R = 0;
					textcolor(Green);
					cout << " perks saves!" << endl;

				}
			}

		}
		textcolor(White);
		flushText();
		return R;

	}

	void DamageMe(int d, bool isCritical = false)
	{
		if (d > Base.BDY)
		{
			isCritical = true;
		}
		if (!isCritical)
		{

			HP--;
			textcolor(LightMagenta);
			cout << endl << name << " wounded ( ";
			wounds.push_back(GetWoundText());
			cout << " )";
		}
		else
		{
			HP -= 2;
			Base.BDY -= rand() % 2;
			textcolor(LightRed);
			cout << endl << name << "heavy wounded ( ";
			wounds.push_back("heavy " + GetWoundText());
			cout << " )";
		}
		textcolor(White);
		flushText();
	}


	void TakeoffItem()
	{
		
		HP = max(HP - InHand.HP, 1);
		SubDicePool -= InHand.SubDice;
		RDP.amount -= InHand.RC;
		Base.Att -= InHand.Att;
		Base.Def -= InHand.Def;
		Base.BDY -= InHand.BDY;
		if (InHand.name.length()>1)
		{
			Inventory.push_back(InHand);
		}		
		InHand = Item();		
	}

	void WearItem()
	{
		TakeoffItem();
		InHand = Used;
		HP = min(HP + InHand.HP, MaxHP);
		SubDicePool += InHand.SubDice;
		RDP.amount += InHand.RC;
		Base.Att += InHand.Att;
		Base.Def += InHand.Def;
		Base.BDY += InHand.BDY;
		Used.Use();
	}

	void UseItem()
	{
		HP = min(HP + Used.HP, MaxHP);
		SubDicePool += Used.SubDice;
		RDP.amount += Used.RC;
		Base.Att += Used.Att;
		Base.Def += Used.Def;
		Base.BDY += Used.BDY;
		Used.Use();
	}
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

	int CollectDamageMod()
	{
		int R = Base.BDY / 3 + (myPerk == Fiechter ? 1 : 0);
		return R;
	}

	Unit(int a, int d, int x, string s)
	{
		SubDicePool = 0;
		Base.Att = a;
		Base.Def = d;
		Base.BDY = x;
		HP = Base.BDY + 2;
		MaxHP = Base.BDY + 2;
		Stamina = Base.BDY;
		name = s;
	}
	Unit()
	{
		Base = Item(1, 0, 1,"fist");
		SubDicePool = 0;
		HP = 3;
		MaxHP = 3;
		Stamina = 1;
		name = "";
	}
	Unit(string s, int CR)
	{
		SubDicePool = 0;
		Base.Att = 1 + CR / 3;
		Base.Def = CR / 3;
		Base.BDY = CR;
		HP = CR + 2 + rand() % 3 - 1;
		MaxHP = HP;
		Stamina = Base.BDY;
		name = s;

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






void ShowStats(Unit Player, Unit Foe)
{
	textcolor(White);
	cout << endl;
	cout << "                      BATTLE STATS:" << endl;
	cout << "____________________________________" << endl;
	cout << "|" << Player.name << " the "<< Perks[Player.myPerk]<<" [BDY:" << Player.Base.BDY << ",  W " << Player.Base.Att << ", A " << Player.Base.Def << "]" << " ad:" << Player.SubDicePool << endl;
	cout << "|" << "in hand: " << Player.InHand.name << " [BDY:" << Player.InHand.BDY << ",  W " << Player.InHand.Att << ", A " << Player.InHand.Def << "]" << " ad:" << Player.InHand.SubDice << endl;
	cout << "|" << " RT: ";
	textcolor(Magenta);
	for (int i = 0; i < Player.RDP.amount; i++)
	{
		cout << "(0) ";
	}
	cout << endl;
	textcolor(White);
	cout << "|" << " HP :";
	textcolor(Green);
	for (int i = 0; i < Player.HP; i++)
	{
		cout << "(+) ";
	}
	textcolor(Red);
	for (int i = Player.HP; i < Player.MaxHP; i++)
	{
		cout << "(X)  ";
	}
	cout << endl;
	textcolor(White);

	cout << "|" << " Backpack: ";
	textcolor(Cyan);
	for (Item S : Player.Inventory)
	{

		cout << S.name << ", ";
	}
	cout << endl;
	textcolor(White);
	cout << "|" << " Belt: ";
	textcolor(Cyan);
	for (Item S : Player.Belt)
	{

		cout << S.name << ", ";
	}
	cout << endl;
	textcolor(White);
	cout << "|" << " Last used:" << Player.Used.name << endl;
	cout << "|" << " Wounds: ";
	textcolor(Red);
	for (string S : Player.wounds)
	{

		cout << S << ", ";
	}

	textcolor(White);
	cout << endl << "|" << "==============================" << endl;
	cout << "|" << Foe.name <<" the "<<Perks[Foe.myPerk]<<endl;
	cout << "|" << " RT: ";
	textcolor(Magenta);
	for (int i = 0; i < Foe.RDP.amount; i++)
	{
		cout << "(0) ";
	}
	textcolor(White);
	cout << endl << "|" << " HP :";
	textcolor(Green);
	for (int i = 0; i < Foe.HP; i++)
	{
		cout << "(+) ";
	}
	textcolor(Red);
	for (int i = Foe.HP; i < Foe.MaxHP; i++)
	{
		cout << "(X)  ";
	}
	textcolor(White);
	cout << endl << "|" << " Wounds: ";
	textcolor(Red);
	for (string S : Foe.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << endl << "|______________________________________" << endl;
	textcolor(White);
}


void ShowStatsMin(Unit Player, Unit Foe)
{
	textcolor(White);
	cout << endl;
	cout << "                      QUICK STATS:" << endl;
	cout << "\n===============================================" << endl;
	cout << "|" << Player.name << endl << "|" << "  HP:";
	textcolor(Green);
	for (int i = 0; i < Player.HP; i++)
	{
		cout << "(+) ";
	}
	textcolor(LightRed);
	for (int i = Player.HP; i < Player.MaxHP; i++)
	{
		cout << "(X)  ";
	}
	cout << endl;
	textcolor(White);
	cout << "|" << "  RT: ";
	textcolor(LightRed);
	for (int i = 0; i < Player.RDP.amount; i++)
	{
		cout << "(0) ";
	}
	cout << endl;
	textcolor(White);
	cout << "|------------------------- " << endl;
	cout << "|" << Foe.name << endl << "|" << "  HP:";
	textcolor(0x02);
	textcolor(Green);
	for (int i = 0; i < Foe.HP; i++)
	{
		cout << "(+) ";
	}
	textcolor(LightRed);
	for (int i = Foe.HP; i < Foe.MaxHP; i++)
	{
		cout << "(X)  ";
	}
	cout << endl;
	textcolor(White);
	cout << "|" << "  RT:";
	textcolor(LightRed);
	for (int i = 0; i < Foe.RDP.amount; i++)
	{
		cout << "(0) ";
	}
	cout << endl;
	textcolor(0x0F);
	cout << "|" << "===============================================" << endl;
	textcolor(White);
}


void UseItem(Unit *Player, bool isRested = true, bool isAI = false, bool isShowDices = true)
{
	if (isRested)
	{
		if (!isAI)
		{
			int R = xd6(1, isShowDices);
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Belt.size())
			{
				cout << "Belt:" << endl;
				for (int i = 0; i < Player->Belt.size(); i++)
				{
					Item ii = Player->Belt[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				flushText();
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Belt[itemnum];
			Player->Used = SR;
			Player->Belt[itemnum] = Player->Belt.back();
			Player->Belt.pop_back();
			cout << "uses: " << Player->Used.name << " (" << R << "vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
			}

			if (R > 0)
			{
				R = R + (Player->myPerk == Cold ? 1 : 0);
				if (R > Player->RDP.amount - Player->Stamina)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->UseItem();

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " opens pocket and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);

			}
			else
			{
				cout << endl << Player->name << " opens pocket and";
				GetBrokeText();
				cout << endl;
				Player->Used.Use();
			}

		}
	}
	flushText();
}

void WearItem(Unit *Player, bool isRested = true, bool isAI = false, bool isShowDices = true)
{
	if (isRested)
	{
		if (!isAI)
		{
			int R = xd6(1, isShowDices);
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Inventory.size())
			{
				cout << "Bagpack:" << endl;
				for (int i = 0; i < Player->Inventory.size(); i++)
				{
					Item ii = Player->Inventory[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				flushText();
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Inventory[itemnum];
			Player->Used = SR;
			Player->Inventory[itemnum] = Player->Inventory.back();
			Player->Inventory.pop_back();
			cout << "uses: " << Player->Used.name << " (" << R << "vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
			}

			if (R > 0)
			{
				R = R + (Player->myPerk == Cold ? 1 : 0);
				if (R > Player->RDP.amount - Player->Stamina)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->UseItem();

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " opens bag and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);

			}
			else
			{
				cout << endl << Player->name << " opens bag and";
				GetBrokeText();
				cout << endl;
				Player->Used.Use();
			}

		}
	}
	flushText();
}


void MakeActions(Unit *Player, Unit *Foe, bool isAI, bool isShowDices = true)
{
	cout << endl << Player->name << " wins " << Player->Stamina << " actions," << Foe->name << " get 1 RT" << endl;
	ShowStatsMin(*Player, *Foe);
	Foe->PushRt(1);
	while (Player->Stamina > 0)
	{
		int n = 0;
		int R = 0;
		if (!isAI)
		{
			while (n <= 0 || n > 8)
			{
				cout << "\nSelect(" << Player->Stamina << " ap left):" << endl;
				cout << "1: Attack(1 ap)" << endl;
				cout << "2: Retreat(1 ap)" << endl;
				cout << "3: Strafe(1 ap)" << endl;
				cout << "4: Charge(2 ap)" << endl;
				cout << "5: Defend(" << Player->Stamina << " ap)" << endl;
				cout << "6: Use(" << Player->Stamina << " ap)" << endl;
				cout << "7: Wear(" << Player->Stamina << " ap)" << endl;
				cout << "8: Takeoff(" << Player->Stamina << " ap)" << endl;
				cout << "Enter 1-8:";
				flushText(30);
				cin >> n;
				cout << "\r             \r";
			}
		}
		else
		{
			n = Player->dna.GetAction(Player->RDP.amount, Player->HP, Player->MaxHP, Player->Stamina);
		}

		cout << endl << Player->name << " makes [";
		R = xd6(1, isShowDices);
		cout << "]";
		switch (n)
		{
		case 1:
			Player->Stamina--;
			cout << " an attack ( " << R << " vs " << Foe->name << "'s " << Foe->Base.Def << "DEF)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Fiechter));
			}
			if (R != 0)
			{
				R = R + (Player->myPerk == Fiechter ? 1 : 0);
				if (R >= Foe->Base.Def)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->DamageMe(Player->Base.Att, R == Foe->Base.Def);
					cout << endl << Foe->name << "  HP:" << Foe->HP << endl;

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " attacks and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				Player->DamageMe(1); //Player->att-- ?
				Foe->DamageMe(0);
				Player->Stamina = 0;
			}

			break;
		case 2:
			Player->Stamina--;
			cout << " a retreat ( " << R << " vs " << Player->name << "'s " << Player->RDP.amount << "RT)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Dancer));
			}

			if (R != 0)
			{
				R = R + (Player->myPerk == Dancer ? 1 : 0);
				if (R > Player->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->RDP.Pop();
					cout << endl << Player->name << "  RT:" << Player->RDP.amount << endl;

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " retreats and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				Player->SubDicePool--;
				Player->Stamina = 0;
			}
			break;
		case 3:
			Player->Stamina--;
			cout << " a strafe ( " << R << " vs " << Foe->name << "'s " << Foe->RDP.amount << "RT)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Dancer));
			}

			if (R != 0)
			{
				R = R + (Player->myPerk == Dancer ? 1 : 0);
				if (R > Foe->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->PushRt(1);
					cout << endl << Foe->name << "  RT:" << Foe->RDP.amount << endl;

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " retreats and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				Player->SubDicePool--;
				Player->Stamina = 0;
			}
			break;
		case 4:
			if (Player->Stamina >= 2)
			{
				Player->Stamina -= 2;
				cout << " a charge ( " << R << " vs " << Foe->name << "'s " << Foe->Base.Def << "DEF)" << endl;
				if (Player->RDP.amount > 0)
				{
					R = Player->AskForReroll(R, isAI, Player->myPerk != Dancer && Player->myPerk != Fiechter);
				}

				if (R != 0)
				{
					R = R + (Player->myPerk == Dancer ? 1 : 0) + (Player->myPerk == Fiechter ? 1 : 0);
					if (R > Foe->Base.Def)
					{
						textcolor(Green);
						cout << " success!" << endl;
						Foe->DamageMe(Player->Base.Att, true);
						cout << endl << Foe->name << "  HP:" << Foe->HP << endl;

					}
					else
					{
						textcolor(Magenta);
						cout << " fails!" << endl;
						cout << endl << Player->name << " charges and";
						GetBrokeText();
						cout << endl;
					}
					textcolor(White);
				}
				else
				{
					Player->DamageMe(3); //Player->att-- ?
					Player->SubDicePool--;
					Foe->DamageMe(1);
					Player->Stamina = 0;
				}
			}
			break;
		case 5:
			Player->SubDicePool += Player->Stamina + (Player->myPerk == Cold ? 1 : 0);
			cout << " defence and stands ground!" << endl;
			Player->Stamina = 0;
			break;
		case 6:
		{
			cout << " a move into pocket,"<<endl;
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Belt.size())
			{
				cout << "Belt:" << endl;
				for (int i = 0; i < Player->Belt.size(); i++)
				{
					Item ii = Player->Belt[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				textcolor(White);
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Belt[itemnum];
			Player->Used = SR;
			Player->Belt[itemnum] = Player->Belt.back();
			Player->Belt.pop_back();
		}
		cout << " takes a: " << Player->Used.name << " ( " << R << " vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
		if (Player->RDP.amount > 0)
		{
			R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
		}

		if (R > 0)
		{
			R = R + (Player->myPerk == Cold ? 1 : 0);
			if (R > Player->RDP.amount - Player->Stamina)
			{
				textcolor(Green);
				cout << endl << Player->name << " opens pocket and use "<<Player->Used.name;
				Player->UseItem();

			}
			else
			{
				textcolor(Magenta);				
				cout << endl << Player->name << " opens pocket and ";
				GetBrokeText();
				cout << endl;
			}
			textcolor(White);

		}
		else
		{
			cout << endl << Player->name << " opens pocket,"<<Player->Used.name<<" falls on the ground and ";
			GetBrokeText();
			cout << endl;
			Player->Used.Use();
		}
		Player->Stamina = 0;
		break;
		case 7:
		{
			cout << " a move into backpack," << endl;
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Inventory.size())
			{
				cout << "Inventory:" << endl;
				for (int i = 0; i < Player->Inventory.size(); i++)
				{
					Item ii = Player->Inventory[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				textcolor(White);
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Inventory[itemnum];
			Player->Used = SR;
			Player->Inventory[itemnum] = Player->Inventory.back();
			Player->Inventory.pop_back();
		}
		cout << " and takes out: " << Player->Used.name << " ( " << R << " vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
		if (Player->RDP.amount > 0)
		{
			R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
		}

		if (R > 0)
		{
			R = R + (Player->myPerk == Cold ? 1 : 0);
			if (R > Player->RDP.amount - Player->Stamina)
			{
				textcolor(Green);
				cout << " success!" << endl;
				Player->WearItem();

			}
			else
			{
				textcolor(Magenta);
				cout << " fails!" << endl;
				cout << endl << Player->name << " opens bag and";
				GetBrokeText();
				cout << endl;
			}
			textcolor(White);

		}
		else
		{
			cout << endl << Player->name << " opens bag and";
			GetBrokeText();
			cout << endl;
			Player->Used.Use();
		}
		Player->Stamina = 0;
		break;
		case 8:
			Player->Stamina--;
			cout << " a move to take of: " << Player->InHand.name << " ( "<<R<<" vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
			}
			if (R > 0)
			{
				R = R + (Player->myPerk == Cold ? 1 : 0);
				if (R > Player->RDP.amount - Player->Stamina)
				{
					textcolor(Green);
					cout << " success!" << endl;										
					Player->TakeoffItem();

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " ";
					Player->InHand.Use();
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);

			}
			else
			{
				cout << endl << Player->name << " ";
				GetBrokeText();
				cout << endl;
				Player->InHand.Use();
			}
			break;

		default:
			break;
		}
		flushText();
	}
}


bool Battle(Unit *Player, Unit *Foe)
{
	bool isPlayerWin = true;
	while (Player->HP > 0 && Foe->HP > 0)
	{
		system("CLS");
		ShowStats(*Player, *Foe);
		system("PAUSE");
		system("CLS");
		Player->isRaged = false;
		Foe->isRaged = false;
		Player->Stamina = 0;
		Foe->Stamina = 0;
		cout << "Fighting for initiative:" << endl;
		cout << Player->name << "                  " << Foe->name << endl;
		int zP = Player->Base.BDY + Player->SubDicePool;
		int zF = Foe->Base.BDY + Foe->SubDicePool;
		if (Foe->RDP.amount >= 6)
		{
			cout << Foe->name << " RAGED!" << endl;
			Foe->RDP.amount = 0;
			zF = 0;
			zP = Player->Base.BDY / 3;
			Foe->isRaged = true;
		}
		if (Player->RDP.amount >= 6)
		{
			Player->isRaged = true;
			cout << Player->name << " RAGED!" << endl;
			Player->RDP.amount = 0;
			zP = 0;
			zF = Player->Base.BDY / 3;
		}

		int Z = zP + zF;
		DicePool BDYCheck;
		for (int i = 0; i < Z; i++)
		{
			int side = rand() % 2;
			if (zP <= 0)
			{
				side = 0;
			}
			if (zF <= 0)
			{
				side = 1;
			}
			cout << endl;
			if (side == 1)
			{
				GetClashText();
				BDYCheck.MakeThrow(1, White);
				zP--;
			}
			else
			{
				cout << "                        ";
				GetClashText();
				BDYCheck.MakeThrow(1, DarkGray);
				zF--;
			}
			textcolor(0x0F);
			cout << "    ";
		}

		Player->SubDicePool = 0;
		Foe->SubDicePool = 0;
		BDYCheck.ShowPool();
		if (Player->RDP.amount > 0)
		{
			if (Player->AskForReroll(-1, false, Player->myPerk != Cold, &BDYCheck) == 0)
			{
				Player->isRaged = true;
			}
		}
		if (Foe->RDP.amount > 0 && Foe->dna.Rerollprob(Foe->RDP.amount))
		{
			if (Foe->AskForReroll(-1, true, Foe->myPerk != Cold, &BDYCheck) == 0)
			{
				Foe->isRaged = true;
			}
		}
		ColoredDice Actions = BDYCheck.GetMaxColoredSeq();
		if (isGodmode)
		{
			Player->isRaged = false;
			Actions.NUM = 3;
			Actions.color = White;
		}

		if (Player->isRaged)
		{
			Actions.NUM = 1;
			Actions.color = DarkGray;
		}
		else if (Foe->isRaged)
		{
			Actions.NUM = 1;
			Actions.color = White;
		}
		else
		{
			BDYCheck.ShowPool(true, Actions.NUM);
		}

		if (Actions.color == White)
		{
			Foe->Stamina = 0;
			Player->Stamina = Actions.NUM;
		}
		else {
			Player->Stamina = 0;
			Foe->Stamina = Actions.NUM;
		}
		cout << endl;
		if (Player->Stamina > 0)
		{
			MakeActions(Player, Foe, false, true);

		}
		else if (Foe->Stamina > 0)
		{
			MakeActions(Foe, Player, true, true);
		}
		else
		{
			cout << "Draw! Retreat! Tuche!" << endl;
			Player->PushRt(1);
			Foe->PushRt(1);

		}

		system("PAUSE");
	}

	if (Player->HP <= 0)
	{
		isPlayerWin = false;
	}
	cout << "Battle is over " << endl;
	system("PAUSE");
	return isPlayerWin || isGodmode;
}



Unit MakeFoe(vector<Unit> Enemies, int PCR = 0)
{

	int FoeLevel = rand() % Enemies.size();
	if (PCR != 0)
	{
		FoeLevel = rand() % PCR;
	}
	Unit Foe = Enemies[FoeLevel];
	Foe.myPerk = Perk(rand() % 5);
	Foe.dna.dt = DNAType(rand() % 6);
	return Foe;
}

int main()
{
	isGodmode = false;
	srand(time(NULL));
	cout << unitbuf;
	old = cout.rdbuf(Buffer.rdbuf());
	textcolor(White);
	vector<Item> Inventory;
	vector<Item> Loot;
	vector<Item> Treasure;
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

	int FoeLevel = -1;



	Unit Player("none", 2);

	int pprk = -1;
	int cr = 1;
	/*while (FoeLevel < 0 && FoeLevel > Enemies.size())
	{
		cout << "Select Foe:" << endl;
		cout << "0 - Random" << endl;
		for (int f = 0; f < Enemies.size(); f++)
		{
			cout << f + 1 << " - " << Enemies[f].name << endl;
		}
		cout << "Enter 0-" << Enemies.size() << ":";
		cin >> FoeLevel;
	}
	if (FoeLevel == 0)
	{
		FoeLevel = rand() % Enemies.size();
	}
	else
	{
		FoeLevel--;
	}
	Unit Foe = Enemies[FoeLevel];


	cout << "Hello stranger, i am " << Foe.name << endl;
	cout << "What is my Perk?" << endl;
	cout << "0 - None" << endl;
	cout << "1 - Cold (no ragetoken gain)" << endl;
	cout << "2 - Fiechter (no rage exploding in attacks)" << endl;
	cout << "3 - Dancer(no rage exploding in moves)" << endl;
	cout << "4 - Tactitian(best reroll dice)" << endl;
	cout << "5 - Random" << endl;





	while (pprk < 0 || pprk>5)
	{
		cout << "Enter 0 - 5:";
		cin >> pprk;
	}
	if (pprk != 5)
	{
		Foe.myPerk = Perk(pprk);
	}
	else
	{
		Foe.myPerk = Perk(rand() % 5);
	}


	cout << "What is my Behevior?" << endl;
	cout << "0 - Chaotic" << endl;
	cout << "1 - Attacker" << endl;
	cout << "2 - Defender" << endl;
	cout << "3 - Strafer" << endl;
	cout << "4 - Charger" << endl;
	cout << "5 - Rager" << endl;
	pprk = -1;
	while (pprk < 0 || pprk>5)
	{
		cout << "Enter 0 - 5:";
		cin >> pprk;
	}
	Foe.dna.dt = DNAType(pprk);

	cout << "[" << Foe.Base.BDY << "," << Foe.Base.Att << "," << Foe.Base.Def << "]" << endl;
	*/
	cout << "Tell me your name, stranger: ";
	flushText();
	cin >> Player.name;
	if (Player.name.compare("IDDQD") == 0)
	{
		isGodmode = true;
		Player.name = "DoomGuy";
	}
	//cout << "Tell me," << Player.name << " your level: ";	
	//cin >> cr;


	cout << "What is your Perk?(trade 1 RT for 1 perk)" << endl;
	cout << "0 - Commoner" << endl;
	cout << "1 - Cold (no ragetoken gain)" << endl;
	cout << "2 - Fiechter (no rage exploding in attacks)" << endl;
	cout << "3 - Dancer(no rage exploding in moves)" << endl;
	cout << "4 - Tactitian(best reroll dice)" << endl;
	flushText(10);
	pprk = -1;
	while (pprk < 0 || pprk>4)
	{
		cout << "Enter 0 - 4:";
		flushText(10);
		cin >> pprk;
	}
	if (pprk != 0)
	{
		Player.Base.RC++;
	}
	Player.myPerk = Perk(pprk);



	textcolor(LightRed);
	cout << Player.name << " the "<< Perks[Player.myPerk]<<",  you must save my daughter from the deeps of this dungeon! Help her!";
	flushText(60);
	textcolor(White);
	///Item(int att, int def, int bd, string nm, int hp = 0, int rt = 0, int sd = 0)
	Treasure.push_back(Item(0, 0, 0, "apple", 1, 0, 0));
	Treasure.push_back(Item(0, 0, 0, "heal potion", 4, 0, 0));
	Treasure.push_back(Item(0, 0, 0, "burn potion", 0, 1, 0));
	Treasure.push_back(Item(0, 0, 0, "chill potion", 0, -1, 0));



	Loot.push_back(Item(1, 0, 0, "dagger", 0, 0, 0));
	Loot.push_back(Item(0, 1, 0, "buckler", 0, 0, 0));
	Loot.push_back(Item(0, 2, 0, "shield", 0, 0, 0));
	Loot.push_back(Item(2, 0, 0, "sword", 0, 0, 0));
	Loot.push_back(Item(1, 1, 0, "pike", 0, 0, 0));
	Loot.push_back(Item(2, 1, 0, "halebard", 0, 0, 0));
	Loot.push_back(Item(0, 1, 0, "chainmale", 0, 0, 0));
	Loot.push_back(Item(0, 2, 0, "cuirass", 0, 0, 0));
	Loot.push_back(Item(3, 1, 0, "katana", 0, 0, 0));
	Loot.push_back(Item(0, 3, 0, "fullplate", 0, 0, 0));





	Player.Belt.push_back(Item(0, 0, 0, "apple", 1, 0, 0));
	Player.Belt.push_back(Item(0, 0, 0, "key", 0, 0, 0));
	Player.Belt.push_back(Item(0, 0, 0, "embarrasing letter", 0, 3,-1));
	Player.Inventory.push_back(Item(1, 0, 0, "dagger", 0, 0, 0));
	Unit Foe = MakeFoe(Enemies, 2);
	Player.RDP.amount = Player.Base.RC;
	Foe.RDP.amount = Foe.Base.RC;
	system("PAUSE");

	textcolor(White);
	cout << "You opened door and see:" << endl;
	textcolor(LightMagenta);
	GetFoeRoomText();
	cout << endl;
	flushText();
	system("PAUSE");
	system("CLS");
	textcolor(LightMagenta);
	cout << endl << "Watch out, there is a ";
	textcolor(Yellow);
	cout << Foe.name;
	textcolor(LightMagenta);
	cout << " ready to attack! To arms!" << endl;
	textcolor(White);
	flushText();
	system("PAUSE");
	bool isEndDungeon = false;
	while (Battle(&Player, &Foe) && !isEndDungeon)
	{
		system("CLS");
		textcolor(White);
		cout << endl << "You win this time. Time to move forward" << endl;
		int i = rand() % Loot.size();
		cout << "You found: " << Loot[i].name << endl;
		Player.Inventory.push_back(Loot[i]);
		if (rand() % 2)
		{
			Player.Base.BDY++;
			cout << "You feel much stronger now!" << endl;
		}
		Player.HP += xdy(1, 2);
		Player.MaxHP = Player.Base.BDY * 2;
		int newCR = max(1, Player.Base.BDY + rand() % 3 - 1);
		if (newCR > 9)
		{
			isEndDungeon = true;
		}
		else
		{
			Foe = MakeFoe(Enemies, newCR);
		}

		textcolor(White);
		flushText();
		system("PAUSE");
		system("CLS");
		if (1)//rand() % 3 == 2)
		{
			cout << endl << "You opened door and see:" << endl;
			textcolor(LightGreen);
			GetFriendRoomText();
			flushText();
			system("PAUSE");
			int i = rand() % Treasure.size();
			cout << "You found: " << Treasure[i].name << endl;
			Player.Belt.push_back(Treasure[i]);
			int ch = -1;
			flushText();
			textcolor(White);
			while (ch != 0)
			{
				textcolor(White);
				while (ch < 0 || ch > 4)
				{
					cout << "What to do?" << endl;
					cout << "1: Use item from pockets" << endl;
					cout << "2: Wear item from Bag" << endl;
					cout << "3: Sleep" << endl;
					cout << "4: Think" << endl;
					cout << "0- Go forward!" << endl;
					flushText();
					cin >> ch;
				}
				switch (ch)
				{
				case 1:
					UseItem(&Player);
					ch = -1;
					break;
				case 2:
					WearItem(&Player);
					ch = -1;
					break;
				case 3:
					Player.HP = Player.MaxHP;
					ch = -1;
					break;
				case 4:
					textcolor(LightCyan);
					cout << " what if i meet " << Enemies[rand() % Enemies.size()].name << " and " << GetBrokeText() << " ? " << endl;
					flushText();
					ch = -1;
					break;
				default:
					break;
				}
				

			}
			textcolor(Yellow);
			cout << Player.name << ", you rest well and ready to fight more!" << endl;
			textcolor(White);
			Player.wounds.clear();
			flushText();
			system("PAUSE");
			system("CLS");
			Player.RDP.amount = Player.Base.RC;
			Foe.RDP.amount = Foe.Base.RC;
		}




		cout << endl << "You opened next door and see:" << endl;
		textcolor(LightMagenta);
		GetFoeRoomText();
		flushText();
		system("PAUSE");
		system("CLS");
		textcolor(LightMagenta);
		cout << endl << "Watch out, there is a ";
		textcolor(Yellow);
		cout << Foe.name;
		textcolor(LightMagenta);
		cout << " ready to attack! To arms!" << endl;
		textcolor(White);
		flushText();
		system("PAUSE");

	}
	if (isEndDungeon)
	{
		cout << endl << "You opened door and see:" << endl;
		textcolor(LightGreen);
		GetFriendRoomText();
		textcolor(Yellow);
		cout << endl << "There is a young lady chained to the wall." << endl << " Would you unchain and save her?:";
		char yn = 'n';
		flushText();
		cin >> yn;
	}
	else
	{
		textcolor(Magenta);
		cout << "Quest is over. Your bones lonely whites in shades of dungeon.";
		flushText();
	}
	cout.rdbuf(old);
	system("PAUSE");
	return 0;
}

