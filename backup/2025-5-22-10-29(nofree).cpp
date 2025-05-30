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


//log预声明
#define LOG "log.txt"//存放日志的文件
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

//写入文件
int f_write_app(const string& file_name,const string& text,const string& mode){
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
        
        //展示可被借阅的书籍信息
        int showinfo_iffree(){
            if(stock){
                showinfo();
                return 1;
            }else{return 0;}
        }

        //是否被借阅
        bool iffree(){return stock;}
        bool ifborrowed(){return quantity-stock;}


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
void Booklist_save(vector<Book>& book_list){

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

//读取可被借阅图书内容
void Booklist_info_iffree(vector<Book>& book_list,int &freesum){
    log("尝试读取可被借阅图书数据");
    int i=1;
    for(auto& book:book_list){
        if(book.iffree()){
            o(i,TAB);
            freesum+=book.showinfo_iffree();//自动计算可借阅的总数，因为不可借阅会返回0
            i++;
        }
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

//克隆可被借阅的书单
void Booklist_iffree_clone(vector<Book>& book_list,vector<Book>& book_list_free){
    for(auto& book:book_list){
        if(book.iffree()){
            book_list_free.push_back(book);
        }
    }
}



///各具体功能函数：
//查询功能






///菜单系统：
//预声明link
int link(int level,int c,vector<Book>& book_list);

/*
0:主菜单
    -借书
    -还书
    -图书管理
    -借阅人管理
    -图书报损
    -退出程序
*/

//主菜单
int main_menu(){
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
    return c;
}

//借书菜单
int borrow_menu(vector<Book>& book_list){
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

    int freenum;
    
    vector<Book> book_list_free;
    Booklist_iffree_clone(book_list,book_list_free);
    Booklist_briefinfo(book_list_free);

    freenum=book_list_free.size();
    cout<<"总计"<<freenum<<"本可被借阅的不同书籍。"<<endl;
    /*
    Booklist_info_iffree(book_list,freenum);
    cout<<"总计"<<freenum<<"本可被借阅的不同书籍。"<<endl;
    */
    int bookid_to_choose;
    o("请输入要借阅的书籍序号（输入0返回上一级）");
    i(bookid_to_choose,0,freenum);
    if(bookid_to_choose==0){
        link(0,main_menu(),book_list);
    }else{
        Booklist_element(book_list_free,bookid_to_choose-1).showinfo();
    }
    /*
        Book test("C++","Me","0-0001","GHP","2006-7-27",72.7,1,1);
        test.showinfo();
        cout<<test.savestr()<<endl;

        int a;
        oi(a,"输入测试");
        o(a);
    */
    return 0;
}

//菜单跳转功能
int link(int level,int c,vector<Book>& book_list){

    switch (level){
        case 0://主菜单层级
            switch(c){
                case 1://借书
                    borrow_menu(book_list);
                    break;

                case 6://退出
                    cls();
                    o("感谢使用，退出程序！");
                    log("程序退出结束");
                    exit(0);
                    break;

                default:
                    break;
                }
            break;
        

        
        default:
            break;
    }
    return 0;
}


//初始化函数



int main(){
    log("-----程序启动-----");

    int menu_level=0;//菜单层级
    cls();
    vector<Book> BookList;
    Booklist_init(BookList,"test.csv");
    link(0,main_menu(),BookList);

    log("程序默认结束");
}