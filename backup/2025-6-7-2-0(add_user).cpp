///Windows cmd下的图书馆管理系统
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
#define LOG "log.txt"//存放日志的文件
#define BOOKCSV "test.csv"//存放图书数据的文件
#define USERCSV "user.csv"//存放用户数据的文件  
#define TAB 20//TAB制表符长度
#define BRIEFTAB 30//简要信息制表符长度
#define PRICE_MAX 10000//最大单价
*/

///全局数据与设置
//预先声明
class Book;
class User;
int log(const string &text);

typedef struct GlobalSettings{
    string LOG;//存放日志的文件
    string BOOKCSV;//存放图书数据的文件
    string USERCSV;//存放用户数据的文件  
    int TAB = 20;//TAB制表符长度
    int BRIEFTAB = 30;//简要信息制表符长度
    double PRICE_MAX = 10000;//最大单价
}Config;

typedef struct ListData{
    vector<Book> BookList;//书籍列表
    vector<User> UserList;//用户列表
}Data;


///常用通用函数：
//CMD清屏
inline void cls(){system("cls");}

//pause
inline void pause(){system("pause");}

//窗口标题
inline void title(const string& t){
    string cmd="title "+t;
    system(cmd.c_str());
}

//一个自带换行的内联输出函数
inline void o(const string &text){cout<<text<<endl;}
//一个制表符长短可控的内联输出函数
inline void o(const string &text,int t){cout<<setw(t)<<text;}
//一个输出指定小数位数带制表符的内联输出函数
inline void o(const double &d,int a,int t){cout<<fixed<<setprecision(a)<<setw(t)<<d;}
//整型映射到普通输出
inline void o(const int a){o(to_string(a));}
inline void o(const int a,int t){o(to_string(a),t);}
//输出并暂停
inline void op(const string &text){o(text);pause();}

//一个使用模板来适配任意类型的变量的输入合法化检测函数
//通用类型仅检测cin是否成功（用于信息输入）
template<typename T>
void i(T& v){
    while(true){
        if(cin>>v){
            //成功
            break;
        }else{
            //失败
            log("输入合法化检测失败");
            cin.clear();//清除错误状态
            cin.sync();//清空缓存数据
            o("输入不合法，请重新输入:");
            //重新输入
        }
    }
}
//int型附带上下限闭区间（用于菜单选项）
void i(int& v,int min,int max){
    while(true){
        if(cin>>v&&v>=min&&v<=max){
            //成功
            break;
        }else{
            //失败
            log("int型输入合法化检测失败");
            cin.clear();//清除错误状态
            cin.sync();//清空缓存数据
            o("输入不合法，请重新输入:");
            //重新输入
        }
    }
}

//输出提示符并接收输入
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

//确认
bool check(string tip){
    string in;
    cout<<tip<<"(y/n):";
    cin>>in;
    while (in!="y"&&in!="n"){
        oi(in,"请重新输入");
    }
    if(in=="y"){return true;}else if(in=="n"){return false;}
    log("no return");
    return false;
}

// 定义颜色常量
const int BLACK=0;
const int BLUE=1;
const int GREEN=2;
const int CYAN=3;
const int RED=4;
const int MAGENTA=5;
const int YELLOW=6;
const int WHITE=7;
// 定义亮色版本
const int BRIGHT_BLACK=8;
const int BRIGHT_BLUE=9;
const int BRIGHT_GREEN=10;
const int BRIGHT_CYAN=11;
const int BRIGHT_RED=12;
const int BRIGHT_MAGENTA=13;
const int BRIGHT_YELLOW=14;
const int BRIGHT_WHITE=15;
//颜色系统
class ColorGuard {
    private:
        HANDLE hConsole;
        WORD originalAttributes;
    public:
        // 构造函数，支持设置文本颜色和背景颜色
        ColorGuard(int textColor, int bgColor = 0){
            hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo(hConsole,&info);
            originalAttributes=info.wAttributes;

            // 设置文本颜色和背景颜色
            WORD color=(bgColor << 4) | textColor; // 背景颜色左移4位，与文本颜色组合
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

//选项
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
        o(".返回");
        i(c,0,options.size());
    }else{
        i(c,1,options.size());
    }
    return c;
}


///文件系统
//读文件，以string形式返回文件内容
string f_readall(const string& file_name){
    log("尝试读取文件："+file_name);
    ifstream file;
    file.open(file_name);
    string content,line;
    //判断是否打开成功
    if(file.is_open()){
        log("成功读取文件");
        while (getline(file,line)){
            content+=line+'\n';
        }
        file.close();
        return content;
    }else{
        log("无法读取文件"+file_name);
        cerr<<"无法打开文件："<<file_name<<endl;
        return "";
    }
}

//逐行读文件
string f_readline(const string& file_name){
    ifstream file;
    file.open(file_name);
    string line;
    //判断是否打开成功
    if(file.is_open()){
        log("成功逐行读取文件");
        getline(file,line);
        file.close();
        return line;
    }else{
        log("无法逐行读取文件"+file_name);
        cerr<<"无法打开文件："<<file_name<<endl;
        return "";
    }
}

//写入文件(app or trunc)
int f_write(const string& file_name,const string& text,const string& mode){
    ofstream file;
    if(mode=="app"){
        file.open(file_name,ios::app);
    }else if(mode=="trunc"){
        file.open(file_name,ios::trunc);
    }else{
        log("无效写入模式");
        cerr << "无法打开文件: "<<file_name<<endl;
        return 1;
    }

    //判断是否打开成功
    if (!file.is_open()) {
        log("无法写入文件"+file_name);
        cerr << "无法打开文件: "<<file_name<<endl;
        return 1;
    }
    file<<text;
    file.close();

    if(mode=="app"){
        log("成功追加写入文件："+file_name);
    }else if(mode=="trunc"){
        log("成功覆盖写入文件："+file_name);
    }

    return 0;
}

//日志记录
int log(const string &text){
    string LOG="log.txt";
    //获取时间
    auto now=chrono::system_clock::now();
    time_t now_c=chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss<<put_time(localtime(&now_c),"%Y-%m-%d %H:%M:%S");
    string log_time;
    log_time=ss.str();
    //写入记录
    //f_write(LOG,log_time+":"+text+'\n');不可以这样写！！因为f_write中也有log的调用，会导致这两个函数互相调用陷入死循环
    ofstream log;
    log.open(LOG,ios::app);
    //判断是否打开成功
    if (!log.is_open()) {
        cerr << "无法写入log记录"<<endl;
        return 1;
    }

    string log_str=log_time+":"+text+'\n';
    log<<log_str;
    log.close();
    return 0;
}


///图书数据的构建
//图书类
class Book{
    public:
        //构造函数
        Book(){}
        Book(string t,string a,string i,string pre,string d,double pri,unsigned int q,unsigned int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

        //获取单个属性
        string TITLE() const{return title;}
        string AUTHOR() const{return author;}
        string ISBN() const{return isbn;}
        string PRESS() const{return press;}
        string DATE() const{return date;}
        double PRICE() const{return price;}
        unsigned int QUANTITY() const{return quantity;}
        unsigned int STOCK() const{return stock;}

        //修改单个属性
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

        //展示信息
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

        //简要信息展示
        void briefinfo(int &BRIEFTAB){
            o(title,BRIEFTAB);
            //o(author,TAB);
            o(to_string(stock),BRIEFTAB);
            o("");
        }
        
        //是否被借阅
        bool isfree(){return stock;}
        bool isborrowed(){return quantity-stock;}

        //借出，返回借出后剩余数量，如果不够则返回-1表示错误
        int borrow(int i){
            if(stock>=i){
                stock-=i;
                return stock-i;
            }else{
                return -1;
            }
        }

        //还书
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

        //整合数据
        string savestr(){
            ostringstream ss;
            ss<<fixed<<setprecision(2);
            ss<<title<<','<<author<<','<<isbn<<','<<press<<','<<date<<','<<price<<','<<quantity<<','<<stock;
            return ss.str();
            
            //return title+','+author+','+isbn+','+press+','+date+','+to_string(price)+','+to_string(quantity)+','+to_string(stock);
        }

    private:
        string title,author,isbn,press,date;//字符串类型：书名、编号、作者、出版社、出版日期
        double price;//浮点类型：价格
        unsigned int quantity,stock;//整型：数量、库存
};

//初始化图书数据
void Booklist_init(vector<Book>& book_list,string csv){
    ifstream file(csv);
    if(!file.is_open()){
        log("初始化图书数据时无法打开csv文件");
        cerr<<"无法打开文件："<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
        stringstream ss(line);//用于处理csv字符串
        string item[8];//9项数据使用数组存储    
        int i=0;
        while(getline(ss,item[i],',')){i++;}
        book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoul(item[6]),stoul(item[7]));
    }
    log("成功初始化图书数据");
    file.close();
}

//保存图书数据
void Booklist_save(vector<Book>& book_list,string csv){
    log("保存图书数据");
    string save="";
    for(auto& book:book_list){
        save+=book.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
    //log("Booklist save:\n"+save);
}

//读取图书内容
void Booklist_info(vector<Book>& book_list,bool isid,int &TAB){
    log("尝试读取图书数据");
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

//读取简要内容
void Booklist_briefinfo(vector<Book>& book_list,bool isid,int &BRIEFTAB){
    log("尝试读取图书数据");
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

//输出一行图书的表头
void Booklist_infoheading(int &TAB,bool isid=false){
    CG C(YELLOW);
    if(isid){o("序号",TAB);}
    o("书名",TAB);
    o("作者",TAB);
    o("ISBN",TAB);
    o("出版社",TAB);
    o("出版日期",TAB);
    o("价格",TAB);
    o("数量",TAB);
    o("库存",TAB);
    o("");
}

//输出一行简要的表头
void Booklist_briefheading(bool isid,int &BRIEFTAB){
    CG C(YELLOW);
    if(isid){o("序号",BRIEFTAB);}
    o("书名",BRIEFTAB);
    //o("作者",TAB);
    o("可借数量",BRIEFTAB);
    o("");
}

//调用指定索引的图书对象
Book& Booklist_element(vector<Book>& book_list,int index){
    log("调用图书对象,index:"+to_string(index));
    if (index < 0 || index >= book_list.size()) {
        log("在调用图书对象时索引超出范围,index:"+to_string(index));
        throw out_of_range("索引超出范围");
    }
    return book_list[index];
}

//线性查找ISBN图书对象
Book& Booklist_isbn(vector<Book>& book_list,string isbn){
    log("调用指定ISBN书籍对象");
    auto it=find_if(book_list.begin(),book_list.end(),[&isbn](const Book& book){
        return book.ISBN()==isbn;
    });
    if (it == book_list.end()) {
        log("ISBN图书对象访问失败");
        throw runtime_error("无法找到对应书籍！");
    }
    return *it;
}

//查询书本是否存在(ISBN)
bool book_exist(vector<Book>& book_list,string isbn){
    for(auto& book:book_list){
        if(book.ISBN()==isbn){return true;}
    }
    return false;
}

//删除指定ISBN图书
void Booklist_delbook(vector<Book>& book_list,string isbn){
    book_list.erase(remove_if(book_list.begin(),book_list.end(),[&isbn](const Book& book) {return book.ISBN()==isbn;}),book_list.end());
}

//按照指定方式返回符合条件的书本对象
vector<Book> Booklist_search(vector<Book>& book_list,string key,string (Book::*pfunc)() const){
    vector<Book> target_list;
    for(auto& book:book_list){
        if((book.*pfunc)()==key){target_list.push_back(book);}
    }
    //log("cash");
    if(target_list.empty()){
        log("无法找到指定书目,key:"+key);
        throw runtime_error("无法找到指定书目");
    }
    return target_list;
    //log("done");
}

/*弃用方法 
//克隆可被借阅的书单
void Booklist_iffree_clone(vector<Book>& book_list,vector<Book>& book_list_free){
    for(auto& book:book_list){
        if(book.iffree()){
            book_list_free.push_back(book);
        }
    }
}

//读取可被借阅图书内容
void Booklist_info_isfree(vector<Book>& book_list,int &freesum){
    log("尝试读取可被借阅图书数据");
    int i=1;
    for(auto& book:book_list){
        if(book.isfree()){
            o(i,TAB);
            freesum+=book.showinfo_iffree();//自动计算可借阅的总数，因为不可借阅会返回0
            i++;
        }
    }
}
*/


///借阅人系统构建

//vector[]转string
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

//用户类
class User{
    public:
        //构造函数（无任何借阅书籍）
        User(){}
        User(string n,string i,string s="N",vector<string> l={}):name(n),id(i),status(s),borrowed_list(l){};

        //获取单个属性
        string NAME() const{return name;}
        string ID() const{return id;}
        string STATUS() const{return status;}
        vector<string> BORROWED_LIST() const{return borrowed_list;}

        //信息展示
        void showinfo(int &TAB){
            if(status!="R"){
                o(name,TAB);
                o(id,TAB);
                string strstatus;
                if(status=="A"){strstatus="管理员用户";}
                if(status=="N"){strstatus="普通用户";}
                if(status=="B"){strstatus="黑名单用户";}
                o(strstatus,TAB);
                //o(vetostr(borrowed_list),TAB);
                o(borrowed_list.size(),TAB);
                o("");
            }
        }

        //借书
        void borrow(string ISBN){
            borrowed_list.push_back(ISBN);
        }

        //还书
        void returnBook(string ISBN){
            borrowed_list.erase(remove(borrowed_list.begin(),borrowed_list.end(),ISBN),borrowed_list.end());
        }
        
        //检查ISBN是否已存在
        bool is_borrowed(string ISBN){
            for(auto& i:borrowed_list){
                if(i==ISBN){return true;}
            }
            return false;
        }

        //整合数据
        string savestr(){
            return name+','+id+','+status+','+vetostr(borrowed_list);
        }

    private:
        string name,id,status;//用户姓名、用户编号、用户状态(N正常、B黑名单)
        vector<string> borrowed_list;//用户借阅的书单(ISBN)
};

//拆解列表
vector<string> apartlist(string data){
    //log("apartlist:"+data);debug
    vector<string> list={};
    if(data=="[]"){return list;}
    data.erase(0,1);//删头
    data.pop_back();//去尾

    stringstream ss(data);
    string temp;
    while(getline(ss,temp,',')){
        list.push_back(temp);
    }
    return list;
}

//初始化用户数据
void Userlist_init(vector<User>& user_list,string csv){
    ifstream file(csv);
    if(!file.is_open()){
        log("初始化用户数据时无法打开csv文件");
        cerr<<"无法打开文件："<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
        stringstream ss(line);//用于处理csv字符串
        vector<string> items;//4项数据使用数组存储
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
    log("成功初始化用户数据");
    file.close();
}

//读取用户数据
void Userlist_info(vector<User>& user_list,int &TAB){
    log("尝试读取用户数据");
    //int i=1;
    for(auto& user:user_list){
        //o(i,TAB);
        user.showinfo(TAB);
        //i++;
    }
}

void Userlist_infoheading(int &TAB){
    CG C(YELLOW);
    o("用户名",TAB);
    o("ID",TAB);
    o("状态",TAB);
    o("借阅总数",TAB);
    o("");
}

//保存用户数据
void Userlist_save(vector<User>& user_list,string csv){
    log("保存用户数据");
    string save="";
    for(auto& user:user_list){
        save+=user.savestr()+'\n';
    }
    f_write(csv,save,"trunc");
   //log("Userlist save:\n"+save);
}

//查询用户是否存在
bool user_exist(vector<User>& user_list,string id){
    for(auto& user:user_list){
        if(user.ID()==id){return true;}
    }
    return false;
}

//检查是否有多个用户借过同一本书
int Userlist_Borrowsamebook(vector<User>& user_list,string isbn){
    int sum=0;
    for(auto& user:user_list){
        if(user.is_borrowed(isbn)){sum++;}
    }
    return sum;
}

//返回指定用户对象（用户名）
User& Userlist_element(vector<User>& user_list,string name){
    log("调用用户对象："+name);
    for(auto& user:user_list){
        if(user.NAME()==name){return user;}
    }
    log("调用用户对象失败(NAME)");
    throw "调用用户对象失败(NAME)";
}

//借过某书的用户（第一个）
User& Userlist_borrowed_isbn(vector<User>& user_list,string isbn){
    log("调用用户对象(ISBN) ："+isbn);
    for(auto& user:user_list){
        if(user.is_borrowed(isbn)){
            return user;
        }
    }
    log("调用用户对象失败(ISBN)");
    throw "调用用户对象失败(ISBN)";
}

//按照指定方式返回符合条件的用户对象
vector<User> Userlist_search(vector<User>& user_list,string key,string (User::*pfunc)() const){
    vector<User> target_list;
    for(auto& user:user_list){
        if((user.*pfunc)()==key){target_list.push_back(user);}
    }
    //log("cash");
    if(target_list.empty()){
        log("无法找到指定用户,key:"+key);
        throw runtime_error("无法找到指定用户");
    }
    return target_list;
    //log("done");
}

//用户登录
bool login(vector<User>& user_list,string &puser){
    log("login:尝试登录");
    string username;
    //o("登录");

    CT("登录");

    oi(username,"请输入借阅人的名字");

    string (User::*pfunc)() const=&User::NAME;
    
    if(Userlist_search(user_list,username,pfunc).size()==1){
        log("login:登录成功");
        //op("登录成功！");
        puser=username;
        return true;
    }else{
        log("login:登录失败");
        op("借阅人不存在！请重新尝试");
        puser="";
        return false;
    }
}


///菜单系统：
/*
主菜单
    -借书
    -还书
    -查书//查询指定条件的图书
        -按书名查找
        -按作者查找
        -按借阅人查找
        -按ISBN查找
    -图书管理
        -查询图书信息//列出所有图书
        -添加新的图书
        -删除已有图书
        -修改已有图书
    -借阅人管理
        -查询借阅人信息
        -添加新的借阅人
        -删除已有借阅人
        -修改已有借阅人
    -图书报损
    -退出程序
*/

//菜单状态机
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

//主菜单
MenuState main_menu(){
    log("进入主菜单");
    title("图书馆管理系统");
    cls();
    //menu_level=0;

    vector<string> ops={"借书","还书","查书","图书管理","借阅人管理","图书报损","退出程序"};
    CT("主菜单");

    int c=option(ops,false);;//存储用户选择的整型变量
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
            log("主菜单默认退出");
            return MAIN;
            break;
    }
}

//借书菜单
MenuState borrow_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    int &BRIEFTAB=Cfg.BRIEFTAB;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log("进入借书菜单");
    title("借书");
    cls();
    string username;
    if(!login(user_list,username)){
        return MAIN;
    }

    //黑名单用户不准借书
    if(Userlist_element(user_list,username).STATUS()=="B"){
        op("该用户已被列入黑名单，无法借书！");
        return MAIN;
    }

    cls();
    CT("借书");


    Booklist_briefheading(true,BRIEFTAB);

    int freenum=0;
    
    //Booklist_info_iffree(book_list,freenum);

    //创建可被借阅的书的索引
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
        o("没有可以借阅的书籍");
        pause();
        return MAIN;
    }

    cout<<"总计"<<freenum<<"本可被借阅的不同书籍。"<<endl;
    
    //vector<Book> book_list_free;

    int bookid_to_choose;
    o("请输入要借阅的书籍序号（输入0返回上一级）");
    i(bookid_to_choose,0,freenum);
    if(bookid_to_choose==0){
        return MAIN;
    }else{
        size_t index=free_index[bookid_to_choose-1];//实际索引
        
        //是否已经借过
        if(Userlist_element(user_list,username).is_borrowed(book_list[index].ISBN())){
            op("你已经借过这本书了！");
            return MAIN;
        }

        //保险条件
        if(book_list[index].STOCK()>0){
            log("书籍"+book_list[index].TITLE()+"由"+Userlist_element(user_list,username).NAME()+"借出1本");
            book_list[index].borrow(1);
            Userlist_element(user_list,username).borrow(book_list[index].ISBN());
            Booklist_save(book_list,BOOKCSV);
            Userlist_save(user_list,USERCSV);
            op("书籍《"+book_list[index].TITLE()+"》借阅成功！");
            return MAIN;
        }
        //Booklist_element(book_list,bookid_to_choose-1).showinfo();
        //book_list[bookid_to_choose-1].showinfo();
    }
    return MAIN;
}

//还书菜单
MenuState return_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    vector<User> &user_list=Dat.UserList;
    string &BOOKCSV=Cfg.BOOKCSV;
    string &USERCSV=Cfg.USERCSV;

    log("进入还书菜单");
    title("还书");
    cls();
    //Book->是否被借,User->是否唯一
    CT("还书");
    string returnbookisbn;
    oi(returnbookisbn,"请输入要归还的书籍的ISBN");

    //是否存在
    if(!book_exist(book_list,returnbookisbn)){
        log("尝试归还一本不存在的书，退出还书菜单");
        op("这本书不存在！");
        return MAIN;
    }

    if(!Booklist_isbn(book_list,returnbookisbn).isborrowed()){
        log("尝试归还一本没有被借过的书，退出还书菜单");
        op("这本书没有被借过！");
        return MAIN;
    }
    switch(Userlist_Borrowsamebook(user_list,returnbookisbn)){
    case 0:
        log("尝试归还一本没有人借过的书，退出还书菜单");
        op("这本书没有人借过！");
        break;
    
    case 1:
        log("尝试归还只有一个人借过的书");

        Booklist_isbn(book_list,returnbookisbn).returnbook();
        Booklist_save(book_list,BOOKCSV);
        Userlist_borrowed_isbn(user_list,returnbookisbn).returnBook(returnbookisbn);
        Userlist_save(user_list,USERCSV);

        log("归还成功！");
        op("归还成功！");
        return MAIN;
        break;
    default:
        log("尝试归还有多个人借过的书");
        cls();
        string username;
        if(!login(user_list,username)){
            return MAIN;
        }else{
            Booklist_isbn(book_list,returnbookisbn).returnbook();
            Booklist_save(book_list,BOOKCSV);
            Userlist_element(user_list,username).returnBook(returnbookisbn);
            Userlist_save(user_list,USERCSV);

            log("归还成功！");
            op("归还成功！");
            return MAIN;
        }

        break;
    }


    return MAIN;
}

//查书菜单
MenuState search_menu(){
    log("进入查书菜单");
    title("查书");
    cls();

    vector<string> ops={"按书名查找","按作者查找","按借阅人查找","按ISBN查找"};
    CT("查书");

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
    
    log("按书名查书");
    string searchbookname;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::TITLE;

    while(retry&&searchbookname!="0"){
        cls();
        CT("查书\\按书名查找");
        oi(searchbookname,"请输入要查询的书籍名称(输入0返回上一级)");
        if(searchbookname=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookname,pfunc);
            log("按书名查书成功");
            retry=false;
        }
        catch(const exception& e){
            log("按书名查书失败");
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

    log("按作者查书");
    string searchbookauthor;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::AUTHOR;

    while(retry&&searchbookauthor!="0"){
        cls();
        CT("查书\\按作者查找");
        oi(searchbookauthor,"请输入要查询书籍的作者名称(输入0返回上一级)");
        if(searchbookauthor=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookauthor,pfunc);
            log("按作者查书成功");
            retry=false;
        }
        catch(const exception& e){
            log("按作者查书失败");
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

    log("按借阅人查书");
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
    CT("查书\\按借阅人查找");
    if(vetostr(target_string)=="[]"){
        o("无借阅！");
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

    log("按ISBN查书");
    string searchbookisbn;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::ISBN;

    while(retry&&searchbookisbn!="0"){
        cls();
        CT("查书\\按ISBN查找");
        oi(searchbookisbn,"请输入要查询书籍的ISBN(输入0返回上一级)");
        if(searchbookisbn=="0"){return SEARCH_BOOK;}
        try{
            target=Booklist_search(book_list,searchbookisbn,pfunc);
            log("按ISBN查书成功");
            retry=false;
        }
        catch(const exception& e){
            log("按ISBN查书失败");
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

//图书管理菜单
MenuState manage_book_menu(){
    log("进入图书管理菜单");
    title("图书管理");
    cls();

    vector<string> ops={"查询所有图书信息","添加新的图书信息","删除已有图书信息","编辑指定图书信息"};
    CT("图书管理");
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

    log("查询所有图书信息");
    CT("图书管理\\查询所有图书信息");
    Booklist_infoheading(TAB);
    Booklist_info(book_list,false,TAB);
    pause();
    return MANAGE_BOOK;
}

MenuState add_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;

    log("添加新的图书");
    string new_title,new_author,new_isbn,new_press,new_date;
    double new_price;
    int new_quantity,new_stock;

    CT("图书管理\\添加新的图书");
    oi(new_title,"书名");
    oi(new_author,"作者");

    oi(new_isbn,"ISBN");
    while(new_isbn=="0"){
        oi(new_isbn,"ISBN不能是0！请重新输入");
    }
    while(book_exist(book_list,new_isbn)){
        oi(new_isbn,"该ISBN已存在！请重新输入");
    }

    oi(new_press,"出版社");
    oi(new_date,"出版日期");

    oi(new_price,"单价");
    while(!(new_price>0)){
        oi(new_price,"单价必须大于0！请重新输入");
    }


    oi(new_quantity,"总数");
    while(!(new_quantity>0)){
        oi(new_quantity,"总数必须大于0！请重新输入");
    }

    oi(new_stock,"库存");
    while(new_stock>new_quantity||new_stock<0){
        oi(new_stock,"库存输入错误！请重新输入");
    }
    
    
    Book new_book(new_title,new_author,new_isbn,new_press,new_date,new_price,new_quantity,new_stock);
    
    Booklist_infoheading(TAB);
    new_book.showinfo(TAB);
    if(check("请确认新的图书信息")){
        book_list.push_back(new_book);
        Booklist_save(book_list,BOOKCSV);
        log("确认添加新的图书信息");

        o("添加成功！");
        pause();
        return MANAGE_BOOK;
    }else{
        log("取消添加新的图书信息");
        return MANAGE_BOOK;
    }

    //pause();
    return MANAGE_BOOK;
}

MenuState del_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book> &book_list=Dat.BookList;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    
    log("删除已有图书");
    string del_isbn;
    Book target;
    bool retry=true;

    while(retry&&del_isbn!="0"){
        cls();
        CT("图书管理\\删除已有图书信息");
        oi(del_isbn,"请输入要删除书籍的ISBN(输入0返回上一级)");
        if(del_isbn=="0"){return MANAGE_BOOK;}
        try{
            target=Booklist_isbn(book_list,del_isbn);
            log("按ISBN寻书成功");
            retry=false;
        }
        catch(const exception& e){
            log("按ISBN寻书失败");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    Booklist_infoheading(TAB);
    target.showinfo(TAB);
    if(check("请确认要删除的图书信息")){
        Booklist_delbook(book_list,del_isbn);
        Booklist_save(book_list,BOOKCSV);
        log("确认删除图书信息，ISBN："+del_isbn);
        o("删除成功！");
        pause();
        return MANAGE_BOOK;
    }else{
        log("取消删除图书信息");
        return MANAGE_BOOK;
    }


    return MANAGE_BOOK;
}

MenuState edit_book(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<Book>& book_list=Dat.BookList;
    double &PRICE_MAX=Cfg.PRICE_MAX;
    string &BOOKCSV=Cfg.BOOKCSV;
    int &TAB=Cfg.TAB;
    log("编辑指定图书");
    string edit_isbn;
    bool retry=true;

    while(retry&&edit_isbn!="0"){
        cls();
        CT("图书管理\\编辑指定图书信息");
        oi(edit_isbn,"请输入要编辑书籍的ISBN(输入0返回上一级)");
        if(edit_isbn=="0"){return MANAGE_BOOK;}
        try{
            Book& target=Booklist_isbn(book_list,edit_isbn);
            log("按ISBN寻书成功");
            Booklist_infoheading(TAB);
            target.showinfo(TAB);
            retry=false;
            o("请输入需要更改的项目"); 
            vector<string> ops={"书名","作者","ISBN","出版社","出版日期","单价","总数","库存"};
            int c=option(ops,false);

            if(c==0){return MANAGE_BOOK;}

            string input;
            oi(input,"请输入修改后的内容");

            switch(c){
            case 1:
                target.change_title(input);
                log("修改书名："+input);
                break;
            case 2:
                target.change_author(input);
                log("修改作者："+input);
                break;
            case 3:
                while(input=="0"){
                    oi(input,"ISBN不能是0！请重新输入");
                }
                while(book_exist(book_list,input)){
                    oi(input,"该ISBN已存在！请重新输入");
                }
                target.change_isbn(input);
                log("修改ISBN："+input);
                break;
            case 4:
                target.change_press(input);
                log("修改出版社："+input);
                break;
            case 5:
                target.change_date(input);
                log("修改出版日期："+input);
                break;
            case 6:
                retry=true;
                while(retry){
                    try{
                        double cast_double=stod(input);
                        if(cast_double>0&&cast_double<=PRICE_MAX){
                            target.change_price(cast_double);
                            log("修改单价："+input);
                            retry=false;
                        }else if(cast_double<0){
                            cerr<<"单价必须大于0！"<<endl;
                            oi(input,"请重新输入"); 
                        }else if(cast_double>PRICE_MAX){
                            cerr<<"单价太大了！"<<endl;
                            oi(input,"请重新输入");
                        }else{
                            cerr<<"单价无效！"<<endl;
                            oi(input,"请重新输入");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"输入的单价无效，请输入一个有效的数字！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const out_of_range& e){
                        cerr<<"输入的单价超出了范围！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"请重新输入");
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
                            log("修改总数："+input);
                            retry=false;
                        }else{
                            cerr<<"总数必须大于0且大于等于当前库存！"<<endl;
                            oi(input,"请重新输入");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"输入的总数无效，请输入一个有效的数字！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const out_of_range& e){
                        cerr<<"输入的总数超出了范围！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"请重新输入");
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
                            log("修改总数："+input);
                            retry=false;
                        }else{
                            cerr<<"库存必须大于0且小于等于总数！"<<endl;
                            oi(input,"请重新输入");
                        }
                    }
                    catch(const invalid_argument& e){
                        cerr<<"输入的总数无效，请输入一个有效的数字！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const out_of_range& e){
                        cerr<<"输入的总数超出了范围！"<<endl;
                        oi(input,"请重新输入");
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        oi(input,"请重新输入");
                    }
                }
                break;
            default:
                break;
            }
            
            Booklist_save(book_list,BOOKCSV);
            op("修改成功！");
        }
        catch(const exception& e){
            log("按ISBN寻书失败");
            cerr<<e.what()<<'\n';
            pause();
        }
    }

    return MANAGE_BOOK;
}

//用户管理菜单
MenuState manage_user_menu(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;

    log("进入用户管理菜单");
    title("用户管理");
    cls();

    User user;
    string username;

    vector<string> ops;
    
    if(!login(user_list,username)){
        return MAIN;
    }else{
        user=Userlist_element(user_list,username);
    }

    //黑名单用户
    if(user.STATUS()=="B"){
        op("该用户已被列入黑名单！");
        return MAIN;
    }else if(user.STATUS()=="N"){
        cls();
        CT("借阅人管理："+username);
        ops={"查询当前借阅人信息"};
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
        CT("借阅人管理："+username);
        ops={"查询所有借阅人信息","添加新的借阅人信息","删除已有借阅人信息","修改已有借阅人信息"};
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
        op("未知错误");
        return MAIN;
    }

    return MAIN;
}

MenuState query_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    int &TAB=Cfg.TAB;
    log("查询所有用户信息");
    
    cls();
    CT("借阅人管理\\查询所有借阅人信息");

    Userlist_infoheading(TAB);
    Userlist_info(user_list,TAB);

    pause();
    return MAIN;
}

MenuState add_user(struct ListData &Dat,struct GlobalSettings &Cfg){
    vector<User> &user_list=Dat.UserList;
    string &USERCSV=Cfg.USERCSV;
    int &TAB=Cfg.TAB;

    log("添加新的用户信息");
    cls();
    CT("借阅人管理\\添加新的借阅人信息");

    string new_name,new_id,new_status;
    oi(new_name,"名字");
    while(new_name=="0"){
        oi(new_name,"名字不能是0！请重新输入");
    }

    bool retry=true;
    while(retry){
        try{
            if(Userlist_search(user_list,new_name,User::NAME).size()!=0){
                oi(new_name,"该名字已存在！请重新输入");
            }
        }
        catch(const std::exception& e){
            retry=false;
        }
    }
    
    oi(new_id,"ID");
    while(new_id=="0"){
        oi(new_id,"ID不能是0！请重新输入");
    }
    while(user_exist(user_list,new_id)){
        oi(new_id,"该ID已存在！请重新输入");
    }

    o("用户类型");
    vector<string> ops={"管理员","普通用户","黑名单用户"};
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
    //oi(new_status,"用户类型(A:管理员、N:普通用户、B:黑名单用户)");
    while(new_status!="A"&&new_status!="N"&&new_status!="B"){
        oi(new_status,"无效的用户类型！请重新输入");
    }
    
    vector<string> new_borrowlist={};
    User new_user(new_name,new_id,new_status,new_borrowlist);

    Userlist_infoheading(TAB);
    new_user.showinfo(TAB);

    if(check("请确认新的用户信息")){
        user_list.push_back(new_user);
        Userlist_save(user_list,USERCSV);
        log("确认添加新的用户信息");

        o("添加成功！");
        pause();
        return MAIN;
    }else{
        log("取消添加新的用户信息");
        return MAIN;
    }
    //pause();
    return MAIN;
}

MenuState del_user(struct ListData &Dat){
    log("删除已有用户信息");
    return MAIN;
}

MenuState edit_user(struct ListData &Dat){
    log("修改指定用户信息");
    return MAIN;
}

//图书报损菜单
MenuState report_book_menu(){
    log("进入图书报损菜单");
    title("图书报损");
    return MAIN;
}


int main(){
    log("-----程序启动-----");
    title("程序初始化");

    //默认设置
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

    log("程序默认结束");
    
    cls();
    pause();
}