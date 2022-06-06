#include<iostream>


class MST_Turystyka {
public:

	int** tabDrogi; //kontener na drogi
	int* miasta;
	int c1; // poczatkowe miast 
	int c2; // drugie miasto
	int p;  // nasza przepustowosc ludzi jaka mozemy przewiezc
	int m;  // liczba miast m<100
	int d;  // liczba drog
	int ilosc_galezi;
	int* poprawne; // beda znajdowaly sie tam poprawne drogi z jednego mista do drugiego 
	int** tabDrogi_pomocna; // jest to temp tablica drog
	int* tab_wspolrzedne; // tablca wielowymiarowa ktora bedzie przechowywala wspolrzedne


	MST_Turystyka();
	void PrintAll();
	void QuickSort(int leftSide, int rightSide);
	void Search(int num);
	void Parser();
	bool CzyMamyJeszczeDroge(int num);
	void Change(int zmien1, int zmien2);
};


int main() {
	MST_Turystyka m;
	m.PrintAll();
	return 0;
}


MST_Turystyka::MST_Turystyka() : tabDrogi(nullptr), miasta(nullptr),
c1(0), c2(0), p(0), m(0), d(0), ilosc_galezi(0),
poprawne(nullptr),
tab_wspolrzedne(nullptr),
tabDrogi_pomocna(nullptr)
{
}

void MST_Turystyka::Search(int num)
{
	bool stan_wyszukaj = false;
	for (int i = 0; i < ilosc_galezi; i++)
	{
		stan_wyszukaj = false;

		for (int j = 0; j < ilosc_galezi; j++)
			if (i == poprawne[j])
				stan_wyszukaj = true;

		if (stan_wyszukaj == false) {
			if (tabDrogi_pomocna[i][0] == num)
			{
				tab_wspolrzedne[0] = i;
				tab_wspolrzedne[1] = tabDrogi_pomocna[i][0];
				tab_wspolrzedne[2] = tabDrogi_pomocna[i][1];
				return;
			}
			if (tabDrogi_pomocna[i][1] == num)
			{
				tab_wspolrzedne[0] = i;
				tab_wspolrzedne[2] = tabDrogi_pomocna[i][0];
				tab_wspolrzedne[1] = tabDrogi_pomocna[i][1];
				std::swap(tabDrogi_pomocna[i][0], tabDrogi_pomocna[i][1]);
				return;
			}
		}
	}
}


void MST_Turystyka::PrintAll()
{
	std::cin >> m >> d; // wczytujemy liczbe miast maks 100 oraz liczbe drog

	tab_wspolrzedne = new int[3]; //alokujemy pamiec dla 3 wspolrzednych jakimi jest kolumna1 kolumna2 i kolumna3 : c1 c2 p
	tabDrogi = new int* [d];


	//wczytujemy dane
	for (int i = 0; i < d; i++)
		tabDrogi[i] = new int[3];

	miasta = new int[m + 1]; //liczba miast +1 poniewaz miasto nie moze miec numeru 0 bo 0 jest zarezerowane dla zakonczenia programu
	for (int i = 1; i <= m; i++)
		miasta[i] = i;

	for (int i = 0; i < d; i++) {
		std::cin >> c1 >> c2 >> p;
		tabDrogi[i][0] = c1;
		tabDrogi[i][1] = c2;
		tabDrogi[i][2] = p;
	}

	//sortujemy po wagach malejoco
	QuickSort(0, d - 1);


	for (int i = 0; i < d; i++) {
		if (miasta[tabDrogi[i][0]] != miasta[tabDrogi[i][1]]) {

			Change(miasta[tabDrogi[i][0]], miasta[tabDrogi[i][1]]);
			//std::cout << drogi[i][0] << " " << drogi[i][1] << " " << drogi[i][2] << std::endl;
			tabDrogi[ilosc_galezi][0] = tabDrogi[i][0];
			tabDrogi[ilosc_galezi][1] = tabDrogi[i][1];
			tabDrogi[ilosc_galezi][2] = tabDrogi[i][2];
			ilosc_galezi++;
		}
	}
	poprawne = new int[ilosc_galezi];
	tabDrogi_pomocna = new int* [ilosc_galezi];
	for (int i = 0; i < ilosc_galezi; i++)
		tabDrogi_pomocna[i] = new int[3];

	std::cin >> c1 >> c2;
	while (c1 != 0 && c2 != 0) {
		std::cin >> p;
		for (int i = 0; i < ilosc_galezi; i++) {
			for (int j = 0; j < 3; j++) {
				tabDrogi_pomocna[i][j] = tabDrogi[i][j];
			}
		}

		if (c1 != c2)
			Parser();
		std::cin >> c1 >> c2;
	}
	for (int i = 0; i < d; i++) delete[] tabDrogi[i];
	for (int i = 0; i < ilosc_galezi; i++) delete[] tabDrogi_pomocna[i];
	delete[] tabDrogi_pomocna, miasta, tabDrogi, tab_wspolrzedne, poprawne;
}


//sprawdza, czy 2 podane miasta sa na koncach jednej drogi
bool MST_Turystyka::CzyMamyJeszczeDroge(int num)
{
	for (int i = 0; i < ilosc_galezi; i++) {
		bool Flag_czyMamyJeszcze = false;
		for (int j = 0; j < ilosc_galezi; j++)
			if (i == poprawne[j]) {
				Flag_czyMamyJeszcze = true;
				break;
			}

		if (Flag_czyMamyJeszcze == false)
			if (tabDrogi_pomocna[i][0] == num || tabDrogi_pomocna[i][1] == num)
				return true;
	}
	return false;
}

void MST_Turystyka::Change(int zmien1, int zmien2)
{
	for (int j = 1; j <= m; j++)
		if (zmien2 == miasta[j])
			miasta[j] = zmien1;
}


void MST_Turystyka::Parser()
{
	for (int i = 0; i < ilosc_galezi; i++)
		poprawne[i] = -1;			// zapelniamy tablice poprawne -1 

	int ile_d = 0;	//liczenie drog
	int ile_p = 1; //zliczanie pasazerow(zaczynamy od 1 poniewaz mamy jeszcze kierowce ktory zajmuje jedno miejsce )
	int minnimum = tabDrogi[0][2]; //min waga
	int s = c1; //poczatek trasy 
	int e = c2; // koniec trasy 
	int t = p; // ilosc pasazerow

	Search(s);
	poprawne[ile_d] = tab_wspolrzedne[0];

	while (tab_wspolrzedne[2] != e)
	{
		if (CzyMamyJeszczeDroge(tab_wspolrzedne[2]) == true)
		{
			Search(tab_wspolrzedne[2]);
			ile_d++;
			poprawne[ile_d] = tab_wspolrzedne[0];
		}
		else
		{
			tab_wspolrzedne[2] = tabDrogi_pomocna[poprawne[ile_d]][0];
			tabDrogi_pomocna[poprawne[ile_d]][0] = -1;
			tabDrogi_pomocna[poprawne[ile_d]][1] = -1;
			tabDrogi_pomocna[poprawne[ile_d]][2] = -1;

			poprawne[ile_d] = -1;
			ile_d--;
		}
	}

	for (int i = 0; i <= ile_d; i++)
		if (tabDrogi[poprawne[i]][2] < minnimum)
			minnimum = tabDrogi[poprawne[i]][2];

	int ogolnie = minnimum;
	t++;
	while (true)
	{
		if (ogolnie <= t)
		{
			ogolnie = ogolnie + minnimum;
			ile_p++;
			t++;
		}
		else
			break;
	}
	std::cout << ile_p << std::endl;
}

//QuickSort, by posortowac drogi malejaco wedlug wag
void MST_Turystyka::QuickSort(int leftSide, int rightSide)
{
	int left = leftSide;
	int right = rightSide;
	int left_right = tabDrogi[(leftSide + rightSide) / 2][2];
	while (left <= right)
	{
		while (tabDrogi[left][2] > left_right) left++;
		while (tabDrogi[right][2] < left_right) right--;

		if (left <= right)
		{
			std::swap(tabDrogi[left][2], tabDrogi[right][2]);
			std::swap(tabDrogi[left][1], tabDrogi[right][1]);
			std::swap(tabDrogi[left][0], tabDrogi[right][0]);

			left++;
			right--;
		}
	}

	if (leftSide < right)QuickSort(leftSide, right);
	if (rightSide > left)QuickSort(left, rightSide);

}
