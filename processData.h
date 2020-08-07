#include "main.h"
const double EPS = 0.000001;
const int LOT = 100000;

//Node la cap ty gia
enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};

class Transaction
{
public:
	string type;
	string BC;
	string QC;
	double BP;
	double AP;
	long time;
	double lot;
	Transaction(string type, string BC, string QC, double BP, double AP, long time, double lot){
		this->type = type;
		this->BC = BC;
		this->QC = QC;
		this->BP = BP;
		this->AP = AP;
		this->time = time;
		this->lot = lot;
	}
};

class Node
{
public:
    long TIME;
	double BP;
	double AP;
    Node *left;
    Node *right;
	BalanceValue balance;
	Node(){
		this->left = this->right = NULL;
	}
	Node(long TIME, double BP, double AP)
	{
		this->TIME = TIME;
		this->BP = BP;
		this->AP = AP;
		this->left = this->right = NULL;
		balance = EH;
	}
	~Node(){
		this->left = this->right = NULL;
	}
};

class AVL
{
public:
	Node *root;

protected:
	void clearRec(Node *&);
	Node *rotateRight(Node *&node);
	Node *rotateLeft(Node *&node);
	Node *leftBalance(Node *&node, bool &taller);
	Node *rightBalance(Node *&node, bool &taller);
	Node *removeLeftBalance(Node *&node, bool &shorter);
	Node *removeRightBalance(Node *&node, bool &shorter);
	Node *insertRec(Node *&node, const long &time, const double &BP, const double &AP, bool &taller);
	Node *removeRec(Node *&node, const long &time, bool &shorter, bool &success);
	Node *removeRec_Range(Node *&node, const long &timeA, const long &timeB, bool &shorter, bool &success);
	Node *updateRec(Node *&node, const long &time, const double &BP, const double &AP, bool &success);
	Node *search_Node_AVL_Lit(Node *node, const long &time);
public:
	AVL()
	{
		this->root = NULL;
	}
	~AVL()
	{
		this->clear();
	}
	void clear();
	void insert(const long &time, const double &BP, const double &AP);
	void update(const long &time, const double &BP, const double &AP, bool &success);
	void remove(const long &time);
	void remove_Range(const long &timeA, const long &timeB);
	// Node *find_Max_AVL();
	// Node *find_Min_AVL();
	Node *search_Node_AVL(const long &time);
};

//Cap Exchange
class Exchange
{
public:
	string BC;
	string QC;
	Exchange *next;
	AVL nodes;
	Exchange(){
		this->next = NULL;
	}
	Exchange(const string &BC, const string &QC)
	{
		this->BC = BC;
		this->QC = QC;
		this->next = NULL;
	}
	~Exchange(){
		this->next = NULL;
	}
};

class LList
{
public:
	Exchange *head;
	LList() { 
		this->head = NULL; 
	}
	~LList(){
		this->head = NULL; 
	}
	void insert_LList(const string &BC, const string &QC)
	{
		Exchange *newExchange = new Exchange(BC, QC);
		newExchange->next = this->head;
		this->head = newExchange;
	}
};

class ProcessData
{
private:
	map<string, Transaction> transaction;
	map<string, int> dict_transaction;
	LList *data;
	double money;
	int LV;
	static const int MAXSIZECODE = 8;
	static int split(string line, string *&sp);
public:
	ProcessData();
	~ProcessData();
	int process(string line);
	//khai bao cac ham da tao

	Exchange *search(string, string);
	bool isSplit(const string *, int);
	bool isTime(string);
	bool isDouble(string);
	bool valid_Ins_Upd(const string *sp, const int n, double &BP, double &AP);
	bool valid_Transaction_Open(const string *sp, const int n);
	bool valid_Transaction_Close(const string *sp, const int n);
	int round_Result(double raw_result);
	int calculate_Transaction_OB_CS(map<string, Transaction>::iterator it, long time_Close);
	int calculate_Transaction_OS_CB(map<string, Transaction>::iterator it, long time_Close);
	bool isExchange(string, string);
	long insert_Node(const string *sp, const int n);
	long update_Node(const string *sp, const int n);
	long delete_Node(const string *sp, const int n);
	int check_Deposit(const string *sp, const int n);
	int set_Deposit(const string *sp, const int n);
	int set_Levarage(const string *sp, const int n);
	int open_Buy(const string *sp, const int n);
	int open_Sell(const string *sp, const int n);
	int close_Buy(const string *sp, const int n);
	int close_Sell(const string *sp, const int n);
};