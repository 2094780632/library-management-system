///Windows cmd�µ�ͼ��ݹ���ϵͳ
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<algorithm>
#include<windows.h>

#include<chrono>
#include<ctime>
#include<vector>
#include<string>
using namespace std;

/*
#define LOG "log.txt"//�����־���ļ�
#define BOOKCSV "test.csv"//���ͼ�����ݵ��ļ�
#define USERCSV "user.csv"//����û����ݵ��ļ�  
#define TAB 20//TAB�Ʊ������
#define BRIEFTAB 30//��Ҫ��Ϣ�Ʊ������
#define PRICE_MAX 10000//��󵥼�
*/

///ȫ������������
//Ԥ������
class Book;
class User;
int log(const string &text);

typedef struct GlobalSettings{
    string LOG;//�����־���ļ�
    string BOOKCSV;//���ͼ�����ݵ��ļ�
    string USERCSV;//����û����ݵ��ļ�  
    int TAB = 20;//TAB�Ʊ������
    int BRIEFTAB = 30;//��Ҫ��Ϣ�Ʊ������
    double PRICE_MAX = 10000;//��󵥼�
}Config;

typedef struct ListData{
    vector<Book> BookList;//�鼮�б�
    vector<User> UserList;//�û��б�
}Data;


///����ͨ�ú�����
//CMD����
inline void cls(){system("cls");}

//pause
inline void pause(){system("pause");}

//���ڱ���
inline void title(const string& t){
    string cmd="title "+t;
    system(cmd.c_str());
}

//һ���Դ����е������������
inline void o(const string &text){cout<<text<<endl;}
//һ���Ʊ�����̿ɿص������������
inline void o(const string &text,int t){cout<<setw(t)<<text;}
//һ�����ָ��С��λ�����Ʊ���������������
inline void o(const double &d,int a,int t){cout<<fixed<<setprecision(a)<<setw(t)<<d;}
//����ӳ�䵽��ͨ���
inline void o(const int a){o(to_string(a));}
inline void o(const int a,int t){o(to_string(a),t);}
//�������ͣ
inline void op(const string &text){o(text);pause();}

//һ��ʹ��ģ���������������͵ı���������Ϸ�����⺯��
//ͨ�����ͽ����cin�Ƿ�ɹ���������Ϣ���룩
template<typename T>
void i(T& v){
    while(true){
        if(cin>>v){
            //�ɹ�
            break;
        }else{
            //ʧ��
            log("����Ϸ������ʧ��");
            cin.clear();//�������״̬
            cin.sync();//��ջ�������
            o("���벻�Ϸ�������������:");
            //��������
        }
    }
}
//int�͸��������ޱ����䣨���ڲ˵�ѡ�
void i(int& v,int min,int max){
    while(true){
        if(cin>>v&&v>=min&&v<=max){
            //�ɹ�
            break;
        }else{
            //ʧ��
            log("int������Ϸ������ʧ��");
            cin.clear();//�������״̬
            cin.sync();//��ջ�������
            o("���벻�Ϸ�������������:");
            //��������
        }
    }
}

//�����ʾ������������
void oi(string& in,string tip){
    cout<<tip<<':';
    i(in);
}

void oi(int& in,string tip){
    cout<<tip<<':';
    i(in);
}

void oi(int& in,string tip,int min,int max){
    cout<<tip<<':';
    i(in,min,max);
}

void oi(unsigned int& in,string tip){
    cout<<tip<<':';
    i(in);
}

void oi(double& in,string tip){
    cout<<tip<<':';
    i(in);
}

//ȷ��
bool check(string tip){
    string in;
    cout<<tip<<"(y/n):";
    cin>>in;
    while (in!="y"&&in!="n"){
        oi(in,"����������");
    }
    if(in=="y"){return true;}else if(in=="n"){return false;}
    log("no return");
    return false;
}

// ������ɫ����
const int BLACK=0;
const int BLUE=1;
const int GREEN=2;
const int CYAN=3;
const int RED=4;
const int MAGENTA=5;
const int YELLOW=6;
const int WHITE=7;
// ������ɫ�汾
const int BRIGHT_BLACK=8;
const int BRIGHT_BLUE=9;
const int BRIGHT_GREEN=10;
const int BRIGHT_CYAN=11;
const int BRIGHT_RED=12;
const int BRIGHT_MAGENTA=13;
const int BRIGHT_YELLOW=14;
const int BRIGHT_WHITE=15;
//��ɫϵͳ
class ColorGuard {
    private:
        HANDLE hConsole;
        WORD originalAttributes;
    public:
        // ���캯����֧�������ı���ɫ�ͱ�����ɫ
        ColorGuard(int textColor, int bgColor = 0){
            hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo(hConsole,&info);
            originalAttributes=info.wAttributes;

            // �����ı���ɫ�ͱ�����ɫ
            WORD color=(bgColor << 4) | textColor; // ������ɫ����4λ�����ı���ɫ���
            SetConsoleTextAttribute(hConsole, color);
        }

        ~ColorGuard(){
            SetConsoleTextAttribute(hConsole, originalAttributes);
        }
};
using CG=ColorGuard;

inline void CT(const string &t){
    CG C(BLACK,WHITE);
    o(t);
}

//ѡ��
int option(vector<string> &options,bool back){
    int c=-1;
    int j=1;
    for(auto& op:options){
        {
            CG C(BRIGHT_CYAN);
            cout<<"  "<<j;
        }
        o("."+op);
        j++;
    }
    if(back){
        {
            CG C(BRIGHT_CYAN);
            cout<<"  "<<'0';
        }
        o(".����");
        i(c,0,options.size());
    }else{
        i(c,1,options.size());
    }
    return c;
}


///�ļ�ϵͳ
//���ļ�����string��ʽ�����ļ�����
string f_readall(const string& file_name){
    log("���Զ�ȡ�ļ���"+file_name);
    ifstream file;
    file.open(file_name);
    string content,line;
    //�ж��Ƿ�򿪳ɹ�
    if(file.is_open()){
        log("�ɹ���ȡ�ļ�");
        while (getline(file,line)){
            content+=line+'\n';
        }
        file.close();
        return content;
    }else{
        log("�޷���ȡ�ļ�"+file_name);
        cerr<<"�޷����ļ���"<<file_name<<endl;
        return "";
    }
}

//���ж��ļ�
string f_readline(const string& file_name){
    ifstream file;
    file.open(file_name);
    string line;
    //�ж��Ƿ�򿪳ɹ�
    if(file.is_open()){
        log("�ɹ����ж�ȡ�ļ�");
        getline(file,line);
        file.close();
        return line;
    }else{
        log("�޷����ж�ȡ�ļ�"+file_name);
        cerr<<"�޷����ļ���"<<file_name<<endl;
        return "";
    }
}

//д���ļ�(app or trunc)
int f_write(const string& file_name,const string& text,const string& mode){
    ofstream file;
    if(mode=="app"){
        file.open(file_name,ios::app);
    }else if(mode=="trunc"){
        file.open(file_name,ios::trunc);
    }else{
        log("��Чд��ģʽ");
        cerr << "�޷����ļ�: "<<file_name<<endl;
        return 1;
    }

    //�ж��Ƿ�򿪳ɹ�
    if (!file.is_open()) {
        log("�޷�д���ļ�"+file_name);
        cerr << "�޷����ļ�: "<<file_name<<endl;
        return 1;
    }
    file<<text;
    file.close();

    if(mode=="app"){
        log("�ɹ�׷��д���ļ���"+file_name);
    }else if(mode=="trunc"){
        log("�ɹ�����д���ļ���"+file_name);
    }

    return 0;
}

//��־��¼
int log(const string &text){
    string LOG="log.txt";
    //��ȡʱ��
    auto now=chrono::system_clock::now();
    time_t now_c=chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss<<put_time(localtime(&now_c),"%Y-%m-%d %H:%M:%S");
    string log_time;
    log_time=ss.str();
    //д���¼
    //f_write(LOG,log_time+":"+text+'\n');����������д������Ϊf_write��Ҳ��log�ĵ��ã��ᵼ�������������������������ѭ��
    ofstream log;
    log.open(LOG,ios::app);
    //�ж��Ƿ�򿪳ɹ�
    if (!log.is_open()) {
        cerr << "�޷�д��log��¼"<<endl;
        return 1;
    }

    string log_str=log_time+":"+text+'\n';
    log<<log_str;
    log.close();
    return 0;
}


///ͼ�����ݵĹ���
//ͼ����
class Book{
    public:
        //���캯��
        Book(){}
        Book(string t,string a,string i,string pre,string d,double pri,unsigned int q,unsigned int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

        //��ȡ��������
        string TITLE() const{return title;}
        string AUTHOR() const{return author;}
        string ISBN() const{return isbn;}
        string PRESS() const{return press;}
        string DATE() const{return date;}
        double PRICE() const{return price;}
        unsigned int QUANTITY() const{return quantity;}
        unsigned int STOCK() const{return stock;}

        //�޸ĵ�������
        void change_title(string value){
            this->title=value;
        }
        void change_author(string value){
            this->author=value;
        }
        void change_isbn(string value){
            this->isbn=value;
        }
        void change_press(string value){
            this->press=value;
        }
        void change_date(string value){
            this->date=value;
        }
        void change_price(double value){
            this->price=value;
        }
        void change_quantity(unsigned int value){
            this->quantity=value;
        }
        void change_stock(unsigned int value){
            this->stock=value;
        }

        //չʾ��Ϣ
        void showinfo(int &TAB){
            o(title,TAB);
            o(author,TAB);
            o(isbn,TAB);
            o(press,TAB);
            o(date,TAB);
            o(price,2,TAB);
            o(to_string(quantity),TAB);
            o(to_string(stock),TAB);
            //o(to_string(isfree),TAB);
            o("");
        }

        //��Ҫ��Ϣչʾ
        void briefinfo(int &BRIEFTAB){
            o(title,BRIEFTAB);
            //o(author,TAB);
            o(to_string(stock),BRIEFTAB);
            o("");
        }
        
        //�Ƿ񱻽���
        bool isfree(){return stock;}
        bool isborrowed(){return quantity-stock;}

        //��������ؽ����ʣ����������������򷵻�-1��ʾ����
        int borrow(int i){
            if(stock>=i){
                stock-=i;
                return stock-i;
            }else{
                return -1;
            }
        }

        //����
        bool returnbook(){
            if(stock+1<=quantity){
                stock++;
                return true;
            }else{
                return false;
            }
        }

        void debug_showinfo(){
            log("DEBUGBOOKINFO");
            o(title);
            o(author);
            o(isbn);
            o(press);
            o(date);
            o(to_string(price));
            o(to_string(quantity));
            o(to_string(stock));
            //o(to_string(isfree));
            o("");
        }

        //��������
        string savestr(){
            ostringstream ss;
            ss<<fixed<<setprecision(2);
            ss<<title<<','<<author<<','<<isbn<<','<<press<<','<<date<<','<<price<<','<<quantity<<','<<stock;
            return ss.str();
            
            //return title+','+author+','+isbn+','+press+','+date+','+to_string(price)+','+to_string(quantity)+','+to_string(stock);
        }

    private:
        string title,author,isbn,press,date;//�ַ������ͣ���������š����ߡ������硢��������
        double price;//�������ͣ��۸�
        unsigned int quantity,stock;//���ͣ����������
};

//��ʼ��ͼ������
void Booklist_init(vector<Book>& book_list,string csv){
    ifstream file(csv);
    if(!file.is_open()){
        log("��ʼ��ͼ������ʱ�޷���csv�ļ�");
        cerr<<"�޷����ļ���"<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
        stringstream ss(line);//���ڴ���csv�ַ���
        string item[8];//9������ʹ������洢    
        int i=0;
        while(getline(ss,item[i],',')){i++;}
        book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoul(item[6]),stoul(item[7]));
    }
    log("�ɹ���ʼ��ͼ������");
    file.close();
}

//����ͼ������
void Booklist_save(vector<Book>& book_list,string csv){
    log("����ͼ������");
    string save="";
    for(auto& book:book_list){
        save+=book.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
    //log("Booklist save:\n"+save);
}

//��ȡͼ������
void Booklist_info(vector<Book>& book_list,bool isid,int &TAB){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        if(isid){
            CG C(BRIGHT_CYAN);
            o(i,TAB);
        }
        book.showinfo(TAB);
        i++;
    }
}

//��ȡ��Ҫ����
void Booklist_briefinfo(vector<Book>& book_list,bool isid,int &BRIEFTAB){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        if(isid){
            CG C(BRIGHT_CYAN);
            o(i,BRIEFTAB);
        }
        book.briefinfo(BRIEFTAB);
        i++;
    }
}

//���һ��ͼ��ı�ͷ
void Booklist_infoheading(int &TAB,bool isid=false){
    CG C(YELLOW);
    if(isid){o("���",TAB);}
    o("����",TAB);
    o("����",TAB);
    o("ISBN",TAB);
    o("������",TAB);
    o("��������",TAB);
    o("�۸�",TAB);
    o("����",TAB);
    o("���",TAB);
    o("");
}

//���һ�м�Ҫ�ı�ͷ
void Booklist_briefheading(bool isid,int &BRIEFTAB){
    CG C(YELLOW);
    if(isid){o("���",BRIEFTAB);}
    o("����",BRIEFTAB);
    //o("����",TAB);
    o("�ɽ�����",BRIEFTAB);
    o("");
}

//����ָ��������ͼ�����
Book& Booklist_element(vector<Book>& book_list,int index){
    log("����ͼ�����,index:"+to_string(index));
    if (index < 0 || index >= book_list.size()) {
        log("�ڵ���ͼ�����ʱ����������Χ,index:"+to_string(index));
        throw out_of_range("����������Χ");
    }
    return book_list[index];
}

//���Բ���ISBNͼ�����
Book& Booklist_isbn(vector<Book>& book_list,string isbn){
    log("����ָ��ISBN�鼮����");
    auto it=find_if(book_list.begin(),book_list.end(),[&isbn](const Book& book){
        return book.ISBN()==isbn;
    });
    if (it == book_list.end()) {
        log("ISBNͼ��������ʧ��");
        throw runtime_error("�޷��ҵ���Ӧ�鼮��");
    }
    return *it;
}

//��ѯ�鱾�Ƿ����(ISBN)
bool book_exist(vector<Book>& book_list,string isbn){
    for(auto& book:book_list){
        if(book.ISBN()==isbn){return true;}
    }
    return false;
}

//ɾ��ָ��ISBNͼ��
void Booklist_delbook(vector<Book>& book_list,string isbn){
    book_list.erase(remove_if(book_list.begin(),book_list.end(),[&isbn](const Book& book) {return book.ISBN()==isbn;}),book_list.end());
}

//����ָ����ʽ���ط����������鱾����
vector<Book> Booklist_search(vector<Book>& book_list,string key,string (Book::*pfunc)() const){
    vector<Book> target_list;
    for(auto& book:book_list){
        if((book.*pfunc)()==key){target_list.push_back(book);}
    }
    //log("cash");
    if(target_list.empty()){
        log("�޷��ҵ�ָ����Ŀ,key:"+key);
        throw runtime_error("�޷��ҵ�ָ����Ŀ");
    }
    return target_list;
    //log("done");
}

/*���÷��� 
//��¡�ɱ����ĵ��鵥
void Booklist_iffree_clone(vector<Book>& book_list,vector<Book>& book_list_free){
    for(auto& book:book_list){
        if(book.iffree()){
            book_list_free.push_back(book);
        }
    }
}

//��ȡ�ɱ�����ͼ������
void Booklist_info_isfree(vector<Book>& book_list,int &freesum){
    log("���Զ�ȡ�ɱ�����ͼ������");
    int i=1;
    for(auto& book:book_list){
        if(book.isfree()){
            o(i,TAB);
            freesum+=book.showinfo_iffree();//�Զ�����ɽ��ĵ���������Ϊ���ɽ��Ļ᷵��0
            i++;
        }
    }
}
*/


///������ϵͳ����

//vector[]תstring
string vetostr(vector<string> list){
    string str="[";
    int i=0;
    for(auto& temp:list){
        str+=temp;
        if(i<list.size()-1){
            str+=',';
            i++;
        }
    }
    return str+']';
}

//�û���
class User{
    public:
        //���캯�������κν����鼮��
        User(){}
        User(string n,string i,string s="N",vector<string> l={}):name(n),id(i),status(s),borrowed_list(l){};

        //��ȡ��������
        string NAME() const{return name;}
        string ID() const{return id;}
        string STATUS() const{return status;}
        vector<string> BORROWED_LIST() const{return borrowed_list;}

        //��Ϣչʾ
        void showinfo(int &TAB){
            if(status!="R"){
                o(name,TAB);
                o(id,TAB);
                string strstatus;
                if(status=="A"){strstatus="����Ա�û�";}
                if(status=="N"){strstatus="��ͨ�û�";}
                if(status=="B"){strstatus="�������û�";}
                o(strstatus,TAB);
                //o(vetostr(borrowed_list),TAB);
                o(borrowed_list.size(),TAB);
                o("");
            }
        }

        //����
        void borrow(string ISBN){
            borrowed_list.push_back(ISBN);
        }

        //����
        void returnBook(string ISBN){
            borrowed_list.erase(remove(borrowed_list.begin(),borrowed_list.end(),ISBN),borrowed_list.end());
        }
        
        //���ISBN�Ƿ��Ѵ���
        bool is_borrowed(string ISBN){
            for(auto& i:borrowed_list){
                if(i==ISBN){return true;}
            }
            return false;
        }

        //��������
        string savestr(){
            return name+','+id+','+status+','+vetostr(borrowed_list);
        }

    private:
        string name,id,status;//�û��������û���š��û�״̬(N������B������)
        vector<string> borrowed_list;//�û����ĵ��鵥(ISBN)
};

//����б�
vector<string> apartlist(string data){
    //log("apartlist:"+data);debug
    vector<string> list={};
    if(data=="[]"){return list;}
    data.erase(0,1);//ɾͷ
    data.pop_back();//ȥβ

    stringstream ss(data);
    string temp;
    while(getline(ss,temp,',')){
        list.push_back(temp);
    }
    return list;
}

//��ʼ���û�����
void Userlist_init(vector<User>& user_list,string csv){
    ifstream file(csv);
    if(!file.is_open()){
        log("��ʼ���û�����ʱ�޷���csv�ļ�");
        cerr<<"�޷����ļ���"<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
        stringstream ss(line);//���ڴ���csv�ַ���
        vector<string> items;//4������ʹ������洢
        string item;
        int i=0;
        while(getline(ss,item,',')&&i<=2){
            items.push_back(item);
            line.erase(0,item.size()+1);
            i++;
        }
        items.push_back(line);
        user_list.emplace_back(items[0],items[1],items[2],apartlist(items[3]));
    }
    log("�ɹ���ʼ���û�����");
    file.close();
}

//��ȡ�û�����
void Userlist_info(vector<User>& user_list,int &TAB){
    log("���Զ�ȡ�û�����");
    //int i=1;
    for(auto& user:user_list){
        //o(i,TAB);
        user.showinfo(TAB);
        //i++;
    }
}

void Userlist_infoheading(int &TAB){
    CG C(YELLOW);
    o("�û���",TAB);
    o("ID",TAB);
    o("״̬",TAB);
    o("��������",TAB);
    o("");
}

//�����û�����
void Userlist_save(vector<User>& user_list,string csv){
    log("�����û�����");
    string save="";
    for(auto& user:user_list){
        save+=user.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
   //log("Userlist save:\n"+save);
}

//��ѯ�û��Ƿ����
bool user_exist(vector<User>& user_list,string id){
    for(auto& user:user_list){
        if(user.ID()==id){return true;}
    }
    return false;
}

//����Ƿ��ж���û����ͬһ����
int Userlist_Borrowsamebook(vector<User>& user_list,string isbn){
    int sum=0;
    for(auto& user:user_list){
        if(user.is_borrowed(isbn)){sum++;}
    }
    return sum;
}

//����ָ���û������û�����
User& Userlist_element(vector<User>& user_list,string name){
    log("�����û�����"+name);
    for(auto& user:user_list){
        if(user.NAME()==name){return user;}
    }
    log("�����û�����ʧ��(NAME)");
    throw "�����û�����ʧ��(NAME)";
}

//���ĳ����û�����һ����
User& Userlist_borrowed_isbn(vector<User>& user_list,string isbn){
    log("�����û�����(ISBN) ��"+isbn);
    for(auto& user:user_list){
        if(user.is_borrowed(isbn)){
            return user;
        }
    }
    log("�����û�����ʧ��(ISBN)");
    throw "�����û�����ʧ��(ISBN)";
}

//����ָ����ʽ���ط����������û�����
vector<User> Userlist_search(vector<User>& user_list,string key,string (User::*pfunc)() const){
    vector<User> target_list;
    for(auto& user:user_list){
        if((user.*pfunc)()==key){target_list.push_back(user);}
    }
    //log("cash");
    if(target_list.empty()){
        log("�޷��ҵ�ָ���û�,key:"+key);
        throw runtime_error("�޷��ҵ�ָ���û�");
    }
    return target_list;
    //log("done");
}

//�û���¼
bool login(vector<User>& user_list,string &puser){
    log("login:���Ե�¼");
    string username;
    //o("��¼");

    CT("��¼");

    oi(username,"����������˵�����");

    string (User::*pfunc)() const=&User::NAME;
    
    if(Userlist_search(user_list,username,pfunc).size()==1){
        log("login:��¼�ɹ�");
        //op("��¼�ɹ���");
        puser=username;
        return true;
    }else{
        log("login:��¼ʧ��");
        op("�����˲����ڣ������³���");
        puser="";
        return false;
    }
}


///�˵�ϵͳ��
/*
���˵�
    -����
    -����
    -����//��ѯָ��������ͼ��
        -����������
        -�����߲���
        -�������˲���
        -��ISBN����
    -ͼ�����
        -��ѯͼ����Ϣ//�г�����ͼ��
        -����µ�ͼ��
        -ɾ������ͼ��
        -�޸�����ͼ��
    -�����˹���
        -��ѯ��������Ϣ
        -����µĽ�����
        -ɾ�����н�����
        -�޸����н�����
    -ͼ�鱨��
    -�˳�����
*/

//�˵�״̬��
enum MenuState{
    MAIN,
        BORROW_BOOK,
        RETURN_BOOK,
        SEARCH_BOOK,
            SEARCH_BOOK_BY_TITLE,
            SEARCH_BOOK_BY_AUTHOR,
            SEARCH_BOOK_BY_USER,
            SEARCH_BOOK_BY_ISBN,
        MANAGE_BOOK,
            QUERY_BOOK,
            ADD_BOOK,
            DEL_BOOK,
            EDIT_BOOK,
        MANAGE_USER,
            QUERY_USER,
            ADD_USER,
            DEL_USER,
            EDIT_USER,
        REPORT_BOOK,
    EXIT
};

//���˵�
MenuState main_menu(){
    log("�������˵�");
    title("ͼ��ݹ���ϵͳ");
    cls();
    //menu_level=0;

    vector<string> ops={"����","����","����","ͼ�����","�����˹���","ͼ�鱨��","�˳�����"};
    CT("���˵�");

    int c=option(ops,false);;//�洢�û�ѡ������ͱ���
    switch(c){
        case 1:
            return BORROW_BOOK;
            break;
        case 2:
            return RETURN_BOOK;
            break;
        case 3:
            return SEARCH_BOOK;
            break;
        case 4:
            return MANAGE_BOOK;
            break;
        case 5:
            return MANAGE_USER;
            break;
        case 6:
            return REPORT_BOOK;
            break;
        case 7:
            return EXIT;
            break;
        default:
            log("���˵�Ĭ���˳�");
            return MAIN;
            break;
    }
}

//����˵�
MenuState borrow_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    int &BRIEFTAB=Cfg.BRIEFTAB;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log("�������˵�");
    title("����");
    cls();
    string username;
    if(!login(user_list,username)){
        return MAIN;
    }

    //�������û���׼����
    if(Userlist_element(user_list,username).STATUS()=="B"){
        op("���û��ѱ�������������޷����飡");
        return MAIN;
    }

    cls();
    CT("����");


    Booklist_briefheading(true,BRIEFTAB);

    int freenum=0;
    
    //Booklist_info_iffree(book_list,freenum);

    //�����ɱ����ĵ��������
    vector<int> free_index;
    vector<Book> templist;
    for(size_t i=0;i<book_list.size();++i){
        if(book_list[i].isfree()){
            templist.push_back(book_list[i]);
            //o(to_string(freenum+1),BRIEFTAB);
            //book_list[i].briefinfo(BRIEFTAB);
            free_index.push_back(i);
            freenum++;
        }
    }

    Booklist_briefinfo(templist,true,BRIEFTAB);


    if(freenum==0){
        o("û�п��Խ��ĵ��鼮");
        pause();
        return MAIN;
    }

    cout<<"�ܼ�"<<freenum<<"���ɱ����ĵĲ�ͬ�鼮��"<<endl;
    
    //vector<Book> book_list_free;

    int bookid_to_choose;
    o("������Ҫ���ĵ��鼮��ţ�����0������һ����");
    i(bookid_to_choose,0,freenum);
    if(bookid_to_choose==0){
        return MAIN;
    }else{
        size_t index=free_index[bookid_to_choose-1];//ʵ������
        
        //�Ƿ��Ѿ����
        if(Userlist_element(user_list,username).is_borrowed(book_list[index].ISBN())){
            op("���Ѿ�����Ȿ���ˣ�");
            return MAIN;
        }

        //��������
        if(book_list[index].STOCK()>0){
            log("�鼮"+book_list[index].TITLE()+"��"+Userlist_element(user_list,username).NAME()+"���1��");
            book_list[index].borrow(1);
            Userlist_element(user_list,username).borrow(book_list[index].ISBN());
            Booklist_save(book_list,BOOKCSV);
            Userlist_save(user_list,USERCSV);
            op("�鼮��"+book_list[index].TITLE()+"�����ĳɹ���");
            return MAIN;
        }
        //Booklist_element(book_list,bookid_to_choose-1).showinfo();
        //book_list[bookid_to_choose-1].showinfo();
    }
    return MAIN;
}

//����˵�
MenuState return_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log("���뻹��˵�");
    title("����");
    cls();
    //Book->�Ƿ񱻽�,User->�Ƿ�Ψһ
    CT("����");
    string returnbookisbn;
    oi(returnbookisbn,"������Ҫ�黹���鼮��ISBN");

    //�Ƿ����
    if(!book_exist(book_list,returnbookisbn)){
        log("���Թ黹һ�������ڵ��飬�˳�����˵�");
        op("�Ȿ�鲻���ڣ�");
        return MAIN;
    }

    if(!Booklist_isbn(book_list,returnbookisbn).isborrowed()){
        log("���Թ黹һ��û�б�������飬�˳�����˵�");
        op("�Ȿ��û�б������");
        return MAIN;
    }
    switch(Userlist_Borrowsamebook(user_list,returnbookisbn)){
    case 0:
        log("���Թ黹һ��û���˽�����飬�˳�����˵�");
        op("�Ȿ��û���˽����");
        break;
    
    case 1:
        log("���Թ黹ֻ��һ���˽������");

        Booklist_isbn(book_list,returnbookisbn).returnbook();
        Booklist_save(book_list,BOOKCSV);
        Userlist_borrowed_isbn(user_list,returnbookisbn).returnBook(returnbookisbn);
        Userlist_save(user_list,USERCSV);

        log("�黹�ɹ���");
        op("�黹�ɹ���");
        return MAIN;
        break;
    default:
        log("���Թ黹�ж���˽������");
        cls();
        string username;
        if(!login(user_list,username)){
            return MAIN;
        }else{
            Booklist_isbn(book_list,returnbookisbn).returnbook();
            Booklist_save(book_list,BOOKCSV);
            Userlist_element(user_list,username).returnBook(returnbookisbn);
            Userlist_save(user_list,USERCSV);

            log("�黹�ɹ���");
            op("�黹�ɹ���");
            return MAIN;
        }

        break;
    }


    return MAIN;
}

//����˵�
MenuState search_menu(){
    log("�������˵�");
    title("����");
    cls();

    vector<string> ops={"����������","�����߲���","�������˲���","��ISBN����"};
    CT("����");

    int c=option(ops,true);
    switch(c)
    {
    case 1:
        return SEARCH_BOOK_BY_TITLE;
        break;
    case 2:
        return SEARCH_BOOK_BY_AUTHOR;
        break;
    case 3:
        return SEARCH_BOOK_BY_USER;
        break;
    case 4:
        return SEARCH_BOOK_BY_ISBN;
        break;
    case 0:
        return MAIN;
        break;
    default:
        return MAIN;
        break;
    }
    return MAIN;
}

MenuState search_book_by_title(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    int &TAB=Cfg.TAB;
    
    log("����������");
    string searchbookname;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::TITLE;

    while(retry&&searchbookname!="0"){
        cls();
        CT("����\\����������");
        oi(searchbookname,"������Ҫ��ѯ���鼮����(����0������һ��)");
        if(searchbookname=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookname,pfunc);
            log("����������ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log("����������ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }
    Booklist_infoheading(TAB);
    for(auto& book:target){
        book.showinfo(TAB);
    }
    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_author(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    int &TAB=Cfg.TAB;

    log("�����߲���");
    string searchbookauthor;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::AUTHOR;

    while(retry&&searchbookauthor!="0"){
        cls();
        CT("����\\�����߲���");
        oi(searchbookauthor,"������Ҫ��ѯ�鼮����������(����0������һ��)");
        if(searchbookauthor=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookauthor,pfunc);
            log("�����߲���ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log("�����߲���ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }
    Booklist_infoheading(TAB);
    for(auto& book:target){
        book.showinfo(TAB);
    }
    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;

    log("�������˲���");
    cls();
    string searchbookuser;
    vector<Book> target;
    vector<string> target_string;
    string (Book::*pfunc)() const=&Book::ISBN;

    string username;
    if(!login(user_list,username)){
        return SEARCH_BOOK;
    }

    target_string=Userlist_element(user_list,username).BORROWED_LIST();
    cls();
    CT("����\\�������˲���");
    if(vetostr(target_string)=="[]"){
        o("�޽��ģ�");
    }else{
        Booklist_infoheading(TAB);
        for(auto& isbn:target_string){
            Booklist_isbn(book_list,isbn).showinfo(TAB);
        }
    }

    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_isbn(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    int &TAB=Cfg.TAB;

    log("��ISBN����");
    string searchbookisbn;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::ISBN;

    while(retry&&searchbookisbn!="0"){
        cls();
        CT("����\\��ISBN����");
        oi(searchbookisbn,"������Ҫ��ѯ�鼮��ISBN(����0������һ��)");
        if(searchbookisbn=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookisbn,pfunc);
            log("��ISBN����ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log("��ISBN����ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }
    Booklist_infoheading(TAB);
    for(auto& book:target){
        book.showinfo(TAB);
    }
    pause();
    return SEARCH_BOOK;
}

//ͼ�����˵�
MenuState manage_book_menu(){
    log("����ͼ�����˵�");
    title("ͼ�����");
    cls();

    vector<string> ops={"��ѯ����ͼ����Ϣ","����µ�ͼ����Ϣ","ɾ������ͼ����Ϣ","�༭ָ��ͼ����Ϣ"};
    CT("ͼ�����");
    int c=option(ops,true);
    switch(c)
    {
    case 1:
        return QUERY_BOOK;
        break;
    case 2:
        return ADD_BOOK;
        break;
    case 3:
        return DEL_BOOK;
        break;
    case 4:
        return EDIT_BOOK;
        break;
    case 0:
        return MAIN;
        break;
    default:
        return MAIN;
        break;
    }
    return MAIN;
}

MenuState query_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    int &TAB=Cfg.TAB;

    log("��ѯ����ͼ����Ϣ");
    CT("ͼ�����\\��ѯ����ͼ����Ϣ");
    Booklist_infoheading(TAB);
    Booklist_info(book_list,false,TAB);
    pause();
    return MANAGE_BOOK;
}

MenuState add_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;

    log("����µ�ͼ��");
    string new_title,new_author,new_isbn,new_press,new_date;
    double new_price;
    int new_quantity,new_stock;

    CT("ͼ�����\\����µ�ͼ��");
    oi(new_title,"����");
    oi(new_author,"����");

    oi(new_isbn,"ISBN");
    while(new_isbn=="0"){
        oi(new_isbn,"ISBN������0������������");
    }
    while(book_exist(book_list,new_isbn)){
        oi(new_isbn,"��ISBN�Ѵ��ڣ�����������");
    }

    oi(new_press,"������");
    oi(new_date,"��������");

    oi(new_price,"����");
    while(!(new_price>0)){
        oi(new_price,"���۱������0������������");
    }


    oi(new_quantity,"����");
    while(!(new_quantity>0)){
        oi(new_quantity,"�����������0������������");
    }

    oi(new_stock,"���");
    while(new_stock>new_quantity||new_stock<0){
        oi(new_stock,"��������������������");
    }
    
    
    Book new_book(new_title,new_author,new_isbn,new_press,new_date,new_price,new_quantity,new_stock);
    
    Booklist_infoheading(TAB);
    new_book.showinfo(TAB);
    if(check("��ȷ���µ�ͼ����Ϣ")){
        book_list.push_back(new_book);
        Booklist_save(book_list,BOOKCSV);
        log("ȷ������µ�ͼ����Ϣ");

        o("��ӳɹ���");
        pause();
        return MANAGE_BOOK;
    }else{
        log("ȡ������µ�ͼ����Ϣ");
        return MANAGE_BOOK;
    }

    //pause();
    return MANAGE_BOOK;
}

MenuState del_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    
    log("ɾ������ͼ��");
    string del_isbn;
    Book target;
    bool retry=true;

    while(retry&&del_isbn!="0"){
        cls();
        CT("ͼ�����\\ɾ������ͼ����Ϣ");
        oi(del_isbn,"������Ҫɾ���鼮��ISBN(����0������һ��)");
        if(del_isbn=="0"){return MANAGE_BOOK;}
        try{
            target=Booklist_isbn(book_list,del_isbn);
            log("��ISBNѰ��ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log("��ISBNѰ��ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    Booklist_infoheading(TAB);
    target.showinfo(TAB);
    if(check("��ȷ��Ҫɾ����ͼ����Ϣ")){
        Booklist_delbook(book_list,del_isbn);
        Booklist_save(book_list,BOOKCSV);
        log("ȷ��ɾ��ͼ����Ϣ��ISBN��"+del_isbn);
        o("ɾ���ɹ���");
        pause();
        return MANAGE_BOOK;
    }else{
        log("ȡ��ɾ��ͼ����Ϣ");
        return MANAGE_BOOK;
    }


    return MANAGE_BOOK;
}

MenuState edit_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book>& book_list=Dat.BookList;
    double &PRICE_MAX=Cfg.PRICE_MAX;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    log("�༭ָ��ͼ��");
    string edit_isbn;
    bool retry=true;

    while(retry&&edit_isbn!="0"){
        cls();
        CT("ͼ�����\\�༭ָ��ͼ����Ϣ");
        oi(edit_isbn,"������Ҫ�༭�鼮��ISBN(����0������һ��)");
        if(edit_isbn=="0"){return MANAGE_BOOK;}
        try{
            Book& target=Booklist_isbn(book_list,edit_isbn);
            log("��ISBNѰ��ɹ�");
            Booklist_infoheading(TAB);
            target.showinfo(TAB);
            retry=false;
            o("��������Ҫ���ĵ���Ŀ"); 
            vector<string> ops={"����","����","ISBN","������","��������","����","����","���"};
            int c=option(ops,false);

            if(c==0){return MANAGE_BOOK;}

            string input;
            oi(input,"�������޸ĺ������");

            switch(c){
            case 1:
                target.change_title(input);
                log("�޸�������"+input);
                break;
            case 2:
                target.change_author(input);
                log("�޸����ߣ�"+input);
                break;
            case 3:
                while(input=="0"){
                    oi(input,"ISBN������0������������");
                }
                while(book_exist(book_list,input)){
                    oi(input,"��ISBN�Ѵ��ڣ�����������");
                }
                target.change_isbn(input);
                log("�޸�ISBN��"+input);
                break;
            case 4:
                target.change_press(input);
                log("�޸ĳ����磺"+input);
                break;
            case 5:
                target.change_date(input);
                log("�޸ĳ������ڣ�"+input);
                break;
            case 6:
                retry=true;
                while(retry){
                    try{
                        double cast_double=stod(input);
                        if(cast_double>0&&cast_double<=PRICE_MAX){
                            target.change_price(cast_double);
                            log("�޸ĵ��ۣ�"+input);
                            retry=false;
                        }else if(cast_double<0){
                            cerr<<"���۱������0��"<<endl;
                            oi(input,"����������"); 
                        }else if(cast_double>PRICE_MAX){
                            cerr<<"����̫���ˣ�"<<endl;
                            oi(input,"����������");
                        }else{
                            cerr<<"������Ч��"<<endl;
                            oi(input,"����������");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"����ĵ�����Ч��������һ����Ч�����֣�"<<endl;
                        oi(input,"����������");
                    }
                    catch(const out_of_range& e){
                        cerr<<"����ĵ��۳����˷�Χ��"<<endl;
                        oi(input,"����������");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"����������");
                    }
                }
                break;
            case 7:
                retry=true;
                while(retry){
                    try{
                        int cast_int=stoi(input);
                        if(cast_int>0&&cast_int>=target.STOCK()){
                            target.change_quantity(cast_int);
                            log("�޸�������"+input);
                            retry=false;
                        }else{
                            cerr<<"�����������0�Ҵ��ڵ��ڵ�ǰ��棡"<<endl;
                            oi(input,"����������");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"�����������Ч��������һ����Ч�����֣�"<<endl;
                        oi(input,"����������");
                    }
                    catch(const out_of_range& e){
                        cerr<<"��������������˷�Χ��"<<endl;
                        oi(input,"����������");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"����������");
                    }
                }
                break;
            case 8:
                retry=true;
                while(retry){
                    try{
                        int cast_int=stoi(input);
                        if(cast_int>0&&cast_int<=target.QUANTITY()){
                            target.change_stock(cast_int);
                            log("�޸�������"+input);
                            retry=false;
                        }else{
                            cerr<<"���������0��С�ڵ���������"<<endl;
                            oi(input,"����������");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"�����������Ч��������һ����Ч�����֣�"<<endl;
                        oi(input,"����������");
                    }
                    catch(const out_of_range& e){
                        cerr<<"��������������˷�Χ��"<<endl;
                        oi(input,"����������");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"����������");
                    }
                }
                break;
            default:
                break;
            }
            
            Booklist_save(book_list,BOOKCSV);
            op("�޸ĳɹ���");
        }
        catch(const exception& e){
            log("��ISBNѰ��ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    return MANAGE_BOOK;
}

//�û�����˵�
MenuState manage_user_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;

    log("�����û�����˵�");
    title("�û�����");
    cls();

    User user;
    string username;

    vector<string> ops;
    
    if(!login(user_list,username)){
        return MAIN;
    }else{
        user=Userlist_element(user_list,username);
    }

    //�������û�
    if(user.STATUS()=="B"){
        op("���û��ѱ������������");
        return MAIN;
    }else if(user.STATUS()=="N"){
        cls();
        CT("�����˹���"+username);
        ops={"��ѯ��ǰ��������Ϣ"};
        int c=option(ops,true);
        switch(c){
        case 1:
            cls();
            Userlist_infoheading(TAB);
            user.showinfo(TAB);
            pause();
            break;
        case 0:
            return MAIN;
            break;
        default:
            return MAIN;
            break;
        }
    }else if(user.STATUS()=="A"){
        cls();
        CT("�����˹���"+username);
        ops={"��ѯ���н�������Ϣ","����µĽ�������Ϣ","ɾ�����н�������Ϣ","�޸����н�������Ϣ"};
        int c=option(ops,true);
        switch(c){
        case 1:
            return QUERY_USER;
            break;
        case 2:
            return ADD_USER;
            break;
        case 3:
            return DEL_USER;
            break;
        case 4:
            return EDIT_USER;
            break;
        default:
            return MAIN;
            break;
        }
    }else{
        op("δ֪����");
        return MAIN;
    }

    return MAIN;
}

MenuState query_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;
    log("��ѯ�����û���Ϣ");
    
    cls();
    CT("�����˹���\\��ѯ���н�������Ϣ");

    Userlist_infoheading(TAB);
    Userlist_info(user_list,TAB);

    pause();
    return MAIN;
}

MenuState add_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    string &USERCSV=Cfg.USERCSV;
    int &TAB=Cfg.TAB;

    log("����µ��û���Ϣ");
    cls();
    CT("�����˹���\\����µĽ�������Ϣ");

    string new_name,new_id,new_status;
    oi(new_name,"����");
    while(new_name=="0"){
        oi(new_name,"���ֲ�����0������������");
    }

    bool retry=true;
    while(retry){
        try{
            if(Userlist_search(user_list,new_name,User::NAME).size()!=0){
                oi(new_name,"�������Ѵ��ڣ�����������");
            }
        }
        catch(const std::exception& e){
            retry=false;
        }
    }
    
    oi(new_id,"ID");
    while(new_id=="0"){
        oi(new_id,"ID������0������������");
    }
    while(user_exist(user_list,new_id)){
        oi(new_id,"��ID�Ѵ��ڣ�����������");
    }

    o("�û�����");
    vector<string> ops={"����Ա","��ͨ�û�","�������û�"};
    int c=option(ops,false);
    switch(c){
    case 1:
        new_status="A";
        break;
    case 2:
        new_status="N";
        break;
    case 3:
        new_status="B";
        break;
    default:
        new_status="ERROR";
        break;
    }
    //oi(new_status,"�û�����(A:����Ա��N:��ͨ�û���B:�������û�)");
    while(new_status!="A"&&new_status!="N"&&new_status!="B"){
        oi(new_status,"��Ч���û����ͣ�����������");
    }
    
    vector<string> new_borrowlist={};
    User new_user(new_name,new_id,new_status,new_borrowlist);

    Userlist_infoheading(TAB);
    new_user.showinfo(TAB);

    if(check("��ȷ���µ��û���Ϣ")){
        user_list.push_back(new_user);
        Userlist_save(user_list,USERCSV);
        log("ȷ������µ��û���Ϣ");

        o("��ӳɹ���");
        pause();
        return MAIN;
    }else{
        log("ȡ������µ��û���Ϣ");
        return MAIN;
    }
    //pause();
    return MAIN;
}

MenuState del_user(struct ListData &Dat){
    log("ɾ�������û���Ϣ");
    return MAIN;
}

MenuState edit_user(struct ListData &Dat){
    log("�޸�ָ���û���Ϣ");
    return MAIN;
}

//ͼ�鱨��˵�
MenuState report_book_menu(){
    log("����ͼ�鱨��˵�");
    title("ͼ�鱨��");
    return MAIN;
}


int main(){
    log("-----��������-----");
    title("�����ʼ��");

    //Ĭ������
    Config Cfg;
    Cfg.LOG="log.txt";
    Cfg.BOOKCSV="test.csv";
    Cfg.USERCSV="user.csv";
    Cfg.TAB=20;
    Cfg.BRIEFTAB=30;
    Cfg.PRICE_MAX=10000;

    Data Dat;

    Booklist_init(Dat.BookList,Cfg.BOOKCSV);

    Userlist_init(Dat.UserList,Cfg.USERCSV);

    cout<<"BookList CSV:"<<Cfg.BOOKCSV<<endl;
    cout<<"UserList CSV:"<<Cfg.USERCSV<<endl;

    pause();

    MenuState menu_state= MAIN;

    while(menu_state!=EXIT){
        cls();
        switch(menu_state){
            case MAIN:
                menu_state=main_menu();
                break;
            case BORROW_BOOK:
                menu_state=borrow_menu(Dat,Cfg);
                break;
            case RETURN_BOOK:
                menu_state=return_menu(Dat,Cfg);
                break;
            case SEARCH_BOOK:
                menu_state=search_menu();
                break;

                case SEARCH_BOOK_BY_TITLE:
                    menu_state=search_book_by_title(Dat,Cfg);
                    break;
                
                case SEARCH_BOOK_BY_AUTHOR:
                    menu_state=search_book_by_author(Dat,Cfg);
                    break;
                
                case SEARCH_BOOK_BY_USER:
                    menu_state=search_book_by_user(Dat,Cfg);
                    break;
                
                case SEARCH_BOOK_BY_ISBN:
                    menu_state=search_book_by_isbn(Dat,Cfg);
                    break;
                
            case MANAGE_BOOK:
                menu_state=manage_book_menu();
                break;

                case QUERY_BOOK:
                    menu_state=query_book(Dat,Cfg);
                    break;

                case ADD_BOOK:
                    menu_state=add_book(Dat,Cfg);
                    break;
                
                case DEL_BOOK:
                    menu_state=del_book(Dat,Cfg);
                    break;
                
                case EDIT_BOOK:
                    menu_state=edit_book(Dat,Cfg);
                    break;
            
            case MANAGE_USER:
                menu_state=manage_user_menu(Dat,Cfg);
                break;
                
                case QUERY_USER:
                menu_state=query_user(Dat,Cfg);
                break;

                case ADD_USER:
                menu_state=add_user(Dat,Cfg);
                break;

                case DEL_USER:
                menu_state=del_user(Dat);
                break;

                case EDIT_USER:
                menu_state=edit_user(Dat);
                break;

            case REPORT_BOOK:
                menu_state=report_book_menu();
                break;

            default:
                menu_state=EXIT;
                break;
        }
    }

    Booklist_save(Dat.BookList,Cfg.BOOKCSV);
    Userlist_save(Dat.UserList,Cfg.USERCSV);

    log("����Ĭ�Ͻ���");
    
    cls();
    pause();
}