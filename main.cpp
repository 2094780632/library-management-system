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
#include<mutex>
using namespace std;

///ȫ������������
//��־��¼ϵͳ
//log����
enum logtype{
    I,
    E,
    F,
    W
};
class Log{
    public:
        static Log& getInstance(){
            static Log instance;
            return instance;
        }

        void setlogfile(const string new_log){
            lock_guard<mutex> lock(mutex_);
            logfilename=new_log;
        }

        
        //��־��¼
        int log(const logtype lt,const string &text){
            lock_guard<mutex> lock(mutex_);
            //��ȡʱ��
            auto now=chrono::system_clock::now();
            time_t now_c=chrono::system_clock::to_time_t(now);
            stringstream ss;
            ss<<put_time(localtime(&now_c),"%Y-%m-%d %H:%M:%S");
            string log_time;
            log_time=ss.str();
            
            ofstream logs;
            logs.open(logfilename,ios::app);
            //�ж��Ƿ�򿪳ɹ�
            if (!logs.is_open()) {
                cerr << "�޷�д��log��¼"<<endl;
                return 1;
            }

            string logt;
            switch(lt){
            case I:
                logt="INFO";
                break;
            case E:
                logt="ERROR";
                break;
            case F:
                logt="FAIL";
                break;
            case W:
                logt="WARNING";
            default:
                logt="?";
                break;
            }

            string log_str=log_time+":["+logt+']'+text+'\n';
            logs<<log_str;
            return 0;
        }
                
    private:
        Log():logfilename("log.txt"){}
        Log(const Log&)=delete;
        Log& operator=(const Log&)=delete;

        string logfilename;
        mutex mutex_;
};
//��־��¼
void log(logtype lt,const string &text){
    Log::getInstance().log(lt,text);
}
void setlogpath(const string LOG){
    Log::getInstance().setlogfile(LOG);
}

//Ԥ������
class Book;
class User;

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
            log(F,"i():����Ϸ������ʧ��");
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
            log(F,"i():int������Ϸ������ʧ��");
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
    log(E,"check():no return");
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
    log(I,"f_readall():���Զ�ȡ�ļ���"+file_name);
    ifstream file;
    file.open(file_name);
    string content,line;
    //�ж��Ƿ�򿪳ɹ�
    if(file.is_open()){
        log(I,"f_readall():�ɹ���ȡ�ļ�"+file_name);
        while (getline(file,line)){
            content+=line+'\n';
        }
        file.close();
        return content;
    }else{
        log(F,"f_readall():�޷���ȡ�ļ�"+file_name);
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
        log(I,"f_readline():�ɹ����ж�ȡ�ļ�"+file_name);
        getline(file,line);
        file.close();
        return line;
    }else{
        log(I,"f_readline():�޷����ж�ȡ�ļ�"+file_name);
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
        log(E,"f_write():��Чд��ģʽ");
        cerr << "�޷����ļ�: "<<file_name<<endl;
        return 1;
    }

    //�ж��Ƿ�򿪳ɹ�
    if (!file.is_open()) {
        log(E,"f_write():�޷�д���ļ�"+file_name);
        cerr << "�޷����ļ�: "<<file_name<<endl;
        return 1;
    }
    file<<text;
    file.close();

    if(mode=="app"){
        log(I,"f_write():�ɹ�׷��д���ļ���"+file_name);
    }else if(mode=="trunc"){
        log(I,"f_write():�ɹ�����д���ļ���"+file_name);
    }

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
        void change_title(string const &value){
            this->title=value;
        }
        void change_author(string const &value){
            this->author=value;
        }
        void change_isbn(string const &value){
            this->isbn=value;
        }
        void change_press(string const &value){
            this->press=value;
        }
        void change_date(string const &value){
            this->date=value;
        }
        void change_price(double const &value){
            this->price=value;
        }
        void change_quantity(unsigned int const &value){
            this->quantity=value;
        }
        void change_stock(unsigned int const &value){
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
            o("");
        }

        //��Ҫ��Ϣչʾ
        void briefinfo(int &BRIEFTAB){
            o(title,BRIEFTAB);
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
            log(I,"BOOK:DEBUGBOOKINFO");
            o(title);
            o(author);
            o(isbn);
            o(press);
            o(date);
            o(to_string(price));
            o(to_string(quantity));
            o(to_string(stock));
            o("");
        }

        //��������
        string savestr(){
            ostringstream ss;
            ss<<fixed<<setprecision(2);
            ss<<title<<','<<author<<','<<isbn<<','<<press<<','<<date<<','<<price<<','<<quantity<<','<<stock;
            return ss.str();
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
        log(F,"Booklist_init():��ʼ��ͼ������ʱ�޷���csv�ļ�");
        throw runtime_error("�޷���ʼ��ͼ�����ݣ�");
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
    log(I,"Booklist_init():�ɹ���ʼ��ͼ������");
    file.close();
}

void Booklist_save(vector<Book>& book_list,string csv);
void Booklist_defaultcsv(){
    log(I,"Booklist_defaultcsv():��ʼ��BOOKCSV");
    Book newbook("title","author","isbn","C++","2025-6-1",61.6,1,0);
    vector<Book> empty={};
    Booklist_save(empty,"book.csv");
}

//����ͼ������
void Booklist_save(vector<Book>& book_list,string csv){
    log(I,"Booklist_save():����ͼ������");
    string save="";
    for(auto& book:book_list){
        save+=book.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
}

//��ȡͼ������
void Booklist_info(vector<Book>& book_list,bool isid,int &TAB){
    log(I,"Booklist_info():���Զ�ȡͼ������");
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
    log(I,"Booklist_briefinfo():���Զ�ȡͼ������");
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
    o("�ɽ�����",BRIEFTAB);
    o("");
}

//����ָ��������ͼ�����
Book& Booklist_element(vector<Book>& book_list,int index){
    log(I,"Booklist_element():����ͼ�����,index:"+to_string(index));
    if (index < 0 || index >= book_list.size()) {
        log(E,"Booklist_element():�ڵ���ͼ�����ʱ����������Χ,index:"+to_string(index));
        throw out_of_range("����������Χ");
    }
    return book_list[index];
}

//���Բ���ISBNͼ�����
Book& Booklist_isbn(vector<Book>& book_list,string isbn){
    log(I,"Booklist_isbn():����ָ��ISBN�鼮����");
    auto it=find_if(book_list.begin(),book_list.end(),[&isbn](const Book& book){
        return book.ISBN()==isbn;
    });
    if (it == book_list.end()) {
        log(E,"Booklist_isbn():ISBNͼ��������ʧ��");
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
    if(target_list.empty()){
        log(F,"Booklist_search():�޷��ҵ�ָ����Ŀ,key:"+key);
        throw runtime_error("�޷��ҵ�ָ����Ŀ");
    }
    return target_list;
}


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

        //�޸ĵ�������
        void change_name(string const &value){
            this->name=value;
        }
        void change_id(string const &value){
            this->id=value;
        }
        void change_status(string const &value){
            this->status=value;
        }


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
        log(F,"Userlist_init():��ʼ���û�����ʱ�޷���csv�ļ�");
        throw runtime_error("�޷���ʼ���û����ݣ�");
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
    log(I,"Userlist_init():�ɹ���ʼ���û�����");
    file.close();
}

void Userlist_save(vector<User>& user_list,string csv);
void Userlist_defaultcsv(){
    log(I,"Userlist_defaultcsv():��ʼ��USERCSV");
    User Report("Report","0","R",{});
    User Admin("Admin","1","A",{});
    vector<User> defaultlist={Report,Admin};
    Userlist_save(defaultlist,"user.csv");
}

//��ȡ�û�����
void Userlist_info(vector<User>& user_list,int &TAB){
    log(I,"Userlist_info():���Զ�ȡ�û�����");
    for(auto& user:user_list){
        user.showinfo(TAB);
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
    log(I,"Userlist_save():�����û�����");
    string save="";
    for(auto& user:user_list){
        save+=user.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
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
    log(I,"Userlist_element():�����û�����"+name);
    for(auto& user:user_list){
        if(user.NAME()==name){return user;}
    }
    log(F,"Userlist_element():�����û�����ʧ��(NAME)");
    throw runtime_error("�����û�����ʧ��(NAME)");
}

//����ָ���û�����ID��
User& Userlist_element_id(vector<User>& user_list,string id){
    log(I,"Userlist_element_id():�����û�����"+id);
    for(auto& user:user_list){
        if(user.ID()==id){return user;}
    }
    log(F,"Userlist_element_id():�����û�����ʧ��(ID)");
    throw runtime_error("�����û�����ʧ��(ID)");
}

//���ĳ����û�����һ����
User& Userlist_borrowed_isbn(vector<User>& user_list,string isbn){
    log(I,"Userlist_borrowed_isbn():�����û�����(ISBN) ��"+isbn);
    for(auto& user:user_list){
        if(user.is_borrowed(isbn)){
            return user;
        }
    }
    log(F,"Userlist_borrowed_isbn():�����û�����ʧ��(ISBN)");
    throw "�����û�����ʧ��(ISBN)";
}

//����ָ����ʽ���ط����������û�����
vector<User> Userlist_search(vector<User>& user_list,string key,string (User::*pfunc)() const){
    vector<User> target_list;
    for(auto& user:user_list){
        if((user.*pfunc)()==key){target_list.push_back(user);}
    }
    if(target_list.empty()){
        log(F,"Userlist_search():�޷��ҵ�ָ���û�,key:"+key);
        throw runtime_error("�޷��ҵ�ָ���û�");
    }
    return target_list;
}

//ɾ��ָ��ID�û�
void Userlist_deluser(vector<User>& user_list,string id){
    user_list.erase(remove_if(user_list.begin(),user_list.end(),[&id](const User& user) {return user.ID()==id;}),user_list.end());
}

//�û���¼
bool login(vector<User>& user_list,string &puser){
    log(I,"login():���Ե�¼");
    string username;

    CT("��¼");

    oi(username,"����������˵�����");

    string (User::*pfunc)() const=&User::NAME;
    
    try{
        Userlist_search(user_list,username,pfunc);
        log(I,"login:��¼�ɹ�");
        puser=username;
        return true;
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
        log(F,"login:��¼ʧ��");
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
    -����
        -����������
        -�����߲���
        -�������˲���
        -��ISBN����
    -ͼ�����
        -��ѯͼ����Ϣ
        -����µ�ͼ��
        -ɾ������ͼ��
        -�޸�����ͼ��
    -�����˹���
        -��ѯ��������Ϣ
        -����µĽ�����
        -ɾ�����н�����
        -�޸����н�����
    -ͼ�鱨��
        -ͼ������
        -ͼ���޸�
    -����
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
            REPORT_DAMAGE,
            REPORT_FIX,
        SETTING,
    EXIT
};

//���˵�
MenuState main_menu(){
    log(I,"main_menu():�������˵�");
    title("ͼ��ݹ���ϵͳ");
    cls();

    vector<string> ops={"����","����","����","ͼ�����","�����˹���","ͼ�鱨��","����","�˳�����"};
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
            return SETTING;
            break;
        case 8:
            return EXIT;
            break;
        default:
            log(I,"main_menu():���˵�Ĭ���˳�");
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

    log(I,"borrow_menu():�������˵�");
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

    //�����ɱ����ĵ��������
    vector<int> free_index;
    vector<Book> templist;
    for(size_t i=0;i<book_list.size();++i){
        if(book_list[i].isfree()){
            templist.push_back(book_list[i]);
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
            log(I,"borrow_menu():�鼮"+book_list[index].TITLE()+"��"+Userlist_element(user_list,username).NAME()+"���1��");
            book_list[index].borrow(1);
            Userlist_element(user_list,username).borrow(book_list[index].ISBN());
            Booklist_save(book_list,BOOKCSV);
            Userlist_save(user_list,USERCSV);
            op("�鼮��"+book_list[index].TITLE()+"�����ĳɹ���");
            return MAIN;
        }
    }
    return MAIN;
}

//����˵�
MenuState return_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log(I,"return_menu():���뻹��˵�");
    title("����");
    cls();
    //Book->�Ƿ񱻽�,User->�Ƿ�Ψһ
    CT("����");
    string returnbookisbn;
    oi(returnbookisbn,"������Ҫ�黹���鼮��ISBN");

    //�Ƿ����
    if(!book_exist(book_list,returnbookisbn)){
        log(E,"return_menu():���Թ黹һ�������ڵ��飬�˳�����˵�");
        op("�Ȿ�鲻���ڣ�");
        return MAIN;
    }

    if(!Booklist_isbn(book_list,returnbookisbn).isborrowed()){
        log(E,"return_menu():���Թ黹һ��û�б�������飬�˳�����˵�");
        op("�Ȿ��û�б������");
        return MAIN;
    }
    switch(Userlist_Borrowsamebook(user_list,returnbookisbn)){
    case 0:
        log(E,"return_menu():���Թ黹һ��û���˽�����飬�˳�����˵�");
        op("�Ȿ��û���˽����");
        break;
    
    case 1:
        log(I,"return_menu():���Թ黹ֻ��һ���˽������");

        Booklist_isbn(book_list,returnbookisbn).returnbook();
        Booklist_save(book_list,BOOKCSV);
        Userlist_borrowed_isbn(user_list,returnbookisbn).returnBook(returnbookisbn);
        Userlist_save(user_list,USERCSV);

        log(I,"return_menu():�黹�ɹ���");
        op("�黹�ɹ���");
        return MAIN;
        break;
    default:
        log(I,"return_menu():���Թ黹�ж���˽������");
        cls();
        string username;
        if(!login(user_list,username)){
            return MAIN;
        }else{
            Booklist_isbn(book_list,returnbookisbn).returnbook();
            Booklist_save(book_list,BOOKCSV);
            Userlist_element(user_list,username).returnBook(returnbookisbn);
            Userlist_save(user_list,USERCSV);

            log(I,"return_menu():�黹�ɹ���");
            op("�黹�ɹ���");
            return MAIN;
        }

        break;
    }


    return MAIN;
}

//����˵�
MenuState search_menu(){
    log(I,"serach_menu():�������˵�");
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
    
    log(I,"search_book_by_title():����������");
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
            log(I,"search_book_by_title():����������ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log(F,"search_book_by_title():����������ʧ��");
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

    log(I,"search_book_by_author():�����߲���");
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
            log(I,"search_book_by_author():�����߲���ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log(F,"search_book_by_author():�����߲���ʧ��");
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

    log(I,"search_book_by_user()�������˲���");
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
    CT("����\\�������˲���\\"+username);
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

    log(I,"search_book_by_isbn():��ISBN����");
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
            log(I,"search_book_by_isbn():��ISBN����ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log(F,"search_book_by_isbn():��ISBN����ʧ��");
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
    log(I,"manage_book_menu():����ͼ�����˵�");
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

    log(I,"query_book():��ѯ����ͼ����Ϣ");
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

    log(I,"add_book():����µ�ͼ��");
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
        log(I,"add_book():ȷ������µ�ͼ����Ϣ");

        o("��ӳɹ���");
        pause();
        return MANAGE_BOOK;
    }else{
        log(I,"add_book():ȡ������µ�ͼ����Ϣ");
        return MANAGE_BOOK;
    }
    return MANAGE_BOOK;
}

MenuState del_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    
    log(I,"del_book():ɾ������ͼ��");
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
            log(I,"del_book():��ISBNѰ��ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log(F,"del_book():��ISBNѰ��ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    Booklist_infoheading(TAB);
    target.showinfo(TAB);
    if(check("��ȷ��Ҫɾ����ͼ����Ϣ")){
        Booklist_delbook(book_list,del_isbn);
        Booklist_save(book_list,BOOKCSV);
        log(I,"del_book():ȷ��ɾ��ͼ����Ϣ��ISBN��"+del_isbn);
        o("ɾ���ɹ���");
        pause();
        return MANAGE_BOOK;
    }else{
        log(I,"del_book():ȡ��ɾ��ͼ����Ϣ");
        return MANAGE_BOOK;
    }


    return MANAGE_BOOK;
}

MenuState edit_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book>& book_list=Dat.BookList;
    double &PRICE_MAX=Cfg.PRICE_MAX;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    log(I,"edit_book():�༭ָ��ͼ��");
    string edit_isbn;
    bool retry=true;

    while(retry&&edit_isbn!="0"){
        cls();
        CT("ͼ�����\\�༭ָ��ͼ����Ϣ");
        oi(edit_isbn,"������Ҫ�༭�鼮��ISBN(����0������һ��)");
        if(edit_isbn=="0"){return MANAGE_BOOK;}
        try{
            Book& target=Booklist_isbn(book_list,edit_isbn);
            log(I,"edit_book():��ISBNѰ��ɹ�");
            Booklist_infoheading(TAB);
            target.showinfo(TAB);
            retry=false;
            o("��������Ҫ���ĵ���Ŀ"); 
            vector<string> ops={"����","����","ISBN","������","��������","����","����","���"};
            int c=option(ops,true);

            if(c==0){return MANAGE_BOOK;}

            string input;
            oi(input,"�������޸ĺ������");

            switch(c){
            case 1:
                target.change_title(input);
                log(I,"edit_book():�޸�������"+input);
                break;
            case 2:
                target.change_author(input);
                log(I,"edit_book():�޸����ߣ�"+input);
                break;
            case 3:
                while(input=="0"){
                    oi(input,"ISBN������0������������");
                }
                while(book_exist(book_list,input)){
                    oi(input,"��ISBN�Ѵ��ڣ�����������");
                }
                target.change_isbn(input);
                log(I,"edit_book():�޸�ISBN��"+input);
                break;
            case 4:
                target.change_press(input);
                log(I,"edit_book():�޸ĳ����磺"+input);
                break;
            case 5:
                target.change_date(input);
                log(I,"edit_book():�޸ĳ������ڣ�"+input);
                break;
            case 6:
                retry=true;
                while(retry){
                    try{
                        double cast_double=stod(input);
                        if(cast_double>0&&cast_double<=PRICE_MAX){
                            target.change_price(cast_double);
                            log(I,"edit_book():�޸ĵ��ۣ�"+input);
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
                    catch(const exception& e){
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
                            log(I,"edit_book():�޸�������"+input);
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
                    catch(const exception& e){
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
                            log(I,"edit_book():�޸�������"+input);
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
                    catch(const exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"����������");
                    }
                }
                break;
            case 0:
                return MANAGE_BOOK;
                break;

            default:
                break;
            }
            
            Booklist_save(book_list,BOOKCSV);
            op("�޸ĳɹ���");
        }
        catch(const exception& e){
            log(F,"edit_book():��ISBNѰ��ʧ��");
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

    log(I,"manage_user_menu():�����û�����˵�");
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
    log(I,"query_user():��ѯ�����û���Ϣ");
    
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

    log(I,"add_user():����µ��û���Ϣ");
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
        catch(const exception& e){
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
        log(I,"add_user():ȷ������µ��û���Ϣ");

        o("��ӳɹ���");
        pause();
        return MAIN;
    }else{
        log(I,"add_user():ȡ������µ��û���Ϣ");
        return MAIN;
    }
    return MAIN;
}

MenuState del_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    string &USERCSV=Cfg.USERCSV;
    int &TAB=Cfg.TAB;
    
    log(I,"del_user():ɾ�������û���Ϣ");
    string del_id;
    vector<User> target;
    bool retry=true;

    while(retry&&del_id!="0"){
        cls();
        CT("�����˹���\\ɾ�������û���Ϣ");
        oi(del_id,"������Ҫɾ���û���ID(����0�������˵�)");
        if(del_id=="0"){return MAIN;}
        try{
            target=Userlist_search(user_list,del_id,User::ID);
            log(I,"del_user():��IDѰ���û��ɹ�");
            retry=false;
        }
        catch(const exception& e){
            log(F,"del_user():��IDѰ���û�ʧ��");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    User deltarget;
    if(target.size()==1){
        deltarget=target[0];
    }else{
        exit(3);
    }

    Userlist_infoheading(TAB);
    deltarget.showinfo(TAB);
    if(check("��ȷ��Ҫɾ���Ľ�������Ϣ")){
        Userlist_deluser(user_list,del_id);
        Userlist_save(user_list,USERCSV);
        log(I,"del_user():ȷ��ɾ���û���Ϣ��ID��"+del_id);
        o("ɾ���ɹ���");
        pause();
        return MAIN;
    }else{
        log(I,"del_user():ȡ��ɾ��ͼ����Ϣ");
        return MAIN;
    }


    return MAIN;
}

MenuState edit_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    string &USERCSV=Cfg.USERCSV;
    int &TAB=Cfg.TAB;
    
    log(I,"edit_user():�޸�ָ���û���Ϣ");
    string edit_id;
    bool retry=true;
    while(retry&&edit_id!="0"){
        cls();
        CT("�����˹���\\�༭ָ���û���Ϣ");
        oi(edit_id,"������Ҫ�༭�Ľ�����ID(����0������һ��)");
        if(edit_id=="0"){return MAIN;}
        try{
           User& target=Userlist_element_id(user_list,edit_id);
           log(I,"edit_user():��IDѰ���û��ɹ�");
           Userlist_infoheading(TAB);
           target.showinfo(TAB);
           retry=false;
           
           o("��������Ҫ���ĵ���Ŀ");
           vector<string> ops={"����","ID","״̬","�����б�"};
           int c=option(ops,true);

           if(c==0||c==4){return MAIN;}

           string input;
           oi(input,"�������޸ĺ������");

           switch(c){
            case 1:
                target.change_name(input);
                log(I,"edit_user():�޸����֣�"+input);
                break;
            case 2:
                while(input=="0"){
                    oi(input,"ID������0������������");
                }
                while(user_exist(user_list,input)){
                    oi(input,"��ID�Ѵ��ڣ�����������");
                }
                target.change_id(input);
                log(I,"edit_user():�޸�ID��"+input);
                break;
            case 3:
                while(input!="A"&&input!="N"&&input!="B"){
                    oi(input,"�û�״ֻ̬����A������Ա�û���,N����ͨ�û�����B���������û���������������");
                }
                target.change_status(input);
                log(I,"edit_user():�޸��û�״̬"+input);
                break;
            case 4:
                op("�����б����ڽ���/����˵��в�����������ת�����˵�");
                return MAIN;
            case 0:
                return MAIN;
                break;
            default:
                return MAIN;
                break;
           }

           Userlist_save(user_list,USERCSV);
           op("�޸ĳɹ���");
        }
        catch(const exception& e){
            log(F,"edit_user():��IDѰ���û�ʧ��");
            cerr << e.what() << '\n';
            pause();
        }
    }
    
    return MAIN;
}

//ͼ�鱨��˵�
MenuState report_book_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    log(I,"report_book_menu():����ͼ�鱨��˵�");
    title("ͼ�鱨��");
    CT("ͼ�鱨��");
    
    vector<string> ops={"ͼ������","ͼ���޸�"};
    int c=option(ops,true);
    switch(c){
    case 1:
        return REPORT_DAMAGE;
        break;
    case 2:
        return REPORT_FIX;
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

MenuState report_damage(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log(I,"report_damage():�������𱨸�˵�");
    CT("ͼ�鱨��\\���𱨸�");

    string report_book_isbn;
    oi(report_book_isbn,"����������ͼ���ISBN");
    while(!book_exist(book_list,report_book_isbn)||!Booklist_isbn(book_list,report_book_isbn).isborrowed()){
        oi(report_book_isbn,"�޷��ҵ��鼮������������");
    }
    string report_user_id;
    oi(report_user_id,"��������ͼ��Ľ�����ID");
    while(!user_exist(user_list,report_user_id)||!Userlist_element_id(user_list,report_user_id).is_borrowed(report_book_isbn)){
        oi(report_user_id,"�޷��ҵ������ˣ�����������");
    }

    Booklist_infoheading(TAB,false);
    Booklist_isbn(book_list,report_book_isbn).showinfo(TAB);
    Userlist_infoheading(TAB);
    Userlist_element_id(user_list,report_user_id).showinfo(TAB);

    if(check("��ȷ��������Ϣ")){
        Booklist_isbn(book_list,report_book_isbn).borrow(1);
        Userlist_element_id(user_list,"0").borrow(report_book_isbn);
        Userlist_element_id(user_list,report_user_id).change_status("B");
        Booklist_save(book_list,BOOKCSV);
        Userlist_save(user_list,USERCSV);
        log(I,"report_damage():�ɹ�����");
        op("�ɹ����𣬼����������˵���");
        return MAIN;
    }else{
        return MAIN;
    }
    return MAIN;
}

MenuState report_fix(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log(I,"report_fix():�������𱨸�˵�");
    CT("ͼ�鱨��\\�޸�����");

    vector<string> damage_list=Userlist_element_id(user_list,"0").BORROWED_LIST();
    string report_book_isbn;

    if(damage_list.size()==0){
        op("��������ͼ�飬�����������˵���");
        return MAIN;
    }

    Booklist_infoheading(TAB,false);
    for(auto& list_isbn:damage_list){
        Booklist_isbn(book_list,list_isbn).showinfo(TAB);
    }
    oi(report_book_isbn,"�������޸����鼮��ISBN");
    auto it=find(damage_list.begin(),damage_list.end(),report_book_isbn);
    while(it==damage_list.end()){
        if(report_book_isbn=="0"){return MAIN;}
        oi(report_book_isbn,"��Ч���룡����������");
        auto it=find(damage_list.begin(),damage_list.end(),report_book_isbn);
    }

    string report_user_id;
    oi(report_user_id,"��������ͼ��Ľ�����ID");
    while(!user_exist(user_list,report_user_id)||!Userlist_element_id(user_list,report_user_id).is_borrowed(report_book_isbn)||Userlist_element_id(user_list,report_user_id).STATUS()=="B"){
        oi(report_user_id,"�޷��ҵ������ˣ�����������");
    }

    Booklist_isbn(book_list,report_book_isbn).returnbook();
    Booklist_save(book_list,BOOKCSV);
    Userlist_borrowed_isbn(user_list,report_book_isbn).returnBook(report_book_isbn);
    Userlist_element_id(user_list,report_user_id).change_status("N");
    Userlist_save(user_list,USERCSV);
    log(I,"report_fix():�ɹ��޸�");
    op("�ɹ��޸��������������˵���");

    return MAIN;
}

//����
MenuState setting_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    string &LOG=Cfg.LOG;
    int &BRIEFTAB=Cfg.BRIEFTAB;
    int &TAB=Cfg.TAB;
    double &PRICE_MAX=Cfg.PRICE_MAX;

    log(I,"setting_menu():�������ò˵�");
    CT("����");

    vector<string> ops={"TAB�Ʊ�����ȣ�չʾ��ϸ��Ϣʱ����"+to_string(TAB),"TAB�Ʊ�����ȣ�չʾ��Ҫ��Ϣʱ����"+to_string(BRIEFTAB),"������鼮����󵥼ۣ�"+to_string(PRICE_MAX),"��־��¼�ļ���"+LOG,"�����־��¼"};
    int c=option(ops,true);

    switch(c){
    case 1:{
        int temp;
        oi(temp,"������һ����1-50�ڵ�һ����\n����ԽС��ϢԽ��ӵ����");
        while(temp<1||temp>50){
            oi(temp,"���벻�Ϸ�������������");
        }
        TAB=temp;
        return SETTING;
        break;
    }
    case 2:{
        int temp;
        oi(temp,"������һ����1-50�ڵ�һ����\n�����TAB��10���Ҽ���");
        while(temp<1||temp>50){
            oi(temp,"���벻�Ϸ�������������");
        }
        BRIEFTAB=temp;
        log(I,"setting_menu():BRIEFTAB"+to_string(BRIEFTAB));
        return SETTING;
        break;
    }
        
    case 3:{
        double temp;
        oi(temp,"������һ����1-10000�ڵ�һ����");
        while(temp<1||temp>10000){
            oi(temp,"���벻�Ϸ�������������");
        }
        PRICE_MAX=temp;
        log(I,"setting_menu():PRICE_MAX:"+to_string(PRICE_MAX));
        return SETTING;
        break;
    }

    case 4:{
        string temp;
        oi(temp,"���������ڴ����־���ļ���");
        Log::getInstance().setlogfile(temp);
        LOG=temp;
        log(I,"setting_menu():LOG:"+temp);
        return SETTING;
        break;
    }

    case 5:
        if(remove(LOG.c_str())==0){
            log(I,"setting_menu():��־����ɹ���");
            op("����ɹ���");
            return SETTING;
        }else{
            cerr<<"��־�ļ�ɾ��ʧ�ܣ�"<<endl;
            perror("����ԭ��");
            pause();
        }
        break;
    
    case 0:
        return MAIN;
        break;

    default:
        return SETTING;
        break;
    }

    return MAIN;
}


//����
void restart(){
    system("start main.exe");
    exit(1);
}

int main(){
    log(I,"-----��������-----");
    title("�����ʼ��");

    //Ĭ������
    Config Cfg;
    Cfg.LOG="log.txt";
    Cfg.BOOKCSV="book.csv";
    Cfg.USERCSV="user.csv";
    Cfg.TAB=20;
    Cfg.BRIEFTAB=30;
    Cfg.PRICE_MAX=10000;

    Data Dat;

    //���Զ�ȡ����
    try{
        Booklist_init(Dat.BookList,Cfg.BOOKCSV);
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
        Booklist_defaultcsv();
        op("������������");
        restart();
    }
    
    try{
        Userlist_init(Dat.UserList,Cfg.USERCSV);
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
        Userlist_defaultcsv();
        op("������������");
        restart();
    }

    //����չʾ
    cout<<"Log file:"<<Cfg.LOG<<endl;
    cout<<"BookList CSV:"<<Cfg.BOOKCSV<<endl;
    cout<<"UserList CSV:"<<Cfg.USERCSV<<endl;

    pause();

    //�˵���ת
    MenuState menu_state=MAIN;

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
                menu_state=del_user(Dat,Cfg);
                break;

                case EDIT_USER:
                menu_state=edit_user(Dat,Cfg);
                break;

            case REPORT_BOOK:
                menu_state=report_book_menu(Dat,Cfg);
                break;

                case REPORT_DAMAGE:
                menu_state=report_damage(Dat,Cfg);
                break;

                case REPORT_FIX:
                menu_state=report_fix(Dat,Cfg);
                break;

            case SETTING:
                menu_state=setting_menu(Dat,Cfg);
                break;

            default:
                menu_state=EXIT;
                break;
        }
    }

    //�������
    Booklist_save(Dat.BookList,Cfg.BOOKCSV);
    Userlist_save(Dat.UserList,Cfg.USERCSV);

    log(I,"����Ĭ�Ͻ���");
    
    cls();
    pause();
}