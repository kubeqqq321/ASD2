#include <fstream>
#include <iostream>
#include <string>
#include <functional>

int main()
{    
	setlocale(LC_ALL, "");      //Dzieki temu możemy uzywać polskich liter 

	std::string szukany_ciag = "";
	std::string caly_ciag;
    std::string nazwa_pliku;
    std::string temp;
    int liczba_przypadkow;
    int dlugosc_caly_ciag;
    int dlugosc_szukany_ciag;
	unsigned long long int ZaHashowanySzukanyCiag;
	unsigned long long int ZaHashowanaCzesc;
	std::hash <std::string> Haszuj;
    std::ifstream plik;

    std::cin >> liczba_przypadkow;
    for (int i = 0; i < liczba_przypadkow; i++)
    {
        std::cin >> nazwa_pliku;
        std::cin.ignore(); // to wywołanie usunie z bufora znak '\n' pozostawiony przez obiekt "cin"
        std::getline(std::cin, szukany_ciag); //wczytujemy nasz szukany ciag

		//owieramy plik
        plik.open(nazwa_pliku, std::ios::in);
        if(plik.good() == true)
        {
	        while (!plik.eof())
	        {
                std::getline(plik, temp);
                caly_ciag += temp;
	        }

            plik.close();
            temp = "";
        }

        ZaHashowanySzukanyCiag = Haszuj(szukany_ciag);       // haszujemy nasza fraze
        dlugosc_caly_ciag = caly_ciag.length();        // dlugosc ciagu calego 
        dlugosc_szukany_ciag = szukany_ciag.length();  // dlugosc ciagu szukanego

        for (int polozenie = 0; polozenie < dlugosc_caly_ciag - dlugosc_szukany_ciag; polozenie++)
        {
            for (int j = polozenie; j < polozenie + dlugosc_szukany_ciag; j++)
                temp += caly_ciag[j];
            
            ZaHashowanaCzesc = Haszuj(temp);

            //porównywanie hashów jezeli takie same wypisujemy polozenie ciagu szukanego
            if (ZaHashowanaCzesc == ZaHashowanySzukanyCiag) 
                if (temp == szukany_ciag)
                    std::cout << polozenie << " ";

            temp = "";
        }

        std::cout << std::endl;
        caly_ciag = "";

    }

    return 0;
}
