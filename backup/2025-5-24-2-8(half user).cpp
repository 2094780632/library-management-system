///Windows cmd下的图书馆管理系统
//智能科学与技术一班 2024413670 管航
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>

#include<chrono>
#include<ctime>
#include<vector>
#include<string>
using namespace std;

#define LOG "log.txt"//存放日志的文件
#define BOOKCSV "test.csv"//存放图书数据的文件
#define USERCSV "user.csv"//存放用户数据的文件  


//log预声明

int log(const string &text);


///常用通用函数：
//一个自带换行的内联输出函数
inline void o(const string &text){cout<<text<<endl;}
//一个制表符长短可控的内联输出函数
#define TAB 10
inline void o(const string &text,int t){cout<<setw(TAB)<<text;}
//一个输出指定小数位数带制表符的内联输出函数
inline void o(const double &d,int a,int t){cout<<fixed<<setprecision(a)<<setw(TAB)<<d;}
//整型映射到普通输出
inline void o(const int a){o(to_string(a));}
inline void o(const int a,int t){o(to_string(a),TAB);}

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
template<typename T>
void oi(T& i,string tip){
    cout<<tip<<':';
    cin>>i;
}

//CMD清屏
inline void cls(){system("cls");}

//pause
inline void pause(){system("pause");}

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
        Book(string t,string a,string i,string pre,string d,double pri,int q,int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

        //获取单个属性
        string TITLE(){return title;}
        string AUTHOR(){return author;}
        string ISBN(){return isbn;}
        string PRESS(){return press;}
        string DATE(){return date;}
        double PRICE(){return price;}
        int QUANTITY(){return quantity;}
        int STOCK(){return stock;}

        //展示信息
        void showinfo(){
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
        void briefinfo(){
            o(title,TAB);
            o(author,TAB);
            o(to_string(stock),TAB);
            o("");
        }
        
        //是否被借阅
        bool isfree(){return stock;}
        bool isborrowed(){return quantity-stock;}

        //借出，返回借出后剩余数量，如果不够则返回-1表示错误
        int borrow(int i){
            if(stock-i>=0){
                stock-=i;
                return stock-i;
            }else{
                return -1;
            }
        }

        void debug_showinfo(){
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
            return title+','+author+','+isbn+','+press+','+date+','+to_string(price)+','+to_string(quantity)+','+to_string(stock);
        }

    private:
        string title,author,isbn,press,date;//字符串类型：书名、编号、作者、出版社、出版日期
        double price;//浮点类型：价格
        int quantity,stock;//整型：数量、库存
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
        book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoi(item[6]),stoi(item[7]));
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
    log("save:\n"+save);
}

//读取图书内容
void Booklist_info(vector<Book>& book_list){
    log("尝试读取图书数据");
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.showinfo();
        i++;
    }
}

//读取简要内容
void Booklist_briefinfo(vector<Book>& book_list){
    log("尝试读取图书数据");
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.briefinfo();
        i++;
    }
}

//调用指定索引的图书对象
Book& Booklist_element(vector<Book>& book_list,int index){
    if (index < 0 || index >= book_list.size()) {
        log("在调用图书对象时索引超出范围,index:"+to_string(index));
        throw out_of_range("索引超出范围");
    }
    log("调用图书对象,index:"+to_string(index));
    return book_list[index];
}

//线性查找ISBN图书对象
Book& Booklist_linear_isbn(vector<Book>& book_list,string isbn){
    return book_list[0];
}

/*弃用方法 h
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

//vector转string
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
        User(string n,string i,string s="N",vector<string> l={}):name(n),id(i),status(s),borrowed_list(l){};

        //获取单个属性
        string NAME(){return name;}
        string ID(){return id;}
        string STATUS(){return status;}
        vector<string> BORROWED_LIST(){return borrowed_list;}

        //信息展示
        void showinfo(){
            o(name,TAB);
            o(id,TAB);
            o(status,TAB);
            o(vetostr(borrowed_list),TAB);
            o("");
        }

    private:
        string name,id,status;//用户姓名、用户编号、用户状态(N正常、B黑名单)
        vector<string> borrowed_list;//用户借阅的书单
};

//拆解列表
vector<string> apartlist(string data){
    log("apartlist:"+data);
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
        log(line);
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

void Userlist_info(vector<User>& user_list){
    log("尝试读取用户数据");
    int i=1;
    for(auto& user:user_list){
        o(i,TAB);
        user.showinfo();
        i++;
    }
}


///菜单系统：
/*
主菜单
    -借书
    -还书
    -查书
        -按书名查找
        -按作者查找
        -按借阅人查找
        -按ISBN查找
    -图书管理
        -添加新的图书
        -删除已有图书
        -修改已有图书
    -借阅人管理
        -添加新的借阅人
        -删除已有借阅人
        -修改已有借阅人
    -图书报损
    -退出程序
*/

//菜单状态机！
enum MenuState{
    MAIN,
        BORROW_BOOK,
        RETURN_BOOK,
        SEARCH_BOOK,
            SEARCH_BOOK_BY_NAME,
            SEARCH_BOOK_BY_AUTHOR,
            SEARCH_BOOK_BY_USER,
            SEARCH_BOOK_BY_ISBN,
        MANAGE_BOOK,
            ADD_BOOK,
            DEL_BOOK,
            EDIT_BOOK,
        MANAGE_USER,
            ADD_USER,
            DEL_USER,
            EDIT_USER,
        REPORT_DAMAGE,
    EXIT
};

//主菜单
MenuState main_menu(){
    log("进入主菜单");
    cls();
    //menu_level=0;
    o(R"(主菜单
    1.借书
    2.还书
    3.图书管理
    4.借阅人管理
    5.图书报损
    6.退出程序
    )");
    int c;//存储用户选择的整型变量
    i(c,1,6);
    switch(c){
        case 1:
            return BORROW_BOOK;
            break;
        case 2:
            return RETURN_BOOK;
            break;
        case 3:
            return MANAGE_BOOK;
            break;
        case 4:
            return MANAGE_USER;
            break;
        case 5:
            return REPORT_DAMAGE;
            break;
        case 6:
            return EXIT;
            break;
        default:
            log("主菜单默认退出");
            return MAIN;
            break;
    }
}

//借书菜单
MenuState borrow_menu(vector<Book>& book_list){
    log("进入借书菜单");
    cls();
    o(R"(借书
    书籍列表：)");
    //表头
    o("序号",TAB);
    o("书名",TAB);
    o("作者",TAB);
    o("可借数量",TAB);
    /*
    o("ISBN",TAB);
    o("出版社",TAB);
    o("出版日期",TAB);
    o("价格",TAB);
    o("数量",TAB);
    o("库存",TAB);
    */
    o("");

    int freenum=0;
    
    //Booklist_info_iffree(book_list,freenum);

    //创建可被借阅的书的索引
    vector<int> free_index;
    for(size_t i=0;i<book_list.size();++i){
        if(book_list[i].isfree()){
            //序号
            o(to_string(freenum+1),TAB);
            book_list[i].briefinfo();
            free_index.push_back(i);
            freenum++;
        }
    }

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
        //保险条件
        if(book_list[index].STOCK()>0){
            book_list[index].borrow(1);
            log("书籍"+book_list[index].TITLE()+"借出1本");
            o("书籍《"+book_list[index].TITLE()+"》借阅成功！");
            Booklist_save(book_list,BOOKCSV);
            pause();
            return MAIN;
        }
        //Booklist_element(book_list,bookid_to_choose-1).showinfo();
        //book_list[bookid_to_choose-1].showinfo();
    }
    return MAIN;
}

//还书菜单
MenuState return_menu(vector<Book>& book_list){
    return MAIN;
}


int main(){
    log("-----程序启动-----");

    vector<Book> BookList;
    Booklist_init(BookList,BOOKCSV);

    vector<User> UserList;
    Userlist_init(UserList,USERCSV);
    Userlist_info(UserList);

    pause();

    MenuState menu_state= MAIN;

    while(menu_state!=EXIT){
        cls();
        switch(menu_state){
            case MAIN:
                menu_state=main_menu();
                break;
            case BORROW_BOOK:
                menu_state=borrow_menu(BookList);
                break;
            case RETURN_BOOK:
                menu_state=return_menu(BookList);
            default:
                menu_state=EXIT;
                break;
        }
    }
    log("程序默认结束");
}