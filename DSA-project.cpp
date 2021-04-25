#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

struct Node
{
    int quantity;
    int price;
    Node * next;
    Node(int x , int y)
    {
        quantity = x;
        price = y;
        next = NULL;
    }
};

class queue
{
    public:
        Node * head , * tail;
    public:
        queue();
        bool isempty();
        void enqueue(int x , int y);
        void dequeue();
        Node * top();
        ~queue();
};

class stocks
{
    int total_gain;
    ofstream fout;
    ifstream fin;
    queue q;
public:
    stocks();
    int get_total_stocks();
    void buy_stocks();
    void sell_stocks();
    int get_total_capital_gain();
    ~stocks();
};

queue::queue()
{
    head = NULL;
    tail = NULL;
    ifstream file_obj;
    file_obj.open("data.txt", ios::in);
    if(file_obj.is_open() && !(file_obj.peek() ==std::ifstream::traits_type::eof()))
    {
        int x,y;
        file_obj>>x;
        file_obj>>y;
        head = new Node(x,y);
        Node * temp = head;
        file_obj>>x;
        file_obj>>y;
        while(!file_obj.eof())
        {
            Node * n = new Node(x,y);
            temp->next = n;
            temp = n;
            file_obj>>x;
            file_obj>>y;
        }
        tail = temp;
    }
}

bool queue::isempty()
{
    if(head == NULL)
        return true;
    return false;
}

void queue::enqueue(int x , int y)
{
    Node * temp = new Node(x , y);
    if(head==NULL)
    {
        head = temp;
        tail = temp;
        return;
    }
    tail->next = temp;
    tail = temp;
}

void queue::dequeue()
{
    if(isempty())
    return;
    if(head->next == NULL)
    {
        tail = NULL;
        Node * temp = head;
        head = NULL;
        delete temp;
        return;
    }
    Node * temp = head;
    head = head->next;
    delete temp;
}

Node * queue::top()
{
    if(head == NULL)
    {
        cout<<"Queue is empty!"<<endl;
        return NULL;
    }
    else
        return head;
}

queue::~queue()
{
    ofstream file_obj;
    file_obj.open("data.txt", ios::out);
    while(head)
    {
        file_obj<<head->quantity<<" ";
        file_obj<<head->price<<endl;
        head = head->next;
    }
    file_obj.close();
}

char * get_time()
{
    time_t curr_time;
    curr_time = time(NULL);
    char *tm = ctime(&curr_time);
    return tm;
}

stocks::stocks()
{
    total_gain = 0;
    fin.open("log.txt");
    fout.open ("log.txt",ios::app);
    ifstream file_gain;
    file_gain.open("gain.txt", ios::in);
    file_gain>>total_gain;
    file_gain.close();
}

int stocks::get_total_stocks()
{
    int total;
    Node * temp = q.head;
    while(temp)
    {
        total+=(temp->quantity);
        temp = temp->next;
    }
    return total;
}

void stocks::buy_stocks()
{
    int x , y;
    cout<<"Enter quantity of stocks you want to buy: ";
    cin>>x;
    cout<<"Enter price of the stocks: ";
    cin>>y;
    q.enqueue(x , y);
    cout<<"Stocks purchased successfully."<<endl;
    cin.get();

    if(fin.is_open())
    {
        fout <<x <<"stocks bought for price of $" <<y <<" per stock at"<<get_time()<<endl;
    }
}
void stocks::sell_stocks()
{
    int quantity , price;
    cout<<"Enter quantity of stocks you want to sell:";
    cin>>quantity;
    int x =quantity;
    if(quantity>get_total_stocks())
    {
        cout<<"Not enough stocks available"<<endl;
        cin.get();
        return;
    }
    cout<<"Enter price of the stock: ";
    cin>>price;
    int tot_gain=0;
    while(quantity)
    {
        int gain = 0;
        Node * temp = q.top();
        if(temp->quantity <= quantity)
        {
            quantity -= temp->quantity;
            gain = (temp->quantity * price) - (temp->quantity *temp->price);
            tot_gain += gain;
            q.dequeue();
        }
        else
        {
            gain = (quantity * price) - (quantity * temp->price);
            tot_gain += gain;
            temp->quantity -= quantity;
            quantity = 0;
        }
    }
    total_gain+=tot_gain;
    cout<<"Stocks sold successfully"<<endl;
    cin.get();
    if(fin.is_open())
    {
        fout<<x<<" stocks sold with gain of $"<<tot_gain<<" at"<<get_time()<<endl;
    }
}

int stocks::get_total_capital_gain()
{
    return total_gain;
}

stocks::~stocks()
{
    fin.close();
    fout.close();
    ofstream file_gain;
    file_gain.open("gain.txt", ios::out);
    file_gain<<total_gain;
    file_gain.close();
}

int main()
{
    stocks st;
    while(true)
    {
        cout<<endl;
        cout<<"\t\t\t\t\t\t\t-------------WELCOME TO STOCK MARKET------------" <<endl;
        cout<<"\t\t\t\t\t\t\t************************************************" <<endl<<endl;
        cout<<"\t\t\t\t\t\t\t* Press 1 to buy stocks*"<<endl;
        cout<<"\t\t\t\t\t\t\t* Press 2 to sell stocks*"<<endl;
        cout<<"\t\t\t\t\t\t\t* Press 3 to get total capital gain*"<<endl;
        cout<<"\t\t\t\t\t\t\t* Press 4 to exit*"<<endl<<endl;
        cout<<"\t\t\t\t\t\t\t*************************************************" <<endl;

        cout<<endl<<"Enter your choice: ";
        int x;
        cin>>x;

        switch(x)
        {
            case 1:
            {
                st.buy_stocks();
                cin.get();
                cin.get();
                break;
            }

            case 2:
            {
                st.sell_stocks();
                cin.get();
                cin.get();
                break;
            }

            case 3:
            {
                cout <<"Total gain is: ";
                cout<<"$ "<<st.get_total_capital_gain()<<endl;
                cin.get();
                cin.get();
                break;
            }

            case 4:
            {
                return 0;
            }

            default:
            {
                cout<<"Choose a valid option."<<endl;
                cin.get();
                cin.get();
            }
        }
    }
    return 0;
}
