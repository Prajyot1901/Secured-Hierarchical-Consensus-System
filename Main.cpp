#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include "gmpxx.h"
#include<map>

#pragma warning(disable:4146);

using namespace std;

class Entity {
public:
	string username;
	string signature;
	Entity(string u, string s)
	{
		username = u;
		signature = s;
	}
};

class Registrar {
public:
	int num;
	vector<Entity*> entities;
	Registrar()
	{
		num = 0;
	}

	bool add_entity(string u, string s)
	{
		
		for (auto it : this->entities)
		{
			if (it->username == u)
			{
				return false;
			}
		}
		Entity* en = new Entity(u, s);
		this->entities.push_back(en);
		num++;
	}
};

class Sub_Registrar {
public:
	int num;
	vector<Entity*> entities;
	Sub_Registrar()
	{
		num = 0;
	}

	bool add_entity(string u, string s)
	{
		for (auto it : this->entities)
		{
			if (it->username == u)
			{
				return false;
			}
		}
		Entity* en = new Entity(u, s);
		this->entities.push_back(en);
		num++;
	}
};

class Assistant_Registrar {
public:
	int num;
	vector<Entity*> entities;
	Assistant_Registrar()
	{
		num = 0;
	}

	bool add_entity(string u, string s)
	{
		for (auto it : this->entities)
		{
			if (it->username == u)
			{
				return false;
			}
		}
		Entity* en = new Entity(u, s);
		this->entities.push_back(en);
		num++;
	}
};

class Transaction {
public:
	string Trx_ID;
	string Documents_Buyer;
	string Documents_Seller;
	long long int q, p1, p2, p3;

	Transaction(string t, string db, string ds)
	{
		Trx_ID = t;
		Documents_Buyer = db;
		Documents_Seller = ds;
		q = 0;
		p1 = 0; 
		p2 = 0; 
		p3 = 0;
	}
};

void Encrypt(long long int m, mpz_t n, mpz_t g,mpz_t E)
{
	mpz_t tempm;
	mpz_init(tempm);
	mpz_init_set_ui(tempm, m);
	gmp_randstate_t randState;
	unsigned long seed = rand();
	gmp_randinit_default(randState);
	gmp_randseed_ui(randState, seed);
	mpz_t r,n_squared,G;
	mpz_inits(r,n_squared,G,NULL);
	mpz_mul(n_squared, n, n);
	mpz_urandomb(r, randState, 512);
	mpz_powm(G, g, tempm, n_squared);
	mpz_powm(r, r, n, n_squared);
	mpz_mul(G, G, r);
	mpz_mod(E, G, n_squared);
	return;
}

void Send_reply(Registrar* Assam_Registrar, Sub_Registrar* Assam_Sub_Registrar, Assistant_Registrar* Assam_Assistant_Registrar)
{
	int i = 0;
	for (auto it : Assam_Registrar->entities)
	{
		i++;
		ifstream in;
		in.open(it->username + ".csv");
		if (!in.is_open())
		{
			cout << it->username << " could not open";
			return;
		}
		string s;
		int j = 0;
		while (getline(in, s))
		{
			j++;
			string cell;
			stringstream ss(s);
			int cnt = 0;
			string trxn_id, N, G;
			long long int q;
			long long int p;
			while (getline(ss, cell, ','))
			{
				cnt++;
				if (cnt == 1)
				{
					trxn_id = cell;
				}
				if (cnt == 4)
				{
					p = stoll(cell);
				}
				if (cnt == 5)
				{
					q = stoll(cell);
				}
				if (cnt == 6)
				{
					N = cell;
				}
				if (cnt == 7)
				{
					G = cell;
				}
			}
			mpz_t n, g, E;
			mpz_inits(n, g, E, NULL);
			mpz_set_str(n, N.data(),16);
			mpz_set_str(g, G.data(), 16);
			if (i % 2 == j % 2)
			{
				Encrypt(p, n, g, E);
			}
			else
			{
				Encrypt(q, n, g, E);
			}


			vector<string> vec;
			fstream out;
			out.open("TrxnPool.csv");
			if (!out.is_open())
			{
				cout << "Out could not be opened";
				return;
			}
			while (getline(out, s))
			{
				string cell;
				stringstream ss(s);
				int cnt = 0;
				int ind = 0;
				string TRXN_ID, Q, p3, p2, p1, mu, lamda, cur, No,n_key;
				while (getline(ss, cell, ','))
				{
					cnt++;
					if (cnt == 1)
					{
						if (trxn_id == cell)
							ind = 1;
						TRXN_ID = cell;
					}
					if (cnt == 2)
					{
						Q = cell;
					}
					if (cnt == 3)
					{
						p3 = cell;
					}
					if (cnt == 4)
					{
						p2 = cell;
					}
					if (cnt == 5)
					{
						p1 = cell;
					}
					if (cnt == 6)
					{
						Q = cell;
					}
					if (cnt == 7)
					{
						mu = cell;
					}
					if (cnt == 8)
					{
						lamda = cell;
					}
					if (cnt == 9)
					{
						n_key = cell;
					}
					if (cnt == 10)
					{
						cur = cell;
					}
					if (cnt == 11)
					{
						No = cell;
					}
					
				}

				if (ind == 0)
				{
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + "," + n_key + "," + cur + "," + No + "\n");
				}
				else
				{
					mpz_t Cur;
					mpz_t N_key;
					mpz_inits(Cur,N_key,NULL);
					mpz_set_str(N_key, n_key.data(), 16);
					mpz_mul(N_key, N_key, N_key);
					mpz_set_str(Cur, cur.data(), 16);
					mpz_mul(Cur, Cur, E);
					mpz_mod(Cur, Cur, N_key);
					cur = mpz_get_str(NULL, 16, Cur);
					int NOO = stoi(No);
					NOO++;
					No = to_string(NOO);
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + ","  + n_key + "," + cur + "," + No + "\n");
					mpz_clear(N_key);
					break;
				}
				
			}
			while (getline(out, s))
			{
				vec.push_back(s + "\n");
			}
			out.close();
			fstream output;
			output.open("TrxnPool.csv");
			for (auto it : vec)
			{
				output << it;
			}
			output.close();
		}
		
	}
	for (auto it : Assam_Sub_Registrar->entities)
	{
		i++;
		ifstream in;
		in.open(it->username + ".csv");
		if (!in.is_open())
		{
			cout << it->username << " could not open";
			return;
		}
		string s;
		int j = 0;
		while (getline(in, s))
		{
			j++;
			string cell;
			stringstream ss(s);
			int cnt = 0;
			string trxn_id, N, G;
			long long int q;
			long long int p;
			while (getline(ss, cell, ','))
			{
				cnt++;
				if (cnt == 1)
				{
					trxn_id = cell;
				}
				if (cnt == 4)
				{
					p = stoll(cell);
				}
				if (cnt == 5)
				{
					q = stoll(cell);
				}
				if (cnt == 6)
				{
					N = cell;
				}
				if (cnt == 7)
				{
					G = cell;
				}
			}
			mpz_t n, g, E;
			mpz_inits(n, g, E, NULL);
			mpz_set_str(n, N.data(), 16);
			mpz_set_str(g, G.data(), 16);
			if (i % 2 == j % 2)
			{
				Encrypt(p, n, g, E);
			}
			else
			{
				Encrypt(q, n, g, E);
			}


			vector<string> vec;
			fstream out;
			out.open("TrxnPool.csv");
			if (!out.is_open())
			{
				cout << "Out could not be opened";
				return;
			}
			while (getline(out, s))
			{
				string cell;
				stringstream ss(s);
				int cnt = 0;
				int ind = 0;
				string TRXN_ID, Q, p3, p2, p1, mu, lamda, cur, No, n_key;
				while (getline(ss, cell, ','))
				{
					cnt++;
					if (cnt == 1)
					{
						if (trxn_id == cell)
							ind = 1;
						TRXN_ID = cell;
					}
					if (cnt == 2)
					{
						Q = cell;
					}
					if (cnt == 3)
					{
						p3 = cell;
					}
					if (cnt == 4)
					{
						p2 = cell;
					}
					if (cnt == 5)
					{
						p1 = cell;
					}
					if (cnt == 6)
					{
						Q = cell;
					}
					if (cnt == 7)
					{
						mu = cell;
					}
					if (cnt == 8)
					{
						lamda = cell;
					}
					if (cnt == 9)
					{
						n_key = cell;
					}
					if (cnt == 10)
					{
						cur = cell;
					}
					if (cnt == 11)
					{
						No = cell;
					}

				}

				if (ind == 0)
				{
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + "," + n_key + "," + cur + "," + No + "\n");
				}
				else
				{
					mpz_t Cur;
					mpz_t N_key;
					mpz_inits(Cur, N_key, NULL);
					mpz_set_str(N_key, n_key.data(), 16);
					mpz_mul(N_key, N_key, N_key);
					mpz_set_str(Cur, cur.data(), 16);
					mpz_mul(Cur, Cur, E);
					mpz_mod(E, E, N_key);
					cur = mpz_get_str(NULL, 16, Cur);
					int NOO = stoi(No);
					NOO++;
					No = to_string(NOO);
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + "," + n_key + "," + cur + "," + No + "\n");
					mpz_clear(N_key);
					break;
				}

			}
			while (getline(out, s))
			{
				vec.push_back(s + "\n");
			}
			out.close();
			fstream output;
			output.open("TrxnPool.csv");
			for (auto it : vec)
			{
				output << it;
			}
			output.close();
		}
	}
	for (auto it : Assam_Assistant_Registrar->entities)
	{
		i++;
		ifstream in;
		in.open(it->username + ".csv");
		if (!in.is_open())
		{
			cout << it->username << " could not open";
			return;
		}
		string s;
		int j = 0;
		while (getline(in, s))
		{
			j++;
			string cell;
			stringstream ss(s);
			int cnt = 0;
			string trxn_id, N, G;
			long long int q;
			long long int p;
			while (getline(ss, cell, ','))
			{
				cnt++;
				if (cnt == 1)
				{
					trxn_id = cell;
				}
				if (cnt == 4)
				{
					p = stoll(cell);
				}
				if (cnt == 5)
				{
					q = stoll(cell);
				}
				if (cnt == 6)
				{
					N = cell;
				}
				if (cnt == 7)
				{
					G = cell;
				}
			}
			mpz_t n, g, E;
			mpz_inits(n, g, E, NULL);
			mpz_set_str(n, N.data(), 16);
			mpz_set_str(g, G.data(), 16);
			if (i % 2 == j % 2)
			{
				Encrypt(p, n, g, E);
			}
			else
			{
				Encrypt(q, n, g, E);
			}


			vector<string> vec;
			fstream out;
			out.open("TrxnPool.csv");
			if (!out.is_open())
			{
				cout << "Out could not be opened";
				return;
			}
			while (getline(out, s))
			{
				string cell;
				stringstream ss(s);
				int cnt = 0;
				int ind = 0;
				string TRXN_ID, Q, p3, p2, p1, mu, lamda, cur, No, n_key;
				while (getline(ss, cell, ','))
				{
					cnt++;
					if (cnt == 1)
					{
						if (trxn_id == cell)
							ind = 1;
						TRXN_ID = cell;
					}
					if (cnt == 2)
					{
						Q = cell;
					}
					if (cnt == 3)
					{
						p3 = cell;
					}
					if (cnt == 4)
					{
						p2 = cell;
					}
					if (cnt == 5)
					{
						p1 = cell;
					}
					if (cnt == 6)
					{
						Q = cell;
					}
					if (cnt == 7)
					{
						mu = cell;
					}
					if (cnt == 8)
					{
						lamda = cell;
					}
					if (cnt == 9)
					{
						n_key = cell;
					}
					if (cnt == 10)
					{
						cur = cell;
					}
					if (cnt == 11)
					{
						No = cell;
					}

				}

				if (ind == 0)
				{
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + "," + n_key + "," + cur + "," + No + "\n");
				}
				else
				{
					mpz_t Cur;
					mpz_t N_key;
					mpz_inits(Cur, N_key, NULL);
					mpz_set_str(N_key, n_key.data(), 16);
					mpz_mul(N_key, N_key, N_key);
					mpz_set_str(Cur, cur.data(), 16);
					mpz_mul(Cur, Cur, E);
					mpz_mod(E, E, N_key);
					cur = mpz_get_str(NULL, 16, Cur);
					int NOO = stoi(No);
					NOO++;
					No = to_string(NOO);
					vec.push_back(TRXN_ID + "," + Q + "," + p3 + "," + p2 + "," + p1 + "," + Q + "," + mu + "," + lamda + "," + n_key + "," + cur + "," + No + "\n");
					mpz_clear(N_key);
					break;
				}

			}
			while (getline(out, s))
			{
				vec.push_back(s + "\n");
			}
			out.close();
			fstream output;
			output.open("TrxnPool.csv");
			for (auto it : vec)
			{
				output << it;
			}
			output.close();
		}
	}
}

void Generate_Key(mpz_t n, mpz_t g, mpz_t mu, mpz_t lamda, mpz_t p, mpz_t q,int& k)
{
	gmp_randstate_t randState;
	unsigned long seed = rand();
	gmp_randinit_default(randState);
	gmp_randseed_ui(randState, seed);
	mpz_t p1, q1, n_square, temp;
	mpz_inits(p1, q1, n_square, temp, NULL);
	mpz_mul(n, p, q);
	mpz_mul(n_square, n, n);

	mpz_sub_ui(p1, p, 1);
	mpz_sub_ui(q1, q, 1);
	mpz_mul(temp, p1, q1);
	mpz_gcd(temp, n, temp);
	if (mpz_cmp_d(temp,1.0))
	{
		return;
	}
	mpz_lcm(lamda, p1, q1);

	vector<mpz_t> vec;
	// μ = L(g^λ mod n^2)^{-1} mod n

	mpz_urandomb(g, randState, 512);
	mpz_mod(g, g, n_square);
	mpz_powm(temp, g, lamda, n_square);
	mpz_sub_ui(temp, temp, 1);
	mpz_divexact(temp, temp, n);
	if (!mpz_invert(mu, temp, n))
		return;
	mpz_clears(p1, q1, temp, n_square, NULL);
	k = 1;
}

void send_requests(Entity* reciever,Transaction* trxn,int level,string p1,string p2)
{
	fstream rec;
	rec.open(reciever->username+".csv", ios_base::app);
	if (!rec.is_open())
	{
		cout << reciever->username << " could not open " << endl;
		return;
	}
	string s;
	/*int cnt = 0;
	while (getline(rec, s))
	{
		cnt++;
	}*/
	if(level==0)
		rec << trxn->Trx_ID << "," << trxn->Documents_Buyer << "," << trxn->Documents_Seller << "," << trxn->p1 << "," << trxn->q<<","<<p1<<","<<p2 << "\n";
	else if (level == 1)
		rec << trxn->Trx_ID << "," << trxn->Documents_Buyer << "," << trxn->Documents_Seller << "," << trxn->p2 << "," << trxn->q<< "," << p1 << "," << p2<<"\n";
	else
		rec << trxn->Trx_ID << "," << trxn->Documents_Buyer << "," << trxn->Documents_Seller << "," << trxn->p3 << "," << trxn->q<< "," << p1 << "," << p2<<"\n";
	rec.close();
}

void Generate_random_sequence(Transaction* trxn,Registrar* Assam_Registrar, Sub_Registrar* Assam_Sub_Registrar, Assistant_Registrar* Assam_Assistant_Registrar )
{
         
	gmp_randstate_t randState;     
	unsigned long seed=rand();     
	gmp_randinit_default(randState);
	gmp_randseed_ui(randState, seed);

	mpz_t p, r, n, lamda, g, mu;
	mpz_inits(p, r, n, lamda, g, mu, NULL);
	int k=0;
	mpz_urandomb(p, randState, 1024);
	seed = rand();
	gmp_randseed_ui(randState, seed);
	mpz_urandomb(r, randState, 1024);
	int t = rand();
	mpz_add_ui(p, p, t);
	t = rand();
	mpz_add_ui(r, r, t);
	while (k == 0)
	{
		mpz_nextprime(p, p);
		mpz_nextprime(r, r);
		Generate_Key(n, g, mu, lamda, p, r, k);
	}

	int q = 15+rand()%35;
	int p3 = 2500+rand()%2500;
	int p2 = 250000+rand()%2500;
	int p1 = 1262500+rand()%2500;
	trxn->p1 = p1;
	trxn->p2 = p2;
	trxn->p3 = p3;
	trxn->q = q;


	int reg_size = Assam_Registrar->num;
	int sreg_size = Assam_Sub_Registrar->num;
	int areg_size = Assam_Assistant_Registrar->num;
	//cout << reg_size << " " << sreg_size << " " << areg_size << endl;
	vector<int> list[3];
	int cnt = 0;
	while(1)
	{
		int k = rand() % reg_size;
		int i = 0;
		for (i=0;i<list[0].size();i++)
		{
			if (list[0][i] == k)
			{
				break;
			}
		}
		if (i == list[0].size())
		{
			list[0].push_back(k);
			cnt++;
		}
		if (cnt == (reg_size+1)/2)
		{
			break;
		}
	}
	cnt = 0;
	while (1)
	{
		int k = rand() % sreg_size;
		int i = 0;
		for (i = 0; i < list[1].size(); i++)
		{
			if (list[1][i] == k)
			{
				break;
			}
		}
		if (i == list[1].size())
		{
			cnt++;
			list[1].push_back(k);
		}
		if (cnt == (sreg_size+1)/2)
		{
			break;
		}
	}
	cnt = 0;
	while (1)
	{
		int k = rand() % areg_size;
		int i = 0;
		for (i = 0; i < list[2].size(); i++)
		{
			if (list[2][i] == k)
			{
				break;
			}
		}
		if (i == list[2].size())
		{
			list[2].push_back(k);
			cnt++;
		}
		if (cnt == (areg_size+1)/2)
		{
			break;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (auto it : list[i])
		{
			cout << it << " ";
		}
		cout << endl;
	}
	
	char* key1 = mpz_get_str(NULL, 16, n); 
	char* key2 = mpz_get_str(NULL, 16, g);
	char* key3=  mpz_get_str(NULL, 16, mu);
	char* key4 = mpz_get_str(NULL, 16, lamda);
	fstream pool;
	pool.open("TrxnPool.csv", ios_base::app);
	pool << trxn->Trx_ID << "," << trxn->q << "," << trxn->p1 << "," << trxn->p2 << "," << trxn->p3 << "," << trxn->q << "," << key3 << "," << key4  <<"," << key1 << ",1,0" << "\n";
	pool.close();
	cout << endl << endl << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < list[i].size(); j++)
		{
			if(i==0)
				send_requests(Assam_Registrar->entities[list[i][j]], trxn,0,key1,key2);
			if(i==1)
				send_requests(Assam_Sub_Registrar->entities[list[i][j]], trxn, 1, key1, key2);
			if (i == 2)
				send_requests(Assam_Assistant_Registrar->entities[list[i][j]], trxn, 2, key1, key2);
		}
	}
}

void decrypt(mpz_t C, mpz_t mu, mpz_t lamda, mpz_t n,mpz_t K)
{
	mpz_t nsquare;
	mpz_inits(nsquare,NULL);
	mpz_mul(nsquare, n, n);
	mpz_powm(K, C, lamda, nsquare);
	mpz_sub_ui(K, K, 1);
	mpz_div(K, K, n);
	mpz_mul(K, K, mu);
	mpz_mod(K, K, n);
	return;
}

void Result()
{
	ifstream res;
	res.open("TrxnPool.csv");
	if (!res.is_open())
	{
		cout << "could not read cipher text";
		return;
	}
	string s;
	while (getline(res, s))
	{
		stringstream ss(s);
		string cell;
		int cnt = 0;
		string trxn_id,q,p1,p2,p3,mu,lamda,n,C,No;
		while (getline(ss, cell, ','))
		{
			cnt++;
			if (cnt == 1)
			{
				trxn_id = cell;
			}
			if (cnt == 2)
			{
				q = cell;
			}
			if (cnt == 3)
			{
				p3 = cell;
			}
			if (cnt == 4)
			{
				p2 = cell;
			}
			if (cnt == 5)
			{
				p1 = cell;
			}
			if (cnt == 6)
			{
				q = cell;
			}
			if (cnt == 7)
			{
				mu = cell;
			}
			if (cnt == 8)
			{
				lamda = cell;
			}
			if (cnt == 9)
			{
				n = cell;
			}
			if (cnt == 10)
			{
				C = cell;
			}
			if (cnt == 11)
			{
				No = cell;
			}
		}
		if(No=="18")
		{
			mpz_t Mu, Lamda, N, c, K;
			mpz_inits(Mu, Lamda, N, c, K, NULL);
			mpz_set_str(Mu, mu.data(), 16);
			mpz_set_str(N, n.data(), 16);
			mpz_set_str(Lamda, lamda.data(), 16);
			mpz_set_str(c, C.data(), 16);
			decrypt(c, Mu, Lamda, N, K);

			char* k = mpz_get_str(NULL, 10, K);
			cout << k << endl;
			long long int total = atoll(k);

			long long int a = stoll(p1);
			long long int b = stoll(p2);
			long long int f = stoll(p3);
			long long int neg = stoll(q);

			int k1 = total / f;
			total = total % f;
			int k2 = total / b;
			total = total % b;
			int k3 = total / a;
			total = total % a;
			int Non = total / neg;
			total = total % neg;
			cout << k1 << " " << k2 << " " << k3 <<" "<<Non << endl;
			//mpz_t P1, P2, P3, Q;
			//mpz_inits(P1, P2, P3, Q);
			//mpz_set_str(P1, p1.data(), 10);
			//mpz_set_str(P2, p2.data(), 10);
			//mpz_set_str(P3, p3.data(), 10);
			//mpz_set_str(Q, q.data(), 10);
			//
			//mpz_t K1, K2, K3, negative;
			//mpz_inits(K1, K2, K3, negative);
			//mpz_div(K3, K, P3);
			//mpz_mod(K, K, P3);
			//mpz_div(K2, K, P2);
			//mpz_mod(K, K, P2);
			//mpz_div(K1, K, P1);
			//mpz_mod(K, K, P1);
			//mpz_div(negative, K, Q);
			//char* k1=mpz_get_str(NULL,10,K1);
			//char* k2 = mpz_get_str(NULL, 10, K2);
			//char* k3 = mpz_get_str(NULL, 10, K3);
			//cout << k1 << " " << k2 << " " << k3 << endl;
			fstream results;
			results.open("results.csv",ios_base::app);
			if (!results.is_open())
			{
				cout << "results could not open";
			}
			results << trxn_id << "," << k1 << "," << k2 << "," << k3 <<","<<Non << "\n";
			results.close();
		}
	}
}


int main()
{
	srand(time(0));
	Registrar* Assam_Registrar = new Registrar();
	Sub_Registrar* Assam_Sub_Registrar = new Sub_Registrar();
	Assistant_Registrar* Assam_Assistant_Registrar = new Assistant_Registrar();
	ifstream registrar_input;
	registrar_input.open("Assam_Registrar_inputs.csv");
	if (!registrar_input.is_open())
	{
		cout << "Not able to open Registrar file"<<endl;
	}

	ifstream sub_registrar_input;
	sub_registrar_input.open("sub_registrars_input.csv");
	if (!sub_registrar_input.is_open())
	{
		cout << "Not able to open Sub_Registrar file" << endl;
	}

	ifstream assistant_registrar_input;
	assistant_registrar_input.open("Assistant_registrars_inputs.csv");
	if (!assistant_registrar_input.is_open())
	{
		cout << "Not able to open Assistant_Registrar file" << endl;
	}

	string s;
	while (getline(registrar_input, s))
	{
		string cell;
		int cnt=0;
		stringstream ss(s);
		string username;
		string signature;
		while (getline(ss, cell, ','))
		{
			cnt++;
			if (cnt == 1)
			{
				username = cell;
			}
			if (cnt == 2)
			{
				signature = cell;
			}
		}
		Assam_Registrar->add_entity(username, signature);
	}

	while (getline(sub_registrar_input, s))
	{
		string cell;
		int cnt = 0;
		stringstream ss(s);
		string username;
		string signature;
		while (getline(ss, cell, ','))
		{
			cnt++;
			if (cnt == 1)
			{
				username = cell;
			}
			if (cnt == 2)
			{
				signature = cell;
			}
		}
		Assam_Sub_Registrar->add_entity(username, signature);
	}

	while (getline(assistant_registrar_input, s))
	{
		string cell;
		int cnt = 0;
		stringstream ss(s);
		string username;
		string signature;
		while (getline(ss, cell, ','))
		{
			cnt++;
			if (cnt == 1)
			{
				username = cell;
			}
			if (cnt == 2)
			{
				signature = cell;
			}
		}
		Assam_Assistant_Registrar->add_entity(username, signature);
	}


	fstream trxn_input;
	trxn_input.open("Transaction_input.csv");
	if (!trxn_input.is_open())
	{
		cout << "couldnt open trxn input file";
		return 0;
	}
	while (getline(trxn_input, s))
	{
		string cell;
		int cnt = 0;
		stringstream ss(s);
		string trnx_id;
		string seller;
		string buyer;
		while (getline(ss, cell, ','))
		{
			cnt++;
			if (cnt == 1)
			{
				trnx_id = cell;
			}
			if (cnt == 2)
			{
				seller = cell;
			}
			if (cnt == 3)
			{
				buyer = cell;
			}
		}
		Transaction* trnx = new Transaction(trnx_id,buyer,seller);
		Generate_random_sequence(trnx, Assam_Registrar, Assam_Sub_Registrar, Assam_Assistant_Registrar);
	}

	Send_reply(Assam_Registrar, Assam_Sub_Registrar, Assam_Assistant_Registrar);
	Result();
}
