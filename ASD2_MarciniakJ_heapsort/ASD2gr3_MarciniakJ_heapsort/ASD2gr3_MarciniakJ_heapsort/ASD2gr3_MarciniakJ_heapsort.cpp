#include <iostream>
#include <vector>

class HeapSort
{
private:
	int left_root_ = 0;
	int right_root_ = 0;

public:
	HeapSort() = default;

	// Sprawdza czy podany index jest wiekszy od indexow dzieci
	// tworzy sterte z podanej tablicy
	void buildNewHeap(std::vector<int>& arr, int size, int index);

	// Sprawdza poprawność nodów
	// W przypadku nieprawidłowiej kolejnośi gdzie child ma wiekszą wartosc jak Parent
	// zamienia wartosci pomiedzy nimi 
	void Heapify(std::vector<int>& arr, int size, int index);

	// Służy do sortowania heap sortem
	void heapSort(std::vector<int>& arr);

	// wypisuje Heapa na konsole 
	static void show(std::vector<int>& arr, int size);

	// Zwraca index lewego dziecka 
	int leftRoot(int index)
	{
		left_root_ = 2 * index + 1;
		return left_root_;
	}

	// Zwraca index prawego dziecka
	int rightRoot(int index)
	{
		right_root_ = 2 * index + 2;
		return right_root_;
	}
};

int main()
{
	HeapSort heap;
	int size; //N
	std::cin >> size;

	std::vector<int> doSortu;
	for (int i = 0; i < size; i++) {
		int iloscDoSortu; // k
		int sort; 
		std::cin >> iloscDoSortu;
		for (int j = 0; j < iloscDoSortu; j++) {
			std::cin >> sort;
			doSortu.push_back(sort);
		}
		heap.heapSort(doSortu);

		for (int l = 0; l < static_cast<int>(doSortu.size()); l++)
			std::cout << doSortu[l] << " ";

		std::cout << std::endl << std::endl;
		doSortu.clear();
	}
}

void HeapSort::Heapify(std::vector<int>& arr, const int size, const int index)
{
	if (leftRoot(index) < size && arr[leftRoot(index)] > arr[index]){
		std::swap(arr[leftRoot(index)], arr[index]);
		Heapify(arr, size, leftRoot(index));
	}

	if (rightRoot(index) < size && arr[rightRoot(index)] > arr[index]) {
		std::swap(arr[rightRoot(index)], arr[index]);
		Heapify(arr, size, rightRoot(index));
	}
}

void HeapSort::buildNewHeap(std::vector<int>& arr, const int size, const int index)
{
	if (leftRoot(index) < size)
		buildNewHeap(arr, size, leftRoot(index));
	if (rightRoot(index) < size)
		buildNewHeap(arr, size, rightRoot(index));
	Heapify(arr, size, index);
}

void HeapSort::heapSort(std::vector<int>& arr)
{
	//heap sort
	for (int i = arr.size(); i >= 1; --i)
	{
		buildNewHeap(arr, i, 0);
		show(arr, i);
		std::swap(arr[0], arr[i - 1]);
	}
}

void HeapSort::show(std::vector<int>& arr, int size)
{
	if (size < 2) return;
	for (int i = 0; i < size; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}