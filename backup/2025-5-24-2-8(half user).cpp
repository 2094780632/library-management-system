///Windows cmd�µ�ͼ��ݹ���ϵͳ
//���ܿ�ѧ�뼼��һ�� 2024413670 �ܺ�
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>

#include<chrono>
#include<ctime>
#include<vector>
#include<string>
using namespace std;

#define LOG "log.txt"//�����־���ļ�
#define BOOKCSV "test.csv"//���ͼ�����ݵ��ļ�
#define USERCSV "user.csv"//����û����ݵ��ļ�  


//logԤ����

int log(const string &text);


///����ͨ�ú�����
//һ���Դ����е������������
inline void o(const string &text){cout<<text<<endl;}
//һ���Ʊ�����̿ɿص������������
#define TAB 10
inline void o(const string &text,int t){cout<<setw(TAB)<<text;}
//һ�����ָ��С��λ�����Ʊ���������������
inline void o(const double &d,int a,int t){cout<<fixed<<setprecision(a)<<setw(TAB)<<d;}
//����ӳ�䵽��ͨ���
inline void o(const int a){o(to_string(a));}
inline void o(const int a,int t){o(to_string(a),TAB);}

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

//CMD����
inline void cls(){system("cls");}

//pause
inline void pause(){system("pause");}

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
        Book(string t,string a,string i,string pre,string d,double pri,int q,int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

        //��ȡ��������
        string TITLE(){return title;}
        string AUTHOR(){return author;}
        string ISBN(){return isbn;}
        string PRESS(){return press;}
        string DATE(){return date;}
        double PRICE(){return price;}
        int QUANTITY(){return quantity;}
        int STOCK(){return stock;}

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
            o(title,TAB);
            o(author,TAB);
            o(to_string(stock),TAB);
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


        //��������
        string savestr(){
            return title+','+author+','+isbn+','+press+','+date+','+to_string(price)+','+to_string(quantity)+','+to_string(stock);
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
    log("save:\n"+save);
}

//��ȡͼ������
void Booklist_info(vector<Book>& book_list){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.showinfo();
        i++;
    }
}

//��ȡ��Ҫ����
void Booklist_briefinfo(vector<Book>& book_list){
    log("���Զ�ȡͼ������");
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.briefinfo();
        i++;
    }
}

//����ָ��������ͼ�����
Book& Booklist_element(vector<Book>& book_list,int index){
    if (index < 0 || index >= book_list.size()) {
        log("�ڵ���ͼ�����ʱ����������Χ,index:"+to_string(index));
        throw out_of_range("����������Χ");
    }
    log("����ͼ�����,index:"+to_string(index));
    return book_list[index];
}

//���Բ���ISBNͼ�����
Book& Booklist_linear_isbn(vector<Book>& book_list,string isbn){
    return book_list[0];
}

/*���÷��� h
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

//vectorתstring
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
        string NAME(){return name;}
        string ID(){return id;}
        string STATUS(){return status;}
        vector<string> BORROWED_LIST(){return borrowed_list;}

        //��Ϣչʾ
        void showinfo(){
            o(name,TAB);
            o(id,TAB);
            o(status,TAB);
            o(vetostr(borrowed_list),TAB);
            o("");
        }

    private:
        string name,id,status;//�û��������û���š��û�״̬(N������B������)
        vector<string> borrowed_list;//�û����ĵ��鵥
};

//����б�
vector<string> apartlist(string data){
    log("apartlist:"+data);
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
        log(line);
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

void Userlist_info(vector<User>& user_list){
    log("���Զ�ȡ�û�����");
    int i=1;
    for(auto& user:user_list){
        o(i,TAB);
        user.showinfo();
        i++;
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
        -����µ�ͼ��
        -ɾ������ͼ��
        -�޸�����ͼ��
    -�����˹���
        -����µĽ�����
        -ɾ�����н�����
        -�޸����н�����
    -ͼ�鱨��
    -�˳�����
*/

//�˵�״̬����
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

//���˵�
MenuState main_menu(){
    log("�������˵�");
    cls();
    //menu_level=0;
    o(R"(���˵�
    1.����
    2.����
    3.ͼ�����
    4.�����˹���
    5.ͼ�鱨��
    6.�˳�����
    )");
    int c;//�洢�û�ѡ������ͱ���
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
            log("���˵�Ĭ���˳�");
            return MAIN;
            break;
    }
}

//����˵�
MenuState borrow_menu(vector<Book>& book_list){
    log("�������˵�");
    cls();
    o(R"(����
    �鼮�б�)");
    //��ͷ
    o("���",TAB);
    o("����",TAB);
    o("����",TAB);
    o("�ɽ�����",TAB);
    /*
    o("ISBN",TAB);
    o("������",TAB);
    o("��������",TAB);
    o("�۸�",TAB);
    o("����",TAB);
    o("���",TAB);
    */
    o("");

    int freenum=0;
    
    //Booklist_info_iffree(book_list,freenum);

    //�����ɱ����ĵ��������
    vector<int> free_index;
    for(size_t i=0;i<book_list.size();++i){
        if(book_list[i].isfree()){
            //���
            o(to_string(freenum+1),TAB);
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
        //��������
        if(book_list[index].STOCK()>0){
            book_list[index].borrow(1);
            log("�鼮"+book_list[index].TITLE()+"���1��");
            o("�鼮��"+book_list[index].TITLE()+"�����ĳɹ���");
            Booklist_save(book_list,BOOKCSV);
            pause();
            return MAIN;
        }
        //Booklist_element(book_list,bookid_to_choose-1).showinfo();
        //book_list[bookid_to_choose-1].showinfo();
    }
    return MAIN;
}

//����˵�
MenuState return_menu(vector<Book>& book_list){
    return MAIN;
}


int main(){
    log("-----��������-----");

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
    log("����Ĭ�Ͻ���");
}