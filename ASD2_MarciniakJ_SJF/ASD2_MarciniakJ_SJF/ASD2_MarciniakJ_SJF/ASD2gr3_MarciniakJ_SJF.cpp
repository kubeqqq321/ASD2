#include <iostream>

struct Punkty {
	long long x;
	long long y;
};
bool operator == (Punkty X, Punkty Y)
{
	if ((X.x == Y.x) && (X.y == Y.y))
		return true;
	return false;
}

long long iloczynWektorowy(Punkty X, Punkty Y, Punkty Z)
{
	return (Y.y - X.y) * (Z.x - Y.x) - (Y.x - X.x) * (Z.y - Y.y);
}

bool CzyWSrodku(Punkty X, Punkty Y, Punkty sprawdzany)
{
	Punkty wektor, wektorSprawdzany;
	wektor.x = Y.x - X.x;
	wektor.y = Y.y - X.y;
	wektorSprawdzany.x = sprawdzany.x - X.x;
	wektorSprawdzany.y = sprawdzany.y - X.y;

	//liczenie iloczynu wektorowego w celu sprawdzenia czy maly lampion lezy na prawo od otoczy czy na lewo
	long long iloczynWektorowy;
	iloczynWektorowy = (wektor.x * wektorSprawdzany.y) - (wektorSprawdzany.x * wektor.y);
	if (iloczynWektorowy < 0)
		return false;
	return true;
}

bool PrzeciwnyDoWskazowekZegara(Punkty X, Punkty Y, Punkty Z)
{
	if (iloczynWektorowy(X, Y, Z) < 0)
		return true;
	return false;
}

Punkty najnizszyPunkt(Punkty tablica[], int ilosc)
{
	Punkty najnizszy = tablica[0];

	//punkt z najnizszym y
	for (int i = 1; i < ilosc; i++)
		if (tablica[i].y < najnizszy.y)
			najnizszy = tablica[i];

	//jezeli mamu kilka punktow w tym samym y to wybieramy ten z najnizszym x
	for (int i = 0; i < ilosc; i++)
		if ((tablica[i].y == najnizszy.y) && (tablica[i].x < najnizszy.x))
			najnizszy = tablica[i];

	return najnizszy;
}

int tworzOtoczkeWypukla(Punkty tablicaDuzychLapmionow[], Punkty Otoczka[], int iloscDuzych)
{
	//poniewaz znamy juz punkt startowy dlatego ileWOtoczce = 1
	int ileWOtoczce = 1;
	int index = 0, q = 0;

	//wpisujemy index punktu zerowego tab duzych lampionwo do zmiennej index
	for (int i = 0; i < iloscDuzych; i++)
	{
		if (tablicaDuzychLapmionow[i] == Otoczka[0])
		{
			index = i;
			break;
		}
	}

	//pierszy przebieg petli do while bez dopiswania do OtoczkiWypukłej
	for (int i = 0; i < iloscDuzych; i++)
		if (PrzeciwnyDoWskazowekZegara(tablicaDuzychLapmionow[index], tablicaDuzychLapmionow[i], tablicaDuzychLapmionow[q]))
			q = i;

	index = q;
	do
	{
		Otoczka[ileWOtoczce] = tablicaDuzychLapmionow[index];
		ileWOtoczce++;
		q = (index + 1) % iloscDuzych;

		for (int i = 0; i < iloscDuzych; i++)
			if (PrzeciwnyDoWskazowekZegara(tablicaDuzychLapmionow[index], tablicaDuzychLapmionow[i], tablicaDuzychLapmionow[q]))
				q = i;

		index = q;
	} while (!(tablicaDuzychLapmionow[index] == Otoczka[0]));
	return ileWOtoczce;
}

int main()
{
	int iloscDluzych = 0; // 3 < L < 10000
	int iloscMalych = 0;  // 1 < S < 50000

	////wczytywanie duzych lampionów
	std::cin >> iloscDluzych;
	Punkty* duze = new Punkty[iloscDluzych];
	for (int i = 0; i < iloscDluzych; i++)
		std::cin >> duze[i].x >> duze[i].y;

	//wczytywanie malych lampionow
	std::cin >> iloscMalych;
	Punkty* male = new Punkty[iloscMalych];
	for (int i = 0; i < iloscMalych; i++)
		std::cin >> male[i].x >> male[i].y;

	//------------------Algorytm grahama here---------------------------------

	//szukamy najnizszego punktu o najnizszym x i y
	Punkty najnizej = najnizszyPunkt(duze, iloscDluzych);

	//przesuwamy wszytskie punkty w taki sposob by znalazly sie one na poczatku ukladu wspolrzednych
	for (int i = 0; i < iloscDluzych; i++)
	{
		duze[i].x -= najnizej.x;
		duze[i].y -= najnizej.y;
	}

	for (int i = 0; i < iloscMalych; i++)
	{
		male[i].x -= najnizej.x;
		male[i].y -= najnizej.y;
	}

	//Szukamy otoczke wypukla dla Punktow ktore sa w duzej otoczce
	//Do znalezienia otoczki wypuklej wykorzystujemy algorytm Jarvisa
	Punkty* otoczkaWypukla = new Punkty[iloscDluzych];
	otoczkaWypukla[0].x = 0;
	otoczkaWypukla[0].y = 0;

	// budujemy otoczke i zwracamy ilosc elementow otoczki
	int iloscElementowOtoczki = tworzOtoczkeWypukla(duze, otoczkaWypukla, iloscDluzych);

	//sprawdzamy czy male lampiony sa w otoczce czy tez nie
	bool* czySrodkowy = new bool[iloscMalych]; //czy male lampiony sa wewnatrz otoczki

	//zakładamy najpierw ze wsytkie male lamipony sa w srodku
	for (int i = 0; i < iloscMalych; i++)
		czySrodkowy[i] = true;

	for (int i = 0; i < iloscElementowOtoczki - 1; i++)
	{
		for (int j = 0; j < iloscMalych; j++)
		{
			//jezeli male lampiony nie sa w srodku to zmiemanimy ich wartosc na false
			if (!CzyWSrodku(otoczkaWypukla[i], otoczkaWypukla[i + 1], male[j]) && czySrodkowy[j])
				czySrodkowy[j] = false;
		}
	}

	//bierzemy pod uwage ostatni odcinek od odtatniego punktu do punktu startowego
	for (int i = 0; i < iloscMalych; i++)
	{
		if (!CzyWSrodku(otoczkaWypukla[iloscElementowOtoczki - 1], otoczkaWypukla[0], male[i]) && czySrodkowy[i])
			czySrodkowy[i] = false;
	}

	//wypisujemy ile bylo malych lampionow w otoczce
	int ileMalychWewnatrz = 0;
	for (int i = 0; i < iloscMalych; i++)
	{
		if (czySrodkowy[i])
			ileMalychWewnatrz++;
	}

	//wyswietlamy odpowiedz w tym miejscu
	std::cout << ileMalychWewnatrz << std::endl;

	delete[] czySrodkowy;
	delete[] otoczkaWypukla;
	delete[] male;
	delete[] duze;
}