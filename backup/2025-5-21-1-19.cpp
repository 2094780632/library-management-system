///Windows cmd下的图书馆管理系统
//智能科学与技术一班 2024413670 管航
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>

#include<vector>
#include<string>
using namespace std;


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
    ifstream file;
    file.open(file_name);
    string content,line;
    //判断是否打开成功
    if(file.is_open()){
        while (getline(file,line)){
            content+=line+'\n';
        }
        file.close();
        return content;
    }else{
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
        getline(file,line);
        file.close();
        return line;
    }else{
        cerr<<"无法打开文件："<<file_name<<endl;
        return "";
    }
}


///图书类的构建
//图书类
class Book{
    public:
        //构造函数
        Book(string t,string a,string i,string pre,string d,double pri,int q,int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

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
                o("");
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
        cerr<<"无法打开文件："<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
            stringstream ss(line);//用于处理csv字符串
            string item[8];//8项数据使用数组存储    
            int i=0;
            while(getline(ss,item[i],',')){i++;}
            book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoi(item[6]),stoi(item[7]));
    }
    file.close();
}

//读取图书内容
void Booklist_info(vector<Book>& book_list){
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.showinfo();
        i++;
    }
}

///各具体功能函数：
//查询功能






///菜单系统：

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
    o(R"(借书
    书籍列表：)");
    //表头
    o("序号",TAB);
    o("书名",TAB);
    o("作者",TAB);
    o("ISBN",TAB);
    o("出版社",TAB);
    o("出版日期",TAB);
    o("价格",TAB);
    o("数量",TAB);
    o("库存",TAB);
    o("");

    Booklist_info(book_list);
    cout<<"总计"<<book_list.size()<<"本可被借阅的书籍"<<endl;
    int cid;
    oi(cid,"请输入要借阅的书籍序号");
    cout<<cid<<endl;
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
    cls();
    switch (level){
        case 0://主菜单层级
            switch(c){
                case 1://借书
                    borrow_menu(book_list);
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
    int menu_level=0;//菜单层级
    cls();
    vector<Book> BookList;
    Booklist_init(BookList,"test.csv");
    link(0,main_menu(),BookList);
}