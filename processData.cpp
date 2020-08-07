#include "processData.h"

enum CodeValue
{
	sdCode = 1,
	cdCode,
	slCode,
	insCode,
	delCode,
	updCode,
	obCode,
	cbCode,
	osCode,
	csCode
};

static map<string, CodeValue> s_mapCodeValues = {
	{"SD", sdCode},
	{"CD", cdCode},
	{"SL", slCode},
	{"INS", insCode},
	{"DEL", delCode},
	{"UPD", updCode},
	{"OB", obCode},
	{"CB", cbCode},
	{"OS", osCode},
	{"CS", csCode}};

//Class AVL-------------------------------------------------------------------------------------------
Node *AVL::rotateRight(Node *&node)
{
	Node *tempPtr = node->left;
	node->left = tempPtr->right;
	tempPtr->right = node;
	return tempPtr;
}

Node *AVL::rotateLeft(Node *&node)
{
	Node *tempPtr = node->right;
	node->right = tempPtr->left;
	tempPtr->left = node;
	return tempPtr;
}

Node *AVL::insertRec(Node *&node, const long &time, const double &BP, const double &AP, bool &taller)
{
	if (node == NULL)
	{
		Node *newNode = new Node(time, BP, AP);
		node = newNode;
		taller = true;
		return node;
	}
	if (time < node->TIME)
	{
		node->left = insertRec(node->left, time, BP, AP, taller);
		if (taller)
		{
			if (node->balance == LH)
			{
				node = leftBalance(node, taller);
			}
			else if (node->balance == EH)
			{
				node->balance = LH;
			}
			else
			{
				node->balance = EH;
				taller = false;
			}
		}
	}
	else if (time > node->TIME)
	{
		node->right = insertRec(node->right, time, BP, AP, taller);
		if (taller)
		{
			if (node->balance == LH)
			{
				node->balance = EH;
				taller = false;
			}
			else if (node->balance == EH)
			{
				node->balance = RH;
			}
			else
			{
				node = rightBalance(node, taller);
			}
		}
	}
	//Da ton tai time r, chi can update gia tri ty gia
	else {
		node->BP = BP;
		node->AP = AP;
	}
	return node;
}

void AVL::insert(const long &time, const double &BP, const double &AP)
{
	bool taller = false;
	this->root = insertRec(this->root, time, BP, AP, taller);
}

Node *AVL::rightBalance(Node *&node, bool &taller)
{
	Node *rightTree = node->right;
	if (rightTree->balance == RH)
	{	
		node->balance = EH;
		rightTree->balance = EH;
		node = rotateLeft(node);
		taller = false;
	}
	else
	{
		Node *leftTree = rightTree->left;
		if (leftTree->balance == RH)
		{
			node->balance = LH;
			rightTree->balance = EH;
		}
		else if (leftTree->balance == EH)
		{
			node->balance = EH;
			rightTree->balance = EH;
		}
		else
		{
			node->balance = EH;
			rightTree->balance = RH;
		}
		leftTree->balance = EH;
		node->right = rotateRight(rightTree);
		node = rotateLeft(node);
		taller = false;
	}
	return node;
}

Node *AVL::leftBalance(Node *&node, bool &taller)
{
	Node *leftTree = node->left;
	if (leftTree->balance == LH)
	{
		node->balance = EH;
		leftTree->balance = EH;
		node = rotateRight(node);
		taller = false;
	}
	else
	{
		Node *rightTree = leftTree->right;
		if (rightTree->balance == LH)
		{
			node->balance = RH;
			leftTree->balance = EH;
		}
		else if (rightTree->balance == EH)
		{
			node->balance = EH;
			leftTree->balance = EH;
		}
		else
		{
			node->balance = EH;
			leftTree->balance = LH;
		}
		rightTree->balance = EH;
		node->left = rotateLeft(leftTree);
		node = rotateRight(node);
		taller = false;
	}
	return node;
}

Node *AVL::removeRec(Node *&node, const long &time, bool &shorter, bool &success)
{
	if (node == NULL){
		shorter = false;
		success = false;
		return NULL;
	}
	if (time < node->TIME){
		node->left = removeRec(node->left, time, shorter, success);
		if (shorter){
			node = removeRightBalance(node, shorter);
		}
	}
	else if (time > node->TIME){
		node->right = removeRec(node->right, time, shorter, success);
		if (shorter){
			node = removeLeftBalance(node, shorter);
		}
	}
	else
	{
		Node * deleteNode = node;
		if (node->right == NULL){
			Node * newNode = node->left;
			success = true;
			shorter = true;
			delete deleteNode;
			return newNode;
		}
		else if (node->left == NULL)
		{
			Node * newNode = node->right;
			success = true;
			shorter = true;
			delete deleteNode;
			return newNode;
		}
		else
		{
			Node * exchangePtr = node->left;
			while (exchangePtr->right != NULL)
			{
				exchangePtr = exchangePtr->right;
			}
			node->TIME = exchangePtr->TIME;
			node->BP = exchangePtr->BP;
			node->AP = exchangePtr->AP;
			node->left = removeRec(node->left, exchangePtr->TIME, shorter, success);
			if (shorter){
				node = removeRightBalance(node, shorter);
			}
		}
	}
	return node;
}

void AVL::remove(const long &time)
{
	bool success, shorter = false;
	this->root = removeRec(this->root, time, shorter, success);
}

Node *AVL::removeRec_Range(Node *&node, const long &timeA, const long &timeB, bool &shorter, bool &success){
	if (node == NULL){
		shorter = false;
		success = false;
		return NULL;
	}
	if (node->TIME >= timeA && node->TIME <= timeB)
	{
		Node * deleteNode = node;
		if (node->right == NULL){
			Node * newNode = node->left;
			success = true;
			shorter = true;
			delete deleteNode;
			return newNode;
		}
		else if (node->left == NULL)
		{
			Node * newNode = node->right;
			success = true;
			shorter = true;
			delete deleteNode;
			return newNode;
		}
		else
		{
			Node * exchangePtr = node->left;
			while (exchangePtr->right != NULL)
			{
				exchangePtr = exchangePtr->right;
			}
			node->TIME = exchangePtr->TIME;
			node->BP = exchangePtr->BP;
			node->AP = exchangePtr->AP;
			node->left = removeRec(node->left, exchangePtr->TIME, shorter, success);
			if (shorter){
				node = removeRightBalance(node, shorter);
			}
		}
	}
	if (timeB < node->TIME){
		node->left = removeRec_Range(node->left, timeA, timeB, shorter, success);
		if (shorter){
			node = removeRightBalance(node, shorter);
		}
	}
	else if (timeA > node->TIME){
		node->right = removeRec_Range(node->right, timeA, timeB, shorter, success);
		if (shorter){
			node = removeLeftBalance(node, shorter);
		}
	}
	return node;
}

void AVL::remove_Range(const long &timeA, const long &timeB)
{
	bool success, shorter = false;
	this->root = removeRec_Range(this->root, timeA, timeB, shorter, success);
	while (success){
		this->root = removeRec_Range(this->root, timeA, timeB, shorter, success);
	}
}

Node *AVL::removeRightBalance(Node *&node, bool &shorter)
{
	if (node->balance == LH){
		node->balance = EH;
	}
	else if (node->balance == EH)
	{
		node->balance = RH;
		shorter = false;
	}
	else
	{
		Node * rightTree = node->right;
		if (rightTree->balance == LH)
		{
			Node * leftTree = rightTree->left;
			if (leftTree->balance == LH){
				rightTree->balance = RH;
				node->balance = EH;
			}
			else if (leftTree->balance == EH)
			{
				rightTree->balance = EH;
				node->balance = EH;
			}
			else
			{
				rightTree->balance = EH;
				node->balance = LH;
			}
			leftTree->balance = EH;
			node->right = rotateRight(rightTree);
			node = rotateLeft(node);			
		}
		else
		{
			if (rightTree->balance != EH){
				node->balance = EH;
				rightTree->balance = EH;
			}
			else
			{
				node->balance = RH;
				rightTree->balance = LH;
				shorter = false;
			}
			node = rotateLeft(node);		
		}			
	}
	return node;
}

Node *AVL::removeLeftBalance(Node *&node, bool &shorter)
{
	if (node->balance == RH){
		node->balance = EH;
	}
	else if (node->balance == EH)
	{
		node->balance = LH;
		shorter = false;
	}
	else
	{
		Node * leftTree = node->left;
		if (leftTree->balance == RH)
		{
			Node * rightTree = leftTree->right;
			if (rightTree->balance == RH){
				leftTree->balance = LH;
				node->balance = EH;
			}
			else if (rightTree->balance == EH)
			{
				leftTree->balance = EH;
				node->balance = EH;
			}
			else
			{
				leftTree->balance = EH;
				node->balance = RH;
			}
			rightTree->balance = EH;
			node->left = rotateLeft(leftTree);
			node = rotateRight(node);			
		}
		else
		{
			if (leftTree->balance != EH){
				node->balance = EH;
				leftTree->balance = EH;
			}
			else
			{
				node->balance = LH;
				leftTree->balance = RH;
				shorter = false;
			}
			node = rotateRight(node);		
		}			
	}
	return node;
}

Node *AVL::updateRec(Node *&node, const long &time, const double &BP, const double &AP, bool &success)
{
	if (node == NULL){
		return NULL;
	}
	if (time < node->TIME){
		node->left = updateRec(node->left, time, BP, AP, success);
	}
	else if (time > node->TIME){
		node->right = updateRec(node->right, time, BP, AP, success);
	}
	else
	{
		node->BP = BP;
		node->AP = AP;
		success = true;
	}
	return node;
}

void AVL::update(const long &time, const double &BP, const double &AP, bool &success)
{
	this->root = updateRec(this->root, time, BP, AP, success);
}

void AVL::clearRec(Node *&node)
{
	if (node != NULL)
	{
		this->clearRec(node->left);
		this->clearRec(node->right);
		delete node;
		node = NULL;
	}
}

void AVL::clear()
{
	this->clearRec(this->root);
}

Node *AVL::search_Node_AVL_Lit(Node *node, const long &time){
	Node *res = node;
	while (node != NULL && node->TIME != time){
		if (time < node->TIME) {
			node = node->left;
			if (node != NULL){
				if (node->TIME <= time) {res = node;}
			}
		}
		else {
			node = node->right;
			if (node != NULL){
				if (node->TIME <= time) {res = node;}
			}
		}
	}
	return res;
}

Node *AVL::search_Node_AVL(const long &time){
	return search_Node_AVL_Lit(this->root, time);
}

//Class Processdata-----------------------------------------------------------------------------------
ProcessData::ProcessData()
{
	LList *myData = new LList;
	this->data = myData;
	money = 0;
	LV = 1;
}

ProcessData::~ProcessData()
{
	Exchange * DelExchange = data->head;
	data->head = NULL;
	while (DelExchange != nullptr){
		Exchange * tempExchange = DelExchange;
		tempExchange->nodes.clear();
		DelExchange = DelExchange->next;
		delete tempExchange;
	}
	transaction.clear();
	dict_transaction.clear();
	delete data;
}

int ProcessData::split(string line, string *&sp){
	sp = new string[MAXSIZECODE];
	const string SEP = " ";
	int pos, lastpos = 0, idx = 0;
	// Them parameters vao sp
	if (line.find(SEP, lastpos) == std::string::npos && line != ""){
		sp[idx] = line;
		return 1;
	}
	else {
		while (idx < MAXSIZECODE - 1 && line.find(SEP, lastpos) != std::string::npos)
		{
			pos = line.find(SEP, lastpos);
			sp[idx] = line.substr(lastpos, pos - lastpos);
			lastpos = pos + 1;
			idx++;
		}
		sp[idx++] = line.substr(lastpos, line.length() - 1 - pos);
		return idx;
	}
}

bool ProcessData::isSplit(const string * p, int n){
	if (p[1] == p[2]) {return false;}
	for (int i = 0; i < n; i++){
		if (p[i] == "") {return false;}
	}
	return true;
}

bool ProcessData::isTime(string s){
	for (size_t i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;
	return true;
}

bool ProcessData::isDouble(string s){
	int count = 0;
	for (size_t i = 0; i < s.length(); i++) {
		if (s[i] == '.') {
			count++;
			if (count > 1) {return false;}
		}
		if (isdigit(s[i]) == false && s[i] != '.') {
			return false;
		}
	}
	return true;
}

bool ProcessData::isExchange(string BC, string QC){
	if (BC.length() != 3 || QC.length() != 3) {return false;}
	for (size_t i = 0; i < BC.length(); i++){
		if (BC[i] < 'A' || BC[i] > 'Z') return false;
	}
	for (size_t i = 0; i < QC.length(); i++){
		if (QC[i] < 'A' || QC[i] > 'Z') return false;
	}
	if ((BC != "USD" || QC == "USD") && (BC == "USD" || QC != "USD")){
		return false;
	}
	return true;
}

bool ProcessData::valid_Ins_Upd(const string *sp, const int n, double &BP, double &AP){
	if (n != 6 || !isExchange(sp[1], sp[2]) || !isTime(sp[3]) || !isDouble(sp[4]) || !isDouble(sp[5])){
		return false;
	}
	BP = stod(sp[4]);
	AP = stod(sp[5]);
	if (BP - AP > EPS) {return -1;}
	return true;
}

bool ProcessData::valid_Transaction_Open(const string *sp, const int n){
	if (n != 6 || !isExchange(sp[1], sp[2]) || !isTime(sp[3]) || !isTime(sp[5]) || !isDouble(sp[4])){
		return false;
	}
	else {return true;}
}

bool ProcessData::valid_Transaction_Close(const string *sp, const int n){
	if (n != 3 || !isTime(sp[1]) || !isTime(sp[2])){
		return false;
	}
	else {return true;}
}

int ProcessData::round_Result(double raw_result){
	double res = fabs(raw_result);
	if (int(res - EPS) == floor(res) - 1) {
		res = floor(res);
	}
	else{
		res = ceil(res);
	}
	return raw_result >= 0 ? res : -1 * res;
}

int ProcessData::calculate_Transaction_OB_CS(map<string, Transaction>::iterator it, long time_Close){
	Exchange *search_Exchange = search(it->second.BC, it->second.QC);
	double lot_Trans = it->second.lot;
	double AP = it->second.AP;
	long time_Open = it->second.time;
	this->transaction.erase(it);
	if (time_Close < time_Open){
		return -1;
	}
	Node *node_Close = search_Exchange->nodes.search_Node_AVL(time_Close);
	double money_Cal = (node_Close->BP - AP) * lot_Trans * LOT;
	if (search_Exchange->BC == "USD"){
		money_Cal /= node_Close->AP;
	}
	this->money += money_Cal;
	return round_Result(money_Cal);
}

int ProcessData::calculate_Transaction_OS_CB(map<string, Transaction>::iterator it, long time_Close){
	Exchange *search_Exchange = search(it->second.BC, it->second.QC);
	double lot_Trans = it->second.lot;
	double BP = it->second.BP;
	long time_Open = it->second.time;
	this->transaction.erase(it);
	if (time_Close < time_Open){
		return -1;
	}
	Node *node_Close = search_Exchange->nodes.search_Node_AVL(time_Close);
	double money_Cal = (BP - node_Close->AP) * lot_Trans * LOT;
	if (search_Exchange->BC == "USD"){
		money_Cal /= node_Close->AP;
	}
	this->money += money_Cal;
	return round_Result(money_Cal);
}

int ProcessData::process(string line){
	string *p;
	int n = ProcessData::split(line, p);
	if (n <= 0){
		delete[] p;
		return -1;
	}
	int res = -1;
	try{
		switch (s_mapCodeValues[p[0]]){
			case sdCode:
				res = set_Deposit(p, n);
				break;
			case cdCode:
				res = check_Deposit(p,n);
				break;
			case slCode:
				res = set_Levarage(p,n);
				break;
			case insCode:
				res = insert_Node(p,n);
				break;
			case delCode:
				res = delete_Node(p,n);
				break;
			case updCode:
				res = update_Node(p,n);
				break;
			case obCode:
				res = open_Buy(p,n);
				break;
			case cbCode:
				res = close_Buy(p,n);
				break;
			case osCode:
				res = open_Sell(p,n);
				break;
			case csCode:
				res = close_Sell(p,n);
				break;
			default:
				res = -1;
		}
	}
	catch (invalid_argument iax){
		delete[] p;
		return res;
	}
	delete[] p;
	return res;
}

Exchange *ProcessData::search(string BC, string QC){
	Exchange *Findptr = data->head;
	while (Findptr != NULL){
		if (Findptr->BC == BC && Findptr->QC == QC)
		{
			break;
		}
		Findptr = Findptr->next;
	}
	return Findptr;
}

long ProcessData::insert_Node(const string *sp, const int n)
{
	double BP;
	double AP;
	if (!valid_Ins_Upd(sp, n, BP, AP)) {return -1;}
	long time = stol(sp[3]);
	Exchange *search_Exchange = search(sp[1], sp[2]);
	if (search_Exchange == NULL){
		data->insert_LList(sp[1], sp[2]);
		data->head->nodes.insert(time, BP, AP);
		return data->head->nodes.root->TIME;
	}
	else{
		search_Exchange->nodes.insert(time, BP, AP);
		return search_Exchange->nodes.root->TIME;
	}
}

long ProcessData::update_Node(const string *sp, const int n)
{
	double BP;
	double AP;
	if (!valid_Ins_Upd(sp, n, BP, AP)) {return -1;}
	long time = stol(sp[3]);
	Exchange *search_Exchange = search(sp[1], sp[2]);
	if (search_Exchange == NULL){
		return -1;
	}
	else{
		bool success = false;
		search_Exchange->nodes.update(time, BP, AP, success);
		if (success){
			return search_Exchange->nodes.root->TIME;
		}
		else {
			return -1;
		}
	}
}

long ProcessData::delete_Node(const string *sp, const int n){
	if (n < 3 || n > 5) {return -1;}
	if (!isSplit(sp, n)) {return -1;}
	if (!isExchange(sp[1],sp[2])) {return -1;}
	if (n == 4 && !isTime(sp[3])) {return -1;}
	if (n == 5 && (!isTime(sp[3]) || !isTime(sp[4]))) {return -1;}
	if (isTime(sp[n-2]) && isTime(sp[n-1]) && stol(sp[n-2]) > stol(sp[n-1])) {return -1;}
	Exchange *search_Exchange = search(sp[1], sp[2]);
	if (search_Exchange == NULL){
		return -1;
	}
	else{
		if (search_Exchange->nodes.root == NULL) {return -1;}
		if (n == 3){
			search_Exchange->nodes.clear();
			return -1;
		}
		else if (n == 4){
			long time = stol(sp[3]);
			search_Exchange->nodes.remove(time);
			if (search_Exchange->nodes.root == NULL){
				return -1;
			}
			else {
				return search_Exchange->nodes.root->TIME;
			}
		}
		else{
			long timeA = stol(sp[3]);
			long timeB = stol(sp[4]);
			search_Exchange->nodes.remove_Range(timeA, timeB);
			if (search_Exchange->nodes.root == NULL){
				return -1;
			}
			else {
				return search_Exchange->nodes.root->TIME;
			}
		}
	}
}

int ProcessData::check_Deposit(const string *sp, const int n){
	if (n != 1) {return -1;}
	return round_Result(this->money);
}

int ProcessData::set_Deposit(const string *sp, const int n){
	if (n != 2 || !isDouble(sp[1])) {return -1;}
	double change_Money = stod(sp[1]);
	if (!this->transaction.empty() && money - change_Money > EPS ) {
		return -1;
	}
	else {
		money = change_Money;
		return 1;
	}
}

int ProcessData::set_Levarage(const string *sp, const int n){
	if (n != 2 || !isDouble(sp[1])) {return -1;}
	double change_Levarage = stod(sp[1]);
	if (money == 0 && LV == 1) {
		return -1;
	}
	else {
		LV = change_Levarage;
		double res = money * LV;
		return round_Result(res);
	}
}

int ProcessData::open_Buy(const string *sp, const int n){
	if (!valid_Transaction_Open(sp, n)) {return -1;}
	double lot_trans = stod(sp[4]);
	if (this->money <= 0) {return -1;}
	map<string, Transaction>::iterator it = this->transaction.find(sp[5]);
	map<string, int>::iterator dict = this->dict_transaction.find(sp[5]);
	Exchange *search_Exchange = search(sp[1], sp[2]);
	if (it == this->transaction.end() && dict == this->dict_transaction.end() && search_Exchange != NULL){
		long time = stol(sp[3]);
		Node *node_Open = search_Exchange->nodes.search_Node_AVL(time);
		double BP = node_Open->BP;
		double AP = node_Open->AP;
		if (search_Exchange->BC == "USD"){
			if ((AP * lot_trans * LOT) / AP - this->money * this->LV > EPS) {return -1;}
		}
		else {
			if (AP * lot_trans * LOT - this->money * this->LV > EPS) {return -1;}
		}
		Transaction trans = Transaction("BUY", sp[1], sp[2], BP, AP, time, lot_trans);
		this->transaction.insert(pair<string, Transaction>(sp[5],trans));
		this->dict_transaction.insert(pair<string, int>(sp[5],1));
		if (search_Exchange->BC == "USD"){
			double res = (AP * lot_trans * LOT) / AP;
			return round_Result(res);
		}
		else
		{
			double res = AP * lot_trans * LOT;
			return round_Result(res);
		}
	}
	else {
		return -1;
	}
}

int ProcessData::open_Sell(const string *sp, const int n){
	if (!valid_Transaction_Open(sp, n)) {return -1;}
	double lot_trans = stod(sp[4]);
	if (this->money <= 0) {return -1;}
	map<string, Transaction>::iterator it = this->transaction.find(sp[5]);
	map<string, int>::iterator dict = this->dict_transaction.find(sp[5]);
	Exchange *search_Exchange = search(sp[1], sp[2]);
	if (it == this->transaction.end() && dict == this->dict_transaction.end() && search_Exchange != NULL){
		long time = stol(sp[3]);
		Node *node_Open = search_Exchange->nodes.search_Node_AVL(time);
		double BP = node_Open->BP;
		double AP = node_Open->AP;
		if (search_Exchange->BC == "USD"){
			if ((BP * lot_trans * LOT) / AP - this->money * this->LV > EPS) {return -1;}
		}
		else {
			if (BP * lot_trans * LOT - this->money * this->LV > EPS) {return -1;}
		}
		Transaction trans = Transaction("SELL", sp[1], sp[2], BP, AP, time, lot_trans);
		this->transaction.insert(pair<string, Transaction>(sp[5],trans));
		this->dict_transaction.insert(pair<string, int>(sp[5],1));
		if (search_Exchange->BC == "USD"){
			double res = (BP * lot_trans * LOT) / AP;
			return round_Result(res);
		}
		else
		{
			double res = BP * lot_trans * LOT;
			return round_Result(res);
		}
	}
	else {
		return -1;
	}
}

int ProcessData::close_Sell(const string *sp, const int n){
	// cout<<dict_transaction.size()<<endl;
	// cout<<transaction.size()<<endl;
	if (!valid_Transaction_Close(sp, n)) {return -1;}
	map<string, Transaction>::iterator it = this->transaction.find(sp[2]);
	if (it != this->transaction.end()){
		if (it->second.type == "BUY"){
			long time_Close = stol(sp[1]);
			//phan thanh toan
			int res = calculate_Transaction_OB_CS(it, time_Close);
			if (this->money <= 0){
				while (!transaction.empty()){
					map<string, Transaction>::iterator i = this->transaction.begin();
					if (i->second.type == "BUY"){
						calculate_Transaction_OB_CS(i, time_Close);
					}
					else {
						calculate_Transaction_OS_CB(i, time_Close);
					}
				}
			}
			return res;
		}
		else {return -1;}
	}
	else {
		return -1;
	}
}

int ProcessData::close_Buy(const string *sp, const int n){
	// cout<<dict_transaction.size()<<endl;
	// cout<<transaction.size()<<endl;
	if (!valid_Transaction_Close(sp, n)) {return -1;}
	map<string, Transaction>::iterator it = this->transaction.find(sp[2]);
	if (it != this->transaction.end()){
		if (it->second.type == "SELL"){
			long time_Close = stol(sp[1]);
			//phan thanh toan
			int res = calculate_Transaction_OS_CB(it, time_Close);
			if (this->money <= 0){
				while (!transaction.empty()){
					map<string, Transaction>::iterator i = this->transaction.begin();
					if (i->second.type == "BUY"){
						calculate_Transaction_OB_CS(i, time_Close);
					}
					else {
						calculate_Transaction_OS_CB(i, time_Close);
					}
				}
			}
			return res;
		}
		else {return -1;}
	}
	else {
		return -1;
	}
}
