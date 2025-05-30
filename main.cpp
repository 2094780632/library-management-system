///Windows cmd�µ�ͼ��ݹ���ϵͳ
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include <algorithm>

#include<chrono>
#include<ctime>
#include<vector>
#include<string>
using namespace std;

#define LOG "log.txt"//�����־���ļ�
#define BOOKCSV "test.csv"//���ͼ�����ݵ��ļ�
#define USERCSV "user.csv"//����û����ݵ��ļ�  
#define TAB 15//TAB�Ʊ������
#define BRIEFTAB 30//��Ҫ��Ϣ�Ʊ������

//logԤ����
int log(const string &text);


///����ͨ�ú�����
//CMD����
inline void cls(){system("cls");}

//pause
inline void pause(){system("pause");}

//һ���Դ����е������������
inline void o(const string &text){cout<<text<<endl;}
//һ���Ʊ�����̿ɿص������������
inline void o(const string &text,int t){cout<<setw(t)<<text;}
//һ�����ָ��С��λ�����Ʊ���������������
inline void o(const double &d,int a,int t){cout<<fixed<<setprecision(a)<<setw(t)<<d;}
//����ӳ�䵽��ͨ���
inline void o(const int a){o(to_string(a));}
inline void o(const int a,int t){o(to_string(a),TAB);}
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
template<typename T>
void oi(T& i,string tip){
    cout<<tip<<':';
    cin>>i;
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
        Book(string t,string a,string i,string pre,string d,double pri,int q,int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

        //��ȡ��������
        string TITLE() const{return title;}
        string AUTHOR() const{return author;}
        string ISBN() const{return isbn;}
        string PRESS() const{return press;}
        string DATE() const{return date;}
        double PRICE() const{return price;}
        int QUANTITY() const{return quantity;}
        int STOCK() const{return stock;}

        //չʾ��Ϣ
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

        //��Ҫ��Ϣչʾ
        void briefinfo(){
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
            if(stock-i>=0){
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
        int quantity,stock;//���ͣ����������
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
        book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoi(item[6]),stoi(item[7]));
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
void Booklist_info(vector<Book>& book_list,bool isid=false){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        if(isid){o(i,TAB);}
        book.showinfo();
        i++;
    }
}

//��ȡ��Ҫ����
void Booklist_briefinfo(vector<Book>& book_list){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        o(i,BRIEFTAB);
        book.briefinfo();
        i++;
    }
}

//���һ��ͼ��ı�ͷ
void Booklist_infoheading(bool isid=false){
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
void Booklist_briefheading(bool isid=false){
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
    for(auto& book:book_list){
        if(book.ISBN()==isbn){
            return book;
        }
    }
    throw "ISBNͼ��������ʧ��";
    return book_list[0];
}

//��ѯ�鱾�Ƿ����(ISBN)
bool book_exist(vector<Book>& book_list,string isbn){
    for(auto& book:book_list){
        if(book.ISBN()==isbn){return true;}
    }
    return false;
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
        User(string n,string i,string s="N",vector<string> l={}):name(n),id(i),status(s),borrowed_list(l){};

        //��ȡ��������
        string NAME() const{return name;}
        string ID() const{return id;}
        string STATUS() const{return status;}
        vector<string> BORROWED_LIST() const{return borrowed_list;}

        //��Ϣչʾ
        void showinfo(){
            o(name,TAB);
            o(id,TAB);
            o(status,TAB);
            o(vetostr(borrowed_list),TAB);
            o("");
        }

        //����
        void borrow(string ISBN){
            borrowed_list.push_back(ISBN);
        }

        //����
        void returnBook(string ISBN){
            borrowed_list.erase(remove(borrowed_list.begin(),borrowed_list.end(),ISBN));
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
void Userlist_info(vector<User>& user_list){
    log("���Զ�ȡ�û�����");
    int i=1;
    for(auto& user:user_list){
        o(i,TAB);
        user.showinfo();
        i++;
    }
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
bool user_exist(vector<User>& user_list,string name){
    for(auto& user:user_list){
        if(user.NAME()==name){return true;}
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

//�û���¼
bool login(vector<User>& user_list,string &puser){
    log("login:���Ե�¼");
    string username;
    //o("��¼");
    oi(username,"����������˵�����");
    if(user_exist(user_list,username)){
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
    cls();
    //menu_level=0;
    o(R"(���˵�
    1.����
    2.����
    3.����
    4.ͼ�����
    5.�����˹���
    6.ͼ�鱨��
    7.�˳�����
    )");
    int c;//�洢�û�ѡ������ͱ���
    i(c,1,7);
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
MenuState borrow_menu(vector<Book>& book_list,vector<User>& user_list){
    log("�������˵�");
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
    Booklist_briefheading(true);

    int freenum=0;
    
    //Booklist_info_iffree(book_list,freenum);

    //�����ɱ����ĵ��������
    vector<int> free_index;
    for(size_t i=0;i<book_list.size();++i){
        if(book_list[i].isfree()){
            //���
            o(to_string(freenum+1),BRIEFTAB);
            book_list[i].briefinfo();
            free_index.push_back(i);
            freenum++;
        }
    }

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
MenuState return_menu(vector<Book>& book_list,vector<User>& user_list){
    log("���뻹��˵�");
    cls();
    //Book->�Ƿ񱻽�,User->�Ƿ�Ψһ
    o("����");
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
    cls();

    o(R"(����
    1.����������
    2.�����߲���
    3.�������˲���
    4.��ISBN����
    0.�������˵�
    )");
    int c;
    i(c,0,4);
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

MenuState search_book_by_title(vector<Book>& book_list){
    log("����������");
    string searchbookname;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::TITLE;

    while(retry&&searchbookname!="0"){
        cls();
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
    Booklist_infoheading();
    for(auto& book:target){
        book.showinfo();
    }
    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_author(vector<Book>& book_list){
    log("�����߲���");
    string searchbookauthor;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::AUTHOR;

    while(retry&&searchbookauthor!="0"){
        cls();
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
    Booklist_infoheading();
    for(auto& book:target){
        book.showinfo();
    }
    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_user(vector<Book>& book_list,vector<User>& user_list){
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
    if(vetostr(target_string)=="[]"){
        o("�޽��ģ�");
    }else{
        Booklist_infoheading();
        for(auto& isbn:target_string){
            Booklist_isbn(book_list,isbn).showinfo();
        }
    }

    pause();
    return SEARCH_BOOK;
}

MenuState search_book_by_isbn(vector<Book>& book_list){
    log("��ISBN����");
    string searchbookisbn;
    vector<Book> target;
    bool retry=true;
    string (Book::*pfunc)() const=&Book::ISBN;

    while(retry&&searchbookisbn!="0"){
        cls();
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
    Booklist_infoheading();
    for(auto& book:target){
        book.showinfo();
    }
    pause();
    return SEARCH_BOOK;
}

//ͼ�����˵�
MenuState manage_book_menu(){
    log("����ͼ�����˵�");
    cls();
    o(R"(ͼ�����
    1.��ѯ����ͼ����Ϣ
    2.����µ�ͼ����Ϣ
    3.ɾ������ͼ����Ϣ
    4.�༭ָ��ͼ����Ϣ
    0.�������˵�
    )");
    int c;
    i(c,0,4);
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

MenuState query_book(vector<Book>& book_list){
    log("��ѯ����ͼ����Ϣ");
    Booklist_infoheading();
    Booklist_info(book_list);
    pause();
    return MANAGE_BOOK;
}

MenuState add_book(vector<Book>& book_list){
    log("����µ�ͼ��");
    return MANAGE_BOOK;
}

MenuState del_book(vector<Book>& book_list){
    log("ɾ������ͼ��");
    return MANAGE_BOOK;
}

MenuState edit_book(vector<Book>& book_list){
    log("�༭ָ��ͼ��");
    return MANAGE_BOOK;
}

//�û�����˵�
MenuState manage_user_menu(){
    log("�����û�����˵�");
    return MAIN;
}

MenuState query_user(vector<User>& user_list){
    log("��ѯ�����û���Ϣ");
    return MANAGE_USER;
}

MenuState add_user(vector<User>& user_list){
    log("����µ��û���Ϣ");
    return MANAGE_USER;
}

MenuState del_user(vector<User>& user_list){
    log("ɾ�������û���Ϣ");
    return MANAGE_USER;
}

MenuState edit_user(vector<User>& user_list){
    log("�޸�ָ���û���Ϣ");
    return MANAGE_USER;
}

//ͼ�鱨��˵�
MenuState report_book_menu(){
    log("����ͼ�鱨��˵�");
    return MAIN;
}


int main(){
    log("-----��������-----");

    vector<Book> BookList;
    Booklist_init(BookList,BOOKCSV);

    vector<User> UserList;
    Userlist_init(UserList,USERCSV);

    pause();

    MenuState menu_state= MAIN;

    while(menu_state!=EXIT){
        cls();
        switch(menu_state){
            case MAIN:
                menu_state=main_menu();
                break;
            case BORROW_BOOK:
                menu_state=borrow_menu(BookList,UserList);
                break;
            case RETURN_BOOK:
                menu_state=return_menu(BookList,UserList);
                break;
            case SEARCH_BOOK:
                menu_state=search_menu();
                break;

                case SEARCH_BOOK_BY_TITLE:
                    menu_state=search_book_by_title(BookList);
                    break;
                
                case SEARCH_BOOK_BY_AUTHOR:
                    menu_state=search_book_by_author(BookList);
                    break;
                
                case SEARCH_BOOK_BY_USER:
                    menu_state=search_book_by_user(BookList,UserList);
                    break;
                
                case SEARCH_BOOK_BY_ISBN:
                    menu_state=search_book_by_isbn(BookList);
                    break;
                
            case MANAGE_BOOK:
                menu_state=manage_book_menu();
                break;

                case QUERY_BOOK:
                    menu_state=query_book(BookList);
                    break;

                case ADD_BOOK:
                    menu_state=add_book(BookList);
                    break;
                
                case DEL_BOOK:
                    menu_state=del_book(BookList);
                    break;
                
                case EDIT_BOOK:
                    menu_state=edit_book(BookList);
                    break;
            
            case MANAGE_USER:
                menu_state=manage_user_menu();
                break;
                
                case QUERY_USER:
                menu_state=query_user(UserList);
                break;

                case ADD_USER:
                menu_state=add_user(UserList);
                break;

                case DEL_USER:
                menu_state=del_user(UserList);
                break;

                case EDIT_USER:
                menu_state=edit_user(UserList);
                break;

            case REPORT_BOOK:
                menu_state=report_book_menu();
                break;

            default:
                menu_state=EXIT;
                break;
        }
    }

    log("����Ĭ�Ͻ���");
}