#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct FlowerInfo
{
	string name;
	int price;
	double weight;
	bool predator;
	void Out();
	bool Seria(char*& data, int& n);
};

void FlowerInfo::Out()
{
	cout << "Название: " << name << "\t" << "Цена: " << price << "\t" << "Вес: " << weight << "\t" << "Хищное ли растение? " << predator << endl << endl;
}

bool FlowerInfo::Seria(char*& data, int& n)
{
	size_t s = name.size();

	int n0 = sizeof(size_t);
	int n1 = s;
	int n2 = sizeof(int);
	int n3 = sizeof(double);
	int n4 = sizeof(bool);

	n = n0 + n1 + n2 + n3 + n4;

	data = new char[n];

	char* d0 = reinterpret_cast<char*>(&s);
	char* d1 = const_cast<char*>(name.c_str());
	char* d2 = reinterpret_cast<char*>(&price);
	char* d3 = reinterpret_cast<char*>(&weight);
	char* d4 = reinterpret_cast<char*>(&predator);

	for (int i = 0; i < n0; i++) data[i] = d0[i];
	for (int i = 0; i < n1; i++) data[i + n0] = d1[i];
	for (int i = 0; i < n2; i++) data[i + n0 + n1] = d2[i];
	for (int i = 0; i < n3; i++) data[i + n0 + n1 + n2] = d3[i];
	for (int i = 0; i < n4; i++) data[i + n0 + n1 + n2 + n3] = d4[i];

	return true;
}

FlowerInfo DeSeria(char* data, int n)
{
	FlowerInfo temp;
	int n0, n1, n2, n3, n4;
	n0 = sizeof(size_t);
	n2 = sizeof(int);
	n3 = sizeof(double);
	n4 = sizeof(bool);

	size_t s = *reinterpret_cast<size_t*>(data);
	n1 = s;

	string ss(data + n0, s);
	temp.name = ss;
	temp.price = *reinterpret_cast<int*>(data + n0 + n1);
	temp.weight = *reinterpret_cast<double*>(data + n0 + n1 + n2);
	temp.predator = *reinterpret_cast<int*>(data + n0 + n1 + n2 + n3);

	return temp;
}

struct MyQueue
{
	struct Node
	{
		int n;
		char* data;
		Node* next;
	};
	Node* First = NULL;
	string Top;
	int Count = 0;
	bool Push(char*, int);
	bool Pop(char*&, int&);
	void Info();
};

bool MyQueue::Push(char* data, int n)
{
	if (!First)
	{
		First = new Node;
		First->next = NULL;
		First->data = new char[n];
		for (int i = 0; i < n; i++)
		{
			First->data[i] = data[i];
		}
		First->n = n;
		Count = 1;
	}
	else
	{
		Node* temp;
		temp = First;
		while (temp->next != NULL) temp = temp->next;
		temp->next = new Node;
		temp->next->next = NULL;
		temp->next->data = new char[n];
		for (int i = 0; i < n; i++)
		{
			temp->next->data[i] = data[i];
		}
		temp->next->n = n;
		Count++;
	}
	return true;
}

bool MyQueue::Pop(char*& data, int& n)
{
	if (!First) return false;
	Node* temp = First->next;
	n = First->n;
	data = new char[n];
	for (int i = 0; i < n; i++)
	{
		data[i] = First->data[i];
	}
	delete First->data;
	delete First;
	First = temp;
	Count--;
	return true;
}

void MyQueue::Info()
{
	if (!First) cout << "Очередь пуста" << endl << endl;
	else
	{
		cout << "Информация об очереди: " << endl;
		cout << "Размер очереди = " << Count << endl << endl;
		cout << "Первый товар:" << endl;
		cout << "Длина серии = " << First->n << endl;
		FlowerInfo Info;
		Info = DeSeria(First->data, First->n);
		Info.Out();
	}
}

FlowerInfo DeSeria(char*, int);
void PrintBinaryFile(char*, int);
void ClearBinaryFile();
bool InputBinaryFile(MyQueue&);
void GetFlower(MyQueue&, FlowerInfo&);
bool RemoveFlower(MyQueue&, FlowerInfo&);

void main()
{
	setlocale(LC_ALL, "Russian");
	FlowerInfo Info;
	MyQueue Queue;
	int key = 0;
	char* data; int ssize;
	InputBinaryFile(Queue);
	do
	{
		cout << "1) Добавить товар" << endl
			<< "2) Вытащить товар" << endl
			<< "3) Опустошить корзину" << endl
			<< "4) Информация о корзине" << endl
			<< "5) Данные серии" << endl
			<< "6) Очистить файл" << endl
			<< "0) Выход" << endl;
		cout << endl << "Выберите действие: ";
		cin >> key;
		cout << endl;
		switch (key)
		{
		case 1:
			cout << "Введите название: ";
			cin >> Info.name;
			cout << endl;
			cout << "Цена: ";
			cin >> Info.price;
			cout << endl;
			cout << "Вес: ";
			cin >> Info.weight;
			cout << endl;
			cout << "Хищное ли растение? ";
			cin >> Info.predator;
			cout << endl;
			Info.Seria(data, ssize);
			Queue.Push(data, ssize);
			Queue.Info();
			delete[] data;
			break;
		case 2:
			cout << "Введите название: ";
			cin >> Info.name;
			cout << endl;
			cout << "Цена: ";
			cin >> Info.price;
			cout << endl;
			cout << "Вес: ";
			cin >> Info.weight;
			cout << endl;
			cout << "Хищное ли растение? ";
			cin >> Info.predator;
			cout << endl;
			RemoveFlower(Queue, Info);
			Queue.Info();
			break;
		case 3:
			while (Queue.Pop(data, ssize))
			{
				Info = DeSeria(data, ssize);
				Info.Out();
				delete[] data;
			}
			Queue.Info();
			break;
		case 4:
			Queue.Info();
			break;
		case 5:
			cout << "Введите название: ";
			cin >> Info.name;
			cout << endl;
			cout << "Введите цену: ";
			cin >> Info.price;
			cout << endl;
			cout << "Введите вес: ";
			cin >> Info.weight;
			cout << endl;
			cout << "Хищное ли растение?: ";
			cin >> Info.predator;
			cout << endl;
			Info.Seria(data, ssize);
			PrintBinaryFile(data, ssize);
			delete[] data;
			break;
		case 6:
			ClearBinaryFile();
			cout << "Файл очищен!" << endl;
			break;
		default:
			if (key != 0)
			{
				cout << "Нет такого действия!" << endl << endl;
			}
			if (key == 0)
			{
				while (Queue.Pop(data, ssize))
				{
					delete[] data;
				}
				Queue.Info();
			}
			break;
		}
	} while (key != 0);
}

void PrintBinaryFile(char* data, int n)
{
	fstream out("out.dat", ios::app | ios::binary);
	out.write((char*)&n, sizeof(int));
	out.write(data, n);
	out.close();
}

void ClearBinaryFile()
{
	fstream out("out.dat", ios::out | ios::binary);
	out.close();
}

bool InputBinaryFile(MyQueue& Queue)
{
	fstream in("out.dat", ios::in | ios::binary);
	if (!in)
	{
		cout << "Нет бинарного файла" << endl;
		return false;
	}

	int i = 1, n;
	FlowerInfo Info;

	while (!in.eof())
	{
		if (in.read((char*)&n, sizeof(int)))
		{
			cout << i << ")" << " Длина серии: " << n << endl << endl;
			char* data = new char[n];
			in.read(data, n);
			i++;
			Queue.Push(data, n);
			Info = DeSeria(data, n);
			Info.Out();
			delete[] data;
		}
	}
	in.close();
	return true;
}

bool RemoveFlower(MyQueue& Queue, FlowerInfo& TempInfo)
{
	MyQueue QueueTemp;
	FlowerInfo Info;
	char* data = NULL;
	int n;
	bool stopFind = false, find = false;
	while (stopFind == false)
	{
		Info = DeSeria(Queue.First->data, Queue.First->n);
		if ((Info.name == TempInfo.name) && (Info.price == TempInfo.price) && (Info.weight == TempInfo.weight) && (Info.predator == TempInfo.predator))
		{
			Queue.Pop(data, n);
			find = true;
			stopFind = true;
			break;
		}
		QueueTemp.Push(Queue.First->data, Queue.First->n);
		Queue.Pop(data, n);
		if (Queue.First == NULL)
		{
			stopFind = true;
			break;
		}
	}
	if (find == true)
	{
		cout << endl << "Цветок успешно удален" << endl << endl;
	}
	else
	{
		cout << endl << "Такого цветка нет" << endl << endl;
	}
	stopFind = false;
	while (stopFind == false)
	{
		if (find != true)
		{
			Queue.Push(QueueTemp.First->data, QueueTemp.First->n);
			QueueTemp.Pop(data, n);
		}
		find = false;
		if (QueueTemp.First == NULL) stopFind = true;
	}
	find = false;
	delete[] data;
	return true;
}
