#pragma once
#include "stdafx.h"
const vector<string> level_words = { "поток", "капитан", "конверсия" };
const vector<string> dictionary_1 = { "пот", "ток", "кот", "коп", "опт", "окоп", "око", "топ", "окот" };
const vector<string> dictionary_2 = { "кит","кат","кап","кипа","кант","капа","пак","пик","пат","пан","пика","пинта","тип","тап","тина","нитка","накат","канат","тик","танк","акт", "ник" };
const vector<string> dictionary_3 = { "ре", "си","як","век","вес","вир","вор","инк","ион","иск","кон","кри","ник","нос","рея","рис","ров","рок","сок","сон","соя","сор","веко","вино","ворс","воин","воск","евро","инок","кино","кров","нерв","овен","риск","сени","сено","соня","срок","явор","янки","веник","венок","висок","ивняк","кисея","носки","оникс","ровня","свояк","серия","синяк","версия","сорняк","керосин","ровесник" };
const vector<vector<string> > dictionaries = { dictionary_1, dictionary_2, dictionary_3 };

class game_level
{
	int level;
	string word;
	vector<string>dictionary;
public:
	game_level(int lev, string level_word, vector<string> dict) :level(lev), word(level_word), dictionary(dict) {};
	game_level() {};
	game_level(const game_level &g) { level = g.level; word = g.word; dictionary = g.dictionary; }
	~game_level() { dictionary.clear(); }

	void set_level(int levell) { level = levell; }
	void set_level_word(string level_word) { word = level_word; }
	void set_level_dictionary(vector<string> level_dictionary) { dictionary = level_dictionary; }

	vector<string> get_level_dictionary() { return dictionary; } const
		int get_level() { return level; }
	string get_word() { return word; }
};
