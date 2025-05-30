///Windows cmd�µ�ͼ��ݹ���ϵͳ
//���ܿ�ѧ�뼼��һ�� 2024413670 �ܺ�
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>

#include<vector>
#include<string>
using namespace std;


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


///�ļ�ϵͳ
//���ļ�����string��ʽ�����ļ�����
string f_readall(const string& file_name){
    ifstream file;
    file.open(file_name);
    string content,line;
    //�ж��Ƿ�򿪳ɹ�
    if(file.is_open()){
        while (getline(file,line)){
            content+=line+'\n';
        }
        file.close();
        return content;
    }else{
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
        getline(file,line);
        file.close();
        return line;
    }else{
        cerr<<"�޷����ļ���"<<file_name<<endl;
        return "";
    }
}


///ͼ����Ĺ���
//ͼ����
class Book{
    public:
        //���캯��
        Book(string t,string a,string i,string pre,string d,double pri,int q,int s):
        title(t),author(a),isbn(i),press(pre),date(d),price(pri),quantity(q),stock(s){}    

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
        cerr<<"�޷����ļ���"<<csv<<endl;
        exit(1);
    }
    string line;
    while(getline(file,line)){
            stringstream ss(line);//���ڴ���csv�ַ���
            string item[8];//8������ʹ������洢    
            int i=0;
            while(getline(ss,item[i],',')){i++;}
            book_list.emplace_back(item[0],item[1],item[2],item[3],item[4],stod(item[5]),stoi(item[6]),stoi(item[7]));
    }
    file.close();
}

//��ȡͼ������
void Booklist_info(vector<Book>& book_list){
    int i=1;
    for(auto& book:book_list){
        o(i,TAB);
        book.showinfo();
        i++;
    }
}

///�����幦�ܺ�����
//��ѯ����






///�˵�ϵͳ��

/*
0:���˵�
    -����
    -����
    -ͼ�����
    -�����˹���
    -ͼ�鱨��
    -�˳�����
*/

//���˵�
int main_menu(){
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
    return c;
}

//����˵�
int borrow_menu(vector<Book>& book_list){
    o(R"(����
    �鼮�б�)");
    //��ͷ
    o("���",TAB);
    o("����",TAB);
    o("����",TAB);
    o("ISBN",TAB);
    o("������",TAB);
    o("��������",TAB);
    o("�۸�",TAB);
    o("����",TAB);
    o("���",TAB);
    o("");

    Booklist_info(book_list);
    cout<<"�ܼ�"<<book_list.size()<<"���ɱ����ĵ��鼮"<<endl;
    int cid;
    oi(cid,"������Ҫ���ĵ��鼮���");
    cout<<cid<<endl;
    /*
        Book test("C++","Me","0-0001","GHP","2006-7-27",72.7,1,1);
        test.showinfo();
        cout<<test.savestr()<<endl;

        int a;
        oi(a,"�������");
        o(a);
    */
    return 0;
}

//�˵���ת����
int link(int level,int c,vector<Book>& book_list){
    cls();
    switch (level){
        case 0://���˵��㼶
            switch(c){
                case 1://����
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


//��ʼ������



int main(){
    int menu_level=0;//�˵��㼶
    cls();
    vector<Book> BookList;
    Booklist_init(BookList,"test.csv");
    link(0,main_menu(),BookList);
}