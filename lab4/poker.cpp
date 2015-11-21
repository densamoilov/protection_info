#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

#include "../lab1/cryptolib.h"

typedef long long llong;


/******************** For pseudo-graphics ***********************/

#include <unistd.h> /* для функций open и write */
#include <fcntl.h> /* режимы открытия файлов */
#include <cstring>
#include <sys/ioctl.h> /* функция управления файлами устройств */
#include <termios.h> /* функции для работы с терминалом */ 

/* цвета для консоли */
enum colors { clBlack, clRed, clGreen, clYellow, clBlue, clPurple, clAqua, clWhite };


int mt_clrscr()
{
    int tty; /* хранит номер дескриптора */
    tty = open("/dev/tty", O_RDWR); /* открытие файла терминала в режиме чтение/запись */
    if (tty == -1) { /* проверка на открытие */
        fprintf(stderr, "\nmt_clrscr()\n: Can not open tty\n");
        close(tty);
        return -1;
    } else {
        /* отправляем в терминал esc-последовательность */
        write(tty, "\033[H\033[J", 6);
        close(tty);
        return 0;
    }
}

/* координаты курсора */
int mt_gotoXY (int row, int column)
{   int len;
    int tty; /* хранит номер дескриптора */
    tty = open("/dev/tty", O_RDWR); 
    if (tty == -1) { /* проверка на открытие */
        fprintf(stderr, "\nmt_gotoXY()\n: Can not open tty\n");
        close(tty);
        return -1; /* ошибка открытия файла терминала */
    } else { 
        if ((row > -1 && row <= 35) && (column > -1 && column <= 150)) {
            char str[20];
            /* собираем последовательность в масс вместе с аргументами */
            len = sprintf(str, "\033[%d;%dH", row, column);
            /* запись в файл терминала */
            write(tty, str, len);
            close(tty);
            return 0; /* успешное завершение */
        } else {
            fprintf(stderr, "\nmt_gotoXY(int, int)\n: Incorrect row or column value\n");
            close (tty);
            return 1; /* некорректные входные данные */
        }
    }
}
/* устанавливает цвет текста */
int mt_setfgcolor(int color)
{
    int tty, len;
    if (color < 0 && color > 7) {
      fprintf(stderr, "\nmt_setfgcolor(int)\n: Incorrect text color\n");
      return -1;
    }
  
    tty = open("/dev/tty", O_RDWR);
    if (tty == -1) {
        fprintf(stderr, "\nmt_setfgcolor(int)\n: Can not open TTY\n");
        close(tty);
        return -1;
    } else {
    /* составление esc-пос. "\E[34m" */
    char str[20];
    /* собираем последовательность в масс вместе с аргументами */
    len = sprintf(str, "\033[3%dm", color);
    /* запись в файл терминала */
    write(tty, str, len);
    close(tty);
    return 0;
  }
}
int mt_setbgcolor(int color)
{
    int tty, len;
    if (color < 0 && color > 7) {
      fprintf(stderr, "\nmt_setfgcolor(int)\n: Incorrect text color\n");
      return -1;
    }
  
    tty = open("/dev/tty", O_RDWR);
    if (tty == -1) {
        fprintf(stderr, "\nmt_setfgcolor(int)\n: Can not open TTY\n");
        close(tty);
        return -1;
    } else {
    /* составление esc-пос.  */
    char str[20];
    /* собираем последовательность в масс вместе с аргументами */
    len = sprintf(str, "\033[4%dm", color);
    /* запись в файл терминала */
    write(tty, str, len);
    close(tty);
    return 0;
  }
}
int mt_setstdcolor()
{
    int tty;
    tty = open("/dev/tty", O_RDWR);
    if (tty == -1) {
        fprintf(stderr, "\nmt_setfgcolor(int)\n: Can not open TTY\n");
        close(tty);
        return -1;
    } else {
        write(tty,"\033[0m",5);
        return 0;
    }
}

/* вывод строки символов с использованием дополнительной таблицы кодировок */
int bc_printA(const char *str)
{
    int tty;
    tty = open("/dev/tty", O_RDWR);
    if (tty == -1) {
        fprintf(stderr, "\nbc_printA(char *)\n: Can not open tty\n");
        close(tty);
        return -1;
    } else {
        write(tty, "\033(0", 3);
        write(tty, str, strlen(str));
        write(tty, "\033(B", 3);
    }
    close(tty);
    return 0;
}


/* рамка x1-y1 - левый верхний угол, x2-y2 - высота, длина */
int bc_box(int x1, int y1, int x2, int y2)
{
    int tty, i, j;
    tty = open("/dev/tty", O_RDWR);
    if (tty == -1) {
        fprintf(stderr, "\nbc_box(int, int, int, int)\n: Can not open tty\n");
        close(tty);
        return -1;
    } else {
        for (i = 1; i <= x2; i++) { 
            mt_gotoXY(x1 + i, y1);
            if (i == 1) {
                bc_printA("l"); /* левый верхний угол */
                for (j = 2; j < y2; j++) {
                    bc_printA("q"); /* горизонталь */
                }
                bc_printA("k"); /* правый верхний угол */
            }
            if (i != 1 && i != x2) { /* если не начало и не конец, то пробелы */
                bc_printA("x"); /* левая вертикаль */
                for (j = 2; j < y2; j++) {
                    bc_printA(" ");
                }
                bc_printA("x"); /* правая вертикаль */
            } 
            if (i == x2) {
                bc_printA("m");/* левый нижний угол */
                for (j = 2; j < y2; j++) {
                    bc_printA("q"); /* горизонталь */
                }
                bc_printA("j"); /* правый нижний угол */
            }
        }
        close(tty);
        return 0;
    }
}


void draw_cards(string content_card, bool is_table)
{
    string lear[4] = {"Spades", "Hearts", "Clubs", "Diamonds"};
    string lear_two[4] = {"♠","♥", "♣", "♦"};

    string worth[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "K", "Q", "J"};
    static int current_x_players{0};
    static int current_x_table{0};

    static int count_cards{0};
    const int between_cards_one_player = 3;
    const int between_cards_different_players = 8;
    const int width_card = 10;

if (!is_table) {
    if (count_cards < 2) {
        count_cards++;
        current_x_players += width_card + between_cards_one_player;
    } else {
        count_cards = 1;
        current_x_players += width_card + between_cards_different_players;
    }
} else {
    current_x_table += width_card + between_cards_one_player;
}

    for (int i = 0; i < 4; ++i) {
        auto pos_lear = content_card.find(lear[i], 0);
        if (pos_lear != string::npos) {
            for (int j = 0; j < 13; ++j) {
                auto pos_worth = content_card.find(worth[j], 0);
                if (pos_worth != string::npos) {
                    // Determined cards
                    //cout << i << " " << j << endl;

                    mt_setbgcolor(clWhite);
                    mt_setfgcolor(clBlack);
                    if (!is_table) {
                        bc_box(2, current_x_players, 10, 12); 
                    } else {
                        bc_box(20, current_x_table, 10, 12); 
                    }
                    //mt_setstdcolor();
                    if (i % 2 == 0) {
                        mt_setfgcolor(clBlack);
                    } else {
                        mt_setfgcolor(clRed);
                    }
                    
                    if (!is_table) {
                        mt_gotoXY(4, current_x_players + 1);
                        write(1, worth[j].c_str(), sizeof(worth[j].c_str()));
                        mt_gotoXY(5, current_x_players + 1);
                        write(1, lear_two[i].c_str(), sizeof(lear_two[i].c_str()));
                    } else {                         
                        mt_gotoXY(22, current_x_table + 1);
                        write(1, worth[j].c_str(), sizeof(worth[j].c_str()));
                        mt_gotoXY(23, current_x_table + 1);
                        write(1, lear_two[i].c_str(), sizeof(lear_two[i].c_str()));
                    }
                    
                    mt_setstdcolor();
                    mt_gotoXY(35, 1);
                    break;
                }
            }
        }
    }
//m1:;
}




llong get_prime_number(llong lower_bound, llong upper_bound)
{
    static default_random_engine gen(time(0));
    static uniform_int_distribution<llong> random(lower_bound, upper_bound);
    bool is_prime{false};

    // Generation a prime number
    llong p{0};

    while (!is_prime) {
        is_prime = true;
        p = random(gen);
        // Check for prime
        for (llong i = 2; i <= static_cast<llong>(sqrt(p)); ++i) {
            if (p % i == 0) {
                 is_prime = false;
                 break;
            }
        }
    }
    return p;
}


class Deck_of_cards 
{
private:
   	string lear_[4] = {"Spades ", "Hearts ", "Clubs ", "Diamonds "}; 
   	string worth_[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "K", "Q", "J"};
   	//llong id_card_ = 2;
    default_random_engine gen;
    uniform_int_distribution<llong> random;

    map<llong, string> deck_;
public:
    Deck_of_cards() 
    : gen(time(0)), random(100, 1000)
    {
    	llong tmp{0};

      	for (int i = 0; i < 4; ++i) {
      		for (int j = 0; j < 13; ++j) {
      			while (true) {
      			    tmp = random(gen);
      			    if (deck_.count(tmp) == 0) {
      				    deck_.emplace(/*id_card_++*/tmp, (lear_[i] + worth_[j]));
      				    break;
      			    } else {
       			        tmp = random(gen);
       			    }
       			}
       		}
       	}
    }

    void get_associate_numbers(vector<llong>& associate_numbers)
    {
    	int i{0};

    	for_each(deck_.begin(), deck_.end(), [&](auto& val){
            associate_numbers[i++] = val.first;
    	});
    }

    void print_deck()
    {
        for (auto& x : deck_) {
        	cout << "ID: " << x.first << " " << x.second << endl;
        }    
    }

    void find_card(const llong& taken_card, pair<llong, string>& tmp_card)
    {
    	auto it = deck_.find(taken_card);

        if (it == deck_.end()) {
        	cerr << "Oops, something went wrong" << endl;
        	exit(1);
        }
        tmp_card = (*it);
    }
};


class Player
{
private:
  	llong p_;
  	llong c_;
  	llong d_;
    llong num_;

  	vector<pair<llong, string> > my_cards_;
public:
   	Player(llong p, llong num) 
   	: p_{p}, num_(num)
   	{
   		// Get c and d
        llong gcd{0}, x{0}, y{0};

        while (true) {
            c_ = get_prime_number(1, p_ -1);
            generalized_euclid(c_, p_ - 1, gcd, x, y);
            if (gcd == 1) {
                break;
            }
        }

        if (x < 0) {
            d_ = (p_ - 1) + x;
        } else {
            d_ = x;
        }
/*
        if (((c_*d_) % (p_-1)) != 1) {
	        cerr << "not" << endl;
  	    } else {
            cout << "OK" << endl;
  	    } 
*/
  	}

   	void shuffle_and_encode(vector<llong>& associate_numbers)
   	{
   		shuffle(associate_numbers.begin(), associate_numbers.end(), default_random_engine(time(0)));
   		for (int i = 0; i < static_cast<int>(associate_numbers.size()); ++i) {
   		    associate_numbers[i] = pow_module(associate_numbers[i], c_, p_);
   		}
   	}

   	void decode_card(llong& val) 
   	{
   		val = pow_module(val, d_, p_);
   	}

   	void get_card(const llong& taken_card, Deck_of_cards& deck)
   	{
        pair<llong, string> tmp_card;
        deck.find_card(taken_card, tmp_card);

        my_cards_.push_back(tmp_card);
   	}

   	void show_my_cards()
   	{
   		//cout << "Player number " << num_ << " has next cards:" << endl;
   		for_each(my_cards_.begin(), my_cards_.end(), [&](const pair<llong, string>& val) {
   			//cout << "ID = " << val.first << " "  << val.second << endl;
            draw_cards(val.second, false);
   		});
   	
   	}
   	
};

class Table 
{
private:
    vector<pair<llong, string> > cards_on_table_;  
public:
    Table()
    {}
    
    void put_card_on_table(const llong& taken_card, Deck_of_cards& deck)
    {
        pair<llong, string> tmp_card;
        deck.find_card(taken_card, tmp_card);

        cards_on_table_.push_back(tmp_card);
    }

    void show_cards_on_table()
    {
        
        //cout << "Cards on table:" << endl;
        bc_box(18, 10, 14, 57);
        mt_gotoXY(19, 32);
        write(1, " Game Table ", 12);
        for_each(cards_on_table_.begin(), cards_on_table_.end(), [](const pair<llong, string>& val) {
           // cout << "ID = " << val.first << " "  << val.second << endl;
            draw_cards(val.second, true);
        });

    }
};


int main(int argc, char** argv)
{
    /*************** Stage of prepare *****************/

    // Determining number of players
    if (argc != 2) {
    	cout << "Usage: " << argv[0] << " <number of players>" << endl;
    	return 1;
    }
    int nplayers = atoi(argv[1]);

    // Choosing parameters of protocol
    llong p = get_prime_number(100, 1000000);
    
    vector<Player*> players;
    players.reserve(nplayers);

    for (int i = 0; i < nplayers; ++i) {
        players.push_back(new Player(p, i + 1));	
    }

    Deck_of_cards deck;
    //deck.print_deck();

    vector<llong> associate_numbers(52, 0);
    deck.get_associate_numbers(associate_numbers);

    shuffle(associate_numbers.begin(), associate_numbers.end(), default_random_engine(time(0)));

    for_each(players.begin(), players.end(), [&](Player*& val) {
    	val->shuffle_and_encode(associate_numbers);
    });

    for (int card = 0; card < 2; ++card) {
    	for (int current_player = 0; current_player < nplayers; ++current_player) {
            // take a card
			llong taken_card = associate_numbers.back();
			associate_numbers.pop_back();
           
            // decode the card
            for (int i = 0; i < nplayers; ++i) {
            	// Skiping current player
            	if (i != current_player) {
            		players[i]->decode_card(taken_card);
            	}
            }
            // Current player in last queue decodes the card
            players[current_player]->decode_card(taken_card);
            players[current_player]->get_card(taken_card, deck);
    	}
    }


    // Create table
    Table table;
    // Put four cards on table
    for (int card = 0; card < 3; ++card) {
        llong taken_card{0};
        for (int current_player = 0; current_player < nplayers; ++current_player) {
            taken_card = associate_numbers.back();
            associate_numbers.pop_back();

            // decode the card
            for (int i = 0; i < nplayers; ++i) {
                players[i]->decode_card(taken_card);
            }
        }
        table.put_card_on_table(taken_card, deck);
    }



mt_setstdcolor();
    /*
    // Draw border
    mt_clrscr();
    mt_setbgcolor(clBlack); 
    mt_setfgcolor(clGreen);
    bc_box(1, 1, 30, 140); 
    mt_setstdcolor();
    mt_gotoXY(33, 1);
*/

    for (auto& x : players) {
    	x->show_my_cards();
    }

    table.show_cards_on_table();

/*
    for (auto& x : associate_numbers) {
        cout << x << endl;
    }
*/
    for (auto& x : players) {
        delete x;
    }

	return 0;
}