#include<iostream>
#include<vector>
#include<string>
#include<utility>

using namespace std;

struct messages2{
        string msg;
        int sender;
        vector<bool> msgReceived;
        vector<int> time;

        messages2(const string& msg, int sender, const vector<bool>& msgReceived, const vector<int>& time) : msg(msg), sender(sender), msgReceived(msgReceived), time(time) {}

};

bool checkBSSRule(const vector<int>& msgTime, const vector<int>& recieverTime, int senderid){
    if(msgTime[senderid] == recieverTime[senderid] + 1){
        for(int i = 0; i < msgTime.size(); i++){
            if(i != senderid){
                if(msgTime[i] > recieverTime[i]){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

void recievetransaction(string str, vector<vector<int>>& time, vector<vector<messages2>>& transaction, vector<vector<string>>& buffer, int n, int processid){
    int recievedFrom = stoi(str.substr(8)); // Extract the process number
    string msg = str.substr(10,str.size()-1);
    vector<int> recieverTime = time[processid];

    bool found = false;
    int msgsender;
    vector<bool> msgReceived;
    vector<int> msgTime;


    for (auto& trans : transaction[recievedFrom]) {
        if (trans.msg == msg) {
            found = true;
            msgsender = trans.sender;
            msgReceived = trans.msgReceived;
            msgTime = trans.time;

            if(recievedFrom == msgsender){
                if(msgReceived[processid] == false){
                    cout<<str;
                    cout<<" (";
                    for (int i = 0; i < n; i++) {
                        cout<<to_string(time[processid][i]);
                    }
                    cout<<")"<<endl; 
                }
                msgReceived[processid] = true;
                trans.msgReceived = msgReceived;
                if(checkBSSRule(msgTime,recieverTime, msgsender)){
                    time[processid][msgsender] += 1;
                    msgReceived[processid] = true;
                    cout <<"recv_A p" << msgsender << " " << msg << " (";
                    for (int i = 0; i < n; i++) {
                        cout<<to_string(time[processid][i]);
                    }
                    cout<<")"<<endl; 
                    if (buffer[processid].size() > 0) {

                        while(buffer[processid].empty() == false){
                            string str = buffer[processid].front();
                            buffer[processid].erase(buffer[processid].begin());
                            recievetransaction(str, time, transaction, buffer, n, processid);
                        }
                    }
                }else{
                    buffer[processid].push_back(str);
                }
            }

            break;
        }
    }

    if (!found) {
        cout << "Message not found in transaction" << endl;
    }
    return ;
}
int main(){

    int n; //number of process
    cin>>n;
    vector<vector<int>> time(n,vector<int>(n, 0));
    cout<<"Initial Time Matrix"<<endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<time[i][j]<<" ";
        }
        cout<<endl;
    }
    

    vector<vector< messages2 >> transaction(n);
    vector<vector<string>> buffer(n);


    string str;
    int processid = -1;
    cin.ignore();
    while(getline(cin,str) && str != "exit"){
        if (str.find("begin process p") == 0 and str.length() == 16){
            processid = stoi(str.substr(15)); // Extract the process number
            if (processid < n){
                cout <<str << endl;
            }else{
                cout << "Invalid process id" << endl;
                processid = -1;
            }
        }else if(str.compare("end process") == 0 and processid != -1){
            cout <<str<< endl;
            processid = -1;
            
        }else if(str.substr(0,4) == "send" and processid != -1){
            time[processid][processid] += 1;
            str += " (";
            for (int i = 0; i < n; i++) {
                str += to_string(time[processid][i]);
            }
            str += ")";
            string msg = str.substr(5); // Extract "m0"
            size_t startPos = msg.find(' '); // Find the first space
            size_t endPos = msg.find(' '); // Find the space before the parenthesis

            if (startPos != string::npos && endPos != string::npos) {
                msg = msg.substr(0, endPos); // Extract "m1"
            }
            vector<bool> msgReceived(n, false);
            msgReceived[processid] = true;

            vector<int> timeSent = time[processid];

            transaction[processid].emplace_back(msg, processid , msgReceived, timeSent);

            cout <<str<< endl;
        }else if(str.substr(0,8) == "recv_B p" and processid != -1){
            recievetransaction(str, time, transaction, buffer, n, processid);
        }
        else{
            cout << "Invalid input" << endl;
        }
        str = "";
    }
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < transaction[i].size(); j++){
    //         cout<<transaction[i][j].msg<<" "<<transaction[i][j].sender<<" ";
    //         cout<<endl;
    //         for(int k = 0; k < n; k++){
    //             cout<<transaction[i][j].msgReceived[k]<<" ";
    //         }
    //         cout<<endl;
    //         for(int k = 0; k < n; k++){
    //             cout<<transaction[i][j].time[k]<<" ";
    //         }
    //         cout<<endl;
    //     }

    // }

    cout<<"Final Time Matrix"<<endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<time[i][j]<<" ";
        }
        cout<<endl;
    }




cout<<"hello world"<<endl;



return 0;
}