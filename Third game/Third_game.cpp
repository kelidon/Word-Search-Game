// Third game.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Third game.h"
#include "game_level.h"

#define MAX_LOADSTRING 100
#undef UNICODE
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

static vector<string> dict;
static vector<string>input;
vector <string>::iterator iter1, iter2;
static map<string, pair<int, int> > statist;
static set<string> stat_names;
static COLORREF lett_back, background, text_color;
static HFONT hfont;
const int num_of_lvls = 3, max_lttrs = 9, buffs_size = 3;
static BOOL indicator = TRUE, word_count = TRUE, not_on_bkgr = TRUE, letter_tick[max_lttrs], visib_prompt;
static string curr_name = "Гость", temp_input;
static POINT p, p1, text_name, text_lvl, text_lvl_num, ell[2], word_cou[3], indicat[2];
static HMENU hMenuWord, hMenuInput, hIndicat, hMenuBack, hMenuPrompt;
static int cx, cy, curr_lvl, dict_size, lttrs_counter, timer;
static HPEN hpen, hpw;
static HDC hdc, ano;
static double letters_num;
static HBRUSH hbword, hb_input[max_lttrs], hb_filled, hb_empty, hb_indic, hred, hgreen, hb_bkgr, hb_prompt;
static PAINTSTRUCT ps;
static HRGN *lvlword_cells, *input_cells, indic, prompt;
static char inp[max_lttrs];

static int test_num = 3;
const BOOL test = TRUE;

void erase(int &lttrs_counter, string &temp_input, double letters_num, HBRUSH hb_input[max_lttrs], HBRUSH hb_empty, BOOL letter_tick[max_lttrs], char inp[max_lttrs]);
void kill_timer(HWND &hWnd, int &timer);
void set_timer(HWND &hWnd, int &timer);
void re_timer(HWND &hWnd, int &timer);


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogW(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogS(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogN(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogST(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogSav(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_THIRDGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THIRDGAME));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THIRDGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_THIRDGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	vector<game_level> game(num_of_lvls);


	for (int i = 0; i < num_of_lvls; i++)
	{
		game[i].set_level(i + 1);
		game[i].set_level_dictionary(dictionaries[i]);
		game[i].set_level_word(level_words[i]);
	}

	switch (message)
	{
	case WM_SIZE:
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		break;
	case WM_CREATE:
	{
		set_timer(hWnd, timer);

		hMenuWord = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
		hMenuBack = GetSubMenu(hMenuWord, 2);
		hMenuInput = GetSubMenu(hMenuWord, 0);
		hIndicat = GetSubMenu(hMenuWord, 3);
		hMenuPrompt = GetSubMenu(hMenuWord, 4);
		hMenuWord = GetSubMenu(hMenuWord, 1);

		hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		lett_back = RGB(0, 200, 0);
		background = RGB(240, 240, 255);
		text_color = RGB(0, 0, 0);

		hb_empty = CreateSolidBrush(RGB(200, 200, 200));
		hb_filled = CreateSolidBrush(RGB(255, 255, 255));
		hred = CreateSolidBrush(RGB(230, 0, 0));
		hgreen = CreateSolidBrush(RGB(0, 205, 0));
		hb_indic = CreateSolidBrush(background);
		hb_prompt = CreateSolidBrush(RGB(230, 230, 0));
		for (int i = 0; i < max_lttrs; i++)
		{
			inp[i] = ' ';
			hb_input[i] = hb_empty;
		}
		hfont = CreateFont(40, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Comic Sans MS");




	}break;
	case WM_TIMER:
		timer++;
		if (timer == 20)
		{
			visib_prompt = TRUE;
			kill_timer(hWnd, timer);
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
	{
		hbword = CreateSolidBrush(lett_back);
		hb_bkgr = CreateSolidBrush(background);
		letters_num = level_words[curr_lvl].length();

		text_name.x = cx / 3;
		text_name.y = text_lvl.y = text_lvl_num.y = cy / 20;
		text_lvl.x = cx / 20;
		text_lvl_num.x = cx / 6;

		ell[0].x = cx* 0.35 / (letters_num + 1);
		ell[0].y = 12.4*cy / 15;
		ell[1].x = cx*0.65 / (letters_num + 1);
		ell[1].y = 12.4 * cy / 15 + cx*0.3 / (letters_num + 1);

		word_cou[0].x = word_cou[2].x = cx*(letters_num + 0.5) / (letters_num + 1);
		word_cou[1].x = cx*(letters_num + 0.48) / (letters_num + 1);
		word_cou[0].y = 4 * cy / 8;
		word_cou[1].y = 4.46 * cy / 8;
		word_cou[2].y = 5 * cy / 8;

		indicat[0].x = cx*(letters_num + 0.4) / (letters_num + 1);
		indicat[0].y = 2.55*cy / 8;
		indicat[1].x = cx*(letters_num + 0.7) / (letters_num + 1);
		indicat[1].y = 2.55*cy / 8 + cx*0.3 / (letters_num + 1);


		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hb_bkgr);
		Rectangle(hdc, -1, -1, cx + 1, cx + 1);
		SelectObject(hdc, hfont);
		SetTextColor(hdc, text_color);
		SelectObject(hdc, hpen);
		SetBkMode(hdc, TRANSPARENT);

		char  lvl_num[buffs_size];
		_itoa_s(game[curr_lvl].get_level(), lvl_num, 10);
		TextOut(hdc, text_lvl.x, text_lvl.y, "Уровень", strlen("Уровень"));
		TextOut(hdc, text_lvl_num.x, text_lvl_num.y, lvl_num, strlen(lvl_num));
		TextOut(hdc, text_name.x, text_name.y, curr_name.c_str(), strlen(curr_name.c_str()));
		if (visib_prompt)
		{
			SelectObject(hdc, hb_prompt);
			prompt = CreateEllipticRgn(ell[0].x, ell[0].y, ell[1].x, ell[1].y);
			Ellipse(hdc, ell[0].x, ell[0].y, ell[1].x, ell[1].y);
		}
		if (word_count)
		{
			char _Buffer[buffs_size], _Buffer1[buffs_size];
			_itoa_s(input.size(), _Buffer, 10);
			_itoa_s(dictionaries[curr_lvl].size(), _Buffer1, 10);

			TextOut(hdc, word_cou[0].x, word_cou[0].y, _Buffer, strlen(_Buffer));
			TextOut(hdc, word_cou[1].x, word_cou[1].y, "из", strlen("из"));
			TextOut(hdc, word_cou[2].x, word_cou[2].y, _Buffer1, strlen(_Buffer1));
		}
		if (indicator)
		{
			SelectObject(hdc, hb_indic);
			indic = CreateEllipticRgn(indicat[0].x, indicat[0].y, indicat[1].x, indicat[1].y);
			Ellipse(hdc, indicat[0].x, indicat[0].y, indicat[1].x, indicat[1].y);
		}

		HRGN *news = new HRGN[letters_num], *olds = new HRGN[letters_num];
		for (int i = 0; i < letters_num; i++)
		{
			SelectObject(hdc, hb_input[i]);
			double ccx = cx*letters_num / (letters_num + 1);
			olds[i] = CreateRectRgn((i + 0.1)*ccx / letters_num, cy / 4, (i + 1.1)*ccx / letters_num, cy / 2);
			Rectangle(hdc, (i + 0.1)*ccx / letters_num, cy / 4, (i + 1.1)*ccx / letters_num, cy / 2);

			TextOut(hdc, (i + 0.5)*ccx / letters_num, 2.7 * cy / 8, &inp[i], 1);
			news[i] = CreateRectRgn((i + 0.1)*ccx / letters_num, cy / 2, (i + 1.1)*ccx / letters_num, 3 * cy / 4);

			SelectObject(hdc, hbword);
			Rectangle(hdc, (i + 0.1)*ccx / letters_num, cy / 2, (i + 1.1)*ccx / letters_num, 3 * cy / 4);

			TextOut(hdc, (i + 0.5)*ccx / letters_num, 4.7 * cy / 8, &game[curr_lvl].get_word()[i], 1);
		}
		lvlword_cells = news;
		input_cells = olds;
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < letters_num; i++)
		{
			if (PtInRegion(lvlword_cells[i], LOWORD(lParam), HIWORD(lParam)) && !letter_tick[i])
			{
				letter_tick[i] = TRUE;
				hb_input[lttrs_counter] = hb_filled;
				inp[lttrs_counter] = game[curr_lvl].get_word()[i];
				temp_input += inp[lttrs_counter];
				lttrs_counter++;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (i == letters_num - 1)
			{
				if (PtInRegion(prompt, LOWORD(lParam), HIWORD(lParam)) && visib_prompt)
				{
					vector<string> for_prmpt = game[curr_lvl].get_level_dictionary();
					list<string> eve;
					sort(for_prmpt.begin(), for_prmpt.end());
					sort(input.begin(), input.end());
					set_difference(for_prmpt.begin(), for_prmpt.end(), input.begin(), input.end(), inserter(eve, eve.end()));
					string prpt = *eve.begin();
					if (MessageBox(hWnd, prpt.c_str(), "Подсказка", MB_OK | MB_ICONINFORMATION) == IDOK)
					{
						visib_prompt = FALSE;
						set_timer(hWnd, timer);
					}
					break;
				}
				else if (PtInRegion(indic, LOWORD(lParam), HIWORD(lParam)) && indicator)
				{
					MessageBox(hWnd, "Этот элемент показывает, зачтено ли введённое слово. Чтобы увидеть список зачтённых слов, выберите 'Найденные слова' в контекстном меню индикатора, либо перейдите Игра->Найденные слова.", "Информация", MB_OK | MB_ICONINFORMATION);
					break;
				}
			}
		}
		re_timer(hWnd, timer);
		break;
	case WM_RBUTTONDOWN:
	{
		re_timer(hWnd, timer);
		ano = GetDC(hWnd);
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		p1 = p;
		ClientToScreen(hWnd, &p1);
		for (int i = 0; i < letters_num; i++)
		{
			if (PtInRegion(lvlword_cells[i], p.x, p.y))
			{

				TrackPopupMenuEx(hMenuWord, TPM_LEFTALIGN | TPM_BOTTOMALIGN, p1.x, p1.y, hWnd, NULL);
				break;
			}
			else if (PtInRegion(input_cells[i], p.x, p.y))
			{
				TrackPopupMenuEx(hMenuInput, TPM_LEFTALIGN | TPM_BOTTOMALIGN, p1.x, p1.y, hWnd, NULL);
				break;
			}
			if (i == letters_num - 1)
			{
				if (PtInRegion(indic, p.x, p.y) && indicator)
				{
					TrackPopupMenuEx(hIndicat, TPM_LEFTALIGN | TPM_BOTTOMALIGN, p1.x, p1.y, hWnd, NULL);
					break;
				}
				else if (PtInRegion(prompt, p.x, p.y) && visib_prompt)
				{
					TrackPopupMenuEx(hMenuPrompt, TPM_LEFTALIGN | TPM_BOTTOMALIGN, p1.x, p1.y, hWnd, NULL);
					break;
				}
				else
				{
					TrackPopupMenuEx(hMenuBack, TPM_LEFTALIGN | TPM_BOTTOMALIGN, p1.x, p1.y, hWnd, NULL);
					break;
				}
			}
		}
		ReleaseDC(hWnd, ano);
		break;
	}
	case WM_KEYDOWN:
	{
	case VK_RETURN:
		re_timer(hWnd, timer);
		dict = game[curr_lvl].get_level_dictionary();
		dict_size = dict.size();
		iter1 = find(dict.begin(), dict.end(), temp_input);
		iter2 = find(input.begin(), input.end(), temp_input);
		if (iter1 != dict.end() && iter2 == input.end())
		{
			input.push_back(temp_input);
			hb_indic = hgreen;
			visib_prompt = FALSE;
			if (test)
				test_num = dict_size;
			if (input.size() >= test_num)
			{
				curr_lvl++;
				kill_timer(hWnd, timer);
				MessageBox(hWnd, "Уровень пройден!", "Достижение", MB_OK | MB_ICONINFORMATION);
				input.clear();
				if (curr_lvl == num_of_lvls)
				{
					MessageBox(hWnd, "Победа!", "Достижение", MB_OK | MB_ICONINFORMATION);
					if (MessageBox(hWnd, "Начать новую игру?", "Выход", MB_YESNO | MB_ICONQUESTION) == IDYES)
						curr_lvl = 0;
					else
					{
						curr_lvl--;
						InvalidateRect(hWnd, NULL, true);
						break;
					}
				}
			}
			erase(lttrs_counter, temp_input, letters_num, hb_input, hb_empty, letter_tick, inp);
		}
		else
		{
			erase(lttrs_counter, temp_input, letters_num, hb_input, hb_empty, letter_tick, inp);
			hb_indic = hred;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	}break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_INPUT_WORDS:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogW);
			break;
		case ID_32772:
		case ID_BACKGROUND_CHANGECOLOR:
		case ID_LEVWORD_CHANGEFONT:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DialogS);
			break;
		case ID_POINT_CHOSENLETTERS:
			erase(lttrs_counter, temp_input, letters_num, hb_input, hb_empty, letter_tick, inp);
			InvalidateRect(hWnd, NULL, true);
			break;

		case ID_POIN_CHOOSELETTER:
			re_timer(hWnd, timer);
			for (int i = 0; i < letters_num; i++)
			{
				if (PtInRegion(lvlword_cells[i], p.x, p.y) && !letter_tick[i])
				{
					letter_tick[i] = TRUE;
					hb_input[lttrs_counter] = hb_filled;
					inp[lttrs_counter] = game[curr_lvl].get_word()[i];
					temp_input += inp[lttrs_counter];
					lttrs_counter++;
					InvalidateRect(hWnd, NULL, true);
					break;
				}
			}
			break;
		case ID_INDIC_INDICATOR:
			re_timer(hWnd, timer);
			if (MessageBox(hWnd, "Этот элемент показывает, зачтено ли введённое слово. Чтобы увидеть список зачтённых слов, выберите 'Найденные слова' в контекстном меню индикатора, либо перейдите Игра->Найденные слова.", "Информация", MB_OK | MB_ICONINFORMATION) == IDOK)
				set_timer(hWnd, timer);;
			break;
		case ID_32784:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hWnd, DialogSav);
			break;
		case ID_INDIC_32781:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogW);
			break;
		case ID_32786:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, DialogST);
			break;
		case ID_32785:
			re_timer(hWnd, timer);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, DialogN);
			erase(lttrs_counter, temp_input, letters_num, hb_input, hb_empty, letter_tick, inp);
			curr_lvl = 0;
			input.clear();
			InvalidateRect(hWnd, NULL, true);
			break;
		case ID_32787:
		{
			re_timer(hWnd, timer);
			map<string, pair<int, int> >::iterator by_key = statist.find(curr_name);
			if (by_key != statist.end())
				by_key->second = pair<int, int>(curr_lvl + 1, input.size());
			else
				statist.insert(pair<string, pair<int, int> >(curr_name, pair<int, int>(curr_lvl + 1, input.size())));
			break;

		}
		case ID_PROMPT_32782:
		{
			vector<string> for_prmpt = game[curr_lvl].get_level_dictionary();
			list<string> eve;
			sort(for_prmpt.begin(), for_prmpt.end());
			sort(input.begin(), input.end());
			set_difference(for_prmpt.begin(), for_prmpt.end(), input.begin(), input.end(), inserter(eve, eve.end()));
			string prpt = *eve.begin();
			if (MessageBox(hWnd, prpt.c_str(), "Подсказка", MB_OK | MB_ICONINFORMATION) == IDOK)
			{
				visib_prompt = FALSE;
				set_timer(hWnd, timer);
			}
		}break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK DialogW(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hd = GetParent(hDlg);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hl = GetDlgItem(hDlg, IDC_LIST1);
		for (int i = 0; i < input.size(); i++)
		{
			SendMessage(hl, LB_ADDSTRING, 0, (LPARAM)input[i].c_str());
		}
		return (INT_PTR)TRUE;
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK: return EndDialog(hDlg, 0);
		case IDCANCEL: return EndDialog(hDlg, 0);
		}
		return TRUE;
	}
	default: return FALSE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK DialogS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static COLORREF color, colorB, colorT;
	static CHOOSECOLOR ccs1, ccs2, ccs3;
	static COLORREF acrCustClr[16], acrCustClr2[16], acrCustClr3[16];
	static string fonts;
	static HFONT htemp;
	switch (message)
	{
	case WM_INITDIALOG:
		color = lett_back;
		colorB = background;
		colorT = text_color;


		ccs1.lStructSize = sizeof(CHOOSECOLOR);
		ccs1.hwndOwner = hDlg;
		ccs1.rgbResult = lett_back;
		ccs1.Flags = CC_RGBINIT | CC_FULLOPEN;
		ccs1.lpCustColors = (LPDWORD)acrCustClr;

		ccs2.lStructSize = sizeof(CHOOSECOLOR);
		ccs2.hwndOwner = hDlg;
		ccs2.rgbResult = background;
		ccs2.Flags = CC_RGBINIT | CC_FULLOPEN;
		ccs2.lpCustColors = (LPDWORD)acrCustClr2;

		ccs3.lStructSize = sizeof(CHOOSECOLOR);
		ccs3.hwndOwner = hDlg;
		ccs3.rgbResult = text_color;
		ccs3.Flags = CC_RGBINIT | CC_FULLOPEN;
		ccs3.lpCustColors = (LPDWORD)acrCustClr3;

		if (indicator)
		{
			SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, TRUE, NULL);
		}
		else
			SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, FALSE, NULL);
		if (word_count)
		{
			SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_SETCHECK, TRUE, NULL);
		}
		else
			SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_SETCHECK, FALSE, NULL);
		return FALSE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			lett_back = color;
			background = colorB;
			text_color = colorT;

			InvalidateRect(GetParent(hDlg), NULL, 1);

		case IDCANCEL: return EndDialog(hDlg, 0);
		case IDC_BUTTON1:
		{
			if (ChooseColor(&ccs1))
			{
				color = ccs1.rgbResult;
			}
		}
		break;
		case IDC_BUTTON2:
		{
			if (ChooseColor(&ccs2))
			{
				colorB = ccs2.rgbResult;
			}
		}
		break;
		case IDC_BUTTON3:
		{
			if (ChooseColor(&ccs3))
			{
				colorT = ccs3.rgbResult;
			}
		}
		break;
		case IDC_RADIO1:
			htemp = CreateFont(40, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
			hfont = htemp;
			break;
		case IDC_RADIO2:
			htemp = CreateFont(40, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman");
			hfont = htemp;
			break;
		case IDC_RADIO3:
			htemp = CreateFont(40, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Comic Sans MS");
			hfont = htemp;
			break;
		case IDC_CHECK1:
			if (Button_GetCheck(GetDlgItem(hDlg, IDC_CHECK1)) == BST_CHECKED)
				indicator = TRUE;
			if (Button_GetCheck(GetDlgItem(hDlg, IDC_CHECK1)) == BST_UNCHECKED)
				indicator = FALSE;
			break;
		case IDC_CHECK2:
			if (Button_GetCheck(GetDlgItem(hDlg, IDC_CHECK2)) == BST_CHECKED)
				word_count = TRUE;
			if (Button_GetCheck(GetDlgItem(hDlg, IDC_CHECK2)) == BST_UNCHECKED)
				word_count = FALSE;
			break;

		}
		return TRUE;
	}
	default: return FALSE;
	}
	return FALSE;
}

void erase(int &lttrs_counter, string &temp_input, double letters_num, HBRUSH hb_input[max_lttrs], HBRUSH hb_empty, BOOL letter_tick[max_lttrs], char inp[max_lttrs])
{
	lttrs_counter = 0;
	temp_input = "";
	for (int i = 0; i < letters_num; i++)
	{
		hb_input[i] = hb_empty;
		inp[i] = ' ';
		letter_tick[i] = FALSE;
	}
}
void kill_timer(HWND &hWnd, int &timer)
{
	KillTimer(hWnd, 10);
	timer = 0;
}
void set_timer(HWND &hWnd, int &timer)
{
	SetTimer(hWnd, 10, 500, NULL);
	timer = 0;
}
void re_timer(HWND &hWnd, int &timer)
{
	kill_timer(hWnd, timer);
	set_timer(hWnd, timer);
}
INT_PTR CALLBACK DialogN(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static string new_name;
	static BOOL* is_converted;
	static char inp_name[20];
	switch (message)
	{
	case WM_INITDIALOG:
		//SetDlgItemInt(hDlg, IDC_EDIT1, , TRUE);
		return FALSE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, inp_name, 20);
			curr_name = inp_name;
			InvalidateRect(GetParent(hDlg), NULL, 1);

		case IDCANCEL: return EndDialog(hDlg, 0);
		}
		return TRUE;
	}
	default: return FALSE;
	}
	return FALSE;
}
INT_PTR CALLBACK DialogST(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hd = GetParent(hDlg);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hl = GetDlgItem(hDlg, IDC_LIST1);
		map<string, pair<int, int> >::iterator iter = statist.begin();
		for (int i = 0; i < statist.size(); i++)
		{
			stringstream s;
			s.clear();

			s << (*iter).first << "                    " << (*iter).second.first << "                                    " << (*iter).second.second;
			SendMessage(hl, LB_ADDSTRING, 0, (LPARAM)s.str().c_str());
			s.clear();
			iter++;
		}
		return (INT_PTR)TRUE;
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK: return EndDialog(hDlg, 0);
		case IDCANCEL: return EndDialog(hDlg, 0);
		}
		return TRUE;
	}
	default: return FALSE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK DialogSav(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hd = GetParent(hDlg);
	HWND hla = GetDlgItem(hDlg, IDC_COMBO1);
	static char chosen_name[20];
	switch (message)
	{
	case WM_INITDIALOG:
	{
		map<string, pair<int, int> >::iterator itera = statist.begin();
		for (int i = 0; i < statist.size(); i++)
		{
			SendMessage(hla, CB_ADDSTRING, 0, (LPARAM)(*itera).first.c_str());
			itera++;
		}
		return (INT_PTR)TRUE;
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			GetDlgItemText(hDlg, IDC_COMBO1, chosen_name, 20);
			string upload_name = chosen_name;
			if (!upload_name.empty())
			{
				curr_lvl = statist[upload_name].first - 1;
				curr_name = upload_name;
				input.clear();
				erase(lttrs_counter, temp_input, letters_num, hb_input, hb_empty, letter_tick, inp);
				InvalidateRect(GetParent(hDlg), NULL, 1);
			}
			return EndDialog(hDlg, 0);
		}
		case IDCANCEL: return EndDialog(hDlg, 0);
		}
		return TRUE;
	}
	default: return FALSE;
	}
	return (INT_PTR)FALSE;
}