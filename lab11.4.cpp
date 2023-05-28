#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>

using namespace std;

struct Product {
	char name[50];
	char store[50];
	float price;
	char quantity[10];
};

void PrintBIN(char* filename)
{
	ifstream fin(filename, ios::binary);

	if (!fin.is_open()) {
		cout << "Помилка відкритя файлу!" << endl;
		return;
	}

	cout << "====================================================================="
		<< endl;
	cout << "|  №  |  Назва товару  |     Магазин     |  Ціна  | Од. вимірювання |"
		<< endl;
	cout << "---------------------------------------------------------------------"
		<< endl;

	Product product;
	int i = 1;
	while (fin.read((char*)&product, sizeof(product))) {
		cout << "| " << setw(3) << right << i << " ";
		cout << "| " << setw(15) << left << product.name
			<< "| " << setw(16) << left << product.store
			<< "| " << setw(7) << left << product.price
			<< "| " << setw(15) << left << product.quantity
			<< " |" << endl;

		i++;
	}
	cout << "====================================================================="
		<< endl;
	cout << endl;

	fin.close();
}

void PrintByStore(char* filename)
{
	ifstream fin(filename, ios::binary);

	if (!fin.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	Product product;
	string storeName;
	cout << "Введіть назву магазину: ";
	cin >> storeName;
	cout << endl;

	cout << "Товари в магазині \"" << storeName << "\":" << endl << endl;

	bool found = false;
	while (fin.read((char*)&product, sizeof(product))) {
		if (product.store == storeName) {
			cout << "Назва товару: " << product.name << endl;
			cout << "Магазин: " << product.store << endl;
			cout << "Ціна: " << product.price << endl;
			cout << "Кількість: " << product.quantity << endl;
			cout << endl;
			found = true;
		}
	}

	if (!found) {
		cout << "Товарів в магазині \"" << storeName << "\" не знайдено." << endl;
	}
}

void PrintByPriceRange(char* filename, double minPrice, double maxPrice)
{
	ifstream fin(filename, ios::binary);

	if (!fin.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	Product product;

	cout << "Продукти в діапазоні вартості від " << minPrice << " до " << maxPrice << ":" << endl << endl;
	cout << setw(15) << left << "Назва"
		<< setw(20) << left << "Магазин"
		<< setw(10) << left << "Ціна"
		<< setw(15) << left << "Кількість" << endl;
	cout << "------------------------------------------------------------" << endl;
	while (fin.read((char*)&product, sizeof(product))) {
		if (product.price >= minPrice && product.price <= maxPrice) {
			cout << setw(15) << left << product.name
				<< setw(20) << left << product.store
				<< setw(10) << left << product.price
				<< setw(15) << left << product.quantity << endl;
		}
	}
}

void AddProduct(char* filename)
{
	Product newProduct;

	ofstream fout(filename, ios::app | ios::binary);

	if (!fout.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	cin.get();
	cout << "Введіть назву продукту: ";
	cin >> newProduct.name;
	cout << "Введіть назву магазину: ";
	cin >> newProduct.store;
	cout << "Введіть ціну продукту: ";
	cin >> newProduct.price;
	cin.ignore();
	cout << "Введіть кількість продукту: ";
	cin.getline(newProduct.quantity, 9);

	fout.write((char*)&newProduct, sizeof(newProduct));

	fout.close();
}

void DeleteProduct(char* filename, const int pos)
{
	char ftmpname[100] = "tmp.dat";
	ifstream fin(filename, ios::binary);
	ofstream tmpFile(ftmpname, ios::binary);

	if (!fin.is_open())
	{
		cerr << "Помилка відкриття файлу: " << filename << "! " << endl;
		return;
	}

	if (!tmpFile.is_open())
	{
		cerr << "Помилка створення тимчасового файлу: " << ftmpname << "! " << endl;
		return;
	}

	Product p;
	int i = 0;

	while (fin.read((char*)&p, sizeof(p)))
	{
		i++;
		if (pos != i)
			tmpFile.write((char*)&p, sizeof(p));
	}

	fin.close();
	tmpFile.close();

	if (remove(filename) == -1)
	{
		cout << "Не можливо видалити початковий файл: " << filename << endl;
		return;
	}

	if (rename(ftmpname, filename) != 0)
	{
		cout << "Не можливо переіменувати тимчасовий файл !" << endl;
		return;
	}

	cout << endl;
	cout << "Товар успішно видалено!" << endl;
	cout << endl;
}

void EditProduct(char* filename, const int pos)
{
	fstream f(filename, ios::binary | ios::in | ios::out);

	if (!f.is_open())
	{
		cout << "Помилка відкриття файлу: " << filename << "! " << endl;
		return;
	}

	Product p;

	f.seekg(0, ios::end);
	long size = (long)f.tellg() / (long)sizeof(p);
	f.seekg(0, ios::beg);

	if (pos > size)
		cout << "Введено не вірний номер товару!";
	else
	{
		cin.get();
		cin.sync();
		cout << "Назва: ";
		cin.getline(p.name, 49);
		cout << "Магазин: ";
		cin.getline(p.store, 49);
		cout << "Ціна: ";
		cin >> p.price;
		cin.ignore();
		cout << "Кількість: ";
		cin.getline(p.quantity, 9);
		cout << endl;

		f.seekp((pos - 1) * sizeof(Product), ios::beg);
		f.write((char*)&p, sizeof(p));
		f.seekg(0, ios::beg);
	}

	f.close();
}

Product fRead(fstream& f, const int pos)
{
	Product p;
	f.seekg(pos * (long)sizeof(p));
	f.read((char*)&p, sizeof(p));
	return p;
}


void fWrite(fstream& f, const int pos, const Product p)
{
	f.seekp(pos * (long)sizeof(p));
	f.write((char*)&p, sizeof(p));
}

void fChange(fstream& f, const int pos1, const int pos2)
{
	Product p1 = fRead(f, pos1);
	Product p2 = fRead(f, pos2);
	fWrite(f, pos1, p2);
	fWrite(f, pos2, p1);
}

void SortByStore(char* filename)
{
	fstream f(filename, ios::binary | ios::in | ios::out);

	if (!f.is_open())
	{
		cerr << "Помилка відкриття файлу: " << filename << "! " << endl;
		return;
	}

	Product a, b;
	f.seekg(0, ios::end);
	long size = (long)f.tellg() / (long)sizeof(a);
	f.seekg(0, ios::beg);
	for (int i0 = 1; i0 < size; i0++)
		for (int i1 = 0; i1 < size - i0; i1++)
		{
			a = fRead(f, i1);
			b = fRead(f, i1 + 1);
			if ((string)a.store > (string)b.store)
			{
				fChange(f, i1, i1 + 1);
			}
		}
	f.seekp(0, ios::end);
	f.close();
}

void SortByTotalPrice(char* filename)
{
	fstream f(filename, ios::binary | ios::in | ios::out);

	if (!f.is_open())
	{
		cerr << "Помилка відкриття файлу: " << filename << "! " << endl;
		return;
	}

	Product a, b;
	f.seekg(0, ios::end);
	long size = (long)f.tellg() / (long)sizeof(a);
	f.seekg(0, ios::beg);
	for (int i0 = 1; i0 < size; i0++)
		for (int i1 = 0; i1 < size - i0; i1++)
		{
			a = fRead(f, i1);
			b = fRead(f, i1 + 1);
			if (a.price > b.price)
			{
				fChange(f, i1, i1 + 1);
			}
		}
	f.seekp(0, ios::end);
	f.close();
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char fileName[50];
	int num;

	cout << "Введіть ім'я файлу: ";
	cin.getline(fileName, 49);

	int menuItem;

	do {
		cout << endl << endl << endl;
		cout << "Виберіть дію:" << endl << endl;
		cout << " [1] - вивід даних на екран" << endl;
		cout << " [2] - + додати продукт" << endl;
		cout << " [3] - - прибрати продукт" << endl;
		cout << " [4] - * змінити продукт" << endl;
		cout << " [5] - сортування" << endl << endl;
		cout << " [0] - вихід та завершення роботи програми" << endl << endl;
		cout << "Введіть значення: "; cin >> menuItem;
		cout << endl << endl << endl;

		switch (menuItem)
		{
		case 1:
			int printMenuItem;
			cout << "Виберіть дію:" << endl << endl;
			cout << " [1] - вивести список всіх товарів" << endl;
			cout << " [2] - вивести список товарів в конкретному магазині" << endl;
			cout << " [3] - вивести список в діапазоні цін" << endl << endl;
			cout << endl << "Введіть значення: "; cin >> printMenuItem;
			cout << endl << endl;

			switch (printMenuItem)
			{
			case 1:
				PrintBIN(fileName);
				break;
			case 2:
				PrintByStore(fileName);
				break;
			case 3:
				double minPrice;
				double maxPrice;

				cout << "Мінімальна ціна: " << endl;
				cin >> minPrice;
				cout << "Максимальна ціна: " << endl;
				cin >> maxPrice;

				PrintByPriceRange(fileName, minPrice, maxPrice);
				break;
			default:
				cout << "Ви ввели помилкове значення! "
					"Слід ввести число - номер вибраного пункту меню" << endl;
			}
			break;
		case 2:
			AddProduct(fileName);
			break;
		case 3:
			cout << "Номер товару: ";
			cin >> num;
			DeleteProduct(fileName, num);
			break;
		case 4:
			cout << "Номер товару: ";
			cin >> num;
			EditProduct(fileName, num);
			break;
		case 5:
			int sortMenuItem;
			cout << "Сортувати за:" << endl;
			cout << " [1] - назвою магазину" << endl;
			cout << " [2] - загальною вартістю" << endl;
			cout << "Введіть значення: "; cin >> sortMenuItem;
			cout << endl << endl << endl;
			switch (sortMenuItem)
			{
			case 1:
				SortByStore(fileName);
				break;
			case 2:
				SortByTotalPrice(fileName);
				break;
			default:
				cout << "Ви ввели помилкове значення! "
					"Слід ввести число - номер вибраного пункту меню" << endl;
			}
			break;
		case 0:
			break;
		default:
			cout << "Ви ввели помилкове значення! "
				"Слід ввести число - номер вибраного пункту меню" << endl;
		}
	} while (menuItem != 0);

	return 0;
}
