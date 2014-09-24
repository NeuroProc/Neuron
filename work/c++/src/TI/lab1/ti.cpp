#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctype.h>

using namespace std;

#define LEN 26
#define LET 'A'

int primes[5000];

class ALPHABETH {
	char letter[LEN];
	char start = LET;
	int length = LEN;
public:
	ALPHABETH()
	{
		for (int i = 0; i < length; i++)
			letter[i] = start++;
	};
	void output() { for (int i = 0; i < length; i++) cout << letter[i] << " "; cout << endl; };
	int size() { return length; };
	char &operator[](int i) { return letter[i]; };
};

ALPHABETH a;

int L_num(char l)
{
	int i;

	for (i = 0; i < a.size(); i++)
        if (l == a[i])
            return i;

	return -1;
}

class TEXT {
	string str;
public:
	TEXT(){};
	TEXT(size_t l) { str.resize(l); };
	void show() { cout << str << endl; };
	int size() { return str.length(); };
	void ToUpper()
    { //transform(str.begin(), str.end(), str.begin(), toupper);

        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] < 'A' || str[i] > 'z' || str[i] == '*')
            {
                str.erase(i, 1);
                i--;
            }
            
            str[i] = (char)toupper(str[i]);
        }
    }
	void MakeLoop(int len) 
	{ 
		int i = str.length();
		for (int j = 0; j < str.length(), i < len; j++, i++)
			str += str[j];
	}
	void MakeLoopP(int len)
	{
		int c = 1;
		string temp = str;
		int j = 0;
		for (int i = str.length(); i < len; i++)
		{
			int ind = L_num(temp[j]) + c;
			if (ind > LEN - 1)
				str += a[ind - LEN];
			else
				str += a[ind];

			if (j == (temp.length() - 1))
			{
				++c;
				j = 0;
				continue;
			}
			j++;
		}
	};
	void MakeLoopA(int len, TEXT Mm)
	{
		string t = Mm.str;
		str += t;
	}
    
	char &operator[](int i) { return str[i]; };
	TEXT operator+(TEXT op2)
	{
		TEXT temp;
		temp.str = str + op2.str;
		return temp;
	}
	friend ostream &operator<<(ostream &stream, TEXT &obj)
	{
		stream << obj.str;
		return stream;
	};
	friend istream &operator>>(istream &stream, TEXT &obj)
	{
		stream >> obj.str;
		return stream;
	};
	friend ifstream &operator>>(ifstream &stream, TEXT &obj)
	{
		stream >> obj.str;
		return stream;
	};
	friend ofstream &operator<<(ofstream &stream, TEXT &obj)
	{
		stream << obj.str;
		return stream;
	}
};

int is_prime(int a)
{
	int half = a / 2;
	int i;

	for (i = 2; i <= half; i++)
        if (a % i == 0)
            return (0);

	return (1);
}

int find_most_active()
{
	int i;
	int most = 0;

	for (i = 0; i < LEN; i++)
        if (primes[i] > primes[most])
            most = i;

	return (most);

}

int prime_factors(int a)
{
	int i;
    int pr = 1;
    
	for (i = 2; i <= a; i++)
    {
        if ((a % i == 0) && (is_prime(i)))
        {
            cout << i << " ";
            //primes[i]++;
            pr *= i;
        }
    }
    primes[pr]++;
	//cout << " : ";

    return primes[pr];

}

void kasiski(char *s)
{
	char store[4]; 	  
	char dummy[4];
    string ss[5000];
	int i;		 
	int j;
    int jj = 0;

	for (i = 0; i<strlen(s); i++, jj++) 
	{	 
		store[0] = s[i];	
		store[1] = s[i + 1];
		store[2] = s[i + 2];
		store[3] = '\0';
        int counter = 0;
        int dist = 0;
        bool flag = false;
        int fact;

		for (j = i + 1; j < strlen(s); j++) 
		{
			dummy[0] = s[j];           
			dummy[1] = s[j + 1];
			dummy[2] = s[j + 2];
			dummy[3] = '\0';
			if (!strcmp(dummy, store)) 
			{
                counter++;
                dist = j - i;
                flag = true;
				//cout << "match of " << dummy << " with a distance of "
				//	<< j - i << " with prime factors of: ";
				fact = prime_factors(j - i);
			}     
		}

        for (i = 0; i < jj; i++)
        {
            if (!strcmp(store, ss[jj].c_str()))
                //goto EXIT;
                flag = false;
        }

        if (flag)
            cout << " : " << fact << " : " << store << " contains " << counter << " times with a distance " << dist << endl;
            
        //EXIT:
        //if (ss.find(dummy) != string::npos)
        //    ;
        //else
        //   

    } 

	cout << endl << "The most commonly occuring prime factor is " <<
		find_most_active() / 2 << endl;

}

int main(int argc, char **argv)
{
	ifstream in("input.txt");
	if (!in)
	{
		cout << "Cannot open a file\n";
		return 1;
	}

	//setlocale(LC_ALL, "RUSSIAN");
	TEXT M, K;

	TEXT temp;
	while (in >> temp)
	{
		M = M + temp;
	}
	M.ToUpper();

	in.close();
	
	
	cout << "Message: "; M.show(); //cout << L_num(M[0]) << endl;
	cout << "    Key: "; cin >> K; K.ToUpper(); //cout << L_num(K[0]) << endl;
	cout << "Select a key type:\n0 - Straight\n1 - Progressive\n2 - Auto\n>";
	int type;
	cin >> type;
	if (M.size() > K.size())
	{
		if (type == 0)
			K.MakeLoop(M.size());
		else if (type == 1)
			K.MakeLoopP(M.size());
		else if (type == 2)
			K.MakeLoopA(M.size() - K.size(), M);
	}
	
	
	TEXT C((size_t)M.size());
	int i;
	for (i = 0; i < M.size(); i++)
	{
		int indC = L_num(M[i]) + L_num(K[i]);

		if (indC > LEN - 1)
			C[i] = a[indC - LEN];
		else
			C[i] = a[indC];
	}
	cout << " Crypt: " << C << endl;

    TEXT D((size_t)C.size());
    for (i = 0; i < C.size(); i++)
    {
        int indM = L_num(C[i]) - L_num(K[i]);

        if (indM < 0)
            D[i] = a[indM + LEN];
        else
            D[i] = a[indM];
    }
    cout << "Decrypt: " << D << endl;

	ofstream out("output.txt");
	if (!out)
	{
		cout << "Cannot creat a file\n";
		return 1;
	}

	for (int i = 0; i < C.size(); i++)
		out << C[i];

	out.close();

	char s[3000];
	for (int i = 0; i < C.size(); i++)
		s[i] = C[i];
	s[i] = '\0';

	cout << endl << "**** Kasisky Analysis ****" << endl;
	kasiski(s);

	return 0;
}
