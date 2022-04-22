#include <vector>
#include <string>
#include <iostream>
#include <optional>

enum class Events
{
	Size, Print, Add, Delete, Stop, End
}; Events ChangerEvents(const std::string& txt)
{
	if (txt == "size") { return Events::Size; }
	if (txt == "add") { return Events::Add; }
	if (txt == "print") { return Events::Print; }
	if (txt == "stop") { return Events::Stop; }
	if (txt == "delete") { return Events::Delete; }
	return Events::End;
}

struct structHash
{
private:
	long wartosc;

public:
	explicit structHash(long wartosc_);
	explicit operator long() const;
	bool operator ==(const structHash& inny) const;
	bool operator !=(const structHash& inny) const;
	structHash operator ++(int);
	structHash operator --(int);
};

class HashTable
{
private:
	int size;
	std::vector<std::optional<std::pair<long, std::string>>> hashPairs;

	int get_size() const;
	structHash tworzHasha(long key);
	std::optional<std::pair<long, std::string>>& data_hash(structHash struct_hash);
	void tworzReHasha(structHash struct_hash);
public:
	explicit HashTable(int size_);
	void add(long key, std::string value);
	void remove(long key);
	void print();
};

int main()
{
	int theBeginNumber; std::cin >> theBeginNumber;
	HashTable* tableHash{};
	while (theBeginNumber--)
	{
		bool end = false;
		while (!end)
		{
			std::string txt; std::cin >> txt;
			switch (ChangerEvents(txt))
			{
			case Events::Size:
			{
				int wielkosc; std::cin >> wielkosc;
				tableHash = new HashTable(wielkosc);
				break;
			}

			case Events::Add:
			{
				int key; std::string str;
				std::cin >> key >> str;
				tableHash->add(key, str);
				break;
			}

			case Events::Print:
			{
				tableHash->print();
				break;
			}

			case Events::Stop:
			{
				theBeginNumber = true;
				delete tableHash;
				break;
			}

			case Events::Delete:
			{
				int key; std::cin >> key;
				tableHash->remove(key);
				break;
			}

			case Events::End:
			{
				exit(1);
			}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//Struktura hashowa Structhash
structHash::structHash(long wartosc_) : wartosc(wartosc_)
{}

structHash::operator long() const
{
	return wartosc;
}

bool structHash::operator==(const structHash& inny) const
{
	return wartosc == inny.wartosc;
}

bool structHash::operator!=(const structHash& inny) const
{
	return wartosc != inny.wartosc;
}

structHash structHash::operator++(int)
{
	return structHash(wartosc++);
}

structHash structHash::operator--(int)
{
	return structHash(wartosc--);
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//Klasa HashTable
//private
int HashTable::get_size() const
{
	return this->size;
}

structHash HashTable::tworzHasha(long key)
{
	return structHash(key % get_size());
}

std::optional<std::pair<long, std::string>>& HashTable::data_hash(structHash struct_hash)
{
	return hashPairs.at(static_cast<long>(struct_hash));
}

void HashTable::tworzReHasha(structHash struct_hash)
{
	auto poz = static_cast<long>(struct_hash);
	while ((++poz % get_size()) != static_cast<long>(struct_hash))
	{
		const auto val = data_hash(structHash(poz % get_size()));
		if (!val) { continue; }											// jeżeli mamy pusty val konczymy
		if (tworzHasha(val->first) != struct_hash) { continue; }		// jeśli hashe nie są takie same kończymy
		data_hash(structHash(poz % get_size())) = std::nullopt;			// czyścimy miejsce gdzie poprzednio był Hash
		add(val->first, val->second);									// wstawiamy tam gdzie jest najbliższe miejsce
	}

	poz = static_cast<long>(struct_hash);
	while ((++poz % get_size()) != static_cast<long>(struct_hash))
	{
		auto val = data_hash(structHash(poz % get_size()));
		if (!val) { continue; }											// jeżeli mamy pusty val konczymy
		if (tworzHasha(val->first) == struct_hash) { continue; }		// jeśli hashe są takie same kończymy
		data_hash(structHash(poz % get_size())) = std::nullopt;			// czyścimy miejsce gdzie poprzednio był Hash
		add(val->first, val->second);									// wstawiamy klucz i wartosc tam gdzie jest najbliższe miejsce
	}
}

//public
HashTable::HashTable(int size_) : size(size_)
{
	for (int i = 0; i < get_size(); ++i)
	{
		hashPairs.emplace_back();
	}
}

void HashTable::add(long key, std::string value)
{
	structHash hashS = tworzHasha(key);
	while (data_hash(structHash(static_cast<long>(hashS) % get_size())).has_value())
	{
		hashS++;										// jeżeli mamy już HAsha w danym miejcu szukamy najbliższego wolnego miejsca
	}
	data_hash(structHash(static_cast<long>(hashS) % get_size())) = std::make_pair(key, value); // jeżeli miejce zostało znalezione tworzymy pare z klucza i jego warosc
}

void HashTable::remove(long key)
{
	structHash hashS = tworzHasha(key);
	while (data_hash(structHash(static_cast<long>(hashS) % get_size()))->first != key)
	{
		hashS++;									// jeżeli nasz szukany klucz jest różny od klucza w Hashu idzemy dalej
	}
	const auto val = data_hash(hashS)->first;	// ustwaimy nowy klucz dla tworzHasha()
	data_hash(hashS) = std::nullopt;				// czyścimy miejsce w którym znajdował się Hash
	tworzReHasha(tworzHasha(val));			// tworzymy nowego hasha i zmieniamy jego pozycje jezeli jest taka potrzreba
}

void HashTable::print()								// printujemy nasza odpowiedz
{
	for (int i = 0; i < get_size(); i++)
	{
		if (data_hash(structHash(i)).has_value())
		{
			std::cout << i << " " <<							//size
				data_hash(structHash(i))->first << " " <<		// klucz hash
				data_hash(structHash(i))->second << std::endl;  // wartosć klucza
		}
	}
	std::cout << std::endl;
}
//--------------------------------------------------------------------------------